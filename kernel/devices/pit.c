#include "devices/pit.h"
#include <sys/io.h>
#include "io/log.h"
#include "devices/pic.h"

#define PIT_DATA0	0x40
#define PIT_DATA1	0x41
#define PIT_DATA2	0x42
#define PIT_CMD		0x43
#define PIT_BASE_FREQ 1193180

void set_pit_freq(uint16_t freq)
{
	uint16_t divisor = PIT_BASE_FREQ/freq;
	outb(PIT_CMD, 0x36);
	outb(PIT_DATA0, divisor & 0xFF);
	outb(PIT_DATA0, divisor >> 8);
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
	set_pit_freq(1000);
	install_irq_handler(0, pit_handler);
	unmask_irq(0);
}