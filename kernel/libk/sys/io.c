#include <sys/io.h>

uint8_t inb(uint16_t port)
{
	unsigned char ret;
	asm volatile("inb %%dx, %%al":"=a"(ret):"d"(port));
	return ret;
}

uint16_t inw(uint16_t port)
{
	unsigned short int ret;
	asm volatile("inw %%dx, %%ax":"=a"(ret):"d"(port));
	return ret;
}

uint32_t inl(uint16_t port)
{
	unsigned int ret;
	asm volatile("inl %%dx, %%eax":"=a"(ret):"d"(port));
	return ret;
}

void outb(uint16_t port, uint8_t val)
{
	asm volatile("outb %%al, %%dx": :"d" (port), "a" (val));
}

void outw(uint16_t port, uint16_t val)
{
	asm volatile("outw %%ax, %%dx": :"d" (port), "a" (val));
}

void outl(uint16_t port, uint32_t val)
{
	asm volatile("outl %%eax, %%dx": :"d" (port), "a" (val));
}