#include "devices/pit.h"
#include <sys/io.h>
#include "io/log.h"
#include "devices/pic.h"

#define PIT_DATA0	0x40
#define PIT_DATA1	0x41
#define PIT_DATA2	0x42
#define PIT_CMD		0x43
#define PIT_BASE_FREQ 1193180

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

void set_pit_freq(uint8_t channel, uint16_t freq)
{
	if(channel > 2)
		panic("There is only 3 PIT channels");

	pit_cmd_t cmd = {.bcd=0, .op_mode = OPMODE_SQUARE_WAVE, .acc_mode_lobyte = 1, .acc_mode_hibyte = 1, .channel = channel};
	outb(PIT_CMD, cmd.numeric);

	uint16_t divisor = PIT_BASE_FREQ/freq;
	outb(PIT_DATA0+channel, divisor & 0xFF);
	outb(PIT_DATA0+channel, divisor >> 8);
}

static uint32_t timer_tick = 0;
static uint16_t timer_subtick = 0;	// Currently millitick

void pit_handler(regs_t* regs)
{
	if(++timer_subtick == 1000)
	{
		timer_subtick = 0;
		timer_tick++;
	}
	if(timer_tick==0xFFFFFFFF)
		panic("Max PIT tick reached");
}

void pit_wait(uint16_t millis)
{
	uint16_t target_subtick = (timer_subtick + millis)%1000;
	uint32_t target_tick = timer_tick + (timer_subtick+millis)/1000;

	while(timer_tick < target_tick);
	while(timer_subtick < target_subtick);
}

void pit_init(void)
{
	set_pit_freq(0, 1000);
	install_irq_handler(0, pit_handler);
	unmask_irq(0);
}