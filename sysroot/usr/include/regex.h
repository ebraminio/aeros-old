#ifndef REGEX_H
#define REGEX_H
#include <sys/types.h>

#define REG_EXTENDED
#define REG_ICASE
#define REG_NOSUB
#define REG_NEWLINE

#define REG_NOTBOL
#define REG_NOTEOL

#define REG_NOMATCH	(1<<0)
#define REG_BADPAT	(1<<1)
#define REG_ECOLLATE (1<<2)
#define REG_ECTYPE	(1<<3)
#define REG_EESCAPE	(1<<4)
#define REG_ESUBREG	(1<<5)
#define REG_EBRACK	(1<<6)
#define REG_EPAREN	(1<<7)
#define REG_EBRACE	(1<<8)
#define REG_BADBR	(1<<9)
#define REG_ERANGE	(1<<10)
#define REG_ESPACE	(1<<11)
#define REG_BADRPT	(1<<12)

int regcomp(regex_t *preg, const char *regex, int cflags);
int regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);
size_t regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size);
void regfree(regex_t *preg);

#endif