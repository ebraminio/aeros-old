#ifndef __IO_LOG_H__
#define __IO_LOG_H__

#include <stdint.h>

void panic(const char* format, ...) __attribute__((format(printf, 1, 2))) __attribute__((noreturn));
void nopanic(const char* format, ...) __attribute__((format(printf, 1, 2)));
void print_size(uint32_t size);

#endif