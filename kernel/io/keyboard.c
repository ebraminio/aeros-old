#include "io/keyboard.h"
#include "devices/8042.h"
#include "devices/pic.h"
#include <sys/io.h>
#include "io/kb_layouts/fr.h"
#include "io/log.h"

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

#define STD_PS2_MOUSE 0x0
#define MOUSE_WT_SCRL 0x3
#define FIVE_BT_MOUSE 0x4
#define MF2_KB_TRANSL 0x41AB
#define MF2_KB		  0x83AB

void keyboard_handler(regs_t* r)
{
	uint8_t scan = inb(0x60);
}

uint8_t ps2_cmd(uint8_t cmd)
{
	ps2_write(cmd);
	return ps2_read();
}

uint8_t kb_type[2] = {0xFF, 0xFF};

void keyboard_init()
{
	ps2_init();
	
	irq_install_handler(1, keyboard_handler);
	unmask_irq(1);
	irq_install_handler(12, keyboard_handler);
	unmask_irq(12);

	ps2_cmd(SCAN_DISABLE);
	if(ps2_cmd(RESET_AND_TEST) == ACK)
		if(ps2_read() != TEST_SUCCESS)
			panic("Self-test fail on keyboard");

	if(ps2_cmd(IDENTIFY_KB) == ACK)
		if((kb_type[0] = ps2_read()) == 0xAB)
			kb_type[1] = ps2_read();

	if(ps2_cmd(ECHO) != ECHO_RES)
		panic("ECHO failed on keyboard");

	ps2_cmd(SCAN_ENABLE);
}
