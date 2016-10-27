#ifndef __VIDEO_VGA_H__
#define __VIDEO_VGA_H__

#include "video/device.h"
#include "video/vbe.h"

#define TAB_SIZE 4

enum vgat_color
{
	VGAT_BLACK = 0,
	VGAT_BLUE = 1,
	VGAT_GREEN = 2,
	VGAT_CYAN = 3,
	VGAT_RED = 4,
	VGAT_MAGENTA = 5,
	VGAT_BROWN = 6,
	VGAT_LIGHT_GREY = 7,
	VGAT_DARK_GREY = 8,
	VGAT_LIGHT_BLUE = 9,
	VGAT_LIGHT_GREEN = 10,
	VGAT_LIGHT_CYAN = 11,
	VGAT_LIGHT_RED = 12,
	VGAT_LIGHT_MAGENTA = 13,
	VGAT_LIGHT_BROWN = 14,
	VGAT_WHITE = 15,
};

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

enum vgag_color
{
	VGAG_BLACK = 0,
	VGAG_BLUE = 0xFF,
	VGAG_GREEN = 0xFF00,
	VGAG_CYAN = VGAG_GREEN|VGAG_BLUE,
	VGAG_RED = 0xFF0000,
	VGAG_MAGENTA = VGAG_BLUE|VGAG_RED,
//	VGAG_BROWN = 6,
//	VGAG_LIGHT_GREY = 7,
//	VGAG_DARK_GREY = 8,
//	VGAG_LIGHT_BLUE = 9,
//	VGAG_LIGHT_GREEN = 10,
//	VGAG_LIGHT_CYAN = 11,
//	VGAG_LIGHT_RED = 12,
//	VGAG_LIGHT_MAGENTA = 13,
//	VGAG_LIGHT_BROWN = 14,
	VGAG_WHITE = VGAG_RED|VGAG_GREEN|VGAG_BLUE,
};

#define VGA_DEFAULT_FB ((void*)0xB8000)

video_device_t* vgat_init(void *text_buffer, uint16_t width, uint16_t height);
video_device_t* vgag_init(vbe_controller_t* controller, vbe_mode_t* mode);

#endif