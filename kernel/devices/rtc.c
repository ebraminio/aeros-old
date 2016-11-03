#include "devices/rtc.h"
#include "devices/pic.h"
#include <stdint.h>
#include <sys/io.h>

#define REG_PORT	0x70
#define DATA_PORT	0x71

#define SEC_REG			0x00
#define SEC_ALARM_REG	0x01
#define MIN_REG			0x02
#define MIN_ALARM_REG	0x03
#define HOUR_REG		0x04
#define HOUR_ALARM_REG	0x05
#define DOW_REG			0x06
#define DOM_REG			0x07
#define MON_REG			0x08
#define YEAR_REG		0x09
#define STATA_REG		0x0A
#define STATB_REG		0x0B
#define STATC_REG		0x0C
#define STATD_REG		0x0D
#define DIAGN_STAT_REG	0x0E
#define SHUTDOWN_STATUS_REG	0x0F
#define FLOPPY_TYPES_REG	0x10
#define SYST_CONFIG_REG	0x11
#define HD_TYPES_REG	0x12
#define TYPEMATIC_PARAM_REG	0x13
#define INSTALLED_EQUIP_REG	0x14
#define POST_INFO_REG	0x33
#define CENTURY_REG		0x48

// STATA
#define UPDATE_IN_PROGRESS (1<<7)

// statB
#define H24_MODE		(1<<1)
#define BIN_MODE		(1<<2)
#define SQUARE_WAVE		(1<<3)
#define UPDATE_ENDED_INT (1<<4)
#define ALARM_INT		(1<<5)
#define PERIODIC_INT	(1<<6)

// STATC
#define UF		(1<<4)
#define AF		(1<<5)
#define PF		(1<<6)
#define IRQF	(1<<7)

// STATD
#define CMOS_BATT_OK (1<<7)

// POST INFO
#define POST_CACHE_TEST_PASSED	(1<<0)
#define BIOS_LENGTH				(1<<7)

static uint8_t read_cmos(uint8_t reg_num)
{
	outb(REG_PORT, 0x80|reg_num);
	return inb(DATA_PORT);
}

static void write_cmos(uint8_t reg_num, uint8_t data)
{
	outb(REG_PORT, 0x80|reg_num);
	outb(DATA_PORT, data);
}

static struct tm rtc_time_nowait(void)
{
	struct tm time =
	{
		.tm_sec = read_cmos(SEC_REG),
		.tm_min = read_cmos(MIN_REG),
		.tm_hour = read_cmos(HOUR_REG),
		.tm_mday = read_cmos(DOM_REG),
		.tm_mon = read_cmos(MON_REG),
		.tm_year = read_cmos(YEAR_REG),
		.tm_wday = read_cmos(DOW_REG),
	};

	uint8_t statB = read_cmos(STATB_REG);

	if(!(statB & BIN_MODE))
	{
		time.tm_mday = (time.tm_mday & 0x0F) + ((time.tm_mday >> 4) * 10);
		time.tm_min = (time.tm_min & 0x0F) + ((time.tm_min >> 4) * 10);
		time.tm_hour = ((time.tm_hour & 0x0F) + (((time.tm_hour & 0x70) >> 4) * 10) ) | (time.tm_hour & 0x80);
		time.tm_mday = (time.tm_mday & 0x0F) + ((time.tm_mday >> 4) * 10);
		time.tm_mon = (time.tm_mon & 0x0F) + ((time.tm_mon >> 4) * 10);
		time.tm_year = (time.tm_year & 0x0F) + ((time.tm_year >> 4) * 10);
	}

	if (!(statB & H24_MODE) && (time.tm_hour & 0x80))
		time.tm_hour = ((time.tm_hour & 0x7F) + 12) % 24;

	return time;
}

struct tm rtc_time(void)
{
	// Wait for transistion from set to clear
	while(!(read_cmos(STATA_REG)&UPDATE_IN_PROGRESS));
	while(read_cmos(STATA_REG)&UPDATE_IN_PROGRESS);

	return rtc_time_nowait();
}

void rtc_handler(regs_t* r)
{
	read_cmos(STATC_REG);
}

void rtc_set_freq(uint16_t freq)
{
	if(freq >> 13)		// Max 8192Hz
		freq = (1<<13);

	uint8_t power = 15;
	while(power && ~freq&(1<<power))
		power--;

	if(freq >= 3*1<<(power-1))	// Round to next
		power++;

	write_cmos(STATA_REG, (16-power)|(read_cmos(STATA_REG)&0xF0));
}

void rtc_init(void)
{
	uint8_t statB = read_cmos(STATB_REG);
	write_cmos(STATB_REG, statB|PERIODIC_INT);
	rtc_set_freq(1000);	// Will be 1024Hz

	install_irq_handler(8, rtc_handler);
	unmask_irq(8);
}