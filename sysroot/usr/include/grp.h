#ifndef GRP_H
#define GRP_H

#include <sys/types.h>

typedef struct group
{
	char* gr_name;
	gid_t gr_gid;
	char** gr_mem;
};

#endif
