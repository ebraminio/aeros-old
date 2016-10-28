#include <string.h>
#include <stdint.h>

size_t strlen(const char* str)
{
	char* p = (char*)str;
	while(*p) p++;
	return p-str;
}