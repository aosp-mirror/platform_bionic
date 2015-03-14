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

struct pthread_rwlock_internal_t {
  atomic_int state; // 0=unlock, -1=writer lock, +n=reader lock
  atomic_int writer_thread_id;
  atomic_uint pending_readers;
  atomic_uint pending_writers;
  int32_t attr;

  bool process_shared() const {
    return attr == PTHREAD_PROCESS_SHARED;
  }

#if defined(__LP64__)
  char __reserved[36];
#else
  char __reserved[20];
#endif
};

static inline pthread_rwlock_internal_t* __get_internal_rwlock(pthread_rwlock_t* rwlock_interface) {
  static_assert(sizeof(pthread_rwlock_t) == sizeof(pthread_rwlock_internal_t),
                "pthread_rwlock_t should actually be pthread_rwlock_internal_t in implementation.");
  return reinterpret_cast<pthread_rwlock_internal_t*>(rwlock_interface);
}

int pthread_rwlock_init(pthread_rwlock_t* rwlock_interface, const pthread_rwlockattr_t* attr) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

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

  atomic_init(&rwlock->state, 0);
  atomic_init(&rwlock->writer_thread_id, 0);
  atomic_init(&rwlock->pending_readers, 0);
  atomic_init(&rwlock->pending_writers, 0);

  return 0;
}

int pthread_rwlock_destroy(pthread_rwlock_t* rwlock_interface) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  if (atomic_load_explicit(&rwlock->state, memory_order_relaxed) != 0) {
    return EBUSY;
  }
  return 0;
}

static int __pthread_rwlock_timedrdlock(pthread_rwlock_internal_t* rwlock,
                                        const timespec* abs_timeout_or_null) {

  if (__predict_false(__get_thread()->tid == atomic_load_explicit(&rwlock->writer_thread_id,
                                                                  memory_order_relaxed))) {
    return EDEADLK;
  }

  while (true) {
    int old_state = atomic_load_explicit(&rwlock->state, memory_order_relaxed);
    if (__predict_true(old_state >= 0)) {
      if (atomic_compare_exchange_weak_explicit(&rwlock->state, &old_state, old_state + 1,
                                                memory_order_acquire, memory_order_relaxed)) {
        return 0;
      }
    } else {
      timespec ts;
      timespec* rel_timeout = NULL;

      if (abs_timeout_or_null != NULL) {
        rel_timeout = &ts;
        if (!timespec_from_absolute_timespec(*rel_timeout, *abs_timeout_or_null, CLOCK_REALTIME)) {
          return ETIMEDOUT;
        }
      }

      // To avoid losing wake ups, the pending_readers increment should be observed before
      // futex_wait by all threads. A seq_cst fence instead of a seq_cst operation is used
      // here. Because only a seq_cst fence can ensure sequential consistency for non-atomic
      // operations in futex_wait.
      atomic_fetch_add_explicit(&rwlock->pending_readers, 1, memory_order_relaxed);

      atomic_thread_fence(memory_order_seq_cst);

      int ret = __futex_wait_ex(&rwlock->state, rwlock->process_shared(), old_state,
                                rel_timeout);

      atomic_fetch_sub_explicit(&rwlock->pending_readers, 1, memory_order_relaxed);

      if (ret == -ETIMEDOUT) {
        return ETIMEDOUT;
      }
    }
  }
}

