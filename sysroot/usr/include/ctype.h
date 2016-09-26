/**
 * \file
 */

#ifndef CTYPE_H
#define CTYPE_H

int isprint(int c);
int isdigit(int c);
int isxdigit(int c);
int islower(int c);
int isupper(int c);
int isalpha(int c);
int isalnum(int c);
int isblank(int c);
int isspace(int c);
int ispunct(int c);
int isgraph(int c);
int iscntrl(int c);

#define isprint(c) ('\t'<=c&&c<='\r' || ' '<=c&&c<='~')
#define isdigit(c) ('0'<=c && c<='9')
#define isxdigit(c) (isdigit(c) || 'a'<=c&&c<='f' || 'A'<=c&&c<='F')
#define islower(c) ('a'<=c && c<='z')
#define isupper(c) ('A'<=c && c<='Z')
#define isalpha(c) (islower(c)||isupper(c))
#define isalnum(c) (isalpha(c)||isdigit(c))
#define isblank(c) (c==' ' || c=='\t')
#define isspace(c) (isblank(c) || c=='\n'||c=='\v'||c=='\f'||c=='\r')
#define ispunct(c) (c=='.'||c==','||c==':'||c=='"'||c=='!'||c=='\''||c==';'||c=='?')
#define isgraph(c) (isalpha(c)||isxdigit(c)||ispunct(c))
#define iscntrl(c) 0

int tolower(int c);
int toupper(int c);

/**
 * \TODO isascii
 * \TODO toascii
 */

#endif