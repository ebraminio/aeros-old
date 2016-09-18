#ifndef __IO_PS2_DEVICE_H__
#define __IO_PS2_DEVICE_H__

#include <stdint.h>

#define PS2_WAIT_INPUT		0
#define PS2_WAIT_SPECIAL	(1<<0)
#define PS2_WAIT_DATA		(1<<1)

typedef struct
{
	uint8_t port2 : 1;
	uint8_t state : 2;
} ps2_device_t;

#endif