static int __pthread_rwlock_timedwrlock(pthread_rwlock_internal_t* rwlock,
                                        const timespec* abs_timeout_or_null) {

  if (__predict_false(__get_thread()->tid == atomic_load_explicit(&rwlock->writer_thread_id,
                                                                  memory_order_relaxed))) {
    return EDEADLK;
  }

  while (true) {
    int old_state = atomic_load_explicit(&rwlock->state, memory_order_relaxed);
    if (__predict_true(old_state == 0)) {
      if (atomic_compare_exchange_weak_explicit(&rwlock->state, &old_state, -1,
                                                memory_order_acquire, memory_order_relaxed)) {
        // writer_thread_id is protected by rwlock and can only be modified in rwlock write
        // owner thread. Other threads may read it for EDEADLK error checking, atomic operation
        // is safe enough for it.
        atomic_store_explicit(&rwlock->writer_thread_id, __get_thread()->tid, memory_order_relaxed);
        return 0;
      }
    } else {
      timespec ts;
      timespec* rel_timeout = NULL;

      if (abs_timeout_or_null != NULL) {
        rel_timeout = &ts;
        if (!timespec_from_absolute_timespec(*rel_timeout, *abs_timeout_or_null, CLOCK_REALTIME)) {
          return ETIMEDOUT;
        }
      }

      // To avoid losing wake ups, the pending_writers increment should be observed before
      // futex_wait by all threads. A seq_cst fence instead of a seq_cst operation is used
      // here. Because only a seq_cst fence can ensure sequential consistency for non-atomic
      // operations in futex_wait.
      atomic_fetch_add_explicit(&rwlock->pending_writers, 1, memory_order_relaxed);

      atomic_thread_fence(memory_order_seq_cst);

      int ret = __futex_wait_ex(&rwlock->state, rwlock->process_shared(), old_state,
                                rel_timeout);

      atomic_fetch_sub_explicit(&rwlock->pending_writers, 1, memory_order_relaxed);

      if (ret == -ETIMEDOUT) {
        return ETIMEDOUT;
      }
    }
  }
}

int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock_interface) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  return __pthread_rwlock_timedrdlock(rwlock, NULL);
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t* rwlock_interface, const timespec* abs_timeout) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  return __pthread_rwlock_timedrdlock(rwlock, abs_timeout);
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock_interface) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  int old_state = atomic_load_explicit(&rwlock->state, memory_order_relaxed);

  while (old_state >= 0 && !atomic_compare_exchange_weak_explicit(&rwlock->state, &old_state,
                             old_state + 1, memory_order_acquire, memory_order_relaxed)) {
  }
  return (old_state >= 0) ? 0 : EBUSY;
}

int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock_interface) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  return __pthread_rwlock_timedwrlock(rwlock, NULL);
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t* rwlock_interface, const timespec* abs_timeout) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  return __pthread_rwlock_timedwrlock(rwlock, abs_timeout);
}

int pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock_interface) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  int old_state = atomic_load_explicit(&rwlock->state, memory_order_relaxed);

  while (old_state == 0 && !atomic_compare_exchange_weak_explicit(&rwlock->state, &old_state, -1,
                                              memory_order_acquire, memory_order_relaxed)) {
  }
  if (old_state == 0) {
    atomic_store_explicit(&rwlock->writer_thread_id, __get_thread()->tid, memory_order_relaxed);
    return 0;
  }
  return EBUSY;
}


int pthread_rwlock_unlock(pthread_rwlock_t* rwlock_interface) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  int old_state = atomic_load_explicit(&rwlock->state, memory_order_relaxed);
  if (__predict_false(old_state == 0)) {
    return EPERM;
  } else if (old_state == -1) {
    if (atomic_load_explicit(&rwlock->writer_thread_id, memory_order_relaxed) != __get_thread()->tid) {
      return EPERM;
    }
    // We're no longer the owner.
    atomic_store_explicit(&rwlock->writer_thread_id, 0, memory_order_relaxed);
    // Change state from -1 to 0.
    atomic_store_explicit(&rwlock->state, 0, memory_order_release);

  } else { // old_state > 0
    // Reduce state by 1.
    while (old_state > 0 && !atomic_compare_exchange_weak_explicit(&rwlock->state, &old_state,
                               old_state - 1, memory_order_release, memory_order_relaxed)) {
    }

    if (old_state <= 0) {
      return EPERM;
    } else if (old_state > 1) {
      return 0;
    }
    // old_state = 1, which means the last reader calling unlock. It has to wake up waiters.
  }

  // If having waiters, wake up them.
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
  if (__predict_false(atomic_load_explicit(&rwlock->pending_readers, memory_order_relaxed) > 0 ||
                      atomic_load_explicit(&rwlock->pending_writers, memory_order_relaxed) > 0)) {
    __futex_wake_ex(&rwlock->state, rwlock->process_shared(), INT_MAX);
  }
  return 0;
}
