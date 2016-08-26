#include "video/video.h"
#include "video/vga.h"
#include <stdio.h>

video_device_t _video_device;
video_device_t* video_device = &_video_device;

void video_init()
{
	video_device = vgat_init(VGA_DEFAULT_FB, 80, 25);
}

//TODO: Check errors
void vbe_init(vbe_controller_t* controller, vbe_mode_t* mode)
{
	video_device = vgag_init(controller, mode);
}


int puts(const char* s)
{
	char* ptr = (char*)s;
	while(*ptr)
		putchar(*(ptr++));
	return ptr-s;
}

#define BS '\b'
#define TAB '\t'
#define LF '\n'
#define CR '\r'

int putchar(int c)
{
	if (video_device->col*video_device->charcell_w >= video_device->width && c != LF)
	{
		video_device->col = 0;
		video_device->row++;
	}
	if (video_device->row*video_device->charcell_h >= video_device->height && video_device->scroll)
		video_device->scroll(1);

	switch(c)
	{
		case BS: video_device->putchar(--video_device->col*video_device->charcell_w, video_device->row*video_device->charcell_h, ' ', video_device->color);
			break;
		case TAB:
			do
			{
				putchar(' ');
			}
			while(video_device->col%TAB_SIZE);
			break;
		case LF:
			if (++video_device->row == video_device->height)
				video_device->scroll(1);
		case CR:
			video_device->col = 0;
			break;
		default:
			video_device->putchar(video_device->col*video_device->charcell_w, video_device->row*video_device->charcell_h, c, video_device->color);
			video_device->col++;
	}

	return c;
}