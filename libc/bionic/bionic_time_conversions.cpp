/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include "private/bionic_time_conversions.h"

#include "private/bionic_constants.h"

bool timespec_from_timeval(timespec& ts, const timeval& tv) {
  // Whole seconds can just be copied.
  ts.tv_sec = tv.tv_sec;

  // But we might overflow when converting microseconds to nanoseconds.
  if (tv.tv_usec >= 1000000 || tv.tv_usec < 0) {
    return false;
  }
  ts.tv_nsec = tv.tv_usec * 1000;
  return true;
}

void timespec_from_ms(timespec& ts, const int ms) {
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * 1000000;
}

void timeval_from_timespec(timeval& tv, const timespec& ts) {
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = ts.tv_nsec / 1000;
}

static void convert_timespec_clocks(timespec& new_time, clockid_t new_clockbase,
                                    const timespec& old_time, clockid_t old_clockbase) {
  // get reference clocks
  timespec new_clock;
  clock_gettime(new_clockbase, &new_clock);
  timespec old_clock;
  clock_gettime(old_clockbase, &old_clock);

  // compute new time by moving old delta to the new clock.
  new_time.tv_sec = old_time.tv_sec - old_clock.tv_sec + new_clock.tv_sec;
  new_time.tv_nsec = old_time.tv_nsec - old_clock.tv_nsec + new_clock.tv_nsec;

  // correct nsec to second wrap.
  if (new_time.tv_nsec >= NS_PER_S) {
    new_time.tv_nsec -= NS_PER_S;
    new_time.tv_sec += 1;
  } else if (new_time.tv_nsec < 0) {
    new_time.tv_nsec += NS_PER_S;
    new_time.tv_sec -= 1;
  }
}

void monotonic_time_from_realtime_time(timespec& monotonic_time, const timespec& realtime_time) {
  convert_timespec_clocks(monotonic_time, CLOCK_MONOTONIC, realtime_time, CLOCK_REALTIME);
}

void realtime_time_from_monotonic_time(timespec& realtime_time, const timespec& monotonic_time) {
  convert_timespec_clocks(realtime_time, CLOCK_REALTIME, monotonic_time, CLOCK_MONOTONIC);
}
