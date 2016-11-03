#include "io/keyboard.h"
#include "io/ps2_device.h"
#include "devices/8042.h"
#include "devices/pic.h"
#include "io/kb_layouts/fr.h"
#include "io/log.h"
#include <stdio.h>

#define ECHO 0xEE
#define GET_SET_SCANCODE 0xF0
#define IDENTIFY_KB 0xF2
#define SCAN_ENABLE 0xF4
#define SCAN_DISABLE 0XF5
#define RESET_PARAMS 0xF6
#define RESEND 0xFE
#define RESET_AND_TEST 0xFF

#define TEST_SUCCESS 0xAA
#define ECHO_RES 0xEE
#define ACK 0xFA
#define TEST_FAIL1 0xFC
#define TEST_FAIL2 0xFD
#define RESEND 0xFE

#define STD_PS2_MOUSE	0x0
#define MOUSE_WT_SCRL	0x3
#define FIVE_BT_MOUSE	0x4
#define MF2_KB_TRANSL	0x41AB
#define MF2_KB			0x83AB

ps2_device_t kb_device = {0};
uint8_t ring_buffer[255] = {0};

uint8_t translate_scan(uint8_t scan)
{
	return scan < sizeof(KEYBOARD_MAP) ? KEYBOARD_MAP[scan] : 0;
}

int getchar(void)
{
	while(kb_device.lock);
	kb_device.lock=1;
	if(kb_device.data_start == kb_device.data_end)
	{
		kb_device.lock=0;
		return EOF;
	}
	int c = *(kb_device.data_start++);
	if(kb_device.data_start == kb_device.buffer_end)
		kb_device.data_start = kb_device.buffer_start;
	kb_device.lock=0;
	return c;
}

void keyboard_handler(regs_t* r)
{
	if(kb_device.state & PS2_WAIT_SPECIAL)
		kb_device.state &= ~PS2_WAIT_SPECIAL;
	else if(kb_device.state & PS2_WAIT_DATA)
		kb_device.state &= ~PS2_WAIT_DATA;
	else
	{
		char scan = ps2_direct_read();
		char c = translate_scan(scan&~0x80);
		if(c && ~scan&0x80)
			*(kb_device.data_end++) = c;
		if(kb_device.data_end == kb_device.buffer_end)
			kb_device.data_end = kb_device.buffer_start;
	}
}

uint8_t ps2_cmd(uint8_t cmd)
{
	kb_device.state |= PS2_WAIT_SPECIAL;
	ps2_write(cmd);
	while(kb_device.state & PS2_WAIT_SPECIAL);
	return ps2_direct_read();
}

uint8_t kb_type[2] = {0xFF, 0xFF};

void keyboard_init()
{
	ps2_init();
	kb_device.lock = 1;
	kb_device.port2 = 0;
	kb_device.state = PS2_WAIT_SPECIAL;	// Workaround to ignore data stuck in buffer
	kb_device.data_end = kb_device.data_start = kb_device.buffer_start = ring_buffer;
	kb_device.buffer_end = ring_buffer+sizeof(ring_buffer);
	kb_device.lock = 0;

	install_irq_handler(1, keyboard_handler);
	unmask_irq(1);
	install_irq_handler(12, keyboard_handler);
	unmask_irq(12);

	if(ps2_cmd(SCAN_DISABLE) != ACK)
		panic("PS2 1 scan disable failed");
	kb_device.state |= PS2_WAIT_DATA;
	if(ps2_cmd(RESET_AND_TEST) != ACK || ps2_read() != TEST_SUCCESS)
		panic("PS2 1 self-test failed");

	kb_device.state |= PS2_WAIT_DATA;
	if(ps2_cmd(IDENTIFY_KB) == ACK)
	{
		kb_device.state |= PS2_WAIT_DATA;
		if((kb_type[0] = ps2_read()) == 0xAB)
		{
			while(kb_device.state & PS2_WAIT_DATA);
			kb_type[1] = ps2_direct_read();
		}
	}

	if(ps2_cmd(ECHO) != ECHO_RES)
		panic("PS2 1 ECHO failed");

	if(ps2_cmd(SCAN_ENABLE) != ACK)
		panic("PS2 1 scan enable failed");
}