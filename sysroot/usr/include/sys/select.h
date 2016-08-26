#ifndef SYS_SELECT_H
#define SYS_SELECT_H

#include <sys/types.h>
#include <signal.h>
#include <time.h>

typedef struct timeval
{
	time_t tv_sec;
	suseconds_t tv_usec;
};

#endif