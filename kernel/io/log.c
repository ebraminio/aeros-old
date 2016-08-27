#include "io/log.h"
#include <stdio.h>
#include <stdarg.h>

static const char* sizes[] = {"B", "KB", "MB", "GB"};

void print_size(uint32_t size)
{
	uint8_t i = 0;
	uint32_t c_size = size;
	while(i<4 && c_size > 1024)
	{
		i++;
		c_size /= 1024;
	}

	printf("%d%s", c_size, sizes[i]);
}

 __attribute__ ((noreturn))
void panic(const char* format, ...)
{
	va_list argp;

	printf(" [ERR ]\t");
	va_start(argp, format);
	vprintf(format, argp);
	va_end(argp);
	for(;;);
}

void nopanic(const char* format, ...)
{
	va_list argp;

	printf(" [ OK ]\t");
	va_start(argp, format);
	vprintf(format, argp);
	va_end(argp);
}