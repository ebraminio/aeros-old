#include "cpu/idt.h"
#include <stdint.h>
#include "devices/pic.h"
#include "io/log.h"

#define TASK32	0x5
#define INT16	0x6
#define TRAP16	0x7
#define INT32	0xE
#define TRAP32	0xF

typedef struct __attribute__((packed))
{
	uint16_t offset_low;
	uint16_t cs;
	uint8_t zero;
	uint8_t type : 4;
	uint8_t dpl: 2;
	uint8_t ss : 1;
	uint8_t present : 1;
	uint16_t offset_high;
} idt_entry_t;

typedef struct __attribute__((packed))
{
	uint16_t limit;
	idt_entry_t* base;
} idt_ptr;

extern void _set_idtr(idt_ptr* idt_ptr);

extern void _isr0(void);
extern void _isr1(void);
extern void _isr2(void);
extern void _isr3(void);
extern void _isr4(void);
extern void _isr5(void);
extern void _isr6(void);
extern void _isr7(void);
extern void _isr8(void);
extern void _isr9(void);
extern void _isr10(void);
extern void _isr11(void);
extern void _isr12(void);
extern void _isr13(void);
extern void _isr14(void);
extern void _isr15(void);
extern void _isr16(void);
extern void _isr17(void);
extern void _isr18(void);
extern void _isr19(void);
extern void _isr20(void);
extern void _isr21(void);
extern void _isr22(void);
extern void _isr23(void);
extern void _isr24(void);
extern void _isr25(void);
extern void _isr26(void);
extern void _isr27(void);
extern void _isr28(void);
extern void _isr29(void);
extern void _isr30(void);
extern void _isr31(void);

extern void _irq0(void);
extern void _irq1(void);
extern void _irq2(void);
extern void _irq3(void);
extern void _irq4(void);
extern void _irq5(void);
extern void _irq6(void);
extern void _irq7(void);
extern void _irq8(void);
extern void _irq9(void);
extern void _irq10(void);
extern void _irq11(void);
extern void _irq12(void);
extern void _irq13(void);
extern void _irq14(void);
extern void _irq15(void);
extern void _irq128(void);

