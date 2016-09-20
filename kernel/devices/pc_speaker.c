#include "devices/pc_speaker.h"
#include "devices/pit.h"
#include <sys/io.h>

void play_sound(uint32_t freq)
{
	set_pit_freq(freq, 2);
}

void shutup(void)
{
	outb(0x61, inb(0x61)&~3);
}

void delay(void)
{
	for(uint32_t i=0; i<0x8FFFF; i++);
}

const static const uint16_t notes[11] = {277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494};

void pc_speaker_init()
{
	outb(0x61, inb(0x61)|3);
	for(uint8_t i=0; i<11; i++)
	{
		play_sound(notes[i]);
		delay();
	}
	shutup();
}
