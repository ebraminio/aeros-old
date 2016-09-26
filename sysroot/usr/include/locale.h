#ifndef LOCALE_H
#define LOCALE_H

#include <stddef.h>

#define LC_ALL		1
#define LC_COLLATE	2
#define LC_CTYPE	3
#define LC_MESSAGES	4
#define LC_MONETARY	5
#define LC_NUMERIC	6
#define LC_TIME		7

char* setlocale(int category, const char *locale);

#endif