static const char* exception_messages[] =
{
	"Division By Zero",				// 0	0x00
	"Debug",						// 1	0x01
	"Non Maskable Interrupt",		// 2	0x02
	"Breakpoint",					// 3	0x03
	"Into Detected Overflow",		// 4	0x04
	"Out of Bounds",				// 5	0x05
	"Invalid Opcode",				// 6	0x06
	"No Coprocessor",				// 7	0x07
	"Double Fault",					// 8	0x08
	"Coprocessor Segment Overrun",	// 9	0x09
	"Bad TSS",						// 10	0x0A
	"Segment Not Present",			// 11	0x0B
	"Stack/Segment Fault",			// 12	0x0C
	"General Protection Fault",		// 13	0x0D
	"Page Fault",					// 14	0x0E
	"Unknown Interrupt",			// 15	0x0F
	"Coprocessor Fault",			// 16	0x10
	"Alignment Check",				// 17	0x11
	"Machine Check",				// 18	0x12
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

static idt_entry_t idt[255] = {0};
static idt_ptr idtr =
{
	.limit = sizeof(idt) - 1,
	.base = idt,
};

static void set_idt_entry(uint8_t num, uint32_t callback)
{
	idt[num].offset_low = callback & 0xFFFF;
	idt[num].cs = 1*8;
	idt[num].zero = 0;
	idt[num].present = 1;
	idt[num].dpl = 0;
	idt[num].ss = 0;	// TODO Explain why
	idt[num].type = INT32;
	idt[num].offset_high = (callback & 0xFFFF0000)>>16;
}

void idt_init(void)
{
	set_idt_entry(0, (uint32_t)_isr0);
	set_idt_entry(1, (uint32_t)_isr1);
	set_idt_entry(2, (uint32_t)_isr2);
	set_idt_entry(3, (uint32_t)_isr3);
	set_idt_entry(4, (uint32_t)_isr4);
	set_idt_entry(5, (uint32_t)_isr5);
	set_idt_entry(6, (uint32_t)_isr6);
	set_idt_entry(7, (uint32_t)_isr7);
	set_idt_entry(8, (uint32_t)_isr8);
	set_idt_entry(9, (uint32_t)_isr9);
	set_idt_entry(10, (uint32_t)_isr10);
	set_idt_entry(11, (uint32_t)_isr11);
	set_idt_entry(12, (uint32_t)_isr12);
	set_idt_entry(13, (uint32_t)_isr13);
	set_idt_entry(14, (uint32_t)_isr14);
	set_idt_entry(15, (uint32_t)_isr15);
	set_idt_entry(16, (uint32_t)_isr16);
	set_idt_entry(17, (uint32_t)_isr17);
	set_idt_entry(18, (uint32_t)_isr18);
	set_idt_entry(19, (uint32_t)_isr19);
	set_idt_entry(20, (uint32_t)_isr20);
	set_idt_entry(21, (uint32_t)_isr21);
	set_idt_entry(22, (uint32_t)_isr22);
	set_idt_entry(23, (uint32_t)_isr23);
	set_idt_entry(24, (uint32_t)_isr24);
	set_idt_entry(25, (uint32_t)_isr25);
	set_idt_entry(26, (uint32_t)_isr26);
	set_idt_entry(27, (uint32_t)_isr27);
	set_idt_entry(28, (uint32_t)_isr28);
	set_idt_entry(29, (uint32_t)_isr29);
	set_idt_entry(30, (uint32_t)_isr30);
	set_idt_entry(31, (uint32_t)_isr31);

	set_idt_entry(32, (uint32_t)_irq0);
	set_idt_entry(33, (uint32_t)_irq1);
	set_idt_entry(34, (uint32_t)_irq2);
	set_idt_entry(35, (uint32_t)_irq3);
	set_idt_entry(36, (uint32_t)_irq4);
	set_idt_entry(37, (uint32_t)_irq5);
	set_idt_entry(38, (uint32_t)_irq6);
	set_idt_entry(39, (uint32_t)_irq7);
	set_idt_entry(40, (uint32_t)_irq8);
	set_idt_entry(41, (uint32_t)_irq9);
	set_idt_entry(42, (uint32_t)_irq10);
	set_idt_entry(43, (uint32_t)_irq11);
	set_idt_entry(44, (uint32_t)_irq12);
	set_idt_entry(45, (uint32_t)_irq13);
	set_idt_entry(46, (uint32_t)_irq14);
	set_idt_entry(47, (uint32_t)_irq15);
	set_idt_entry(0x80, (uint32_t)_irq128);

	pic_remap();
	_set_idtr(&idtr);
}
void fault_handler(regs_t* r)
{
	if(r->int_no == 3)
	{
		nopanic("Breakpoint interrupt\n"
			"EAX:0x%08x\tEBX:0x%08x\tECX:0x%08x\tEDX:0x%08x\n"
			"EDI:0x%08x\tESI:0x%08x\tEIP:0x%08x\tESP:0x%08x\tEBP:0x%08x\n"
			"CS:0x%04x\tDS:0x%04x\tSS:0x%04x\n",
			r->eax, r->ebx, r->ecx, r->edx,
			r->edi, r->esi, r->eip, r->esp, r->ebp,
			r->cs&0xFFFF, r->ds&0xFFFF, r->ss&0xFFFF);
		return;
	}

	if (r->int_no < 32)
		panic("%s Exception(%d) err code:0x%x. System Halted!\n"
			"EAX:0x%08x\tEBX:0x%08x\tECX:0x%08x\tEDX:0x%08x\n"
			"ESI:0x%08x\tEDI:0x%08x\tEIP:0x%08x\tESP:0x%08x\tEBP:0x%08x\n"
			"CS:0x%04x\tDS:0x%04x\tSS:0x%04x",
			exception_messages[r->int_no], r->int_no, r->err_code,
			r->eax, r->ebx, r->ecx, r->edx,
			r->esi, r->edi, r->eip, r->esp, r->ebp,
			r->cs&0xFFFF, r->ds&0xFFFF, r->ss&0xFFFF);
}
