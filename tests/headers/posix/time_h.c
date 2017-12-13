/*
 * Copyright (C) 2017 The Android Open Source Project
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

#if !defined(DO_NOT_INCLUDE_TIME_H)
#include <time.h>
#endif

#include "header_checks.h"

static void time_h() {
  TYPE(clock_t);
  TYPE(size_t);
  TYPE(time_t);

  TYPE(clockid_t);
  TYPE(timer_t);

  TYPE(locale_t);

  TYPE(pid_t);

  TYPE(struct tm);
  STRUCT_MEMBER(struct tm, int, tm_sec);
  STRUCT_MEMBER(struct tm, int, tm_min);
  STRUCT_MEMBER(struct tm, int, tm_hour);
  STRUCT_MEMBER(struct tm, int, tm_mday);
  STRUCT_MEMBER(struct tm, int, tm_mon);
  STRUCT_MEMBER(struct tm, int, tm_year);
  STRUCT_MEMBER(struct tm, int, tm_wday);
  STRUCT_MEMBER(struct tm, int, tm_yday);
  STRUCT_MEMBER(struct tm, int, tm_isdst);

  TYPE(struct timespec);
  STRUCT_MEMBER(struct timespec, time_t, tv_sec);
  STRUCT_MEMBER(struct timespec, long, tv_nsec);

  TYPE(struct itimerspec);
  STRUCT_MEMBER(struct itimerspec, struct timespec, it_interval);
  STRUCT_MEMBER(struct itimerspec, struct timespec, it_value);

  MACRO(NULL);
  MACRO(CLOCKS_PER_SEC);

  MACRO(CLOCK_MONOTONIC);
  MACRO(CLOCK_PROCESS_CPUTIME_ID);
  MACRO(CLOCK_REALTIME);
  MACRO(CLOCK_THREAD_CPUTIME_ID);

  MACRO(TIMER_ABSTIME);

  FUNCTION(asctime, char* (*f)(const struct tm*));
  FUNCTION(asctime_r, char* (*f)(const struct tm*, char*));
  FUNCTION(clock, clock_t (*f)(void));
  FUNCTION(clock_getcpuclockid, int (*f)(pid_t, clockid_t*));
  FUNCTION(clock_getres, int (*f)(clockid_t, struct timespec*));
  FUNCTION(clock_gettime, int (*f)(clockid_t, struct timespec*));
  FUNCTION(clock_nanosleep, int (*f)(clockid_t, int, const struct timespec*, struct timespec*));
  FUNCTION(clock_settime, int (*f)(clockid_t, const struct timespec*));
  FUNCTION(ctime, char* (*f)(const time_t*));
  FUNCTION(ctime_r, char* (*f)(const time_t*, char*));
  FUNCTION(difftime, double (*f)(time_t, time_t));
#if !defined(__BIONIC__)
  FUNCTION(getdate, struct tm* (*f)(const char*));
#endif
  FUNCTION(gmtime, struct tm* (*f)(const time_t*));
  FUNCTION(gmtime_r, struct tm* (*f)(const time_t*, struct tm*));
  FUNCTION(localtime, struct tm* (*f)(const time_t*));
  FUNCTION(localtime_r, struct tm* (*f)(const time_t*, struct tm*));
  FUNCTION(mktime, time_t (*f)(struct tm*));
  FUNCTION(nanosleep, int (*f)(const struct timespec*, struct timespec*));
  FUNCTION(strftime, size_t (*f)(char*, size_t, const char*, const struct tm*));
  FUNCTION(strftime_l, size_t (*f)(char*, size_t, const char*, const struct tm*, locale_t));
  FUNCTION(strptime, char* (*f)(const char*, const char*, struct tm*));
  FUNCTION(time, time_t (*f)(time_t*));
  FUNCTION(timer_create, int (*f)(clockid_t, struct sigevent*, timer_t*));
  FUNCTION(timer_delete, int (*f)(timer_t));
  FUNCTION(timer_getoverrun, int (*f)(timer_t));
  FUNCTION(timer_gettime, int (*f)(timer_t, struct itimerspec*));
  FUNCTION(timer_settime, int (*f)(timer_t, int, const struct itimerspec*, struct itimerspec*));
  FUNCTION(tzset, void (*f)(void));

  int i = daylight;
  long l = timezone;
  char** sp = tzname;
}
