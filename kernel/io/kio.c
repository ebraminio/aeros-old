#include <stdio.h>

static const char* digits = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

int printf(const char* format, ...)
{
	va_list argp;
	va_start(argp, format);
	int ret = vprintf(format, argp);
	va_end(argp);
	return ret;
}

int vprintf(const char* format, va_list argp)
{
	int char_num = 0;
	const char* p;
	char buffer[25];

	for(p=format; *p != 0; p++)
		if(*p != '%')
		{
			putchar(*p);
			char_num++;
		}
		else
		{
			uint8_t long_mod = 0;
			uint8_t unsigned_mod = 0;
			uint8_t hex_mod = 0;
			uint8_t oct_mod = 0;
			uint8_t digit_num = 0;
			size_t precision = 0;
			uint8_t precision_mod = 0;
			uint8_t done=0;

			while(!done)
				switch(*(++p))
				{
					case 'n':
						done=1;
						*va_arg(argp, int*) = char_num;
						break;
					case '%':
						done=1;
						putchar('%');
						char_num++;
						break;
					case 'c':
						done=1;
						putchar(va_arg(argp, int));
						char_num++;
						break;
					case '0':
						p++;
						if('0' <= *p && *p <= '9')
							digit_num=*p-'0';
						else putchar(*(--p));	// default
						break;
					case 'l':
						if(long_mod<2)
							long_mod++;
						else
						{
							done=1;
							putchar('l');
						}
						break;
					case 'p':
						digit_num=8;
					case 'x':
						hex_mod=1;
					case 'u':
						unsigned_mod=1;
					case 'd':
						done = 1;
						uint8_t base = hex_mod ? 16 : (oct_mod ? 8 : 10);

						long long value;
						if(long_mod==0 || long_mod==1)	//Same size on 32bit arch
							value = va_arg(argp, int);
						else if(long_mod==2)
							value = va_arg(argp, long long);

						if(!unsigned_mod && value < 0 && base == 10)
						{
							putchar('-');
							char_num++;
						}

						char* ptr = buffer;
						do
						{
							*ptr++ = digits[35+value%base];
							value /= base;
						} while (value);

						char_num += ptr-buffer;

						if(digit_num != 0)
							for(uint8_t i=0; i<digit_num-(ptr-buffer); i++)
							{
								putchar('0');
								char_num++;
							}

						while(ptr > buffer)
							putchar(*(--ptr));

						break;
					case 's':
						done=1;
						if(!precision_mod)
							char_num += puts(va_arg(argp, char*));
						else for(char* str = va_arg(argp, char*); *str != 0 && precision; str++)
						{
							putchar(*str);
							precision--;
							char_num++;
						}
						break;
					case '.':
						precision_mod=1;
						if(*(p+1) == '*')
							precision = va_arg(argp, size_t);
						else if('0' <= *(p+1) && *(p+1) <= '9')
							precision = *(p+1)-'0';
						else
						{
							putchar(*(p+1));
							precision_mod=0;
						}
						p++;
						break;
					default:
						putchar(*(p-1));
				}
		}
	return char_num;
}

int vsprintf(char *restrict s, const char *restrict format, va_list argp)
{
	int char_num = 0;
	const char* p;
	char buffer[25];

	for(p=format; *p != 0; p++)
		if(*p != '%')
			s[char_num++] = *p;
		else
		{
			uint8_t long_mod = 0;
			uint8_t unsigned_mod = 0;
			uint8_t hex_mod = 0;
			uint8_t oct_mod = 0;
			uint8_t digit_num = 0;
			size_t precision = 0;
			uint8_t precision_mod = 0;
			uint8_t done=0;

			while(!done)
				switch(*(++p))
				{
					case 'n':
						done=1;
						*va_arg(argp, int*) = char_num;
						break;
					case '%':
						done=1;
						s[char_num++] = '%';
						break;
					case 'c':
						done=1;
						s[char_num++] = va_arg(argp, int);
						break;
					case '0':
						p++;
						if('0' <= *p && *p <= '9')
							digit_num=*p-'0';
						else s[char_num++] = *(--p);	// default
						break;
					case 'l':
						if(long_mod<2)
							long_mod++;
						else
						{
							done=1;
							s[char_num++] = '1';
						}
						break;
					case 'p':
						digit_num=8;
					case 'x':
						hex_mod=1;
					case 'u':
						unsigned_mod=1;
					case 'd':
						done = 1;
						uint8_t base = hex_mod ? 16 : (oct_mod ? 8 : 10);

						long long value;
						if(long_mod==0 || long_mod==1)	//Same size on 32bit arch
							value = va_arg(argp, int);
						else if(long_mod==2)
							value = va_arg(argp, long long);

						if(!unsigned_mod && value < 0 && base == 10)
							s[char_num++] = '-';

						char* ptr = buffer;
						do
						{
							*ptr++ = digits[35+value%base];
							value /= base;
						} while (value);

						if(digit_num != 0)
							for(uint8_t i=0; i<digit_num-(ptr-buffer); i++)
								s[char_num++] = '0';

						while(ptr > buffer)
							s[char_num++] = *(--ptr);
						break;
					case 's':
						done=1;
						for(char* str = va_arg(argp, char*); *str != 0 && (!precision_mod || precision); str++)
						{
							s[char_num++] = *str;	/** \TODO Check for standard function to copy string with max length */
							precision--;
						}
						precision_mod = 0;
						break;
					case '.':
						precision_mod=1;
						if(*(p+1) == '*')
							precision = va_arg(argp, size_t);
						else if('0' <= *(p+1) && *(p+1) <= '9')
							precision = *(p+1)-'0';
						else
						{
							s[char_num++] = *(p+1);
							precision_mod = 0;
						}
						p++;
						break;
					default:
						s[char_num++] = *(p-1);
				}
		}
	s[char_num++] = '\0';
	return char_num;
}