#ifndef __DEVICES_PIT_H__
#define __DEVICES_PIT_H__

#include <stdint.h>

void pit_init(void);
void set_pit_freq(uint8_t channel, uint16_t freq);
void pit_wait(uint16_t millis);

#endif