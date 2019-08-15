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
#include <string.h>

#include "pthread_internal.h"
#include "private/bionic_futex.h"
#include "private/bionic_lock.h"
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
 */

// A rwlockattr is implemented as a 32-bit integer which has following fields:
//  bits    name              description
//   1     rwlock_kind       have rwlock preference like PTHREAD_RWLOCK_PREFER_READER_NP.
//   0      process_shared    set to 1 if the rwlock is shared between processes.

#define RWLOCKATTR_PSHARED_SHIFT 0
#define RWLOCKATTR_KIND_SHIFT    1

#define RWLOCKATTR_PSHARED_MASK  1
#define RWLOCKATTR_KIND_MASK     2
#define RWLOCKATTR_RESERVED_MASK (~3)

static inline __always_inline __always_inline bool __rwlockattr_getpshared(const pthread_rwlockattr_t* attr) {
  return (*attr & RWLOCKATTR_PSHARED_MASK) >> RWLOCKATTR_PSHARED_SHIFT;
}

static inline __always_inline __always_inline void __rwlockattr_setpshared(pthread_rwlockattr_t* attr, int pshared) {
  *attr = (*attr & ~RWLOCKATTR_PSHARED_MASK) | (pshared << RWLOCKATTR_PSHARED_SHIFT);
}

static inline __always_inline int __rwlockattr_getkind(const pthread_rwlockattr_t* attr) {
  return (*attr & RWLOCKATTR_KIND_MASK) >> RWLOCKATTR_KIND_SHIFT;
}

static inline __always_inline void __rwlockattr_setkind(pthread_rwlockattr_t* attr, int kind) {
  *attr = (*attr & ~RWLOCKATTR_KIND_MASK) | (kind << RWLOCKATTR_KIND_SHIFT);
}


int pthread_rwlockattr_init(pthread_rwlockattr_t* attr) {
  *attr = 0;
  return 0;
}

int pthread_rwlockattr_destroy(pthread_rwlockattr_t* attr) {
  *attr = -1;
  return 0;
}

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t* attr, int* pshared) {
  if (__rwlockattr_getpshared(attr)) {
    *pshared = PTHREAD_PROCESS_SHARED;
  } else {
    *pshared = PTHREAD_PROCESS_PRIVATE;
  }
  return 0;
}

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t* attr, int pshared) {
  switch (pshared) {
    case PTHREAD_PROCESS_PRIVATE:
      __rwlockattr_setpshared(attr, 0);
      return 0;
    case PTHREAD_PROCESS_SHARED:
      __rwlockattr_setpshared(attr, 1);
      return 0;
    default:
      return EINVAL;
  }
}

int pthread_rwlockattr_getkind_np(const pthread_rwlockattr_t* attr, int* pref) {
  *pref = __rwlockattr_getkind(attr);
  return 0;
}

int pthread_rwlockattr_setkind_np(pthread_rwlockattr_t* attr, int pref) {
  switch (pref) {
    case PTHREAD_RWLOCK_PREFER_READER_NP:   // Fall through.
    case PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP:
      __rwlockattr_setkind(attr, pref);
      return 0;
    default:
      return EINVAL;
  }
}

// A rwlock state is implemented as a 32-bit integer which has following rules:
//  bits      name                              description
//   31      owned_by_writer_flag              set to 1 if the lock is owned by a writer now.
//  30-2     reader_count                      the count of readers holding the lock.
//   1       have_pending_writers              set to 1 if having pending writers.
//   0       have_pending_readers              set to 1 if having pending readers.

#define STATE_HAVE_PENDING_READERS_SHIFT    0
#define STATE_HAVE_PENDING_WRITERS_SHIFT    1
#define STATE_READER_COUNT_SHIFT            2
#define STATE_OWNED_BY_WRITER_SHIFT        31

#define STATE_HAVE_PENDING_READERS_FLAG     (1 << STATE_HAVE_PENDING_READERS_SHIFT)
#define STATE_HAVE_PENDING_WRITERS_FLAG     (1 << STATE_HAVE_PENDING_WRITERS_SHIFT)
#define STATE_READER_COUNT_CHANGE_STEP  (1 << STATE_READER_COUNT_SHIFT)
#define STATE_OWNED_BY_WRITER_FLAG      (1 << STATE_OWNED_BY_WRITER_SHIFT)

#define STATE_HAVE_PENDING_READERS_OR_WRITERS_FLAG \
          (STATE_HAVE_PENDING_READERS_FLAG | STATE_HAVE_PENDING_WRITERS_FLAG)

struct pthread_rwlock_internal_t {
  atomic_int state;
  atomic_int writer_tid;

