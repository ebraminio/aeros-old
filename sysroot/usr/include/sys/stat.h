/**
 * \file
 */

#ifndef SYS_STAT_H
#define SYS_STAT_H
#include <sys/types.h>

struct stat
{
	dev_t st_dev;			/**< ID of device containing file */
	ino_t st_ino;			/**< inode number */
	mode_t st_mode;			/**< protection */
	nlink_t st_nlink;		/**< number of hard links */
	uid_t st_uid;			/**< user ID of owner */
	gid_t st_gid;			/**< group ID of owner */
	dev_t st_rdev;			/**< device ID (if special file) */
	off_t st_size;			/**< total size, in bytes */
	blksize_t st_blksize;	/**< blocksize for file system I/O */
	blkcnt_t st_blocks;		/**< number of 512B blocks allocated */
	time_t st_atime;		/**< time of last access */
	time_t st_mtime;		/**< time of last modification */
	time_t st_ctime;		/**< time of last status change */
};

#define S_IFCHR		((mode_t)0020000)	/**< Character special. */
#define S_IFBLK		((mode_t)0060000)	/**< Block special. */
#define S_IFDIR		((mode_t)0040000)	/**< Directory. */
#define S_IFREG		((mode_t)0100000)	/**< Regular. */
#define S_IFLNK		((mode_t)0120000)	/**< Symbolic link. */
#define S_IFSOCK	((mode_t)0140000)	/**< Socket. */

#define S_ISBLK(m) (m==S_IFBLK)
#define S_ISCHR(m) (m==S_IFCHR)
#define S_ISDIR(m) (m==S_IFDIR)
#define S_ISREG(m) (m==S_IFREG)
#define S_ISLNK(m) (m==S_IFLNK)
#define S_ISSOCK(m) (m==S_IFSOCK)

int chmod(const char* , mode_t);
int fchmod(int, mode_t);
int stat(const char* path, struct stat* buff);
int fstat(int fd, struct stat* buf);
int lstat(const char* path, struct stat* buf);
int mkdir(const char* , mode_t);
int mkfifo(const char* , mode_t);
int mknod(const char* , mode_t, dev_t);
mode_t umask(mode_t);

#endif