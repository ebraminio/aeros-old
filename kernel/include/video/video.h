#ifndef __VIDEO_VIDEO_H__
#define __VIDEO_VIDEO_H__

#include <stdint.h>

typedef struct
{
	uint32_t width, height, depth, row, col;
	uint8_t *buffer, *buffer_end;
	uint8_t buffer_cell_size;
	uint8_t graphic_supported;
	uint32_t color;

	void (*init)(void);

	void (*putchar)(uint32_t x, uint32_t y, int c, uint32_t color);
	void (*puts)(uint32_t x,uint32_t y, int c, uint32_t color);
	void (*scroll)(uint32_t n);
	void (*clear)(uint32_t color);
	void (*move_cursor)(uint32_t x, uint32_t y);
	void (*hide_cursor)();
	void (*set_fgcolor)(uint32_t color);
	void (*set_bgcolor)(uint32_t color);
} video_device_t;

void video_init();

#endif