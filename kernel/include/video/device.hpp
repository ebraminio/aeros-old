#ifndef __VIDEO_DEVICE_HPP__
#define __VIDEO_DEVICE_HPP__

#include "video/vga.hpp"
#include "video/vbe.hpp"
#include <cstddef>

class Video_device
{
	public:
		Video_device(void* buffer=VGA_DEFAULT_FB, size_t width=80, \
			size_t height=25, uint8_t depth=8, uint8_t cell_w=1, \
			uint8_t cell_h=1, bool graphic=false);
		void putchar(int c);
		void putchar(int c, uint32_t color);
		void scroll(uint32_t n);
		void clear(void);
		void clear(uint32_t color);
		void move_cursor(uint32_t x, uint32_t y);
		void hide_cursor(void);
		void set_fgcolor(uint32_t color);
		void set_bgcolor(uint32_t color);

	private:
		size_t _width, _height;
		uint8_t _depth, _cell_w, _cell_h;
		bool _graphic;
		size_t _row, _col;
		uint32_t _color;
		union
		{
			void* _buffer;
			uint8_t* buf8;
			uint16_t* buf16;
			uint32_t* buf32;
			uint64_t* buf64;
		};
		size_t size;
		uint8_t* font;

		void fgcolor(uint32_t color);
		void bgcolor(uint32_t color);
		void writechar(size_t x, size_t y, int c, uint32_t color);

		friend void vbe_init(vbe_controller_t*, vbe_mode_t*);
};

#endif