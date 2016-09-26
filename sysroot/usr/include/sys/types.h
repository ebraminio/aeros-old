#ifndef SYS_TYPES_H
#define SYS_TYPES_H

#include <stddef.h>

typedef int pid_t;
typedef int time_t;
typedef time_t timer_t;
typedef unsigned clock_t;
typedef clock_t clockid_t;
typedef int dev_t;
typedef int mode_t;
typedef int uid_t;
typedef int gid_t;
typedef int blkcnt_t;
typedef int off_t;
typedef int blksize_t;
typedef unsigned ino_t;
typedef int nlink_t;

typedef long signed int ssize_t;

typedef long long unsigned useconds_t;
typedef long long signed suseconds_t;

typedef void (*func_t)();
typedef int (*main_func_t)(int argc, char* argv[], char* const envp[]);

#endif