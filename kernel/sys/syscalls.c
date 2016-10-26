#include "sys/syscalls.h"
#include <sys/syscall.h>
#include "devices/pic.h"
#include <stddef.h>
#include <errno.h>
#include <unistd.h>

int errno = 0;

typedef uint32_t (*SYSCALL)(regs_t*);

#define SYSCALL_NUM 130
SYSCALL syscalls[SYSCALL_NUM] =
{
	NULL,//read
	NULL,//write
	NULL,//open
	NULL,//close
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,//mmap,
	NULL,//mprotect,
	NULL,//munmap,
	(SYSCALL)brk,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,//pread,
	NULL,//pwrite,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,//mremap,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	(SYSCALL)getpid,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,//exit,
	NULL,//uname,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,//getcwd,
	NULL,//chdir,
	NULL,//fchdir,
	NULL,//rename,
	NULL,//mkdir,
	NULL,//rmdir,
	NULL,//creat,
	NULL,//link,
	NULL,//unlink,
	NULL,//symlink,
	NULL,//readlink,
	NULL,//chmod,
	NULL,//fchmod,
	NULL,//chown,
	NULL,//fchown,
	NULL,//lchown,
	NULL,//umask,
	NULL,//gettimeofday,
	NULL,
	NULL,
	NULL,//sysinfo,
	NULL,
	NULL,
	NULL,//getuid,
	NULL,//syslog,
	NULL,//getgid,
	NULL,//setuid,
	NULL,//setgid,
	NULL,//geteuid,
	NULL,//getegid,
	NULL,//setpgid,
	NULL,//getpgid,
	NULL,//getpgrp,
	NULL,//setsid,
	NULL,//setreuid,
	NULL,//setregid,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

pid_t getpid(void)
{
	return 0;
}

void syscall_handler(regs_t* r)
{
	if(r->eax < SYSCALL_NUM && syscalls[r->eax])
		r->eax = syscalls[r->eax](r);
	else errno = ENOSYS;
}

void syscalls_init(void)
{
	irq_install_handler(SYSCALL_IRQ, syscall_handler);
}