#ifndef __DEVICES_RTC_H__
#define __DEVICES_RTC_H__

#include <time.h>

void rtc_init(void);
struct tm rtc_time(void);

#endif