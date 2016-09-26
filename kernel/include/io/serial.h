#ifndef __IO_SERIAL_H__
#define __IO_SERIAL_H__

#include <stdint.h>

void serial_init(uint8_t com);
uint8_t serial_received(uint8_t com);
uint8_t read_serial(uint8_t com);
void write_serial(uint8_t com, char c);
void put_serial(uint8_t com, char* s);
uint8_t read_serial_nowait(uint8_t com);

#endif