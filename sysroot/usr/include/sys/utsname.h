#ifndef SYS_UTSNAME_H
#define SYS_UTSNAME_H

struct utsname
{
    char sysname[20];
    char nodename[255];
    char release[20];
    char version[20];
    char machine[20];
};

extern int uname(struct utsname *buf);

#endif
