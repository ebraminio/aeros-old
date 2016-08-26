#ifndef SEARCH_H
#define SEARCH_H

#include <sys/types.h>

typedef struct entry
{
	char* key;
	void* data;
} ENTRY;

enum
{
	FIND,
	ENTER,
} ACTION;

enum
{
	preorder,
	postorder,
	endorder,
	leaf,
} VISIT;

#endif
