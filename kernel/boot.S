#define ASM_FILE

#ifdef MULTIBOOT1
#define MULTIBOOT_SPECIFIED

#include "multiboot.h"
# align modules on pages boundaries, provide memory map and video info
.set FLAGS, MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE
# checksum of above, to prove we are multiboot
.set CHECKSUM, -(MULTIBOOT_HEADER_MAGIC + FLAGS)
.set WIDTH, 800
.set HEIGHT, 600
.set DEPTH, 32

.section .multiboot
.align MULTIBOOT_HEADER_ALIGN
.long MULTIBOOT_HEADER_MAGIC
.long FLAGS
.long CHECKSUM
.long 0, 0, 0, 0, 0	#AOUT fields
.long 0	#graphic mode
.long WIDTH, HEIGHT, DEPTH
#endif

#ifdef MULTIBOOT2
#ifdef MULTIBOOT_SPECIFIED
#error Multiboot 1 already specified
#endif
#define MULTIBOOT_SPECIFIED

#include "multiboot2.h"
.set MULTIBOOT_ARCHITECTURE, MULTIBOOT_ARCHITECTURE_I386

.section .multiboot
.align MULTIBOOT_HEADER_ALIGN
multiboot_header:
.long MULTIBOOT2_HEADER_MAGIC
.long MULTIBOOT_ARCHITECTURE
.long multiboot_header_end - multiboot_header
.long -(MULTIBOOT2_HEADER_MAGIC+MULTIBOOT_ARCHITECTURE+multiboot_header_end-multiboot_header)
.short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
.short MULTIBOOT_HEADER_TAG_OPTIONAL
.long 20	# tag size
.long 800
.long 600
.long 32
.short MULTIBOOT_HEADER_TAG_MODULE_ALIGN
.short 0	# Not optional
.long 8		# tag size
.short MULTIBOOT_HEADER_TAG_END
.short 0	# Not optional (of course...)
.long 8		#tag size
multiboot_header_end:
#endif

#ifndef MULTIBOOT_SPECIFIED
#error Multiboot 1/2 must be specified with -DMULTIBOOT1 or -DMULTIBOOT2
#endif

.section .bootstrap_stack, "aw", @nobits
.global _stack_bottom
_stack_bottom:
.skip 16384 # 16 KiB
.global _stack_top
_stack_top:

.section .text
.global start
.type start, @function
start:
	movl $_stack_top, %esp

	# Multiboot pointer
	push %ebx
	# Multiboot magic
	push %eax

	call kernel_main

	cli
	hlt
.Lhang:
	jmp .Lhang

.size start, . - start
