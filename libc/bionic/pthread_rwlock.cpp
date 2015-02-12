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
#include <stdatomic.h>

#include "pthread_internal.h"
#include "private/bionic_futex.h"
#include "private/bionic_time_conversions.h"

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
    if (!timespec_from_absolute_timespec(*rel_timeout, *abs_timeout, CLOCK_REALTIME)) {
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

static inline atomic_int* STATE_ATOMIC_POINTER(pthread_rwlock_t* rwlock) {
    static_assert(sizeof(atomic_int) == sizeof(rwlock->state),
                  "rwlock->state should actually be atomic_int in implementation.");

    // We prefer casting to atomic_int instead of declaring rwlock->state to be atomic_int directly.
    // Because using the second method pollutes pthread.h, and causes an error when compiling libcxx.
    return reinterpret_cast<atomic_int*>(&rwlock->state);
}

static inline atomic_int* WRITER_THREAD_ID_ATOMIC_POINTER(pthread_rwlock_t* rwlock) {
    static_assert(sizeof(atomic_int) == sizeof(rwlock->writer_thread_id),
                  "rwlock->writer_thread_id should actually be atomic_int in implementation.");

    return reinterpret_cast<atomic_int*>(&rwlock->writer_thread_id);
}

static inline atomic_uint* PENDING_READERS_ATOMIC_POINTER(pthread_rwlock_t* rwlock) {
    static_assert(sizeof(atomic_uint) == sizeof(rwlock->pending_readers),
                  "rwlock->pending_readers should actually be atomic_uint in implementation.");

    return reinterpret_cast<atomic_uint*>(&rwlock->pending_readers);
}

static inline atomic_uint* PENDING_WRITERS_ATOMIC_POINTER(pthread_rwlock_t* rwlock) {
    static_assert(sizeof(atomic_uint) == sizeof(rwlock->pending_writers),
                  "rwlock->pending_writers should actually be atomic_uint in implementation.");

    return reinterpret_cast<atomic_uint*>(&rwlock->pending_writers);
}

int pthread_rwlock_init(pthread_rwlock_t* rwlock, const pthread_rwlockattr_t* attr) {
  if (__predict_true(attr == NULL)) {
    rwlock->attr = 0;
  } else {
    switch (*attr) {
      case PTHREAD_PROCESS_SHARED:
      case PTHREAD_PROCESS_PRIVATE:
        rwlock->attr= *attr;
        break;
      default:
        return EINVAL;
    }
  }

  atomic_init(STATE_ATOMIC_POINTER(rwlock), 0);
  atomic_init(WRITER_THREAD_ID_ATOMIC_POINTER(rwlock), 0);
  atomic_init(PENDING_READERS_ATOMIC_POINTER(rwlock), 0);
  atomic_init(PENDING_WRITERS_ATOMIC_POINTER(rwlock), 0);

  return 0;
}

int pthread_rwlock_destroy(pthread_rwlock_t* rwlock) {
  if (rwlock->state != 0) {
    return EBUSY;
  }
  return 0;
}

static int __pthread_rwlock_timedrdlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  if (__predict_false(__get_thread()->tid ==
      atomic_load_explicit(WRITER_THREAD_ID_ATOMIC_POINTER(rwlock), memory_order_relaxed))) {
    return EDEADLK;
  }

  timespec ts;
  timespec* rel_timeout = (abs_timeout == NULL) ? NULL : &ts;

  atomic_int* state_ptr = STATE_ATOMIC_POINTER(rwlock);

  while (true) {
    int cur_state = atomic_load_explicit(state_ptr, memory_order_relaxed);
    if (__predict_true(cur_state >= 0)) {
      if (atomic_compare_exchange_weak_explicit(state_ptr, &cur_state, cur_state + 1,
                                                memory_order_acquire, memory_order_relaxed)) {
        return 0;
      }
    } else {
      if (!timespec_from_absolute(rel_timeout, abs_timeout)) {
        return ETIMEDOUT;
      }
      atomic_uint* pending_readers_ptr = PENDING_READERS_ATOMIC_POINTER(rwlock);

      // To avoid losing wake ups, the pending_readers increment should be observed before
      // futex_wait by all threads. A seq_cst fence instead of a seq_cst operation is used
      // here. Because only a seq_cst fence can ensure sequential consistency for non-atomic
      // operations in futex_wait.
      atomic_fetch_add_explicit(pending_readers_ptr, 1, memory_order_relaxed);
      atomic_thread_fence(memory_order_seq_cst);
      int ret = __futex_wait_ex(state_ptr, rwlock_is_shared(rwlock), cur_state, rel_timeout);
      atomic_fetch_sub_explicit(pending_readers_ptr, 1, memory_order_relaxed);
      if (ret == -ETIMEDOUT) {
        return ETIMEDOUT;
      }
    }
  }
}

