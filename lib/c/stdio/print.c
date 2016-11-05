#include <stdio.h>
#include <unistd.h>

const char* const digits = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

int vprintf(const char* restrict format, va_list ap)
{
	return vdprintf(STDOUT_FILENO, format, ap);
}

int printf(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	int ret = vdprintf(STDOUT_FILENO, format, ap);
	va_end(ap);
	return ret;
}

#define PRINT_NUM(T, S) T S##value = va_arg(ap, T);	\
	do												\
	{												\
		*ptr++ = digits[35+S##value%base];			\
		S##value /= base;							\
	} while(S##value)

#define PARSE_PRINT() 													\
for(p=format; *p != 0; p++)												\
	if(*p != '%')														\
	{PUT_CHAR(*p);}														\
	else																\
	{																	\
		uint8_t long_mod = 0;											\
		uint8_t unsigned_mod = 0;										\
		uint8_t hex_mod = 0;											\
		uint8_t oct_mod = 0;											\
		uint8_t digit_num = 0;											\
		size_t precision = 0;											\
		uint8_t precision_mod = 0;										\
		uint8_t done=0;													\
																		\
		while(!done)													\
			switch(*(++p))												\
			{															\
				case 'n':												\
					done=1;												\
					*va_arg(ap, int*) = char_num;						\
					break;												\
				case '%':												\
					done=1;												\
					PUT_CHAR('%');										\
					break;												\
				case 'c':												\
					done=1;												\
					PUT_CHAR(va_arg(ap, int));							\
					break;												\
				case '0':												\
					p++;												\
					if('0' <= *p && *p <= '9')							\
						digit_num=*p-'0';								\
					else PUT_CHAR(*(--p));								\
					break;												\
				case 'l':												\
					if(long_mod<2)										\
						long_mod++;										\
					else												\
					{													\
						done=1;											\
						PUT_CHAR('1');									\
					}													\
					break;												\
				case 'p':												\
					digit_num=8;										\
				case 'x':												\
					hex_mod=1;											\
				case 'u':												\
					unsigned_mod=1;										\
				case 'd':												\
					done = 1;											\
					uint8_t base = hex_mod ? 16 : (oct_mod ? 8 : 10);	\
					char* ptr = buffer;									\
																		\
					if(long_mod==0 || long_mod==1)						\
					{													\
						if(unsigned_mod)								\
							{PRINT_NUM(unsigned, u);}					\
						else											\
						{												\
							PRINT_NUM(int,);							\
							if(value<0 && base==10)						\
								PUT_CHAR('-');							\
						}												\
					}													\
					else if(long_mod==2)								\
					{													\
						if(unsigned_mod)								\
							{PRINT_NUM(long long unsigned, llu);}		\
						else											\
						{												\
							PRINT_NUM(long long, ll);					\
							if(llvalue<0 && base==10)					\
								PUT_CHAR('-');							\
						}												\
					}													\
																		\
					char_num += ptr-buffer;								\
					if(digit_num != 0)									\
						for(uint8_t i=0; i<digit_num-(ptr-buffer); i++)	\
							PUT_CHAR('0');								\
																		\
					while(ptr > buffer)									\
						PUT_CHAR(*(--ptr));								\
					break;												\
				case 's':												\
					done=1;												\
					for(char* str = va_arg(ap, char*);					\
							*str != 0 && (!precision_mod || precision);	\
							str++)										\
					{													\
						PUT_CHAR(*str);									\
						precision--;									\
					}													\
					precision_mod = 0;									\
					break;												\
				case '.':												\
					precision_mod=1;									\
					if(*(p+1) == '*')									\
						precision = va_arg(ap, size_t);					\
					else if('0' <= *(p+1) && *(p+1) <= '9')				\
						precision = *(p+1)-'0';							\
					else												\
					{													\
						PUT_CHAR(*(p+1));								\
						precision_mod = 0;								\
					}													\
					p++;												\
					break;												\
				default:												\
					PUT_CHAR(*(p-1));									\
			}															\
	}

int vsprintf(char *restrict s, const char *restrict format, va_list ap)
{
	int char_num = 0;
	const char* p;
	char buffer[25];

#define PUT_CHAR(c) s[char_num++] = c
	PARSE_PRINT();

	s[char_num++] = '\0';
	return char_num;
}

int vdprintf(int fildes, const char* restrict format, va_list ap)
{
	if(fildes != STDOUT_FILENO)
		return 0;

	int char_num = 0;
	const char* p;
	char buffer[25];

#undef PUT_CHAR
#define PUT_CHAR(c) {putchar(c); char_num++;}
	PARSE_PRINT();

	return char_num;
}