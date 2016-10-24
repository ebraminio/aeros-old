#include <unistd.h>

const void* heap_start = &_end;
const void* heap_end = &_end;

void* sbrk(intptr_t increment)
{
	const void* previous_heap_end = heap_end;

	if(brk((uint8_t*)heap_end+increment) != 0)
		return (void*)-1;
	return (void*)previous_heap_end;
}
