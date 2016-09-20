#include "devices/8042.h"
#include <stdint.h>
#include <sys/io.h>
#include "io/log.h"

#define DATA_PORT	0x60
#define STATUS_PORT	0x64
#define CMD_PORT	0x64

#define READ_BYTE(n) (0x20|n&0x1F)
#define WRITE_BYTE(n) (0x60|n&0x1F)
#define DISABLE_PORT2 0xA7
#define ENABLE_PORT2 0xA8
#define TEST_PORT2 0xA9
#define SELF_TEST 0xAA
#define TEST_PORT1 0xAB
#define DISABLE_PORT1 0xAD
#define ENABLE_PORT1 0xAE
#define READ_CONTROLLER_IN 0xC0
#define READ_CONTROLLER_OUT 0xD0
#define WRITE_CONTROLLER_OUT 0xD1
#define WRITE_PORT1_OUT 0xD2
#define WRITE_PORT2_OUT 0xD3
#define WRITE_PORT2_IN 0xD4

#define SELF_TEST_SUCCESS 0x55

typedef union
{
	struct
	{
		uint8_t out_full : 1;
		uint8_t in_full : 1;
		uint8_t post_passed : 1;
		uint8_t for_controller : 1;
		uint8_t unknown : 2;
		uint8_t time_out_err : 1;
		uint8_t parity_error : 1;
	};
	uint8_t numeric;
} ps2_status_t;

typedef union
{
	struct
	{
		uint8_t port1_int : 1;
		uint8_t port2_int : 1;
		uint8_t post_passed : 1;
		uint8_t zero1 : 1;
		uint8_t port1_clock_disabled : 1;
		uint8_t port2_clock_disabled : 1;
		uint8_t port1_translation : 1;
		uint8_t zero2 : 1;
	};
	uint8_t numeric;
} ps2_config_t;

typedef union
{
	struct
	{
		uint8_t system_reset : 1;
		uint8_t a20_gate : 1;
		uint8_t port2_clock : 1;
		uint8_t port2_data : 1;
		uint8_t out_full_from_p1 : 1;
		uint8_t out_full_from_p2 : 1;
		uint8_t port1_clock : 1;
		uint8_t port1_data : 1;
	};
	uint8_t numeric;
} controller_out_t;

ps2_status_t status;
ps2_config_t config;

uint8_t ps2_read(void)
{
	status.numeric = inb(STATUS_PORT);
	while(!status.out_full)
		status.numeric = inb(STATUS_PORT);
	uint8_t data = inb(DATA_PORT);
	status.numeric = inb(STATUS_PORT);
	return data;
}

uint8_t ps2_direct_read(void)
{
	return inb(DATA_PORT);
}

void ps2_write(uint8_t data)
{
	status.numeric = inb(STATUS_PORT);
	while(status.in_full)
		status.numeric = inb(STATUS_PORT);
	outb(DATA_PORT, data);
}

void ps2_init(void)
{
	//Disable controllers until configuration is done
	outb(CMD_PORT, DISABLE_PORT2);
	outb(CMD_PORT, DISABLE_PORT1);
	
	// Flush out buffer
	status.numeric = inb(STATUS_PORT);
	while(status.out_full)
	{
		inb(DATA_PORT);
		status.numeric = inb(STATUS_PORT);
	}

	outb(CMD_PORT, READ_BYTE(0));
	config.numeric = ps2_read();

	outb(CMD_PORT, SELF_TEST);
	if(ps2_read() != SELF_TEST_SUCCESS)
		panic("8042 self test failed");

	outb(CMD_PORT, TEST_PORT1);
	if(ps2_read())
		panic("PS2 port 1 test failed");

	// Enable controller
	outb(CMD_PORT, ENABLE_PORT1);
	config.port1_int = 1;
	config.port1_translation = 1;
	config.port1_clock_disabled = 0;
	outb(CMD_PORT, WRITE_BYTE(0));
	ps2_write(config.numeric);
	outb(CMD_PORT, READ_BYTE(0));
	config.numeric = ps2_read();
	
	// Flush out buffer
	status.numeric = inb(STATUS_PORT);
	while(status.out_full)
	{
		inb(DATA_PORT);
		status.numeric = inb(STATUS_PORT);
	}
}
