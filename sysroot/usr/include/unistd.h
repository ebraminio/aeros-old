#ifndef UNISTD_H
#define UNISTD_H

#include <sys/types.h>
#include <stdint.h>
#include <sys/syscall.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 3

extern char **environ;

#if __STDC_HOSTED__ == 1
long syscall(enum syscall number, ...);
#define syscall(...) syscall(__VA_ARGS__, 0, 0, 0, 0)
#endif

int access(const char* path, int amode);
unsigned alarm(unsigned seconds);
int chdir(const char* path);
int chown(const char* path, uid_t owner, gid_t group);
int close(int filedes);
int dup(int filedes);
int dup2(int filedes, int filedes2);
void _exit(int status);
int execl(const char *path, const char *arg0, ... /*, (char *)0 */);
int execle(const char *path, const char *arg0, ... /*, (char *)0, char *const envp[]*/);
int execlp(const char *file, const char *arg0, ... /*, (char *)0 */);
int execv(const char *path, char *const argv[]);
int execve(const char *path, char *const argv[], char *const envp[]);
int execvp(const char *file, char *const argv[]);
int faccessat(int fd, const char* path, int amode, int flag);
int fchdir(int filedes);
int fchown(int filedes, uid_t owner, gid_t group);
int fexecve(int fd, char *const argv[], char *const envp[]);
pid_t fork(void);
int fsync(int filedes);
int ftruncate(int filedes, off_t length);
char* getcwd(char *buf, size_t size);
gid_t getegid(void);
uid_t geteuid(void);
gid_t getgid(void);
long gethostid(void);
int gethostname(char* name, size_t len);
char* getlogin(void);
pid_t getpid(void);
uid_t getuid(void);
int isatty(int fd);
int lchown(const char* path, uid_t owner, gid_t group);
int link(const char* oldpath, const char* newpath);
off_t lseek(int fd, off_t offset, int whence);
int pause(void);
int pipe(int filedes[2]);
ssize_t read(int fd, void* buf, size_t nbyte);
ssize_t readlink(const char* restrict path, char* restrict buf, size_t bufsize);
int rmdir(const char* path);
int setegid(gid_t);
int seteuid(uid_t);
int setgid(gid_t);
int setuid(uid_t);
unsigned sleep(unsigned);
void swab(const void* restrict, void* restrict, ssize_t);
int symlink(const char*, const char*);
void sync(void);
long sysconf(int name);
int truncate(const char*, off_t);
int unlink(const char *pathname);
ssize_t write(int fd, const void *buf, size_t count);

void* sbrk(intptr_t increment);
int usleep(useconds_t usec);

#endif