#include "mem/vmem.h"
#include "mem/pmem.h"
#include <string.h>
#include <unistd.h>

#define PAGE_SIZE 4096

typedef struct __attribute__((packed))
{
	uint8_t present : 1;
	uint8_t writeable : 1;
	uint8_t user_mode : 1;
	uint8_t write_through : 1;	// 0: Write back caching enabled. 1: Write through caching enabled
	uint8_t cache_disabled : 1;
	uint8_t accessed : 1;
	uint8_t dirty : 1;
	uint8_t reserved : 1;
	uint8_t global : 1;			// Require Page Global Enable in CR4. Prevents TLB update on CR3 reset for this page
	uint8_t available : 3;
	uint32_t page_address : 20;	// 4KB aligned
} page_table_entry_t;

typedef struct
{
	page_table_entry_t pages[1024];
} page_table_t;

typedef struct __attribute__((packed))
{
	uint8_t present : 1;
	uint8_t writeable : 1;
	uint8_t user_mode : 1;
	uint8_t write_through : 1;
	uint8_t cache_disabled : 1;
	uint8_t accessed : 1;
	uint8_t reserved : 1;
	uint8_t large_page : 1;
	uint8_t global : 1;			// Ignored
	uint8_t available : 3;
	uint32_t table_address : 20;	// 4KB aligned
} page_dir_entry_t;

typedef struct
{
	page_dir_entry_t tables[1024];
} page_dir_t;

page_dir_t _page_dir __attribute((aligned(4096))) = {0};
page_dir_t* page_dir = &_page_dir;
page_table_t first_page_table __attribute((aligned(4096))) = {0};

void vmap(uintptr_t vstart, uintptr_t pstart, size_t size)
{
	if(!size)
		return;

	const size_t page_num = size/PAGE_SIZE + (size%PAGE_SIZE ? 1 : 0);
	for(size_t n=0; n<page_num; n++)
	{
		page_dir_entry_t* const dir_entry = &page_dir->tables[vstart>>22];

		dir_entry->present = 1;
		dir_entry->writeable = 1;
		if(!dir_entry->table_address)
		{
			uintptr_t table_ptr = (uintptr_t)palloc();
			dir_entry->table_address = table_ptr>>12;
			IDENTITY_MAP(table_ptr, PAGE_SIZE);
			memset((void*)table_ptr, 0, PAGE_SIZE);
		}

		page_table_entry_t* page = &((page_table_t*)(dir_entry->table_address<<12))->pages[(vstart>>12)&0x3FF];
		page->present = 1;
		page->writeable = 1;
		page->page_address = pstart>>12;

		vstart += PAGE_SIZE;
		pstart += PAGE_SIZE;
	}
}

extern const void _stack_bottom;
extern const void _stack_top;

void vmem_init(void)
{
	page_dir->tables[0].table_address = (uintptr_t)&first_page_table>>12;
	IDENTITY_MAP(0, (size_t)&_end);
	IDENTITY_MAP((uintptr_t)&_stack_bottom, (uintptr_t)&_stack_top-(uintptr_t)&_stack_bottom);

	asm ("mov %0, %%cr3"
		::"r" (page_dir));
	asm ("xchgw %bx, %bx;"
		 "mov %cr0, %eax;"
		 "or $0x80000000, %eax;"
		 "mov %eax, %cr0");
}

int brk(void* addr)
{
	while(addr > heap_end)
	{
		vmap((uintptr_t)heap_end, (uintptr_t)palloc(), PAGE_SIZE);
		heap_end = (uint8_t*)heap_end + PAGE_SIZE;
	}
	return 0;
}