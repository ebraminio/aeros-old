#include "video/vga.h"
#include "video/video.h"
#include <stddef.h>
#include "video/vga_fonts.h"
#include <string.h>

typedef union
{
	struct
	{
		uint8_t b;
		uint8_t g;
		uint8_t r;
		uint8_t unused;
	};
	uint32_t numeric;
} bgra_t;

static void vgag_putchar(uint32_t x, uint32_t y, int c, uint32_t color);
static void vgag_scroll(uint32_t n);
static void vgag_clear(uint32_t color);

video_device_t* vgag_init( vbe_controller_t* controller, vbe_mode_t* mode)
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

	video_device->color = 0xFFFFFF;
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
		{
			if(font[c*video_device->charcell_h+j] & (1<<(7-i)))
				((bgra_t*)video_device->buffer)[video_device->width*(y+j)+x+i].numeric = color;
			else ((bgra_t*)video_device->buffer)[video_device->width*(y+j)+x+i].numeric = 0;
		}
}

static void vgag_scroll(uint32_t n)
{
	video_device->row -= n;

	const size_t row_size = video_device->width * video_device->charcell_w * video_device->charcell_h;
	const size_t offset = row_size * n;

	for(uint8_t* p = video_device->buffer; p<video_device->buffer_end - offset; p += offset)
		memcpy(p, p+offset, row_size);
}

static void vgag_clear(uint32_t color)
{
	// No color option for now
	memset(video_device->buffer, 0, video_device->buffer_end-video_device->buffer);
	video_device->row = 0;
	video_device->col = 0;
}