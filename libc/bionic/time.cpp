/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <time.h>

static clockid_t __base_to_clock(int base) {
  switch (base) {
    case TIME_UTC:
      return CLOCK_REALTIME;
    case TIME_MONOTONIC:
      return CLOCK_MONOTONIC;
    case TIME_ACTIVE:
      return CLOCK_PROCESS_CPUTIME_ID;
    case TIME_THREAD_ACTIVE:
      return CLOCK_THREAD_CPUTIME_ID;
    default:
      return -1;
  }
}

int timespec_get(timespec* ts, int base) {
  clockid_t clock = __base_to_clock(base);
  return (clock != -1 && clock_gettime(clock, ts) != -1) ? base : 0;
}

int timespec_getres(timespec* ts, int base) {
  clockid_t clock = __base_to_clock(base);
  return (clock != -1 && clock_getres(clock, ts) != -1) ? base : 0;
}
