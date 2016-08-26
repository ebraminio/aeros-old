/**
 *  \file
 */

#ifndef FCNTL_H
#define FCNTL_H

#include <stdio.h>
#include <sys/types.h>

/**
 *  \brief For the cmd argument used by fcntl()
 */
#define F_DUPFD			0	/**< Duplicate file descriptor. */
#define F_DUPFD_CLOEXEC	1	/**< Duplicate file descriptor with the close-on- exec flag FD_CLOEXEC set. */
#define F_GETFD			2	/**< Get file descriptor flags. */
#define F_SETFD			3	/**< Set file descriptor flags. */
#define F_GETFL			4	/**< Get file status flags and file access modes. */
#define F_SETFL			5	/**< Set file status flags. */
#define F_GETLK			6	/**< Get record locking information. */
#define F_SETLK			7	/**< Set record locking information. */
#define F_SETLKW		8	/**< Set record locking information; wait if blocked. */
#define F_GETOWN		9	/**< Get process or process group ID to receive SIGURG signals. */
#define F_SETOWN		10	/**< Set process or process group ID to receive SIGURG signals */

/**
 *  \brief For the fcntl() file descriptor flags
 */
#define FD_CLOEXEC	1	/**< Close the file descriptor upon execution of an exec family function. */

/**
 *  \brief For the l_type argument used for record locking with fcntl()
 */
#define F_RDLCK	0	/**< Shared or read lock. */
#define F_UNLCK	1	/**< Unlock. */
#define F_WRLCK	2	/**< Exclusive or write lock. */

/**
 *  \brief File creation flags for use in the oflag value to open() and openat()
 */
#define O_CLOEXEC	(1<<0)	/**< The FD_CLOEXEC flag associated with the new descriptor shall be set to close the file descriptor upon execution of an exec family function. */
#define O_CREAT		(1<<1)	/**< Create file if it does not exist. */
#define O_DIRECTORY	(1<<2)	/**< Fail if not a directory. */
#define O_EXCL		(1<<3)	/**< Exclusive use flag. */
#define O_NOCTTY	(1<<4)	/**< Do not assign controlling terminal. */
#define O_NOFOLLOW	(1<<5)	/**< Do not follow symbolic links. */
#define O_TRUNC		(1<<6)	/**< Truncate flag. */
#define O_TTY_INIT	(1<<7)	/**< Set the \b termios structure terminal parameters to a state that provides conforming behavior */

/**
 *  \brief For use as file status flags for open(), openat(), and fcntl()
 */
#define O_APPEND	(1<<8)	/**< Set append mode. */
#define O_DSYNC		(1<<9)	/**< Write according to synchronized I/O data integrity completion. */
#define O_NONBLOCK	(1<<10)	/**< Non-blocking mode. */
#define O_RSYNC		(1<<11)	/**< Synchronized read I/O operations. */
#define O_SYNC		(1<<12)	/**< Write according to synchronized I/O file integrity completion. */

#define O_EXEC		(1<<13)	/**< Open for execute only (non-directory files). The result is unspecified if this flag is applied to a directory. */
#define O_RDONLY	(1<<14)	/**< Open for reading only. */
#define O_RDWR		(1<<15)	/**< Open for reading and writing. */
#define O_SEARCH	(1<<16)	/**< Open directory for search only. The result is unspecified if this flag is applied to a non-directory file. */
#define O_WRONLY	(1<<17)	/**< Open for writing only. */

int creat(const char *path, mode_t mode);
int fcntl(int fildes, int cmd, ...);
int open(const char *path, int oflag, ...);
int openat(int fd, const char *path, int oflag, ...);

#endif
