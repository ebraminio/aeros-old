#include "video/vga.h"
#include "video/video.h"
#include <stddef.h>

static void vgat_putchar(uint32_t x, uint32_t y, int c, uint32_t color);
static void vgat_scroll(uint32_t n);
static void vgat_fgcolor(uint32_t color);
static void vgat_bgcolor(uint32_t color);
static void vgat_clear(uint32_t color);

video_device_t* vgat_init(void *text_buffer, uint16_t width, uint16_t height)
{

	video_device->depth = 8,
	video_device->graphic_supported = 0,
	video_device->putchar = vgat_putchar,
	video_device->scroll = vgat_scroll,
	video_device->clear = vgat_clear,
	video_device->set_fgcolor = vgat_fgcolor,
	video_device->set_bgcolor = vgat_bgcolor,
	video_device->width = width;
	video_device->height = height;
	video_device->charcell_w = 1;
	video_device->charcell_h = 1;
	video_device->buffer = text_buffer;
	video_device->buffer_end = (uint8_t*)text_buffer + width*height * sizeof(uint16_t);
	video_device->color = COLOR_LIGHT_GREY | COLOR_BLACK<<4;
	vgat_clear(video_device->color);
	return video_device;
}

static void vgat_putchar(uint32_t x, uint32_t y, int c, uint32_t color)
{
	((uint16_t*)video_device->buffer)[y * video_device->width + x] = (uint16_t)c | ((uint16_t)color<<8);
}

static void vgat_scroll(uint32_t n)
{
	video_device->row -= n;
	uint16_t *ptr = (uint16_t*)video_device->buffer + video_device->width*n;
	uint32_t offset = video_device->width * n;
	while(ptr+offset < (uint16_t*)video_device->buffer_end)
	{
		*ptr = *(ptr+offset);
		ptr++;
	}
}

static void vgat_clear(uint32_t color)
{
	uint16_t* ptr = (uint16_t*)video_device->buffer;
	while(ptr < (uint16_t*)video_device->buffer_end)
		*(ptr++) = ' ' | (uint16_t)color<<8;
	video_device->row = 0;
	video_device->col = 0;
}

static void vgat_fgcolor(uint32_t color)
{
	video_device->color = ((uint8_t)color&0xF) | (video_device->color&0xF0);
}

static void vgat_bgcolor(uint32_t color)
{
	video_device->color = ((uint8_t)video_device->color&0xF) | (((uint8_t)color&0xF)<<4);
}