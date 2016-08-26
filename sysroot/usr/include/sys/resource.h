#ifndef SYS_RESOURCE_H
#define SYS_RESOURCE_H

#include <stdint.h>
#include <sys/time.h>

typedef uint32_t rlim_t;

typedef struct rlimit
{
	struct timeval ru_utime;
	struct timeval ru_stime;
};


int getrlimit(int resource, struct rlimit* rlp);
int setrlimit(int resource, const struct rlimit* rlp);

#endif