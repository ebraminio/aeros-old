#include "video/vga.h"
#include "video/video.h"
#include <stddef.h>
#include <string.h>
#include "mem/vmem.h"

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
	IDENTITY_MAP((uintptr_t)text_buffer, width*height * sizeof(uint16_t), true);
	video_device->fgcolor = VGAT_LIGHT_GREY | VGAT_BLACK<<4;
	vgat_clear(video_device->fgcolor);
	return video_device;
}

static void vgat_putchar(uint32_t x, uint32_t y, int c, uint32_t color)
{
	((uint16_t*)video_device->buffer)[y * video_device->width + x] = (uint16_t)c | ((uint16_t)color<<8);
}

static void vgat_scroll(uint32_t n)
{
	const size_t row_size = video_device->width * 2;
	const uint32_t offset = row_size * n;
	for(uint8_t* ptr = video_device->buffer; ptr<video_device->buffer_end-offset; ptr += offset)
		memcpy(ptr, ptr+offset, row_size);

	for(uint32_t y=video_device->height-n; y<video_device->height; y++)
		for(uint32_t x=0; x<video_device->width; x++)
			vgat_putchar(x, y, ' ', video_device->fgcolor);

	video_device->row -= n;
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
	video_device->fgcolor = ((uint8_t)color&0xF) | (video_device->fgcolor&0xF0);
}

static void vgat_bgcolor(uint32_t color)
{
	video_device->fgcolor = ((uint8_t)video_device->fgcolor&0xF) | (((uint8_t)color&0xF)<<4);
}