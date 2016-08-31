#include "cpu/gdt.h"
#include <stdint.h>
#include <stddef.h>

#define GDT_ACCESSED 1
#define GDT_UNREADABLE_CODE 0
#define GDT_READABLE_CODE 2
#define GDT_READ_ONLY_DATA 0
#define GDT_READ_WRITE_DATA 2
#define GDT_DATA_GROW_UP 0
#define GDT_DATA_GROW_DOWN 4
#define GDT_GROW_UP 0
#define GDT_CODE_CONFORMING 4	/**< Conforming code segment allog execution from lower privilege. Not conforming restrict to equal privilege */
#define GDT_EXECUTABLE 8
#define GDT_NOT_TSS 0x10
#define GDT_RING0 0
#define GDT_RING1 0x20
#define GDT_RING2 0x40
#define GDT_RING3 0x60
#define GDT_KERNEL_MODE GDT_RING0
#define GDT_USER_MODE GDT_RING3
#define GDT_PRESENT 0x80

#define GDT_SIZE_16 0
#define GDT_SIZE_32 4
#define GDT_UNIT_BYTE 0
#define GDT_UNIT_4KiB 8

typedef struct __attribute__((packed))
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t accessed : 1;
	uint8_t rcode_wdata : 1;			// Readable code or writable data
	uint8_t direction_conforming : 1;	// Code: can be accessed by lower dpl
										// Data: grow down
	uint8_t executable : 1;
	uint8_t not_tss : 1;
	uint8_t dpl : 2;
	uint8_t present : 1;
	uint8_t limit_high : 4;
	uint8_t zero : 2;
	uint8_t size : 1;			// 0: 16bit protected mode. 1: 32bit protected mode
	uint8_t granularity : 1;	// 0: 1B. 1: 4KB
	/*union
	{
		uint8_t data_direction : 1;		// 1: grow down
		uint8_t conforming_code : 1;	// Conforming segments allow execution from
	};	// lower or equal DPL. Non-conforming segments restrict to ring set in dpl*/
	uint8_t base_high;
} gdt_entry_t;

typedef struct __attribute__((packed))
{
	uint16_t limit;
	gdt_entry_t* gdt_base;
} gdt_ptr_t;

typedef struct
{
	uint32_t prev_tss;
	uint32_t esp0;		// stack pointer
	uint32_t ss0;		// stack segment
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
} tss_t;

uint8_t tss_stack[16*1024];

static tss_t tss =
{
	.prev_tss = 0,
	.esp0 = (uint32_t)tss_stack,
	.ss0 = 8*2,
	.esp1 = 0,
	.ss1 = 0,
	.esp2 = 0,
	.ss2 = 0,
	.cr3 = 0,
	.eip = 0,
	.eflags = 0,
	.eax = 0,
	.ecx = 0,
	.edx = 0,
	.ebx = 0,
	.esp = 0,
	.ebp = 0,
	.esi = 0,
	.edi = 0,
	.cs = (8*1)|3,
	.es = (8*2)|3,
	.ss = (8*2)|3,
	.ds = (8*2)|3,
	.fs = (8*2)|3,
	.gs = (8*2)|3,
	.ldt = 0,
	.trap = 0,
	.iomap_base = sizeof(tss),
};

gdt_entry_t gdt[4] =
{
	{0},
	{
		.limit_low = 0xFFFF,
		.base_low = 0,
		.base_mid = 0,
		.accessed = 0,
		.rcode_wdata = 1,
		.direction_conforming = 1,
		.executable = 1,
		.not_tss = 1,
		.dpl = 0,
		.present = 1,
		.limit_high = 0x0F,
		.zero = 0,
		.size = 1,
		.granularity = 1,
		.base_high = 0,
	},
	{
		.limit_low = 0xFFFF,
		.base_low = 0,
		.base_mid = 0,
		.accessed = 0,
		.rcode_wdata = 1,
		.direction_conforming = 0,
		.executable = 0,
		.not_tss = 1,
		.dpl = 0,
		.present = 1,
		.limit_high = 0x0F,
		.zero = 0,
		.size = 1,
		.granularity = 1,
		.base_high = 0,
	},
	{
		.limit_low = sizeof(tss)&0xFFFF,
		.accessed = 1,
		.rcode_wdata = 0,
		.direction_conforming = 0,
		.executable = 1,
		.not_tss = 0,
		.dpl = 0,
		.present = 1,
		.limit_high = (sizeof(tss)>>16)&0xFFFF,
		.zero = 0,
		.size = 1,
		.granularity = 0,
	},
};

gdt_ptr_t gdt_ptr =
{
	.limit = sizeof(gdt_entry_t)*sizeof(gdt)/8 - 1,
	.gdt_base = gdt,
};

void _set_gdtr(gdt_ptr_t* gdt_ptr);

void gdt_init(void)
{
	gdt[3].base_low = (uintptr_t)&tss&0xFFFF;
	gdt[3].base_mid = ((uintptr_t)&tss>>16)&0xFF;
	gdt[3].base_high = ((uintptr_t)&tss>>24)&0xFF;
	_set_gdtr(&gdt_ptr);
}
