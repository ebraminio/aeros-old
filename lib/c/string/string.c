#include <string.h>
#include <stdint.h>

size_t strlen(const char* str)
{
	char* p = (char*)str;
	while(*p) p++;
	return p-str;
}

int strcmp(const char* s1, const char* s2)
{
	size_t i=0;
	while(s1[i] && s2[i] && s1[i]==s2[i])
		i++;
	return s1[i]-s2[i];
}