static int __pthread_rwlock_timedwrlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  if (__predict_false(__get_thread()->tid ==
      atomic_load_explicit(WRITER_THREAD_ID_ATOMIC_POINTER(rwlock), memory_order_relaxed))) {
    return EDEADLK;
  }

  timespec ts;
  timespec* rel_timeout = (abs_timeout == NULL) ? NULL : &ts;

  atomic_int* state_ptr = STATE_ATOMIC_POINTER(rwlock);

  while (true) {
    int cur_state = atomic_load_explicit(state_ptr, memory_order_relaxed);
    if (__predict_true(cur_state == 0)) {
      if (atomic_compare_exchange_weak_explicit(state_ptr, &cur_state, -1,
                                                memory_order_acquire, memory_order_relaxed)) {
        // writer_thread_id is protected by rwlock and can only be modified in rwlock write
        // owner thread. Other threads may read it for EDEADLK error checking, atomic operation
        // is safe enough for it.
        atomic_store_explicit(WRITER_THREAD_ID_ATOMIC_POINTER(rwlock), __get_thread()->tid,
                              memory_order_relaxed);
        return 0;
      }
    } else {
      if (!timespec_from_absolute(rel_timeout, abs_timeout)) {
        return ETIMEDOUT;
      }

      atomic_uint* pending_writers_ptr = PENDING_WRITERS_ATOMIC_POINTER(rwlock);

      // To avoid losing wake ups, the pending_writers increment should be observed before
      // futex_wait by all threads. A seq_cst fence instead of a seq_cst operation is used
      // here. Because only a seq_cst fence can ensure sequential consistency for non-atomic
      // operations in futex_wait.
      atomic_fetch_add_explicit(pending_writers_ptr, 1, memory_order_relaxed);
      atomic_thread_fence(memory_order_seq_cst);
      int ret = __futex_wait_ex(state_ptr, rwlock_is_shared(rwlock), cur_state, rel_timeout);
      atomic_fetch_sub_explicit(pending_writers_ptr, 1, memory_order_relaxed);
      if (ret == -ETIMEDOUT) {
        return ETIMEDOUT;
      }
    }
  }
}

int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock) {
  return __pthread_rwlock_timedrdlock(rwlock, NULL);
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  return __pthread_rwlock_timedrdlock(rwlock, abs_timeout);
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock) {
  atomic_int* state_ptr = STATE_ATOMIC_POINTER(rwlock);
  int cur_state = atomic_load_explicit(state_ptr, memory_order_relaxed);

  while (cur_state >= 0) {
    if (atomic_compare_exchange_weak_explicit(state_ptr, &cur_state, cur_state + 1,
                                              memory_order_acquire, memory_order_relaxed)) {
      return 0;
    }
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
  atomic_int* state_ptr = STATE_ATOMIC_POINTER(rwlock);
  int cur_state = atomic_load_explicit(state_ptr, memory_order_relaxed);

  while (cur_state == 0) {
    if (atomic_compare_exchange_weak_explicit(state_ptr, &cur_state, -1,
                                              memory_order_acquire, memory_order_relaxed)) {
      int tid = __get_thread()->tid;
      atomic_store_explicit(WRITER_THREAD_ID_ATOMIC_POINTER(rwlock), tid, memory_order_relaxed);
      return 0;
    }
  }
  return EBUSY;
}


int pthread_rwlock_unlock(pthread_rwlock_t* rwlock) {
  int tid = __get_thread()->tid;
  atomic_int* state_ptr = STATE_ATOMIC_POINTER(rwlock);
  atomic_uint* pending_readers_ptr = PENDING_READERS_ATOMIC_POINTER(rwlock);
  atomic_uint* pending_writers_ptr = PENDING_WRITERS_ATOMIC_POINTER(rwlock);

  int cur_state = atomic_load_explicit(state_ptr, memory_order_relaxed);
  if (__predict_false(cur_state == 0)) {
    return EPERM;
  } else if (cur_state == -1) {
    atomic_int* writer_thread_id_ptr = WRITER_THREAD_ID_ATOMIC_POINTER(rwlock);
    if (atomic_load_explicit(writer_thread_id_ptr, memory_order_relaxed) != tid) {
      return EPERM;
    }
    // We're no longer the owner.
    atomic_store_explicit(writer_thread_id_ptr, 0, memory_order_relaxed);
    // Change state from -1 to 0.
    atomic_store_explicit(state_ptr, 0, memory_order_release);
    goto wakeup_waiters;

  } else { // cur_state > 0
    // Reduce state by 1.
    while (!atomic_compare_exchange_weak_explicit(state_ptr, &cur_state, cur_state - 1,
                                                  memory_order_release, memory_order_relaxed)) {
      if (cur_state <= 0) {
        return EPERM;
      }
    }
    if (cur_state == 1) {
      goto wakeup_waiters;
    }
  }
  return 0;

wakeup_waiters:
  // To avoid losing wake ups, the update of state should be observed before reading
  // pending_readers/pending_writers by all threads. Use read locking as an example:
  //     read locking thread                        unlocking thread
  //      pending_readers++;                         state = 0;
  //      seq_cst fence                              seq_cst fence
  //      read state for futex_wait                  read pending_readers for futex_wake
  //
  // So when locking and unlocking threads are running in parallel, we will not get
  // in a situation that the locking thread reads state as negative and needs to wait,
  // while the unlocking thread reads pending_readers as zero and doesn't need to wake up waiters.
  atomic_thread_fence(memory_order_seq_cst);
  if (__predict_false(atomic_load_explicit(pending_readers_ptr, memory_order_relaxed) > 0 ||
                      atomic_load_explicit(pending_writers_ptr, memory_order_relaxed) > 0)) {
    __futex_wake_ex(state_ptr, rwlock_is_shared(rwlock), INT_MAX);
  }
  return 0;
}
