#include "video/vga.h"
#include <stddef.h>

static void vga_putchar(uint32_t x, uint32_t y, int c, uint32_t color);
static void vga_scroll(uint32_t n);
static void vga_fgcolor(uint32_t color);
static void vga_bgcolor(uint32_t color);
static void vga_clear(uint32_t color);

video_device_t vga_device =
{
	.depth = 8,
	.row = 0,
	.col = 0,
	.buffer_cell_size = sizeof(uint16_t),
	.graphic_supported = 0,
	.init = NULL,
	.putchar = vga_putchar,
	.scroll = vga_scroll,
	.clear = vga_clear,
	.set_fgcolor = vga_fgcolor,
	.set_bgcolor = vga_bgcolor,
};

video_device_t *vga_tinit(void *text_buffer, uint16_t width, uint16_t height)
{
	vga_device.width = width;
	vga_device.height = height;
	vga_device.buffer = text_buffer;
	vga_device.buffer_end = (uint8_t*)text_buffer + width*height * vga_device.buffer_cell_size;
	vga_device.color = COLOR_LIGHT_GREY | COLOR_BLACK<<4;
	return &vga_device;
}

static void vga_putchar(uint32_t x, uint32_t y, int c, uint32_t color)
{
	((uint16_t*)vga_device.buffer)[y * vga_device.width + x] = (uint16_t)c | ((uint16_t)color<<8);
}

static void vga_scroll(uint32_t n)
{
	vga_device.row -= n;
	uint16_t *ptr = (uint16_t*)vga_device.buffer + vga_device.width*n;
	uint32_t offset = vga_device.width * n;
	while(ptr+offset < (uint16_t*)vga_device.buffer_end)
	{
		*ptr = *(ptr+offset);
		ptr++;
	}
}

static void vga_clear(uint32_t color)
{
	uint16_t* ptr = (uint16_t*)vga_device.buffer;
	while(ptr < (uint16_t*)vga_device.buffer_end)
		*(ptr++) = ' ' | (uint16_t)color<<8;
	vga_device.row = 0;
	vga_device.col = 0;
}

static void vga_fgcolor(uint32_t color)
{
	vga_device.color = ((uint8_t)color&0xF) | (vga_device.color&0xF0);
}

static void vga_bgcolor(uint32_t color)
{
	vga_device.color = ((uint8_t)vga_device.color&0xF) | (((uint8_t)color&0xF)<<4);
}