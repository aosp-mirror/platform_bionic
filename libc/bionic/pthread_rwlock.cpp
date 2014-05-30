/*
 * Copyright (C) 2010 The Android Open Source Project
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

#include <errno.h>

#include "pthread_internal.h"
#include "private/bionic_futex.h"

/* Technical note:
 *
 * Possible states of a read/write lock:
 *
 *  - no readers and no writer (unlocked)
 *  - one or more readers sharing the lock at the same time (read-locked)
 *  - one writer holding the lock (write-lock)
 *
 * Additionally:
 *  - trying to get the write-lock while there are any readers blocks
 *  - trying to get the read-lock while there is a writer blocks
 *  - a single thread can acquire the lock multiple times in read mode
 *
 *  - Posix states that behavior is undefined (may deadlock) if a thread tries
 *    to acquire the lock
 *      - in write mode while already holding the lock (whether in read or write mode)
 *      - in read mode while already holding the lock in write mode.
 *  - This implementation will return EDEADLK in "write after write" and "read after
 *    write" cases and will deadlock in write after read case.
 *
 * TODO: VERY CAREFULLY convert this to use C++11 atomics when possible. All volatile
 * members of pthread_rwlock_t should be converted to atomics<> and __sync_bool_compare_and_swap
 * should be changed to compare_exchange_strong accompanied by the proper ordering
 * constraints (comments have been added with the intending ordering across the code).
 *
 * TODO: As it stands now, pending_readers and pending_writers could be merged into a
 * a single waiters variable.  Keeping them separate adds a bit of clarity and keeps
 * the door open for a writer-biased implementation.
 *
 */

#define RWLOCKATTR_DEFAULT     0
#define RWLOCKATTR_SHARED_MASK 0x0010

static inline bool rwlock_is_shared(const pthread_rwlock_t* rwlock) {
  return rwlock->attr == PTHREAD_PROCESS_SHARED;
}

static bool timespec_from_absolute(timespec* rel_timeout, const timespec* abs_timeout) {
  if (abs_timeout != NULL) {
    if (__timespec_from_absolute(rel_timeout, abs_timeout, CLOCK_REALTIME) < 0) {
      return false;
    }
  }
  return true;
}

int pthread_rwlockattr_init(pthread_rwlockattr_t* attr) {
  *attr = PTHREAD_PROCESS_PRIVATE;
  return 0;
}

int pthread_rwlockattr_destroy(pthread_rwlockattr_t* attr) {
  *attr = -1;
  return 0;
}

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t* attr, int pshared) {
  switch (pshared) {
    case PTHREAD_PROCESS_PRIVATE:
    case PTHREAD_PROCESS_SHARED:
      *attr = pshared;
      return 0;
    default:
      return EINVAL;
  }
}

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t* attr, int* pshared) {
  *pshared = *attr;
  return 0;
}

int pthread_rwlock_init(pthread_rwlock_t* rwlock, const pthread_rwlockattr_t* attr) {
  if (attr != NULL) {
    switch (*attr) {
      case PTHREAD_PROCESS_SHARED:
      case PTHREAD_PROCESS_PRIVATE:
        rwlock->attr= *attr;
        break;
      default:
        return EINVAL;
    }
  }

  rwlock->state = 0;
  rwlock->pending_readers = 0;
  rwlock->pending_writers = 0;
  rwlock->writer_thread_id = 0;

  return 0;
}

int pthread_rwlock_destroy(pthread_rwlock_t* rwlock) {
  if (rwlock->state != 0) {
    return EBUSY;
  }
  return 0;
}

static int __pthread_rwlock_timedrdlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  if (__predict_false(__get_thread()->tid == rwlock->writer_thread_id)) {
    return EDEADLK;
  }

  timespec ts;
  timespec* rel_timeout = (abs_timeout == NULL) ? NULL : &ts;
  bool done = false;
  do {
    // This is actually a race read as there's nothing that guarantees the atomicity of integer
    // reads / writes. However, in practice this "never" happens so until we switch to C++11 this
    // should work fine. The same applies in the other places this idiom is used.
    int32_t cur_state = rwlock->state;  // C++11 relaxed atomic read
    if (__predict_true(cur_state >= 0)) {
      // Add as an extra reader.
      done = __sync_bool_compare_and_swap(&rwlock->state, cur_state, cur_state + 1);  // C++11 memory_order_aquire
    } else {
      if (!timespec_from_absolute(rel_timeout, abs_timeout)) {
        return ETIMEDOUT;
      }
      // Owner holds it in write mode, hang up.
      // To avoid losing wake ups the pending_readers update and the state read should be
      // sequentially consistent. (currently enforced by __sync_fetch_and_add which creates a full barrier)
      __sync_fetch_and_add(&rwlock->pending_readers, 1);  // C++11 memory_order_relaxed (if the futex_wait ensures the ordering)
      int ret = __futex_wait_ex(&rwlock->state, rwlock_is_shared(rwlock), cur_state, rel_timeout);
      __sync_fetch_and_sub(&rwlock->pending_readers, 1);  // C++11 memory_order_relaxed
      if (ret == -ETIMEDOUT) {
        return ETIMEDOUT;
      }
    }
  } while (!done);

  return 0;
}

