#ifndef __DEVICES_8042_H__
#define __DEVICES_8042_H__

#include <stdint.h>

void ps2_init(void);
void ps2_write(uint8_t data);
uint8_t ps2_read(void);
uint8_t ps2_direct_read(void);

#endif