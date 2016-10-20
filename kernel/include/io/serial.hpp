#ifndef __IO_SERIAL_HPP__
#define __IO_SERIAL_HPP__

#include <cstdint>
//#include "io/device.hpp"

class Serial// : IO_device
{
	public:
		Serial(uint8_t num);
//		~Serial(void);
		void open(void);
		void close(void);
		uint8_t read(void);
		uint8_t read_nowait(void) const;
		void write(uint8_t c);
		void put(char const* s);

	private:
		uint8_t _num;
		uint16_t _iobase;
};

uint8_t serial_received(uint8_t com);

#endif