static int __pthread_rwlock_timedwrlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  int tid = __get_thread()->tid;
  if (__predict_false(tid == rwlock->writer_thread_id)) {
    return EDEADLK;
  }

  timespec ts;
  timespec* rel_timeout = (abs_timeout == NULL) ? NULL : &ts;
  bool done = false;
  do {
    int32_t cur_state = rwlock->state;
    if (__predict_true(cur_state == 0)) {
      // Change state from 0 to -1.
      done =  __sync_bool_compare_and_swap(&rwlock->state, 0 /* cur state */, -1 /* new state */);  // C++11 memory_order_aquire
    } else {
      if (!timespec_from_absolute(rel_timeout, abs_timeout)) {
        return ETIMEDOUT;
      }
      // Failed to acquire, hang up.
      // To avoid losing wake ups the pending_writers update and the state read should be
      // sequentially consistent. (currently enforced by __sync_fetch_and_add which creates a full barrier)
      __sync_fetch_and_add(&rwlock->pending_writers, 1);  // C++11 memory_order_relaxed (if the futex_wait ensures the ordering)
      int ret = __futex_wait_ex(&rwlock->state, rwlock_is_shared(rwlock), cur_state, rel_timeout);
      __sync_fetch_and_sub(&rwlock->pending_writers, 1);  // C++11 memory_order_relaxed
      if (ret == -ETIMEDOUT) {
        return ETIMEDOUT;
      }
    }
  } while (!done);

  rwlock->writer_thread_id = tid;
  return 0;
}

int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock) {
  return __pthread_rwlock_timedrdlock(rwlock, NULL);
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  return __pthread_rwlock_timedrdlock(rwlock, abs_timeout);
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock) {
  int32_t cur_state = rwlock->state;
  if ((cur_state >= 0) &&
      __sync_bool_compare_and_swap(&rwlock->state, cur_state, cur_state + 1)) {  // C++11 memory_order_acquire
    return 0;
  }
  return EBUSY;
}

int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock) {
  return __pthread_rwlock_timedwrlock(rwlock, NULL);
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  return __pthread_rwlock_timedwrlock(rwlock, abs_timeout);
}

int pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock) {
  int tid = __get_thread()->tid;
  int32_t cur_state = rwlock->state;
  if ((cur_state == 0) &&
      __sync_bool_compare_and_swap(&rwlock->state, 0 /* cur state */, -1 /* new state */)) {  // C++11 memory_order_acquire
    rwlock->writer_thread_id = tid;
    return 0;
  }
  return EBUSY;
}


int pthread_rwlock_unlock(pthread_rwlock_t* rwlock) {
  int tid = __get_thread()->tid;
  bool done = false;
  do {
    int32_t cur_state = rwlock->state;
    if (cur_state == 0) {
      return EPERM;
    }
    if (cur_state == -1) {
      if (rwlock->writer_thread_id != tid) {
        return EPERM;
      }
      // We're no longer the owner.
      rwlock->writer_thread_id = 0;
      // Change state from -1 to 0.
      // We use __sync_bool_compare_and_swap to achieve sequential consistency of the state store and
      // the following pendingX loads. A simple store with memory_order_release semantics
      // is not enough to guarantee that the pendingX loads are not reordered before the
      // store (which may lead to a lost wakeup).
      __sync_bool_compare_and_swap( &rwlock->state, -1 /* cur state*/, 0 /* new state */);  // C++11 maybe memory_order_seq_cst?

      // Wake any waiters.
      if (__predict_false(rwlock->pending_readers > 0 || rwlock->pending_writers > 0)) {
        __futex_wake_ex(&rwlock->state, rwlock_is_shared(rwlock), INT_MAX);
      }
      done = true;
    } else { // cur_state > 0
      // Reduce state by 1.
      // See the comment above on why we need __sync_bool_compare_and_swap.
      done = __sync_bool_compare_and_swap(&rwlock->state, cur_state, cur_state - 1);  // C++11 maybe memory_order_seq_cst?
      if (done && (cur_state - 1) == 0) {
        // There are no more readers, wake any waiters.
        if (__predict_false(rwlock->pending_readers > 0 || rwlock->pending_writers > 0)) {
          __futex_wake_ex(&rwlock->state, rwlock_is_shared(rwlock), INT_MAX);
        }
      }
    }
  } while (!done);

  return 0;
}
