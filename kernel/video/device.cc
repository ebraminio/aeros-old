#include "video/device.hpp"
#include <cstring>

#define TAB_SIZE 4
#define BS '\b'
#define TAB '\t'
#define LF '\n'
#define CR '\r'

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

Video_device::Video_device(void* buffer, size_t width, size_t height, \
	uint8_t depth, uint8_t cell_w, uint8_t cell_h, bool graphic) : \
	_width(width), _height(height), _depth(depth), _cell_w(cell_w), \
	_cell_h(cell_h), _graphic(graphic), _row(0), _col(0), _buffer(buffer), \
	size(width*height*depth)
{
	if(!graphic)
		_color = COLOR_LIGHT_GREY | COLOR_BLACK<<4;
	else _color = 0xFFFFFF;

	clear();
}

void Video_device::putchar(int c)
{
	this->putchar(c, _color);
}

void Video_device::putchar(int c, uint32_t color)
{
	if (_col*_cell_w >= _width && c != LF)
	{
		_col = 0;
		_row++;
	}
	if (_row*_cell_h >= _height)
		scroll(1);

	switch(c)
	{
		case BS: writechar(--_col*_cell_w, _row*_cell_h, ' ', color);
			break;
		case TAB:
			do this->putchar(' ', color);
			while(_col%TAB_SIZE);
			break;
		case LF:
			if (++_row == _height)
				scroll(1);
		case CR: _col = 0;
			break;
		default:
			writechar(_col*_cell_w, _row*_cell_h, c, color);
			_col++;
	}

}

void Video_device::writechar(size_t x, size_t y, int c, uint32_t color)
{
	if(_graphic)
		for(size_t j=0; j<_cell_h; j++)
			for(uint8_t i=0; i<8; i++)
			{
				if(font[c*_cell_h+j] & (1<<(7-i)))
					buf32[_width*(y+j)+x+i] = color;
				else buf32[_width*(y+j)+x+i] = 0;
			}
	else buf16[y * _width + x] = (uint16_t)c | ((uint16_t)color<<8);
}

void Video_device::scroll(uint32_t n)
{
	const size_t row_size = _graphic ? _width*_cell_w*_cell_h : _width * 2;
	const uint32_t offset = row_size * n;

	for(uint8_t* ptr = buf8; ptr+offset<buf8+size; ptr += offset)
		memcpy(ptr, ptr+offset, row_size);
	for(uint32_t y=_height-n; y<_height; y++)
		for(uint32_t x=0; x<_width; x++)
			writechar(x, y, ' ', _color);

	_row -= n;
}

void Video_device::clear(void)
{
	clear(_color);
}

void Video_device::clear(uint32_t color)
{
	if(!_graphic)
	{
		uint16_t* ptr = buf16;
		while(ptr < (uint16_t*)(buf8+size))
			*(ptr++) = ' ' | (uint16_t)color<<8;
	}
	else memset(_buffer, 0, size);

	_row = _col = 0;
}

void Video_device::fgcolor(uint32_t color)
{
	_color = ((uint8_t)color&0xF) | (_color&0xF0);
}

void Video_device::bgcolor(uint32_t color)
{
	_color = ((uint8_t)_color&0xF) | (((uint8_t)color&0xF)<<4);
}