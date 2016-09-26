/**
 * \file
 */

#ifndef DIRENT_H
#define DIRENT_H

#include <limits.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct dirent
{
	char d_name[NAME_MAX];
	unsigned char d_type;
};

enum
{
	DT_UNKNOWN = 0,
	DT_FIFO = 1,
	DT_CHR = 2,
	DT_DIR = 4,
	DT_BLK = 6,
	DT_REG = 8,
	DT_LNK = 10,
	DT_SOCK = 12,
	DT_WHT = 14
};

#define IFTODT(mode)	(((mode) & 0170000) >> 12)
#define DTTOIF(dirtype)	((dirtype) << 12)

typedef struct
{
	int fd;
	char path[PATH_MAX];
	long filepos;			/**< In "child" unit : when filepos=1, next child returned by readdir is the 2nd */
	off_t offset;			/**< In bytes */
	struct dirent ent;
	uint32_t fs_specific;	/**< 32 bits allowing FS drivers to store some data */
} DIR;

int alphasort(const struct dirent** d1, const struct dirent* d2);
int closedir(DIR* dirp);
int dirfd(DIR* dirp);
DIR* fdopendir(int fd);
DIR* opendir(const char* dirname);
struct dirent* readdir(DIR* dirp);
void rewinddir(DIR* dirp);
int scandir(const char* dir, struct dirent *** namelist,
	int (*sel)(const struct dirent *),
	int (*compar)(const struct dirent **, const struct dirent **));
void seekdir(DIR* dirp, long loc);
long telldir(DIR* dirp);

#endif