/**
 *  \file
 */

#include <stdint.h>
#include <stdio.h>
#include "multiboot.h"
#include "multiboot_compl.h"
#include "video/video.h"
#include "video/vbe.h"
#include "io/log.h"
#include <cpuid.h>
#include "cpu/acpi.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "devices/pit.h"
#include "io/keyboard.h"
#include "io/serial.h"
#include "mem/pmem.h"

#define LOG_CPU_SUPPORT(X); if(__builtin_cpu_supports(X)) printf(" "X);

extern const uint8_t _stack_bottom;
extern const uint8_t _stack_top;
extern const uint8_t _initial_esp;
extern void _enter_usermode(void);

/**
 *  \param magic Multiboot 1 or 2 magic number
 *  \param address Address of a multiboot_info_t structure
 *  \return Does not return
 *  \see multiboot.h multiboot2.h
 */
__attribute__((noreturn))
void kernel_main(unsigned long magic, unsigned long address)
{
	multiboot_info_t* mboot_info = (multiboot_info_t*) address;

	if(mboot_info->flags & MULTIBOOT_INFO_VBE_INFO && ((vbe_mode_t*)mboot_info->vbe_mode_info)->mode_attributes & VBE_MODE_ATTRIB_LINEAR_FRAME_BUFFER_MODE_AVAILABLE)
		vbe_init((vbe_controller_t*)mboot_info->vbe_control_info, (vbe_mode_t*)mboot_info->vbe_mode_info);
	else video_init();

	if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
		panic("Not loaded by Multiboot-compliant bootloader\n");

	if((mboot_info->flags & (MULTIBOOT_INFO_MEMORY|MULTIBOOT_INFO_MEM_MAP)) != (MULTIBOOT_INFO_MEMORY|MULTIBOOT_INFO_MEM_MAP))
		panic("AerOS needs memory information from Multiboot");

	serial_init(1);
	nopanic("Serial");
	gdt_init();
	printf(" GDT");
	idt_init();
	printf(" IDT");
	keyboard_init();
	printf(" KB");
	pit_init();
	printf(" PIT");
	acpi_init();
	printf(" ACPI");
	pmem_init(mboot_info->mem_lower, mboot_info->mem_upper, mboot_info->mmap_addr, mboot_info->mmap_length);
	printf(" PMEM");

	char id[13];
	char brand[48];
	unsigned int unused_register;
	__get_cpuid(0, &unused_register, (unsigned int*)id, (unsigned int*)(id+8), (unsigned int*)(id+4));

	unsigned int eax;
	unsigned int* ptr = (unsigned int*)brand;
	if(__get_cpuid(0x80000000, &eax, &unused_register, &unused_register, &unused_register) && eax == 0x80000004)
		for(unsigned int level = 0x80000002; level < 0x80000005; level++)
		{
			__get_cpuid(level, ptr, ptr+1,	ptr+2,	ptr+3);
			ptr+=4;
		}
	else brand[0]='\0';
	printf("CPU : %s - %s\nSupported features:", id, brand);

	__builtin_cpu_init();
	LOG_CPU_SUPPORT("cmov");
	LOG_CPU_SUPPORT("mmx");
	LOG_CPU_SUPPORT("popcnt");
	LOG_CPU_SUPPORT("sse");
	LOG_CPU_SUPPORT("sse2");
	LOG_CPU_SUPPORT("sse3");
	LOG_CPU_SUPPORT("ssse3");
	LOG_CPU_SUPPORT("sse4.1");
	LOG_CPU_SUPPORT("sse4.2");
	LOG_CPU_SUPPORT("avx");
	LOG_CPU_SUPPORT("avx2");
	putchar('\n');

	if(mboot_info->flags & MULTIBOOT_INFO_BOOTDEV)
	{
		printf("\t\tDevice: 0x%x\tDrive: %d", mboot_info->boot_device, mboot_info->boot_device >> 24);
		printf("\tPartition: %d.%d.%d\n", mboot_info->boot_device>>16 & 0xFF, mboot_info->boot_device>>8 & 0xFF, (mboot_info->boot_device & 0xFF));
	}

	if(mboot_info->flags & MULTIBOOT_INFO_CMDLINE)
		printf("\t\tKernel command-line: %s\n", (char*)mboot_info->cmdline);
	if(mboot_info->flags & MULTIBOOT_INFO_DRIVE_INFO)
	{
		multiboot_drive_t* drivelist = (multiboot_drive_t*)mboot_info->drives_addr;
		printf("\t\tDrives: %d", mboot_info->drives_length);
		for(uint8_t i=0; i<mboot_info->drives_length; i++)
			printf("\n\t\t\t-n°%d Mode %s CHS:%d-%d-%d", drivelist[i].drive_number, drivelist[i].mode == MULTIBOOT_CHS ? "CHS" : "LBA",
				drivelist[i].drive_cylinders, drivelist[i].drive_heads, drivelist[i].drive_sectors);
		putchar('\n');
	}
	if(mboot_info->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME)
		printf("\t\tBootloader name: %s\n", (char*)mboot_info->boot_loader_name);

	if(mboot_info->flags & MULTIBOOT_INFO_VBE_INFO)
	{
		vbe_controller_t* vbe_cinfo = (vbe_controller_t*)mboot_info->vbe_control_info;
		vbe_mode_t* vbe_minfo = (vbe_mode_t*)mboot_info->vbe_mode_info;

		if(vbe_minfo->mode_attributes & VBE_MODE_ATTRIB_LINEAR_FRAME_BUFFER_MODE_AVAILABLE)
			printf("\t\tLinear frame buffer mode available : 0x%p", (void*)vbe_minfo->frame_buffer);

		printf("\n\t\tVBE version %d.%d mode:0x%x.", vbe_cinfo->major_version, vbe_cinfo->minor_version, mboot_info->vbe_mode);
		if(vbe_cinfo->major_version > 1 || (vbe_cinfo->major_version==1 && vbe_cinfo->minor_version>2))	// 1.2 and above
		{
			printf("\b (%dx%dx%d). char cell is %dx%d.", vbe_minfo->x_resolution, vbe_minfo->y_resolution,
				vbe_minfo->bits_per_pixel, vbe_minfo->char_cell_width, vbe_minfo->char_cell_heigth);
			printf(" Memory model type %s.", vbe_memory_models[vbe_minfo->memory_model_type]);
		}
		if(vbe_minfo->mode_attributes & VBE_MODE_ATTRIB_TTY_SUPPORTED)
			printf(" TTY supported.");
		if(vbe_minfo->mode_attributes & VBE_MODE_ATTRIB_COLOR_MODE)
			printf(" Color");
		else printf(" Monochrome");
		if(vbe_minfo->mode_attributes & VBE_MODE_ATTRIB_GRAPHIC_MODE)
			puts(" graphic mode.");
		else puts(" text mode.");

		putchar('\n');
		printf("\t\tVBE 2.0+ interface : Segment:0x%x\tOffset:0x%08x\tLength:0x%x\n",
			mboot_info->vbe_interface_seg, mboot_info->vbe_interface_off, mboot_info->vbe_interface_len);
	}

	for(;;);
}
