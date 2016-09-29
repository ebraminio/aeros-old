#include "mem/pmem.h"
#include <string.h>

#define BLOCK_SIZE 4096

extern const uint8_t _kernel_base;
extern const uint8_t _kernel_end;
extern const uint8_t _stack_bottom;
extern const uint8_t _stack_top;

size_t memory_size = 0;
size_t used_blocks = 0;
uint8_t block_map[4*1024*1024/BLOCK_SIZE*1024/8];

void dealloc_blocks(uintptr_t start, uintptr_t end)
{
	size_t bit = start/BLOCK_SIZE;
	size_t bit_end = end/BLOCK_SIZE;
	if(start%BLOCK_SIZE)	// Only use full pages
		bit++;

	while(bit%8)	// Complete current 8bits cell
	{
		block_map[bit/8] &= ~(1 << (bit%8));
		bit++;
	}

	while(bit+8 < bit_end)	// Fill every 8bits cell we can
	{
		block_map[bit/8] = 0;
		bit += 8;
	}

	while(bit<bit_end)
	{
		block_map[bit/8] &= ~(1 << (bit%8));
		bit++;
	}
}

void alloc_blocks(uintptr_t start, uintptr_t end)
{
	size_t bit = start/BLOCK_SIZE;
	size_t bit_end = end/BLOCK_SIZE;
	if(start%BLOCK_SIZE)
		start--;

	while(bit%8)
	{
		block_map[bit/8] |= 1<<(bit%8);
		bit--;
	}

	while(bit+8 < bit_end)
	{
		block_map[bit/8] = 0xFF;
		bit+=8;
	}

	while(bit<bit_end)
	{
		block_map[bit/8] |= 1<<(bit%8);
		bit++;
	}
}

void pmem_init(multiboot_info_t* mboot_info)
{
	memset(block_map, 0xFF, sizeof(block_map));

	const uint32_t page_end = 0x100000 + mboot_info->mem_upper*1024;

	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mboot_info->mmap_addr;

	while((uintptr_t)mmap < mboot_info->mmap_addr+mboot_info->mmap_length)
	{
		if(mmap->type == 1)
		{
			dealloc_blocks(mmap->addr, mmap->addr+mmap->len);
			memory_size += mmap->len;
		}
		mmap = (multiboot_memory_map_t*)((uintptr_t)mmap+mmap->size+4);
	}

	alloc_blocks(0, 0x100000);
	alloc_blocks((uintptr_t)&_kernel_base, (uintptr_t)&_kernel_end);
	alloc_blocks((uintptr_t)&_stack_bottom, (uintptr_t)&_stack_top);

	if(mboot_info->flags & MULTIBOOT_INFO_CMDLINE)
		alloc_blocks(mboot_info->cmdline, (mboot_info->cmdline&~0xFFF)+BLOCK_SIZE);
	if(mboot_info->flags & MULTIBOOT_INFO_DRIVE_INFO)
		alloc_blocks(mboot_info->drives_addr, (mboot_info->drives_addr&~0xFFF)+mboot_info->drives_length);
	if(mboot_info->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME)
		alloc_blocks(mboot_info->boot_loader_name, (mboot_info->boot_loader_name&~0xFFF)+BLOCK_SIZE);
	if(mboot_info->flags & MULTIBOOT_INFO_VBE_INFO);
}

size_t p_find_free(void)
{
	for(size_t i=0; i<sizeof(block_map); i++)
		if(block_map[i] != 0xFF)
			for(uint8_t j=0; j<8; j++)
				if((block_map[i]&(1<<j)) == 0)
					return i*8+j;
	return 0;
}

void* palloc(void)
{
	const size_t block = p_find_free();

	if(block == 0)
		return 0;

	block_map[block/8] |= 1<<(block%8);
	used_blocks++;
	return (void*)(block*BLOCK_SIZE);
}

void pfree(void* block)
{
	const size_t bit = (uintptr_t)block/BLOCK_SIZE;
	block_map[bit/8] &= ~(1 << (bit%8));
	used_blocks--;
}