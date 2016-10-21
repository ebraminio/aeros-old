#include "video/video.hpp"
#include "video/device.hpp"
#include "video/vbe.hpp"
#include "video/vga_fonts.h"

const char* const vbe_memory_models[VBE_MAX_MEM_MODEL] =
{
	"text",
	"CGA graphics",
	"Hercules graphics",
	"Planar",
	"Packed",
	"Non-chain 4,256 color",
	"Direct Color",
	"YUV",
};

Video_device _device;

void video_init()
{
	_device = Video_device();
}

extern "C" int putchar(int c);
int putchar(int c)
{
	_device.putchar(c);
	return c;
}

void vbe_init(vbe_controller_t* controller, vbe_mode_t* mode)
{
	if(!(mode->mode_attributes & VBE_MODE_ATTR_LFB_MODE_AVAILABLE))
		return;

	if(controller->major_version <= 1 && (controller->major_version != 1 || \
		controller->minor_version<=2))
		return;

	bool graphic_supported = (mode->mode_attributes & VBE_MODE_ATTR_GRAPHIC_MODE) != 0;
	if(!graphic_supported)
		return;

	if(mode->char_cell_w != 8)
		return;
	if(mode->char_cell_h != 8 && mode->char_cell_h != 16)
		return;

	_device = Video_device((void*)mode->frame_buffer, mode->x_res, mode->y_res, \
			mode->bits_per_pixel, mode->char_cell_w, mode->char_cell_h, \
			graphic_supported);

	if(mode->char_cell_h == 8)
		_device.font = (uint8_t*)g_8x8_font;
	else if(mode->char_cell_h == 16)
		_device.font = (uint8_t*)g_8x16_font;
}