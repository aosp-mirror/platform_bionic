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

#include <semaphore.h>
#include <errno.h>
#include <limits.h>
#include <sys/time.h>
#include <time.h>

#include "private/bionic_atomic_inline.h"
#include "private/bionic_constants.h"
#include "private/bionic_futex.h"
#include "private/bionic_time_conversions.h"

// In this implementation, a semaphore contains a
// 31-bit signed value and a 1-bit 'shared' flag
// (for process-sharing purpose).
//
// We use the value -1 to indicate contention on the
// semaphore, 0 or more to indicate uncontended state,
// any value lower than -2 is invalid at runtime.
//
// State diagram:
//
// post(1)  ==> 2
// post(0)  ==> 1
// post(-1) ==> 1, then wake all waiters
//
// wait(2)  ==> 1
// wait(1)  ==> 0
// wait(0)  ==> -1 then wait for a wake up + loop
// wait(-1) ==> -1 then wait for a wake up + loop

// Use the upper 31-bits for the counter, and the lower one
// for the shared flag.
#define SEMCOUNT_SHARED_MASK      0x00000001
#define SEMCOUNT_VALUE_MASK       0xfffffffe
#define SEMCOUNT_VALUE_SHIFT      1

// Convert a value into the corresponding sem->count bit pattern.
#define SEMCOUNT_FROM_VALUE(val)    (((val) << SEMCOUNT_VALUE_SHIFT) & SEMCOUNT_VALUE_MASK)

// Convert a sem->count bit pattern into the corresponding signed value.
static inline int SEMCOUNT_TO_VALUE(uint32_t sval) {
  return (static_cast<int>(sval) >> SEMCOUNT_VALUE_SHIFT);
}

// The value +1 as a sem->count bit-pattern.
#define SEMCOUNT_ONE              SEMCOUNT_FROM_VALUE(1)

// The value -1 as a sem->count bit-pattern.
#define SEMCOUNT_MINUS_ONE        SEMCOUNT_FROM_VALUE(-1)

#define SEMCOUNT_DECREMENT(sval)    (((sval) - (1U << SEMCOUNT_VALUE_SHIFT)) & SEMCOUNT_VALUE_MASK)
#define SEMCOUNT_INCREMENT(sval)    (((sval) + (1U << SEMCOUNT_VALUE_SHIFT)) & SEMCOUNT_VALUE_MASK)

// Return the shared bitflag from a semaphore.
static inline uint32_t SEM_GET_SHARED(sem_t* sem) {
  return (sem->count & SEMCOUNT_SHARED_MASK);
}


int sem_init(sem_t* sem, int pshared, unsigned int value) {
  if (sem == NULL) {
    errno = EINVAL;
    return -1;
  }

  // Ensure that 'value' can be stored in the semaphore.
  if (value > SEM_VALUE_MAX) {
    errno = EINVAL;
    return -1;
  }

  sem->count = SEMCOUNT_FROM_VALUE(value);
  if (pshared != 0) {
    sem->count |= SEMCOUNT_SHARED_MASK;
  }
  return 0;
}

int sem_destroy(sem_t* sem) {
  if (sem == NULL) {
    errno = EINVAL;
    return -1;
  }
  return 0;
}

sem_t* sem_open(const char*, int, ...) {
  errno = ENOSYS;
  return SEM_FAILED;
}

int sem_close(sem_t*) {
  errno = ENOSYS;
  return -1;
}

int sem_unlink(const char*) {
  errno = ENOSYS;
  return -1;
}

// Decrement a semaphore's value atomically,
// and return the old one. As a special case,
// this returns immediately if the value is
// negative (i.e. -1)
static int __sem_dec(volatile uint32_t* sem) {
  volatile int32_t* ptr = reinterpret_cast<volatile int32_t*>(sem);
  uint32_t shared = (*sem & SEMCOUNT_SHARED_MASK);
  uint32_t old_value, new_value;
  int ret;

  do {
    old_value = (*sem & SEMCOUNT_VALUE_MASK);
    ret = SEMCOUNT_TO_VALUE(old_value);
    if (ret < 0) {
      break;
    }

    new_value = SEMCOUNT_DECREMENT(old_value);
  } while (__bionic_cmpxchg((old_value|shared), (new_value|shared), ptr) != 0);

  return ret;
}

// Same as __sem_dec, but will not touch anything if the
// value is already negative *or* 0. Returns the old value.
static int __sem_trydec(volatile uint32_t* sem) {
  volatile int32_t* ptr = reinterpret_cast<volatile int32_t*>(sem);
  uint32_t shared = (*sem & SEMCOUNT_SHARED_MASK);
  uint32_t old_value, new_value;
  int          ret;

  do {
    old_value = (*sem & SEMCOUNT_VALUE_MASK);
    ret = SEMCOUNT_TO_VALUE(old_value);
    if (ret <= 0) {
      break;
    }

    new_value = SEMCOUNT_DECREMENT(old_value);
  } while (__bionic_cmpxchg((old_value|shared), (new_value|shared), ptr) != 0);

  return ret;
}


