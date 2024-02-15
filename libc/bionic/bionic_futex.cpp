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

#include "private/bionic_futex.h"

#include <stdatomic.h>
#include <time.h>

#include "private/bionic_time_conversions.h"

static inline __always_inline int FutexWithTimeout(volatile void* ftx, int op, int value,
                                                   bool use_realtime_clock,
                                                   const timespec* abs_timeout, int bitset) {
  // pthread's and semaphore's default behavior is to use CLOCK_REALTIME, however this behavior is
  // essentially never intended, as that clock is prone to change discontinuously.
  //
  // What users really intend is to use CLOCK_MONOTONIC, however only pthread_cond_timedwait()
  // provides this as an option and even there, a large amount of existing code does not opt into
  // CLOCK_MONOTONIC.
  //
  // We have seen numerous bugs directly attributable to this difference.  Therefore, we provide
  // this general workaround to always use CLOCK_MONOTONIC for waiting, regardless of what the input
  // timespec is.
  timespec converted_timeout;
  if (abs_timeout) {
    if ((op & FUTEX_CMD_MASK) == FUTEX_LOCK_PI) {
      if (!use_realtime_clock) {
        realtime_time_from_monotonic_time(converted_timeout, *abs_timeout);
        abs_timeout = &converted_timeout;
      }
    } else {
      op &= ~FUTEX_CLOCK_REALTIME;
      if (use_realtime_clock) {
        monotonic_time_from_realtime_time(converted_timeout, *abs_timeout);
        abs_timeout = &converted_timeout;
      }
    }
    if (abs_timeout->tv_sec < 0) {
      return -ETIMEDOUT;
    }
  }

  return __futex(ftx, op, value, abs_timeout, bitset);
}

int __futex_wait_ex(volatile void* ftx, bool shared, int value, bool use_realtime_clock,
                    const timespec* abs_timeout) {
  return FutexWithTimeout(ftx, (shared ? FUTEX_WAIT_BITSET : FUTEX_WAIT_BITSET_PRIVATE), value,
                          use_realtime_clock, abs_timeout, FUTEX_BITSET_MATCH_ANY);
}

int __futex_pi_lock_ex(volatile void* ftx, bool shared, bool use_realtime_clock,
                       const timespec* abs_timeout) {
  // We really want FUTEX_LOCK_PI2 which is default CLOCK_MONOTONIC, but that isn't supported
  // on linux before 5.14.  FUTEX_LOCK_PI uses CLOCK_REALTIME.  Here we verify support.

  static atomic_int lock_op = 0;
  int op = atomic_load_explicit(&lock_op, memory_order_relaxed);
  if (op == 0) {
    uint32_t tmp = 0;
    if (__futex(&tmp, FUTEX_LOCK_PI2, 0, nullptr, 0) == 0) {
      __futex(&tmp, FUTEX_UNLOCK_PI, 0, nullptr, 0);
      op = FUTEX_LOCK_PI2;
    } else {
      op = FUTEX_LOCK_PI;
    }
    atomic_store_explicit(&lock_op, op, memory_order_relaxed);
  }

  if (!shared) op |= FUTEX_PRIVATE_FLAG;
  return FutexWithTimeout(ftx, op, 0 /* value */, use_realtime_clock, abs_timeout, 0 /* bitset */);
}
