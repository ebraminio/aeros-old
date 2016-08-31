#ifndef __DEVICES_PIT_H__
#define __DEVICES_PIT_H__

#include <stdint.h>

void pit_init(void);
void set_pit_freq(uint16_t hz);

#endif