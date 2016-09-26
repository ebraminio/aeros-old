/**
 * \file
 */

#ifndef TIME_H
#define TIME_H

#include <sys/types.h>

struct sigevent;

struct tm
{
	int tm_sec;		// seconds [0,61]
	int tm_min;		// minutes [0,59]
	int tm_hour;	// hour [0,23]
	int tm_mday;	// day of month [1,31]
	int tm_mon;		// month of year [0,11]
	int tm_year;	// years since 1900
	int tm_wday;	// day of week [0,6] (Sunday = 0)
	int tm_yday;	// day of year [0,365]
	int tm_isdst;	// daylight savings flag
};

struct timespec
{
	time_t tv_sec;	// seconds
	long tv_nsec;	// nanoseconds
};

struct itimerspec
{
	struct timespec it_interval;	// timer period
	struct timespec it_value;		// timer expiration
};

char* asctime(const struct tm* tm);
char* asctime_r(const struct tm* tm, char* buf);
clock_t clock(void);
int clock_getres(clockid_t clk_id, struct timespec* res);
int clock_gettime(clockid_t clk_id, struct timespec* tp);
int clock_settime(clockid_t clk_id, const struct timespec* tp);
char* ctime(const time_t* timep);
char* ctime_r(const time_t* timep, char* buf);
double difftime(time_t, time_t);
struct tm* getdate(const char *);
struct tm* gmtime(const time_t* timep);
struct tm* gmtime_r(const time_t* timep, struct tm* result);
struct tm* localtime(const time_t* timep);
struct tm* localtime_r(const time_t* timep, struct tm* result);
time_t mktime(struct tm* tm);
int nanosleep(const struct timespec *, struct timespec *);
size_t strftime(char *, size_t, const char *, const struct tm *);
char* strptime(const char *, const char *, struct tm *);
time_t time(time_t *);
int timer_create(clockid_t, struct sigevent *, timer_t *);
int timer_delete(timer_t);
int timer_gettime(timer_t, struct itimerspec *);
int timer_getoverrun(timer_t);
int timer_settime(timer_t, int, const struct itimerspec *, struct itimerspec *);
void tzset(void);

extern int daylight;
extern long int timezone;
extern char* tzname[];

#endif