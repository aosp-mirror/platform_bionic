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

// Memory order requirements for POSIX semaphores appear unclear and are
// currently interpreted inconsistently.
// We conservatively prefer sequentially consistent operations for now.
// CAUTION: This is more conservative than some other major implementations,
// and may change if and when the issue is resolved.

#include <semaphore.h>
#include <errno.h>
#include <limits.h>
#include <stdatomic.h>
#include <sys/time.h>
#include <time.h>

#include "private/bionic_constants.h"
#include "private/bionic_futex.h"
#include "private/bionic_sdk_version.h"
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
static inline int SEMCOUNT_TO_VALUE(unsigned int sval) {
  return (static_cast<int>(sval) >> SEMCOUNT_VALUE_SHIFT);
}

// The value +1 as a sem->count bit-pattern.
#define SEMCOUNT_ONE              SEMCOUNT_FROM_VALUE(1)

// The value -1 as a sem->count bit-pattern.
#define SEMCOUNT_MINUS_ONE        SEMCOUNT_FROM_VALUE(~0U)

#define SEMCOUNT_DECREMENT(sval)    (((sval) - (1U << SEMCOUNT_VALUE_SHIFT)) & SEMCOUNT_VALUE_MASK)
#define SEMCOUNT_INCREMENT(sval)    (((sval) + (1U << SEMCOUNT_VALUE_SHIFT)) & SEMCOUNT_VALUE_MASK)

static inline atomic_uint* SEM_TO_ATOMIC_POINTER(sem_t* sem) {
  static_assert(sizeof(atomic_uint) == sizeof(sem->count),
                "sem->count should actually be atomic_uint in implementation.");

  // We prefer casting to atomic_uint instead of declaring sem->count to be atomic_uint directly.
  // Because using the second method pollutes semaphore.h.
  return reinterpret_cast<atomic_uint*>(&sem->count);
}

// Return the shared bitflag from a semaphore counter.
static inline unsigned int SEM_GET_SHARED(atomic_uint* sem_count_ptr) {
  // memory_order_relaxed is used as SHARED flag will not be changed after init.
  return (atomic_load_explicit(sem_count_ptr, memory_order_relaxed) & SEMCOUNT_SHARED_MASK);
}

int sem_init(sem_t* sem, int pshared, unsigned int value) {
  // Ensure that 'value' can be stored in the semaphore.
  if (value > SEM_VALUE_MAX) {
    errno = EINVAL;
    return -1;
  }

  unsigned int count = SEMCOUNT_FROM_VALUE(value);
  if (pshared != 0) {
    count |= SEMCOUNT_SHARED_MASK;
  }

  atomic_uint* sem_count_ptr = SEM_TO_ATOMIC_POINTER(sem);
  atomic_init(sem_count_ptr, count);
  return 0;
}

