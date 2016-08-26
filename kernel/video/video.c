#include "video/video.h"
#include "video/vga.h"
#include <stdio.h>

static video_device_t *video_device;

void video_init()
{
	video_device = vga_tinit(VGA_DEFAULT_FB, 80, 25);
	video_device->clear(video_device->color);
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
	if (video_device->col == video_device->width && c != LF)
	{
		video_device->col = 0;
		video_device->row++;
	}
	if (video_device->row == video_device->height && video_device->scroll)
		video_device->scroll(1);

	switch(c)
	{
		case BS: video_device->putchar(--video_device->col, video_device->row, ' ', video_device->color);
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
			video_device->putchar(video_device->col, video_device->row, c, video_device->color);
			video_device->col++;
	}

	return c;
}