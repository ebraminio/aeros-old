#include "io/serial.h"
#include "devices/pic.h"
#include <sys/io.h>

#define COM1 0X3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

#define DATA_REG		0
#define INT_ENABLE_REG	1
#define RATE_LO_REG		0
#define RATE_HI_REG 	1
#define INT_ID_FIFO_CTR	2
#define LINE_CTRL_REG	3
#define MODEM_CTRL_REG	4
#define LINE_STUS_REG	5
#define MODEM_STUS_REG	6
#define SCRATCH_REG		7

typedef union
{

} line_status_reg_t;

typedef union
{
	struct
	{
		uint8_t unknown : 7;
		uint8_t enable_dlab : 1;
	};
	uint8_t numeric;
} line_ctrl_reg_t;

typedef union
{
	struct
	{
		uint8_t data_available : 1;
		uint8_t transmitter_empty : 1;
		uint8_t break_error : 1;
		uint8_t status_change : 1;
		uint8_t unused : 4;
	};
	uint8_t numeric;
} int_reg_t;

void serial_handler(regs_t* r)
{
}

void write_serial(uint8_t com, int c)
{
	//Only COM1 currently
	while(inb(COM1+LINE_STUS_REG)&1);
	outb(COM1+DATA_REG, c);
}

void serial_init(uint8_t com)
{
	const uint16_t baud_rate = 0x300;

	// Only COM1 currently
	outb(COM1 + INT_ENABLE_REG, 0);
	outb(COM1 + LINE_CTRL_REG, 0x80);	// Enable DLAB
	outb(COM1 + RATE_LO_REG, baud_rate>>8);
	outb(COM1 + RATE_HI_REG, baud_rate&0xFF);
	outb(COM1 + LINE_CTRL_REG, 3);
	outb(COM1 + INT_ID_FIFO_CTR, 0xC7);
	outb(COM1 + MODEM_CTRL_REG, 0xC8);

	const char msg[] = "COM1 open\n";
	for(uint8_t i=0; i<sizeof(msg); i++)
		write_serial(1, msg[i]);

	irq_install_handler(3, serial_handler);
	irq_install_handler(4, serial_handler);
	unmask_irq(3);
	unmask_irq(4);
}