// "Increment" the value of a semaphore atomically and
// return its old value. Note that this implements
// the special case of "incrementing" any negative
// value to +1 directly.
//
// NOTE: The value will _not_ wrap above SEM_VALUE_MAX
static int __sem_inc(volatile uint32_t* sem) {
  volatile int32_t* ptr = reinterpret_cast<volatile int32_t*>(sem);
  uint32_t shared = (*sem & SEMCOUNT_SHARED_MASK);
  uint32_t old_value, new_value;
  int ret;

  do {
    old_value = (*sem & SEMCOUNT_VALUE_MASK);
    ret = SEMCOUNT_TO_VALUE(old_value);

    // Can't go higher than SEM_VALUE_MAX.
    if (ret == SEM_VALUE_MAX) {
      break;
    }

    // If the counter is negative, go directly to +1, otherwise just increment.
    if (ret < 0) {
        new_value = SEMCOUNT_ONE;
    } else {
      new_value = SEMCOUNT_INCREMENT(old_value);
    }
  } while (__bionic_cmpxchg((old_value|shared), (new_value|shared), ptr) != 0);

  return ret;
}

int sem_wait(sem_t* sem) {
  if (sem == NULL) {
    errno = EINVAL;
    return -1;
  }

  uint32_t shared = SEM_GET_SHARED(sem);

  while (true) {
    if (__sem_dec(&sem->count) > 0) {
      ANDROID_MEMBAR_FULL();
      return 0;
    }

    __futex_wait_ex(&sem->count, shared, shared|SEMCOUNT_MINUS_ONE, NULL);
  }
}

int sem_timedwait(sem_t* sem, const timespec* abs_timeout) {
  if (sem == NULL) {
    errno = EINVAL;
    return -1;
  }

  // POSIX says we need to try to decrement the semaphore
  // before checking the timeout value. Note that if the
  // value is currently 0, __sem_trydec() does nothing.
  if (__sem_trydec(&sem->count) > 0) {
    ANDROID_MEMBAR_FULL();
    return 0;
  }

  // Check it as per POSIX.
  if (abs_timeout == NULL || abs_timeout->tv_sec < 0 || abs_timeout->tv_nsec < 0 || abs_timeout->tv_nsec >= NS_PER_S) {
    errno = EINVAL;
    return -1;
  }

  uint32_t shared = SEM_GET_SHARED(sem);

  while (true) {
    // POSIX mandates CLOCK_REALTIME here.
    timespec ts;
    if (!timespec_from_absolute_timespec(ts, *abs_timeout, CLOCK_REALTIME)) {
      errno = ETIMEDOUT;
      return -1;
    }

    // Try to grab the semaphore. If the value was 0, this will also change it to -1.
    if (__sem_dec(&sem->count) > 0) {
      ANDROID_MEMBAR_FULL();
      break;
    }

    // Contention detected. Wait for a wakeup event.
    int ret = __futex_wait_ex(&sem->count, shared, shared|SEMCOUNT_MINUS_ONE, &ts);

    // Return in case of timeout or interrupt.
    if (ret == -ETIMEDOUT || ret == -EINTR) {
      errno = -ret;
      return -1;
    }
  }
  return 0;
}

int sem_post(sem_t* sem) {
  if (sem == NULL) {
    return EINVAL;
  }

  uint32_t shared = SEM_GET_SHARED(sem);

  ANDROID_MEMBAR_FULL();
  int old_value = __sem_inc(&sem->count);
  if (old_value < 0) {
    // Contention on the semaphore. Wake up all waiters.
    __futex_wake_ex(&sem->count, shared, INT_MAX);
  } else if (old_value == SEM_VALUE_MAX) {
    // Overflow detected.
    errno = EOVERFLOW;
    return -1;
  }

  return 0;
}

int sem_trywait(sem_t* sem) {
  if (sem == NULL) {
    errno = EINVAL;
    return -1;
  }

  if (__sem_trydec(&sem->count) > 0) {
    ANDROID_MEMBAR_FULL();
    return 0;
  } else {
    errno = EAGAIN;
    return -1;
  }
}

int sem_getvalue(sem_t* sem, int* sval) {
  if (sem == NULL || sval == NULL) {
    errno = EINVAL;
    return -1;
  }

  int val = SEMCOUNT_TO_VALUE(sem->count);
  if (val < 0) {
    val = 0;
  }

  *sval = val;
  return 0;
}