int sem_destroy(sem_t*) {
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
static int __sem_dec(atomic_uint* sem_count_ptr) {
  unsigned int old_value = atomic_load_explicit(sem_count_ptr, memory_order_relaxed);
  unsigned int shared = old_value & SEMCOUNT_SHARED_MASK;

  // Use memory_order_seq_cst in atomic_compare_exchange operation to ensure all
  // memory access made by other threads can be seen in current thread.
  // An acquire fence may be sufficient, but it is still in discussion whether
  // POSIX semaphores should provide sequential consistency.
  do {
    if (SEMCOUNT_TO_VALUE(old_value) < 0) {
      break;
    }
  } while (!atomic_compare_exchange_weak(sem_count_ptr, &old_value,
           SEMCOUNT_DECREMENT(old_value) | shared));

  return SEMCOUNT_TO_VALUE(old_value);
}

// Same as __sem_dec, but will not touch anything if the
// value is already negative *or* 0. Returns the old value.
static int __sem_trydec(atomic_uint* sem_count_ptr) {
  unsigned int old_value = atomic_load_explicit(sem_count_ptr, memory_order_relaxed);
  unsigned int shared = old_value & SEMCOUNT_SHARED_MASK;

  // Use memory_order_seq_cst in atomic_compare_exchange operation to ensure all
  // memory access made by other threads can be seen in current thread.
  // An acquire fence may be sufficient, but it is still in discussion whether
  // POSIX semaphores should provide sequential consistency.
  do {
    if (SEMCOUNT_TO_VALUE(old_value) <= 0) {
      break;
    }
  } while (!atomic_compare_exchange_weak(sem_count_ptr, &old_value,
           SEMCOUNT_DECREMENT(old_value) | shared));

  return SEMCOUNT_TO_VALUE(old_value);
}

// "Increment" the value of a semaphore atomically and
// return its old value. Note that this implements
// the special case of "incrementing" any negative
// value to +1 directly.
//
// NOTE: The value will _not_ wrap above SEM_VALUE_MAX
static int __sem_inc(atomic_uint* sem_count_ptr) {
  unsigned int old_value = atomic_load_explicit(sem_count_ptr, memory_order_relaxed);
  unsigned int shared = old_value  & SEMCOUNT_SHARED_MASK;
  unsigned int new_value;

  // Use memory_order_seq_cst in atomic_compare_exchange operation to ensure all
  // memory access made before can be seen in other threads.
  // A release fence may be sufficient, but it is still in discussion whether
  // POSIX semaphores should provide sequential consistency.
  do {
    // Can't go higher than SEM_VALUE_MAX.
    if (SEMCOUNT_TO_VALUE(old_value) == SEM_VALUE_MAX) {
      break;
    }

    // If the counter is negative, go directly to one, otherwise just increment.
    if (SEMCOUNT_TO_VALUE(old_value) < 0) {
      new_value = SEMCOUNT_ONE | shared;
    } else {
      new_value = SEMCOUNT_INCREMENT(old_value) | shared;
    }
  } while (!atomic_compare_exchange_weak(sem_count_ptr, &old_value,
           new_value));

  return SEMCOUNT_TO_VALUE(old_value);
}

int sem_wait(sem_t* sem) {
  atomic_uint* sem_count_ptr = SEM_TO_ATOMIC_POINTER(sem);
  unsigned int shared = SEM_GET_SHARED(sem_count_ptr);

  while (true) {
    if (__sem_dec(sem_count_ptr) > 0) {
      return 0;
    }

    int result = __futex_wait_ex(sem_count_ptr, shared, shared | SEMCOUNT_MINUS_ONE, false, nullptr);
    if (bionic_get_application_target_sdk_version() > 23) {
      if (result ==-EINTR) {
        errno = EINTR;
        return -1;
      }
    }
  }
}

int sem_timedwait(sem_t* sem, const timespec* abs_timeout) {
  atomic_uint* sem_count_ptr = SEM_TO_ATOMIC_POINTER(sem);

  // POSIX says we need to try to decrement the semaphore
  // before checking the timeout value. Note that if the
  // value is currently 0, __sem_trydec() does nothing.
  if (__sem_trydec(sem_count_ptr) > 0) {
    return 0;
  }

  // Check it as per POSIX.
  int result = check_timespec(abs_timeout, false);
  if (result != 0) {
    errno = result;
    return -1;
  }

  unsigned int shared = SEM_GET_SHARED(sem_count_ptr);

  while (true) {
    // Try to grab the semaphore. If the value was 0, this will also change it to -1.
    if (__sem_dec(sem_count_ptr) > 0) {
      return 0;
    }

    // Contention detected. Wait for a wakeup event.
    int result = __futex_wait_ex(sem_count_ptr, shared, shared | SEMCOUNT_MINUS_ONE, true, abs_timeout);

    // Return in case of timeout or interrupt.
    if (result == -ETIMEDOUT || result == -EINTR) {
      errno = -result;
      return -1;
    }
  }
}

int sem_post(sem_t* sem) {
  atomic_uint* sem_count_ptr = SEM_TO_ATOMIC_POINTER(sem);
  unsigned int shared = SEM_GET_SHARED(sem_count_ptr);

  int old_value = __sem_inc(sem_count_ptr);
  if (old_value < 0) {
    // Contention on the semaphore. Wake up all waiters.
    __futex_wake_ex(sem_count_ptr, shared, INT_MAX);
  } else if (old_value == SEM_VALUE_MAX) {
    // Overflow detected.
    errno = EOVERFLOW;
    return -1;
  }

  return 0;
}

int sem_trywait(sem_t* sem) {
  atomic_uint* sem_count_ptr = SEM_TO_ATOMIC_POINTER(sem);
  if (__sem_trydec(sem_count_ptr) > 0) {
    return 0;
  } else {
    errno = EAGAIN;
    return -1;
  }
}

int sem_getvalue(sem_t* sem, int* sval) {
  atomic_uint* sem_count_ptr = SEM_TO_ATOMIC_POINTER(sem);

  // Use memory_order_seq_cst in atomic_load operation.
  // memory_order_relaxed may be fine here, but it is still in discussion
  // whether POSIX semaphores should provide sequential consistency.
  int val = SEMCOUNT_TO_VALUE(atomic_load(sem_count_ptr));
  if (val < 0) {
    val = 0;
  }

  *sval = val;
  return 0;
}
