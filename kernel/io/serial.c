#include "io/serial.h"
#include "devices/pic.h"
#include <sys/io.h>
#include "cpu/bda.h"
#include <stdio.h>

#define DATA_REG			0
#define DIV_LATCH_LO_REG	0

#define DIV_LATCH_HI_REG	1
#define INT_ENABLE_REG		1
#define INT_EN_DATA_AVAILABLE			(1<<0)
#define INT_EN_OUT_HOLDING_REG_EMPTY	(1<<1)
#define INT_EN_IN_LINE_STATUS			(1<<2)
#define INT_EN_MODEM_STATUS_CHANGE		(1<<3)
#define ENABLE_SLEEP_MODE				(1<<4)
#define ENABLE_LOW_POWER_MODE			(1<<5)

#define INT_IDENTIF_REG		2	// Read-only
#define INT_PENDING			(1<<0)
#define GET_INT_TYPE(x)		((x>>1)&3)
#define TIMEOUT_INT_PENDING	(1<<4)
#define FIFO_64BYTE_ENABLED	(1<<5)
#define FIFO_WORKING		(1<<6)
#define FIFO_ENABLED		(1<<7)

#define FIFO_CONTROL_REG	2	// Write-only
#define ENABLE_FIFO		(1<<0)
#define CLEAR_IN_FIFO	(1<<1)
#define CLEAR_OUT_FIFO	(1<<2)
#define DMA_MODE_SELECT	(1<<3)
#define FIFO_64BYTE_EN	(1<<5)
#define GET_INT_TRIG_LVL(x) ((x>>6) ? ((x>>6)==3 ? 14 : (x>>6)*2) : 1)
#define GET_INT_TRIG_LVL_64(X) ((x>>6) ? GET_INT_TRIG_LVL(x)*4 : 1)
#define SET_INT_TRIG_LVL(x) (x==1 ? 0 : (x==14 ? (3<<6) : (x<<5)))
#define SET_INT_TRIG_LVL64(x) (x==1 ? 0 : SET_INT_TRIG_LVL((x/4)))

#define LINE_CTRL_REG		3
#define GET_WORD_LENGTH(x)	((x&3)+5)
#define SET_WORD_LENGTH(x)	(x-5)
#define LONG_STOP_BIT		(1<<2)
#define PARITY				((1<<3)|(1<<4)|(1<<5))
#define SET_BREAK_EN		(1<<6)
#define ENABLE_DLAB			(1<<7)

#define MODEM_CTRL_REG		4
#define DATA_TERMINAL_READY	(1<<0)
#define REQUEST_TO_SEND		(1<<1)
#define AUX_OUT1			(1<<2)
#define AUX_OUT2			(1<<3)
#define LOOPBACK_MODE		(1<<4)
#define AUTOFLOW_CONTROL	(1<<5)

#define LINE_STATUS_REG		5	// Read-only
#define DATA_READY		(1<<0)
#define OVERRUN_ERROR	(1<<1)
#define PARITY_ERROR	(1<<2)
#define FRAMING_EROR	(1<<3)
#define BREAK_INT		(1<<4)
#define EMPTY_OUT_HOLDING_REGS	(1<<5)
#define EMPY_IN_HOLDING_REGS	(1<<6)
#define FIFO_IN_EROR	(1<<7)

#define MODEM_STATUS_REG	6	// Read-only
#define DELTA_CLEAR_TO_SEND			(1<<0)
#define DELTA_DATA_SET_READY		(1<<1)
#define TRAILING_EDGE_RING_INDIC	(1<<2)
#define DELTA_DATA_CARRIER_DETECT	(1<<3)
#define CLEAR_TO_SEND				(1<<4)
#define DATA_SET_READY				(1<<5)
#define RING_INDICATOR				(1<<6)
#define CARRIER_DETECT				(1<<7)

#define SCRATCH_REG			7

#define MODEM_STATUS_INT	0
#define IN_HOLDING_REG_INT	1
#define DATA_AVAILABLE_INT	2
#define IN_LINE_STATUS_INT	3

uint16_t COM[] = {0x3F8, 0x2F8, 0x3E8, 0x2E8};

void serial_handler(regs_t* r)
{
	if(r->int_no == 32+4)
	{
		if(GET_INT_TYPE(inb(COM[1-1] + INT_IDENTIF_REG)) == DATA_AVAILABLE_INT)
			read_serial_nowait(1);
		if(GET_INT_TYPE(inb(COM[1-1] + INT_IDENTIF_REG)) == DATA_AVAILABLE_INT)
			read_serial_nowait(3);
	}
	else
	{
		if(GET_INT_TYPE(inb(COM[2-1] + INT_IDENTIF_REG)) == DATA_AVAILABLE_INT)
			read_serial_nowait(2);
		if(GET_INT_TYPE(inb(COM[4-1] + INT_IDENTIF_REG)) == DATA_AVAILABLE_INT)
			read_serial_nowait(4);
	}
}

uint8_t serial_received(uint8_t com)
{
	return inb(COM[com-1] + LINE_STATUS_REG) & DATA_READY;
}

uint8_t read_serial_nowait(uint8_t com)
{
	return inb(COM[com-1] + DATA_REG);
}

uint8_t read_serial(uint8_t com)
{
	while(!serial_received(com));
	return inb(COM[com-1] + DATA_REG);
}

void write_serial(uint8_t com, char c)
{
	while(inb(COM[com-1]+LINE_STATUS_REG) & DATA_READY);
	outb(COM[com-1]+DATA_REG, c);
}

void put_serial(uint8_t com, char* s)
{
	char* p = s;
	while(*p != '\0')
		write_serial(com, *(p++));
}

void serial_init(uint8_t com)
{
	const uint16_t baud_rate_divisor = 0x3;
	*(uint64_t*)COM = *(uint64_t*)&bda->com1;

	outb(COM[com-1] + INT_ENABLE_REG, INT_EN_DATA_AVAILABLE|INT_EN_OUT_HOLDING_REG_EMPTY|INT_EN_MODEM_STATUS_CHANGE);
	outb(COM[com-1] + LINE_CTRL_REG, ENABLE_DLAB);
	outb(COM[com-1] + DIV_LATCH_LO_REG, baud_rate_divisor&0xFF);
	outb(COM[com-1] + DIV_LATCH_HI_REG, baud_rate_divisor>>8);
	outb(COM[com-1] + LINE_CTRL_REG, SET_WORD_LENGTH(8));
	outb(COM[com-1] + FIFO_CONTROL_REG, SET_INT_TRIG_LVL(14)|CLEAR_OUT_FIFO|CLEAR_IN_FIFO|ENABLE_FIFO);
	outb(COM[com-1] + MODEM_CTRL_REG, DATA_TERMINAL_READY|REQUEST_TO_SEND|AUX_OUT2);

	put_serial(com, "COM");
	write_serial(com, '0'+com);
	put_serial(com, " open\n");

	irq_install_handler(3, serial_handler);
	irq_install_handler(4, serial_handler);
	unmask_irq(3);
	unmask_irq(4);
}