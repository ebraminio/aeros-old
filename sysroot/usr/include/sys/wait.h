#ifndef SYS_WAIT_H
#define SYS_WAIT_H

#include <sys/types.h>

#define WCONTINUED	(1<<0)
#define WNOHANG		(1<<1)
#define WUNTRACED	(1<<2)

pid_t wait(int* stat_loc);
pid_t waitpid(pid_t pid, int* stat_loc, int options);

#endif