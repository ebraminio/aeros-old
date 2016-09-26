#include "devices/pic.h"
#include <sys/io.h>
#include "io/log.h"

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC1_CMD	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC2_CMD	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20

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

void (*irq_routines[0x81])(regs_t* r) = {0};

void irq_install_handler(uint32_t irq, void (*handler)(regs_t*))
{
	irq_routines[irq] = handler;
}

void pic_remap(void)
{
	outb(PIC1_CMD, 0x11);	// Initialization sequence
	outb(PIC2_CMD, 0x11);	//

	outb(PIC1_DATA, 0x20);	// Vector offset
	outb(PIC2_DATA, 0x28);	//
	
	outb(PIC1_DATA, 0x04);	// Tell PIC1 PIC2 is present at IRQ2
	outb(PIC2_DATA, 0x02);	// Tell PIC2 it's slave
	
	outb(PIC1_DATA, 0x01);
	outb(PIC2_DATA, 0x01);
	
	outb(PIC1_DATA, 0xFF);	// Masks
	outb(PIC2_DATA, 0xFF);	//
}

void mask_irq(uint8_t irq)
{
	uint8_t port;

	if(irq < 8)
		port = PIC1_DATA;
	else
	{
		port = PIC2_DATA;
		irq -= 8;
	}

	//Get current mask, set bit of irq
	outb(port, inb(port) | (1 << irq));
}

void unmask_irq(uint8_t irq)
{
	uint8_t port;

	if(irq < 8)
		port = PIC1_DATA;
	else
	{
		port = PIC2_DATA;
		irq -= 8;
	}

	outb(port, inb(port) & ~(1 << irq));
}

void pic_send_EOI(uint8_t irq)
{
	if (irq >= 8)
		outb(PIC2_CMD, PIC_EOI);
	outb(PIC1_CMD, PIC_EOI);
}

void irq_handler(regs_t* r)
{
	if (irq_routines[r->int_no - 32])
		irq_routines[r->int_no - 32](r);
	else nopanic("IRQ%d from %x\n", r->int_no-32, r->eip);

	pic_send_EOI(r->int_no-32);
}