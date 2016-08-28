#include "cpu/gdt.h"
#include <stdint.h>

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

gdt_entry_t gdt[3] =
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
		.direction_conforming = 1,
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
};

gdt_ptr_t gdt_ptr =
{
	.limit = sizeof(gdt_entry_t)*sizeof(gdt)/8 - 1,
	.gdt_base = gdt,
};

void _set_gdtr(gdt_ptr_t* gdt_ptr);

void gdt_init(void)
{
	_set_gdtr(&gdt_ptr);
}