#ifndef SYS_MMAN_H
#define SYS_MMAN_H

#define PROT_NONE	0
#define PROT_READ	(1<<0)
#define PROT_WRITE	((1<<1)|PROT_READ)
#define PROT_EXEC	((1<<2)|PROT_READ)

void* mmap(void* addr, size_t len, int prot, int flags, int fildes, off_t off);
int munmap(void* addr, size_t len);

// POSIX.1-2008 (2016 ed.) says :
// 	- The implementation may require that addr be a multiple of the page size as returned by sysconf().
// 	- The behavior of this function is unspecified if the mapping was not established by a call to mmap().
// I says :
// 	- If addr is not a multiple of the page size, it is rounded down
// 	- addr can be any value in the calling process address space
int mprotect(void* addr, size_t len, int prot);

#endif