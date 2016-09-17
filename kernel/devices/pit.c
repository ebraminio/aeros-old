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

void set_pit_freq(uint16_t hz)
{
	uint16_t divisor = PIT_FREQ/hz;
	outb(PIT_CMD, 0x36);
	outb(PIT_DATA0, divisor & 0xFF);
	outb(PIT_DATA0, divisor >> 8);
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
	set_pit_freq(1000);
	irq_install_handler(0, pit_handler);
	unmask_irq(0);
}