  bool pshared;
  bool writer_nonrecursive_preferred;
  uint16_t __pad;

// When a reader thread plans to suspend on the rwlock, it will add STATE_HAVE_PENDING_READERS_FLAG
// in state, increase pending_reader_count, and wait on pending_reader_wakeup_serial. After woken
// up, the reader thread decreases pending_reader_count, and the last pending reader thread should
// remove STATE_HAVE_PENDING_READERS_FLAG in state. A pending writer thread works in a similar way,
// except that it uses flag and members for writer threads.

  Lock pending_lock;  // All pending members below are protected by pending_lock.
  uint32_t pending_reader_count;  // Count of pending reader threads.
  uint32_t pending_writer_count;  // Count of pending writer threads.
  uint32_t pending_reader_wakeup_serial;  // Pending reader threads wait on this address by futex_wait.
  uint32_t pending_writer_wakeup_serial;  // Pending writer threads wait on this address by futex_wait.

#if defined(__LP64__)
  char __reserved[20];
#else
  char __reserved[4];
#endif
};

static inline __always_inline bool __state_owned_by_writer(int state) {
  return state < 0;
}

static inline __always_inline bool __state_owned_by_readers(int state) {
  // If state >= 0, the owned_by_writer_flag is not set.
  // And if state >= STATE_READER_COUNT_CHANGE_STEP, the reader_count field is not empty.
  return state >= STATE_READER_COUNT_CHANGE_STEP;
}

static inline __always_inline bool __state_owned_by_readers_or_writer(int state) {
  return state < 0 || state >= STATE_READER_COUNT_CHANGE_STEP;
}

static inline __always_inline int __state_add_writer_flag(int state) {
  return state | STATE_OWNED_BY_WRITER_FLAG;
}

static inline __always_inline bool __state_is_last_reader(int state) {
  return (state >> STATE_READER_COUNT_SHIFT) == 1;
}

static inline __always_inline bool __state_have_pending_writers(int state) {
  return state & STATE_HAVE_PENDING_WRITERS_FLAG;
}

static inline __always_inline bool __state_have_pending_readers_or_writers(int state) {
  return state & STATE_HAVE_PENDING_READERS_OR_WRITERS_FLAG;
}

static_assert(sizeof(pthread_rwlock_t) == sizeof(pthread_rwlock_internal_t),
              "pthread_rwlock_t should actually be pthread_rwlock_internal_t in implementation.");

// For binary compatibility with old version of pthread_rwlock_t, we can't use more strict
// alignment than 4-byte alignment.
static_assert(alignof(pthread_rwlock_t) == 4,
             "pthread_rwlock_t should fulfill the alignment requirement of pthread_rwlock_internal_t.");

static inline __always_inline pthread_rwlock_internal_t* __get_internal_rwlock(pthread_rwlock_t* rwlock_interface) {
  return reinterpret_cast<pthread_rwlock_internal_t*>(rwlock_interface);
}

int pthread_rwlock_init(pthread_rwlock_t* rwlock_interface, const pthread_rwlockattr_t* attr) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  memset(rwlock, 0, sizeof(pthread_rwlock_internal_t));

  if (__predict_false(attr != nullptr)) {
    rwlock->pshared = __rwlockattr_getpshared(attr);
    int kind = __rwlockattr_getkind(attr);
    switch (kind) {
      case PTHREAD_RWLOCK_PREFER_READER_NP:
        rwlock->writer_nonrecursive_preferred = false;
        break;
      case PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP:
        rwlock->writer_nonrecursive_preferred = true;
        break;
      default:
        return EINVAL;
    }
    if ((*attr & RWLOCKATTR_RESERVED_MASK) != 0) {
      return EINVAL;
    }
  }

  atomic_init(&rwlock->state, 0);
  rwlock->pending_lock.init(rwlock->pshared);
  return 0;
}

int pthread_rwlock_destroy(pthread_rwlock_t* rwlock_interface) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  if (atomic_load_explicit(&rwlock->state, memory_order_relaxed) != 0) {
    return EBUSY;
  }
  return 0;
}

static inline __always_inline bool __can_acquire_read_lock(int old_state,
                                                             bool writer_nonrecursive_preferred) {
  // If writer is preferred with nonrecursive reader, we prevent further readers from acquiring
  // the lock when there are writers waiting for the lock.
  bool cannot_apply = __state_owned_by_writer(old_state) ||
                      (writer_nonrecursive_preferred && __state_have_pending_writers(old_state));
  return !cannot_apply;
}

static inline __always_inline int __pthread_rwlock_tryrdlock(pthread_rwlock_internal_t* rwlock) {
  int old_state = atomic_load_explicit(&rwlock->state, memory_order_relaxed);

  while (__predict_true(__can_acquire_read_lock(old_state, rwlock->writer_nonrecursive_preferred))) {

    int new_state = old_state + STATE_READER_COUNT_CHANGE_STEP;
    if (__predict_false(!__state_owned_by_readers(new_state))) { // Happens when reader count overflows.
      return EAGAIN;
    }
    if (__predict_true(atomic_compare_exchange_weak_explicit(&rwlock->state, &old_state, new_state,
                                              memory_order_acquire, memory_order_relaxed))) {
      return 0;
    }
  }
  return EBUSY;
}

