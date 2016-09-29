#include "mem/vmem.h"
#include <stdint.h>
#include "mem/pmem.h"
#include <stddef.h>

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
	uint8_t global : 1;		// Ignored
	uint8_t available : 3;
	uint32_t page_table : 20;	// 4KB aligned
} page_dir_entry_t;

typedef struct
{
	page_dir_entry_t tables[1024];
} page_dir_t;

page_dir_t* page_dir;

void vmap(uintptr_t vstart, uintptr_t pstart, size_t size)
{
	const size_t page_num = size/PAGE_SIZE;
	for(size_t page=0; page<page_num; page++)
	{
		const uint16_t dir_index = vstart>>22;
		const uint16_t ptable_index = (vstart>>12)&0x3FF;

		page_dir->tables[dir_index].present = 1;
		page_dir->tables[dir_index].writeable = 1;
		if(!page_dir->tables[dir_index].page_table)
		{
			uintptr_t alloc = (uintptr_t)palloc();
			page_dir->tables[dir_index].page_table = alloc>>12;
			vmap(alloc, alloc, PAGE_SIZE);
		}
		page_table_t* page_table = (page_table_t*)(page_dir->tables[dir_index].page_table<<12);
		page_table->pages[ptable_index].present = 1;
		page_table->pages[ptable_index].writeable = 1;
		page_table->pages[ptable_index].page_address = pstart>>12;

		vstart += PAGE_SIZE;
		pstart += PAGE_SIZE;
	}
}

extern const uint8_t _kernel_end;
extern const uint8_t _stack_bottom;
extern const uint8_t _stack_top;

void vmem_init(void)
{
	page_dir = (page_dir_t*)palloc();
	vmap((uintptr_t)page_dir, (uintptr_t)page_dir, PAGE_SIZE);
	vmap(0, 0, (uintptr_t)&_kernel_end+PAGE_SIZE);
	uintptr_t stack_start_page = (uintptr_t)&_stack_bottom&~0xFFF;
	uintptr_t stack_end_page = ((uintptr_t)&_stack_top&~0xFFF)+PAGE_SIZE;
	vmap(stack_start_page, stack_start_page, stack_end_page-stack_start_page);

	asm("mov %0, %%cr3" : : "r"(page_dir));
	asm("xchgw %bx, %bx;"
		"mov %cr0, %eax;"
		"or $0x80000000, %eax;"
		"mov %eax, %cr0");
}
