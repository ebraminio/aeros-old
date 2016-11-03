#ifndef __DEVICES_RTC_H__
#define __DEVICES_RTC_H__

#include <time.h>
#include <stdint.h>

void rtc_init(void);
struct tm rtc_time(void);
void rtc_wait(uint16_t millis);

#endif