static int __pthread_rwlock_timedrdlock(pthread_rwlock_internal_t* rwlock, bool use_realtime_clock,
                                        const timespec* abs_timeout_or_null) {
  if (atomic_load_explicit(&rwlock->writer_tid, memory_order_relaxed) == __get_thread()->tid) {
    return EDEADLK;
  }

  while (true) {
    int result = __pthread_rwlock_tryrdlock(rwlock);
    if (result == 0 || result == EAGAIN) {
      return result;
    }
    result = check_timespec(abs_timeout_or_null, true);
    if (result != 0) {
      return result;
    }

    int old_state = atomic_load_explicit(&rwlock->state, memory_order_relaxed);
    if (__can_acquire_read_lock(old_state, rwlock->writer_nonrecursive_preferred)) {
      continue;
    }

    rwlock->pending_lock.lock();
    rwlock->pending_reader_count++;

    // We rely on the fact that all atomic exchange operations on the same object (here it is
    // rwlock->state) always appear to occur in a single total order. If the pending flag is added
    // before unlocking, the unlocking thread will wakeup the waiter. Otherwise, we will see the
    // state is unlocked and will not wait anymore.
    old_state = atomic_fetch_or_explicit(&rwlock->state, STATE_HAVE_PENDING_READERS_FLAG,
                                         memory_order_relaxed);

    int old_serial = rwlock->pending_reader_wakeup_serial;
    rwlock->pending_lock.unlock();

    int futex_result = 0;
    if (!__can_acquire_read_lock(old_state, rwlock->writer_nonrecursive_preferred)) {
      futex_result = __futex_wait_ex(&rwlock->pending_reader_wakeup_serial, rwlock->pshared,
                                     old_serial, use_realtime_clock, abs_timeout_or_null);
    }

    rwlock->pending_lock.lock();
    rwlock->pending_reader_count--;
    if (rwlock->pending_reader_count == 0) {
      atomic_fetch_and_explicit(&rwlock->state, ~STATE_HAVE_PENDING_READERS_FLAG,
                                memory_order_relaxed);
    }
    rwlock->pending_lock.unlock();

    if (futex_result == -ETIMEDOUT) {
      return ETIMEDOUT;
    }
  }
}

static inline __always_inline bool __can_acquire_write_lock(int old_state) {
  return !__state_owned_by_readers_or_writer(old_state);
}

static inline __always_inline int __pthread_rwlock_trywrlock(pthread_rwlock_internal_t* rwlock) {
  int old_state = atomic_load_explicit(&rwlock->state, memory_order_relaxed);

  while (__predict_true(__can_acquire_write_lock(old_state))) {
    if (__predict_true(atomic_compare_exchange_weak_explicit(&rwlock->state, &old_state,
          __state_add_writer_flag(old_state), memory_order_acquire, memory_order_relaxed))) {

      atomic_store_explicit(&rwlock->writer_tid, __get_thread()->tid, memory_order_relaxed);
      return 0;
    }
  }
  return EBUSY;
}

static int __pthread_rwlock_timedwrlock(pthread_rwlock_internal_t* rwlock, bool use_realtime_clock,
                                        const timespec* abs_timeout_or_null) {
  if (atomic_load_explicit(&rwlock->writer_tid, memory_order_relaxed) == __get_thread()->tid) {
    return EDEADLK;
  }
  while (true) {
    int result = __pthread_rwlock_trywrlock(rwlock);
    if (result == 0) {
      return result;
    }
    result = check_timespec(abs_timeout_or_null, true);
    if (result != 0) {
      return result;
    }

    int old_state = atomic_load_explicit(&rwlock->state, memory_order_relaxed);
    if (__can_acquire_write_lock(old_state)) {
      continue;
    }

    rwlock->pending_lock.lock();
    rwlock->pending_writer_count++;

    old_state = atomic_fetch_or_explicit(&rwlock->state, STATE_HAVE_PENDING_WRITERS_FLAG,
                                         memory_order_relaxed);

    int old_serial = rwlock->pending_writer_wakeup_serial;
    rwlock->pending_lock.unlock();

    int futex_result = 0;
    if (!__can_acquire_write_lock(old_state)) {
      futex_result = __futex_wait_ex(&rwlock->pending_writer_wakeup_serial, rwlock->pshared,
                                     old_serial, use_realtime_clock, abs_timeout_or_null);
    }

    rwlock->pending_lock.lock();
    rwlock->pending_writer_count--;
    if (rwlock->pending_writer_count == 0) {
      atomic_fetch_and_explicit(&rwlock->state, ~STATE_HAVE_PENDING_WRITERS_FLAG,
                                memory_order_relaxed);
    }
    rwlock->pending_lock.unlock();

    if (futex_result == -ETIMEDOUT) {
      return ETIMEDOUT;
    }
  }
}

