#include "devices/pc_speaker.h"
#include "devices/pit.h"
#include <sys/io.h>

#define PIT_CHANNEL 2

void play_sound(uint32_t freq, uint16_t duration)
{
	enable_sound(true);
	set_pit_freq(PIT_CHANNEL, freq);
	pit_wait(duration);
	enable_sound(false);
}

void enable_sound(bool enable)
{
	outb(0x61, enable ? inb(0x61)|3 : inb(0x61)&~3);
}