/**
 * \file
 */

#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

size_t strlen(const char* str);
size_t strnlen(const char *s, size_t maxlen);

int strcmp(const char * s1, const char * s2);
int strncmp(const char* s1, const char* s2, size_t n);

char* strcpy(char* dest, const char* src);
char* strncpy(char *dest, const char *src, size_t n);
char* stpcpy(char *dest, const char *src);
char* stpncpy(char *dest, const char *src, size_t n);

char* strstr(const char* haystack, const char* needle);
char* strtok(char *str, const char *delim);
char* strchr(const char *s, int c);
char* strrchr(const char *s, int c);
char* strchrnul(const char *s, int c);
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
char* strpbrk(const char *s, const char *accept);

char* strcat(char *dest, const char *src);
char* strncat(char *dest, const char *src, size_t n);

char* strdup(const char *s);
char* strndup(const char *s, size_t n);

void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memchr(const void *s, int c, size_t n);
void* memrchr(const void *s, int c, size_t n);
void* rawmemchr(const void *s, int c);

char* strerror(int errnum);

char* strsignal(int signum);

/**
 * \TODO bcmp
 * \TODO bcopy
 * \TODO bzero
 * \TODO index
 * \TODO memccpy
 * \TODO memchr
 * \TODO memcmp
 * \TODO memcpy
 * \TODO memmem
 * \TODO memmove
 * \TODO mempcpy
 * \TODO memset
 * \TODO rindex
 * \TODO stpncpy
 * \TODO strcasecmp
 * \TODO strcasestr
 * \TODO strcat
 * \TODO strchr
 * \TODO strcmp
 * \TODO strcoll
 * \TODO strcspn
 * \TODO strerror
 * \TODO strlen
 * \TODO strlwr
 * \TODO strncasecmp
 * \TODO strncat
 * \TODO strncmp
 * \TODO strncpy
 * \TODO strnlen
 * \TODO strpbrk
 * \TODO strrchr
 * \TODO strsignal
 * \TODO strspn
 * \TODO strtok
 * \TODO strupr
 * \TODO swab
 * \TODO ffs
 */

#endif