int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock_interface) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);
  // Avoid slowing down fast path of rdlock.
  if (__predict_true(__pthread_rwlock_tryrdlock(rwlock) == 0)) {
    return 0;
  }
  return __pthread_rwlock_timedrdlock(rwlock, false, nullptr);
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t* rwlock_interface, const timespec* abs_timeout) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  return __pthread_rwlock_timedrdlock(rwlock, true, abs_timeout);
}

int pthread_rwlock_timedrdlock_monotonic_np(pthread_rwlock_t* rwlock_interface,
                                            const timespec* abs_timeout) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  return __pthread_rwlock_timedrdlock(rwlock, false, abs_timeout);
}

int pthread_rwlock_clockrdlock(pthread_rwlock_t* rwlock_interface, clockid_t clock,
                               const struct timespec* abs_timeout) {
  switch (clock) {
    case CLOCK_MONOTONIC:
      return pthread_rwlock_timedrdlock_monotonic_np(rwlock_interface, abs_timeout);
    case CLOCK_REALTIME:
      return pthread_rwlock_timedrdlock(rwlock_interface, abs_timeout);
    default:
      return EINVAL;
  }
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock_interface) {
  return __pthread_rwlock_tryrdlock(__get_internal_rwlock(rwlock_interface));
}

int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock_interface) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);
  // Avoid slowing down fast path of wrlock.
  if (__predict_true(__pthread_rwlock_trywrlock(rwlock) == 0)) {
    return 0;
  }
  return __pthread_rwlock_timedwrlock(rwlock, false, nullptr);
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t* rwlock_interface, const timespec* abs_timeout) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  return __pthread_rwlock_timedwrlock(rwlock, true, abs_timeout);
}

int pthread_rwlock_timedwrlock_monotonic_np(pthread_rwlock_t* rwlock_interface,
                                            const timespec* abs_timeout) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  return __pthread_rwlock_timedwrlock(rwlock, false, abs_timeout);
}

int pthread_rwlock_clockwrlock(pthread_rwlock_t* rwlock_interface, clockid_t clock,
                               const struct timespec* abs_timeout) {
  switch (clock) {
    case CLOCK_MONOTONIC:
      return pthread_rwlock_timedwrlock_monotonic_np(rwlock_interface, abs_timeout);
    case CLOCK_REALTIME:
      return pthread_rwlock_timedwrlock(rwlock_interface, abs_timeout);
    default:
      return EINVAL;
  }
}

int pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock_interface) {
  return __pthread_rwlock_trywrlock(__get_internal_rwlock(rwlock_interface));
}

int pthread_rwlock_unlock(pthread_rwlock_t* rwlock_interface) {
  pthread_rwlock_internal_t* rwlock = __get_internal_rwlock(rwlock_interface);

  int old_state = atomic_load_explicit(&rwlock->state, memory_order_relaxed);
  if (__state_owned_by_writer(old_state)) {
    if (atomic_load_explicit(&rwlock->writer_tid, memory_order_relaxed) != __get_thread()->tid) {
      return EPERM;
    }
    atomic_store_explicit(&rwlock->writer_tid, 0, memory_order_relaxed);
    old_state = atomic_fetch_and_explicit(&rwlock->state, ~STATE_OWNED_BY_WRITER_FLAG,
                                          memory_order_release);
    if (!__state_have_pending_readers_or_writers(old_state)) {
      return 0;
    }

  } else if (__state_owned_by_readers(old_state)) {
    old_state = atomic_fetch_sub_explicit(&rwlock->state, STATE_READER_COUNT_CHANGE_STEP,
                                          memory_order_release);
    if (!__state_is_last_reader(old_state) || !__state_have_pending_readers_or_writers(old_state)) {
      return 0;
    }

  } else {
    return EPERM;
  }

  // Wake up pending readers or writers.
  rwlock->pending_lock.lock();
  if (rwlock->pending_writer_count != 0) {
    rwlock->pending_writer_wakeup_serial++;
    rwlock->pending_lock.unlock();

    __futex_wake_ex(&rwlock->pending_writer_wakeup_serial, rwlock->pshared, 1);

  } else if (rwlock->pending_reader_count != 0) {
    rwlock->pending_reader_wakeup_serial++;
    rwlock->pending_lock.unlock();

    __futex_wake_ex(&rwlock->pending_reader_wakeup_serial, rwlock->pshared, INT_MAX);

  } else {
    // It happens when waiters are woken up by timeout.
    rwlock->pending_lock.unlock();
  }
  return 0;
}
