#ifndef __CPU_REGS_H__
#define __CPU_REGS_H__

#include <stdint.h>

typedef struct
{
	uint32_t gs, fs, es, ds;
	uint32_t cr0, cr2, cr3, cr4;
	uintptr_t edi, esi, ebp, esp;
	uint32_t ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uintptr_t eip;						/**< Automatically pushed by the processor */
	uint32_t cs, eflags, useresp, ss;	/**< Automatically pushed by the processor */
} regs_t;

#endif