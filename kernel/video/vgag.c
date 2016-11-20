#include "video/vga.h"
#include "video/video.h"
#include <stddef.h>
#include "video/vga_fonts.h"
#include <string.h>
#include "mem/vmem.h"

static void vgag_putchar(uint32_t x, uint32_t y, int c, uint32_t color);
static void vgag_scroll(uint32_t n);
static void vgag_clear(uint32_t color);

video_device_t* vgag_init(vbe_controller_t* controller, vbe_mode_t* mode)
{
	if(mode->mode_attributes & VBE_MODE_ATTRIB_LINEAR_FRAME_BUFFER_MODE_AVAILABLE)
		video_device->buffer = (void*)mode->frame_buffer;
	else return NULL;

	if(controller->major_version > 1 || (controller->major_version==1 && controller->minor_version>2))
	{
		video_device->width = mode->x_resolution;
		video_device->height = mode->y_resolution;
		video_device->depth = mode->bits_per_pixel;
		video_device->charcell_w = mode->char_cell_width;
		video_device->charcell_h = mode->char_cell_heigth;
	}
	else return NULL;

	size_t buf_size = video_device->width * video_device->height \
		* (video_device->depth/8);
	video_device->buffer_end = (void*)(mode->frame_buffer+buf_size);
	IDENTITY_MAP(mode->frame_buffer, buf_size, true);

	video_device->graphic_supported = (mode->mode_attributes & VBE_MODE_ATTRIB_GRAPHIC_MODE) != 0;
	if(!video_device->graphic_supported)
		return NULL;

	if(video_device->charcell_w != 8)
		return NULL;

	if(video_device->charcell_h == 8)
		video_device->font = (uint8_t*)g_8x8_font;
	else if(video_device->charcell_h == 16)
		video_device->font = (uint8_t*)g_8x16_font;
	else return NULL;

	video_device->fgcolor = VGAG_WHITE;
	video_device->bgcolor = VGAG_BLACK;
	video_device->row = video_device->col = 0;
	video_device->putchar = vgag_putchar;
	video_device->scroll = vgag_scroll;
	video_device->clear = vgag_clear;
	video_device->move_cursor = NULL;
	video_device->hide_cursor = NULL;
	video_device->set_fgcolor = NULL;
	video_device->set_bgcolor = NULL;

	return video_device;
}

static void vgag_putchar(uint32_t x, uint32_t y, int c, uint32_t color)
{
	uint8_t* font = video_device->font;
	for(size_t j=0; j<video_device->charcell_h; j++)
		for(uint8_t i=0; i<8; i++)
			if(font[c*video_device->charcell_h+j] & (1<<(7-i)))
				((bgra_t*)video_device->buffer)[video_device->width*(y+j)+x+i].numeric = color;
			else ((bgra_t*)video_device->buffer)[video_device->width*(y+j)+x+i].numeric = video_device->bgcolor;
}

static void vgag_scroll(uint32_t n)
{
	video_device->row -= n;

	const size_t row_size = video_device->width * video_device->charcell_w * video_device->charcell_h;
	const size_t offset = row_size * n;
	uint8_t* p = video_device->buffer;

	while(p+offset+row_size<video_device->buffer_end)
	{
		memcpy(p, p+offset, row_size);
		p += offset;
	}
	for(bgra_t* bgra=(bgra_t*)p; (uint8_t*)bgra<video_device->buffer_end; bgra++)
		bgra->numeric = video_device->bgcolor;
}

static void vgag_clear(uint32_t color)
{
	bgra_t* p = (bgra_t*)video_device->buffer;
	while((uint8_t*)p<video_device->buffer_end)
		(p++)->numeric = color;
	video_device->row = 0;
	video_device->col = 0;
}