#ifndef LANGINFO_H
#define LANGINFO_H

#include <locale.h>
#include <nl_types.h>

char* nl_langinfo(nl_item item);
char* nl_langinfo_l(nl_item item, locale_t locale);

#endif