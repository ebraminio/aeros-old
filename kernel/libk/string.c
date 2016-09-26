#include <string.h>
#include <stdint.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
	for(size_t i=0; i<n; i++)
		if(((unsigned char*)s1)[i] != ((unsigned char*)s2)[i])
			return ((unsigned char*)s2)[i] - ((unsigned char*)s1)[i];
	return 0;
}

void* memset(void* s, int c, size_t n)
{
	uint64_t* s64 = (uint64_t*)s;
	unsigned char c8 = (unsigned char)c;
	uint64_t c64 = c8|(c8|(c8|(c8|(c8|(c8|(c8|(c8<<8)<<8)<<8)<<8)<<8)<<8)<<8);
	size_t i=0;

	for(; i<n/8; i++)
		s64[i] = c64;
	for(i*=8; i<n; i++)
		((uint8_t*)s)[i] = c8;
	
	return s;
}

void* memcpy(void* dest, const void* src, size_t n)
{
	uint64_t* d = (uint64_t*)dest;
	const uint64_t* s = (const uint64_t*)src;
	size_t i=0;

	for(; i < n/8; i++)
		d[i] = s[i];
	for(i*=8; i<n; i++)
		((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
	
	return (void*)src;
}