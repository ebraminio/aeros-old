#ifndef __VIDEO_DEVICE_H__
#define __VIDEO_DEVICE_H__

#include <stdint.h>

typedef struct
{
	uint32_t width, height, depth, row, col;
	uint8_t *buffer, *buffer_end;
	uint8_t charcell_w, charcell_h;
	uint8_t graphic_supported;
	uint32_t color;
	uint8_t* font;

	void (*putchar)(uint32_t x, uint32_t y, int c, uint32_t color);
	void (*scroll)(uint32_t n);
	void (*clear)(uint32_t color);
	void (*move_cursor)(uint32_t x, uint32_t y);
	void (*hide_cursor)(void);
	void (*set_fgcolor)(uint32_t color);
	void (*set_bgcolor)(uint32_t color);
} video_device_t;

#endif