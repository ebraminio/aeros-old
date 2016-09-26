#ifndef SYS_IPC_H
#define SYS_IPC_H

#include <sys/types.h>

typedef struct ipc_perm
{
	uid_t uid;
	gid_t gid;
	uit_t cuid;
	gid_t cgid;
	mode_t mode;
};

key_t ftok(const char* path, int id);

#endif