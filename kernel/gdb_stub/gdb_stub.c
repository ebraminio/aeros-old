#ifdef WITH_GDB_STUB

#include <stdio.h>
#include <stddef.h>
#include "io/serial.h"
#include <io/log.h>

int getDebugChar(void)
{
	return read_serial(1);
}

void putDebugChar(int dchar)
{
	write_serial(1, dchar&0xFF);
}

extern void _catchException0(void);
extern void _catchException1(void);
extern void _catchException3(void);
extern void _catchException4(void);
extern void _catchException5(void);
extern void _catchException6(void);
extern void _catchException7(void);
extern void _catchException8(void);
extern void _catchException9(void);
extern void _catchException10(void);
extern void _catchException11(void);
extern void _catchException12(void);
extern void _catchException13(void);
extern void _catchException14(void);

void exceptionHandler(int exception_number, void* exception_address)
{
	switch(exception_number)
	{
		case 0: _catchException0(); break;
		case 1: _catchException1(); break;
		case 3: _catchException3(); break;
		case 4: _catchException4(); break;
		case 5: _catchException5(); break;
		case 6: _catchException6(); break;
		case 7: _catchException7(); break;
		case 8: _catchException8(); break;
		case 9: _catchException9(); break;
		case 10: _catchException10(); break;
		case 11: _catchException11(); break;
		case 12: _catchException12(); break;
		case 13: _catchException13(); break;
		case 14: _catchException14(); break;
		default: panic("Unsupported exception handler %d", exception_number);
	}
}

FILE* stderr = NULL;

int fprintf(FILE* stream, const char* format, ...)
{
	return 0;
}

void set_debug_traps(void);

void gdb_stub_init(void)
{
	set_debug_traps();
	nopanic("Debug traps set for GDB stub. Syncing with int3...");
	asm("int $3");
}

#endif
