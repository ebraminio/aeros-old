#include "devices/pc_speaker.h"
#include "devices/pit.h"
#include <sys/io.h>

#define PIT_CHANNEL 2

void play_sound(uint32_t freq)
{
	set_pit_freq(PIT_CHANNEL, freq);
}

void shutup(void)
{
	outb(0x61, inb(0x61)&~3);
}

void pc_speaker_init()
{
	outb(0x61, inb(0x61)|3);
	shutup();
}