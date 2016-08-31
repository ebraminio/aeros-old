#ifndef __VIDEO_VBE_H__
#define __VIDEO_VBE_H__

#include <stdint.h>
#include "video/device.h"

// Fixed DAC width = 6bits. Switchable to 8bits. Resetted upon mode set.
#define VBE_CAPA_DAC_WITDH_SWICHABLE	(1<<0)
// Controller is VGA compatible
#define VBE_CAPA_VGE_COMP	(1<<1)

// Mode supported by hardware
#define VBE_MODE_ATTRIB_MODE_SUPPORTED	(1<<0)
// TTY Output functions supported by BIOS (set cursor size, position, scroll up
// and down, write char and attrib at position, write char only at position,
// write char and advance cursor)
#define VBE_MODE_ATTRIB_TTY_SUPPORTED	(1<<2)
#define VBE_MODE_ATTRIB_COLOR_MODE		(1<<3)
#define VBE_MODE_ATTRIB_GRAPHIC_MODE	(1<<4)
// Incompatible with VGA hardware registers
#define VBE_MODE_ATTRIB_NOT_VGA_COMP	(1<<5)
// FIXME Shorten next fields...
#define VBE_MODE_ATTRIB_VGA_COMP_WINDWD_MEM_MODE_AVAILABLE	(1<<6)
#define VBE_MODE_ATTRIB_LINEAR_FRAME_BUFFER_MODE_AVAILABLE	(1<<7)

#define VBE_MAX_MEM_MODEL 8

const char* vbe_memory_models[VBE_MAX_MEM_MODEL];

typedef struct vbe_controlle
{
	uint32_t signature;					/**< 'VESA' 0x41534556 or VBE2 0x32454256 */
	uint8_t minor_version;
	uint8_t major_version;
	char** oem_string;					/**< Only if VBE2 is preset in sig */
	uint32_t capabilities;				/**< See VBE_CAPA_*. Bit 2 is about RAMDAC operations. Bits 3-31 are reserved */
	uint16_t* supported_vmodes;			/**< End with -1 0xFFFF. Only if VBE2 is preset in sig */
	uint16_t total_memory;				/**< Counted in number of 64kb memory blocks */
	uint8_t oem_software_minor_version;	/**< Only if VBE2 is preset in sig */
	uint8_t oem_software_major_version;	/**< Only if VBE2 is preset in sig */
	char** oem_vendor_name;				/**< Only if VBE2 is preset in sig */
	char** oem_product_name;			/**< Only if VBE2 is preset in sig */
	char** oem_product_revision;		/**< Only if VBE2 is preset in sig */
	uint8_t reserved[222];
	uint8_t data_area[256];				/**< For OEM strings. Only if VBE2 is preset in sig */
} vbe_controller_t;

typedef struct vbe_mode
{
	uint16_t mode_attributes;		/**< Bit 1 is reserved. Bits 8-15 are reserved */
	uint8_t winA_attributes;
	uint8_t winB_attributes;
	uint16_t granularity;
	uint16_t windows_size;			/**< In KB */
	uint16_t winA_start_seg;
	uint16_t winB_start_seg;
	uint32_t window_function;		/**< Alternative to VBE function 0x5 */
	uint16_t bytes_per_scanline;
	// Only for VBE1.2 and above :
	uint16_t x_resolution;
	uint16_t y_resolution;
	uint8_t char_cell_width;		/**< In pixels; */
	uint8_t char_cell_heigth;
	uint8_t mem_planes_number;
	uint8_t bits_per_pixel;
	uint8_t banks_number;
	uint8_t memory_model_type;
	uint8_t bank_size;				/**< In KB */
	uint8_t images_number;
	uint8_t reserved_byte;			/**< 1. For page function */
	uint8_t direct_color_fields[9];	/**< TODO Required for direct/6 and YUV/7 memory models. */
									// Only VBE2.0 and above :
	uint32_t frame_buffer;			/**< Physical address for flat memory fb */
	uint32_t offscreen_memory;
	uint32_t offscreen_mem_size;	/**< In 1k units */
	uint8_t reserved[206];
} vbe_mode_t;

#endif