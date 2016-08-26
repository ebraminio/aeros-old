#ifndef __MULTIBOOT_COMPL_H__
#define __MULTIBOOT_COMPL_H__

#ifndef MULTIBOOT_HEADER
#error No multiboot specification included
#endif

#ifndef ASM_FILE

#include <stddef.h>

#define MULTIBOOT_CHS 0
#define MULTIBOOT_LBA 1

typedef struct
{
	size_t size;
	uint8_t drive_number;
	uint8_t mode;
	uint8_t drive_cylinders;
	uint8_t drive_heads;
	uint8_t drive_sectors;
	uint16_t* ports;
} multiboot_drive_t;

#endif

#endif