#ifndef SYS_IO_H
#define SYS_IO_H

#include <stdint.h>

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %%dx, %%al":"=a"(ret):"d"(port));
	return ret;
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t ret;
	asm volatile("inw %%dx, %%ax":"=a"(ret):"d"(port));
	return ret;
}

static inline uint32_t inl(uint16_t port)
{
	uint32_t ret;
	asm volatile("inl %%dx, %%eax":"=a"(ret):"d"(port));
	return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
	asm volatile("outb %%al, %%dx"::"d"(port), "a"(val));
}

static inline void outw(uint16_t port, uint16_t val)
{
	asm volatile("outw %%ax, %%dx"::"d"(port), "a"(val));
}

static inline void outl(uint16_t port, uint32_t val)
{
	asm volatile("outl %%eax, %%dx"::"d"(port), "a"(val));
}

#endif
