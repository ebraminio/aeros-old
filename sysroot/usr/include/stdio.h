/**
 *  \file
 */

#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>
#include <stdint.h>
#include <limits.h>
#include <sys/types.h>

typedef struct
{
	int fileno;
	ino_t inode;
	//char path[PATH_MAX];
	//char name[NAME_MAX];
	//uint32_t attrib;				/**< \TODO Find POSIX defined structure */
	//char create_time_fine;
	//uint16_t create_time;
	//uint16_t create_date;
	//uint16_t access_date;
	//uint16_t last_modif_time;
	//uint16_t last_modif_date;
	//uint32_t first_cluster;
	size_t size;
	size_t position;			/**< Position in file */
	uint32_t error_flags;
	uint8_t* buffer_start;
	uint8_t* buffer_end;
	uint8_t* data_start;
	uint8_t* data_end;
} FILE;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

#define BUFSIZ 255

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define EOF ((int)-1)

int printf(const char* format, ...) __attribute__((format (printf, 1, 2)));
int vprintf(const char* format, va_list ap) __attribute__((format (printf, 1, 0)));
int fprintf(FILE* stream, const char* format, ...) __attribute__((format (printf, 2, 3)));
int vfprintf(FILE* stream, const char* format, va_list ap) __attribute__((format (printf, 2, 0)));
int sprintf(char* s, const char* format, ...) __attribute__((format (printf, 2, 3)));
int vsprintf(char* restrict s, const char* restrict format, va_list ap) __attribute__((format (printf, 2, 0)));
int puts(const char* );
int putc(int c, FILE* stream);
int fputc(int c, FILE* stream);
int fputs(const char* s, FILE* stream);
int putchar(int c);

int getchar(void);
int fgetc(FILE* stream);
int getc(FILE* stream);
int ungetc(int c, FILE* stream);
int fscanf(FILE *restrict stream, const char *restrict format, ...) __attribute__((format (scanf, 2, 3)));

int fflush(FILE* stream);

FILE* fopen(const char* path, const char* mode);
int fclose(FILE* fp);
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);
int fseek(FILE* stream, long offset, int whence);
long ftell(FILE* stream);
void setbuf(FILE* stream, char* buf);
FILE* fdopen(int fildes, const char* mode);
int fileno(FILE* stream);
int ferror(FILE* stream);
void clearerr(FILE* stream);

#endif