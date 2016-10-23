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
	uint32_t page_table : 20;	// 4KB aligned
} page_dir_entry_t;

typedef struct
{
	page_dir_entry_t tables[1024];
} page_dir_t;

page_dir_t _page_dir __attribute((aligned(4096))) = {0};
page_dir_t* page_dir = &_page_dir;
page_table_t page_tables[1024] __attribute((aligned(4096))) = {0};

#define IDENTITY_MAP(p,s) vmap(p,p,s)

void vmap(uintptr_t vstart, uintptr_t pstart, size_t size)
{
	const size_t page_num = size/PAGE_SIZE;
	for(size_t page=0; page<page_num; page++)
	{
		const uint16_t dir_index = vstart>>22;
		const uint16_t ptable_index = (vstart>>12)&0x3FF;

		page_dir->tables[dir_index].present = 1;
		page_dir->tables[dir_index].writeable = 1;
		page_dir->tables[dir_index].page_table = (uintptr_t)&page_tables[dir_index]>>12;
		if(!page_dir->tables[dir_index].page_table)
		{
			void* alloc = palloc();
			IDENTITY_MAP((uintptr_t)alloc, PAGE_SIZE);
			memset(alloc, 0, PAGE_SIZE);
			page_dir->tables[dir_index].page_table = (uintptr_t)alloc>>12;
		}
		page_table_t* page_table = (page_table_t*)(page_dir->tables[dir_index].page_table<<12);
		page_table->pages[ptable_index].present = 1;
		page_table->pages[ptable_index].writeable = 1;
		page_table->pages[ptable_index].page_address = pstart>>12;

		vstart += PAGE_SIZE;
		pstart += PAGE_SIZE;
	}
}

extern const void _stack_bottom;
extern const void _stack_top;

void vmem_init(void)
{
//	page_dir = (page_dir_t*)palloc();
	IDENTITY_MAP((uintptr_t)page_dir, PAGE_SIZE);
//	memset(page_dir, 0, sizeof(*page_dir));
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