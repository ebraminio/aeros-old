/**
 * \file
 */

#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <sys/types.h>


#define EXIT_FAILURE    1
#define EXIT_SUCCESS    0
#define RANDMAX 32767
#define MB_CUR_MAX 1

int abs(int);
long labs(long);
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);
char* getenv(const char *name);
void abort(void) __attribute__ ((__noreturn__));
int atoi(const char *nptr);
long atol(const char *nptr);
long long atoll(const char *nptr);
double atof(const char *str);
int atexit(void (*function)(void));
void* malloc(size_t size);
void* calloc(size_t nmemb, size_t size);
void free(void* mem);
void* realloc(void *ptr, size_t size);
void _Exit(int status);
void exit(int status);
int system(const char *command);
int rand(void);
void srand(unsigned int seed);
int mbtowc(wchar_t *restrict pwc, const char *restrict s, size_t n);

/**
 * \TODO Use as much builtin as possible here
 * \TODO a64l
 * \TODO l64a
 * \TODO abs
 * \TODO assert
 * \TODO atexit
 * \TODO atof
 * \TODO atoff
 * \TODO bsearch
 * \TODO div
 * \TODO ecvt
 * \TODO ecvtf
 * \TODO fcvt
 * \TODO fcvtf
 * \TODO gvcvt
 * \TODO gvcvtf
 * \TODO ecvtbug
 * \TODO fcvtbuf
 * \TODO getenv
 * \TODO labs
 * \TODO ldiv
 * \TODO llabs
 * \TODO lldiv
 * \TODO mallinfo
 * \TODO malloc_stats
 * \TODO mallopt
 * \TODO on_exit
 * \TODO qsort
 * \TODO rand
 * \TODO srand
 * \TODO rand48
 * \TODO drand48
 * \TODO erand48
 * \TODO lrand48
 * \TODO nrand48
 * \TODO mrand48
 * \TODO jrand48
 * \TODO srand48
 * \TODO seed48
 * \TODO lcong48
 * \TODO strtod
 * \TODO strtof
 * \TODO strtol
 * \TODO strtoll
 * \TODO stroul
 * \TODO stroull
 */

#endif
