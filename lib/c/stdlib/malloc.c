#include <stdlib.h>
#include <unistd.h>

#define HEAP_MAGIC 0x434F4C4D
#define PAGE_SIZE 4096

#define HBLK_FIT_IN_PAGE(b,s,p) ((uint8_t*)b+sizeof(heap_block_t)+s<=(uint8_t*)p+PAGE_SIZE)
#define NEXT_HBLK(b) (heap_blk_t*)((uint8_t*)b+sizeof(*b)+b->size)

typedef struct heap_blk
{
	uint32_t magic;
	size_t size;
	uint8_t used;
	struct heap_blk* previous;
} heap_blk_t;

void* malloc(size_t size)
{
	heap_blk_t* previous = NULL;
	heap_blk_t* heap_blk = (heap_blk_t*)heap_start;
	while(1)
	{
		if((uint8_t*)heap_blk+sizeof(*heap_blk)+size > (uint8_t*)heap_end)
			sbrk(size + (size%PAGE_SIZE ? PAGE_SIZE-(size%PAGE_SIZE) : 0));
		if(heap_blk->magic != HEAP_MAGIC)
		{
			heap_blk->magic = HEAP_MAGIC;
			heap_blk->size = size;
			heap_blk->used = 0;
			heap_blk->previous = previous;
		}
		if(heap_blk->used || heap_blk->size < size)
		{
			previous = heap_blk;
			heap_blk = NEXT_HBLK(heap_blk);
			continue;
		}

		if(heap_blk->size > size+sizeof(heap_blk_t*))
		{
			size_t oldsize = heap_blk->size;
			heap_blk_t* oldnext = NEXT_HBLK(heap_blk);

			heap_blk->size=size;
			heap_blk_t* new_blk = NEXT_HBLK(heap_blk);
			new_blk->magic = HEAP_MAGIC;
			new_blk->size = oldsize-size-sizeof(*new_blk);
			new_blk->used = 0;
			new_blk->previous = heap_blk;

			if(oldnext->magic == HEAP_MAGIC)
				oldnext->previous = new_blk;
		}

		heap_blk->used = 1;
		return (void*)((uint8_t*)heap_blk+sizeof(*heap_blk));
	}
}

static void merge_next(heap_blk_t* heap_blk)
{
	heap_blk_t* oldnext = NEXT_HBLK(heap_blk);
	heap_blk->size += oldnext->size + sizeof(*oldnext);

	heap_blk_t* newnext = NEXT_HBLK(oldnext);
	if(newnext->magic == HEAP_MAGIC)
		newnext->previous = heap_blk;
}

void free(void* p)
{
	heap_blk_t* heap_blk = (heap_blk_t*)((uint8_t*)p-sizeof(*heap_blk));
	if(heap_blk->magic != HEAP_MAGIC || heap_blk->used==0)
		return;

	heap_blk->used = 0;

	heap_blk_t* next = NEXT_HBLK(heap_blk);
	if(next->magic==HEAP_MAGIC && next->used == 0)
		merge_next(heap_blk);

	if(heap_blk->previous && heap_blk->previous->used == 0)
		merge_next(heap_blk->previous);
}