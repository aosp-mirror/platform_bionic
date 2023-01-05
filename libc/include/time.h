/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _TIME_H_
#define _TIME_H_

#include <sys/cdefs.h>
#include <sys/time.h>
#include <xlocale.h>

__BEGIN_DECLS

#define CLOCKS_PER_SEC 1000000

extern char* _Nonnull tzname[];
extern int daylight;
extern long int timezone;

struct sigevent;

struct tm {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
  long int tm_gmtoff;
  const char* _Nullable tm_zone;
};

#define TM_ZONE tm_zone

time_t time(time_t* _Nullable __t);
int nanosleep(const struct timespec* _Nonnull __request, struct timespec* _Nullable __remainder);

char* _Nullable asctime(const struct tm* _Nonnull __tm);
char* _Nullable asctime_r(const struct tm* _Nonnull __tm, char* _Nonnull __buf);

double difftime(time_t __lhs, time_t __rhs);
time_t mktime(struct tm* _Nonnull __tm);

struct tm* _Nullable localtime(const time_t* _Nonnull __t);
struct tm* _Nullable localtime_r(const time_t* _Nonnull __t, struct tm* _Nonnull __tm);

struct tm* _Nullable gmtime(const time_t* _Nonnull __t);
struct tm* _Nullable gmtime_r(const time_t* _Nonnull __t, struct tm* _Nonnull __tm);

char* _Nullable strptime(const char* _Nonnull __s, const char* _Nonnull __fmt, struct tm* _Nonnull __tm) __strftimelike(2);
char* _Nullable strptime_l(const char* _Nonnull __s, const char* _Nonnull __fmt, struct tm* _Nonnull __tm, locale_t _Nonnull __l) __strftimelike(2) __INTRODUCED_IN(28);

size_t strftime(char* _Nonnull __buf, size_t __n, const char* _Nonnull __fmt, const struct tm* _Nullable __tm) __strftimelike(3);
#if __ANDROID_API__ >= 21
size_t strftime_l(char* _Nonnull __buf, size_t __n, const char* _Nonnull __fmt, const struct tm* _Nullable __tm, locale_t _Nonnull __l) __strftimelike(3) __INTRODUCED_IN(21);
#else
// Implemented as static inline before 21.
#endif


char* _Nullable ctime(const time_t* _Nonnull __t);
char* _Nullable ctime_r(const time_t* _Nonnull __t, char* _Nonnull __buf);

void tzset(void);

clock_t clock(void);

int clock_getcpuclockid(pid_t __pid, clockid_t* _Nonnull __clock) __INTRODUCED_IN(23);


int clock_getres(clockid_t __clock, struct timespec* _Nullable __resolution);
int clock_gettime(clockid_t __clock, struct timespec* _Nonnull __ts);
int clock_nanosleep(clockid_t __clock, int __flags, const struct timespec* _Nonnull __request, struct timespec* _Nullable __remainder);
int clock_settime(clockid_t __clock, const struct timespec* _Nonnull __ts);

int timer_create(clockid_t __clock, struct sigevent* _Nullable __event, timer_t _Nonnull * _Nonnull __timer_ptr);
int timer_delete(timer_t _Nonnull __timer);
int timer_settime(timer_t _Nonnull __timer, int __flags, const struct itimerspec* _Nonnull __new_value, struct itimerspec* _Nullable __old_value);
int timer_gettime(timer_t _Nonnull _timer, struct itimerspec* _Nonnull __ts);
int timer_getoverrun(timer_t _Nonnull __timer);

/* Non-standard extensions that are in the BSDs and glibc. */
time_t timelocal(struct tm* _Nonnull __tm);
time_t timegm(struct tm* _Nonnull __tm);

#define TIME_UTC 1
int timespec_get(struct timespec* _Nonnull __ts, int __base) __INTRODUCED_IN(29);

__END_DECLS

#endif
