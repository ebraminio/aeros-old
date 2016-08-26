#ifndef PWD_H
#define PWD_H

#include <sys/types.h>

typedef struct passwd
{
    char* pw_name;
    uid_t pw_uid;
    gid_t pw_gid;
    char* pw_dir;
    char* pw_shell;
}; 

struct passwd* getpwnam(const char*);
int getpwnam_r(const char*, struct passwd*, char*, size_t, struct passwd**);
struct passwd *getpwuid(uid_t);
int getpwuid_r(uid_t, struct passwd*, char*, size_t, struct passwd**);

#endif
