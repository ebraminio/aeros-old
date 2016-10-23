#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>

#define XSTR(s) STR(s)
#define STR(s) #s

#pragma push_macro("syscall")
#undef syscall
long syscall(enum syscall number, ...)
#pragma pop_macro("syscall")
{
	va_list argp;
	va_start(argp, number);
	uint32_t b = va_arg(argp, uint32_t);
	uint32_t c = va_arg(argp, uint32_t);
	uint32_t d = va_arg(argp, uint32_t);
	va_end(argp);

	uint32_t ret;
	asm("int $"XSTR(SYSCALL_IRQ)
		:"=a"(ret)
		:"a"(number),
		"b"(b),
		"c"(c),
		"d"(d));
	return ret;
}

int brk(void* addr)
{
	return (int)syscall(SYS_BRK, addr);
}

