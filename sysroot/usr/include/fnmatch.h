#ifndef FNMATCH_H
#define FNMATCH_H

#define FNM_NOMATCH		0
#define FNM_PATHNAME	1
#define FNM_PERIOD		2
#define FNM_NOESCAPE	3

int fnmatch(const char* pattern, const char* string, int flags);

#endif