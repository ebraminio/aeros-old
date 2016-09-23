#ifndef __CPU_BDA_H__
#define __CPU_BDA_H__

typedef struct __attribute__((packed))
{
	uint16_t com1, com2,com3, com4;
	uint16_t lpt1, lpt2, lpt3;
	uint16_t ebda;
	struct __attribute__((packed))
	{
		uint8_t boot_floppy : 1;
		uint8_t math_coprocessor : 1;
		uint8_t ps2_mouse : 1;
		uint8_t reserved1 : 1;
		uint8_t video_mode : 2;
		uint8_t floppy_num : 2;
		uint8_t reserved2 : 1;
		uint8_t serial_num : 3;
		uint8_t reserved3 : 2;
		uint8_t parallel_num : 2;
	};
	uint8_t int_flag;
	uint16_t mem_size_kb;
	uint8_t unknown1[2];
	struct
	{
		uint8_t rshift_k : 1;
		uint8_t lshift_k : 1;
		uint8_t ctrl_k : 1;
		uint8_t alt_k : 1;
		uint8_t scroll_lock : 1;
		uint8_t numlock : 1;
		uint8_t capslock : 1;
		uint8_t insert : 1;
		uint8_t ralt_k : 1;
		uint8_t lalt_k : 1;
		uint8_t sysreg_k : 1;
		uint8_t pause_k : 1;
		uint8_t scroll_lock_k : 1;
		uint8_t numlock_k : 1;
		uint8_t capslock_k : 1;
		uint8_t insert_k : 1;
	};
	uint8_t unknown2;
	uint16_t kb_data_start, kb_data_end;
	uint8_t kb_buffer[32];
	uint8_t floppy_calib_status;
	struct
	{
		uint8_t motor0_on : 1;
		uint8_t motor1_on : 1;
		uint8_t motor2_on : 1;
		uint8_t motor3_on : 1;
		uint8_t sel_drive : 2;
		uint8_t unused : 1;
		uint8_t write_op : 1;
	};
	uint8_t floppy_timeout;
	struct
	{
		uint8_t errors : 5;
		uint8_t controller_test_fail : 1;
		uint8_t seek_error : 1;
		uint8_t timeout : 1;
	};
	uint8_t todo[7];
	uint8_t display_mode;
	uint16_t text_mode_width;
	uint16_t active_video_page_size;
	uint16_t active_video_page_offset;
	uint16_t cursor_pos_in_video_pages[8];
	uint16_t cursor_shape;
	uint8_t active_video_page;
	uint16_t video_io_port;
	uint8_t todo2[6];
	struct
	{
		uint8_t irq0_occured : 1;
		uint8_t irq1_occured : 1;
		uint8_t irq2_occured : 1;
		uint8_t irq3_occured : 1;
		uint8_t irq4_occured : 1;
		uint8_t irq5_occured : 1;
		uint8_t irq6_occured : 1;
		uint8_t irq7_occured : 1;
	};
	uint32_t irq0_since_boot;
	uint8_t timer_is_24h;
	uint8_t kb_ctrl_brk_flag;
	uint16_t soft_reset_flag;
	uint8_t todo3;
	uint8_t hd_num;
	uint8_t todo4;
	uint8_t hd_io_port;
	uint8_t parallel_ports_timeout[4];
	uint8_t serial_ports_timeout[4];
	uint16_t kb_buffer_start, kb_buffer_end;
	uint8_t video_rows_minus_1;
	uint16_t char_scan_lines;
	uint8_t todo5[2];
} bda_t;

static const bda_t* const bda = (const bda_t* const)0x400;

#endif
