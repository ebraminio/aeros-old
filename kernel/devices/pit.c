#include "devices/pit.h"
#include <sys/io.h>
#include "io/log.h"
#include "devices/pic.h"

#define PIT_PORT 0x40
#define PIT_DATA0 (PIT_PORT+0)
#define PIT_DATA1 (PIT_PORT+1)
#define PIT_DATA2 (PIT_PORT+2)
#define PIT_CMD (PIT_PORT+3)
#define PIT_FREQ 1193180

#define OPMODE_COUNT_INT	0
#define OPMODE_ONE_SHOT		1
#define OPMODE_RATE_GEN		2
#define OPMODE_SQUARE_WAVE	3
#define OPMODE_SOFT_STROBE	4
#define OPMODE_HARD_STROBE	5
// 6 is also rate generator
// 7 is also square wave generator

typedef union
{
	struct
	{
		uint8_t bcd : 1;
		uint8_t op_mode : 3;
		uint8_t acc_mode_lobyte : 1;
		uint8_t acc_mode_hibyte : 1;
		uint8_t channel : 2;
	};
	uint8_t numeric;
} pit_cmd_t;

void set_pit_freq(uint16_t freq, uint8_t channel)
{
	if(channel > 2)
		panic("There is only 3 PIT channels");
	uint16_t divisor = PIT_FREQ/freq;
	pit_cmd_t cmd = {.bcd=0, .op_mode = OPMODE_SQUARE_WAVE, .acc_mode_lobyte = 1, .acc_mode_hibyte = 1, .channel = channel};
	outb(PIT_CMD, cmd.numeric);
	outb(PIT_DATA0+channel, divisor & 0xFF);
	outb(PIT_DATA0+channel, divisor >> 8);
}

uint32_t timer_ticks = 0;
uint8_t timer_subticks = 0;

void pit_handler(regs_t* regs)
{
	if(++timer_subticks == 100)
	{
		timer_ticks++;
		timer_subticks = 0;
	}
	if(timer_ticks==0xFFFFFFFF)
		panic("Max ticks reached");
}

void pit_init(void)
{
	set_pit_freq(1000, 0);
	irq_install_handler(0, pit_handler);
	unmask_irq(0);
}
