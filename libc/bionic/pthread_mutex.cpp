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

#include <pthread.h>

#include <errno.h>
#include <limits.h>
#include <stdatomic.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <unistd.h>

#include "pthread_internal.h"

#include "private/bionic_constants.h"
#include "private/bionic_futex.h"
#include "private/bionic_systrace.h"
#include "private/bionic_time_conversions.h"
#include "private/bionic_tls.h"

/* a mutex is implemented as a 32-bit integer holding the following fields
 *
 * bits:     name     description
 * 31-16     tid      owner thread's tid (recursive and errorcheck only)
 * 15-14     type     mutex type
 * 13        shared   process-shared flag
 * 12-2      counter  counter of recursive mutexes
 * 1-0       state    lock state (0, 1 or 2)
 */

/* Convenience macro, creates a mask of 'bits' bits that starts from
 * the 'shift'-th least significant bit in a 32-bit word.
 *
 * Examples: FIELD_MASK(0,4)  -> 0xf
 *           FIELD_MASK(16,9) -> 0x1ff0000
 */
#define  FIELD_MASK(shift,bits)           (((1 << (bits))-1) << (shift))

/* This one is used to create a bit pattern from a given field value */
#define  FIELD_TO_BITS(val,shift,bits)    (((val) & ((1 << (bits))-1)) << (shift))

/* And this one does the opposite, i.e. extract a field's value from a bit pattern */
#define  FIELD_FROM_BITS(val,shift,bits)  (((val) >> (shift)) & ((1 << (bits))-1))

/* Mutex state:
 *
 * 0 for unlocked
 * 1 for locked, no waiters
 * 2 for locked, maybe waiters
 */
#define  MUTEX_STATE_SHIFT      0
#define  MUTEX_STATE_LEN        2

#define  MUTEX_STATE_MASK           FIELD_MASK(MUTEX_STATE_SHIFT, MUTEX_STATE_LEN)
#define  MUTEX_STATE_FROM_BITS(v)   FIELD_FROM_BITS(v, MUTEX_STATE_SHIFT, MUTEX_STATE_LEN)
#define  MUTEX_STATE_TO_BITS(v)     FIELD_TO_BITS(v, MUTEX_STATE_SHIFT, MUTEX_STATE_LEN)

#define  MUTEX_STATE_UNLOCKED            0   /* must be 0 to match __PTHREAD_MUTEX_INIT_VALUE */
#define  MUTEX_STATE_LOCKED_UNCONTENDED  1   /* must be 1 due to atomic dec in unlock operation */
#define  MUTEX_STATE_LOCKED_CONTENDED    2   /* must be 1 + LOCKED_UNCONTENDED due to atomic dec */

#define  MUTEX_STATE_BITS_UNLOCKED            MUTEX_STATE_TO_BITS(MUTEX_STATE_UNLOCKED)
#define  MUTEX_STATE_BITS_LOCKED_UNCONTENDED  MUTEX_STATE_TO_BITS(MUTEX_STATE_LOCKED_UNCONTENDED)
#define  MUTEX_STATE_BITS_LOCKED_CONTENDED    MUTEX_STATE_TO_BITS(MUTEX_STATE_LOCKED_CONTENDED)

/* return true iff the mutex if locked with no waiters */
#define  MUTEX_STATE_BITS_IS_LOCKED_UNCONTENDED(v)  (((v) & MUTEX_STATE_MASK) == MUTEX_STATE_BITS_LOCKED_UNCONTENDED)

/* return true iff the mutex if locked with maybe waiters */
#define  MUTEX_STATE_BITS_IS_LOCKED_CONTENDED(v)   (((v) & MUTEX_STATE_MASK) == MUTEX_STATE_BITS_LOCKED_CONTENDED)

/* used to flip from LOCKED_UNCONTENDED to LOCKED_CONTENDED */
#define  MUTEX_STATE_BITS_FLIP_CONTENTION(v)      ((v) ^ (MUTEX_STATE_BITS_LOCKED_CONTENDED ^ MUTEX_STATE_BITS_LOCKED_UNCONTENDED))

/* Mutex counter:
 *
 * We need to check for overflow before incrementing, and we also need to
 * detect when the counter is 0
 */
#define  MUTEX_COUNTER_SHIFT         2
#define  MUTEX_COUNTER_LEN           11
#define  MUTEX_COUNTER_MASK          FIELD_MASK(MUTEX_COUNTER_SHIFT, MUTEX_COUNTER_LEN)

#define  MUTEX_COUNTER_BITS_WILL_OVERFLOW(v)    (((v) & MUTEX_COUNTER_MASK) == MUTEX_COUNTER_MASK)
#define  MUTEX_COUNTER_BITS_IS_ZERO(v)          (((v) & MUTEX_COUNTER_MASK) == 0)

/* Used to increment the counter directly after overflow has been checked */
#define  MUTEX_COUNTER_BITS_ONE      FIELD_TO_BITS(1, MUTEX_COUNTER_SHIFT,MUTEX_COUNTER_LEN)

/* Mutex shared bit flag
 *
 * This flag is set to indicate that the mutex is shared among processes.
 * This changes the futex opcode we use for futex wait/wake operations
 * (non-shared operations are much faster).
 */
#define  MUTEX_SHARED_SHIFT    13
#define  MUTEX_SHARED_MASK     FIELD_MASK(MUTEX_SHARED_SHIFT,1)

/* Mutex type:
 *
 * We support normal, recursive and errorcheck mutexes.
 *
 * The constants defined here *cannot* be changed because they must match
 * the C library ABI which defines the following initialization values in
 * <pthread.h>:
 *
 *   __PTHREAD_MUTEX_INIT_VALUE
 *   __PTHREAD_RECURSIVE_MUTEX_VALUE
 *   __PTHREAD_ERRORCHECK_MUTEX_INIT_VALUE
 */
#define  MUTEX_TYPE_SHIFT      14
#define  MUTEX_TYPE_LEN        2
#define  MUTEX_TYPE_MASK       FIELD_MASK(MUTEX_TYPE_SHIFT,MUTEX_TYPE_LEN)

#define  MUTEX_TYPE_NORMAL          0  /* Must be 0 to match __PTHREAD_MUTEX_INIT_VALUE */
#define  MUTEX_TYPE_RECURSIVE       1
#define  MUTEX_TYPE_ERRORCHECK      2

#define  MUTEX_TYPE_TO_BITS(t)       FIELD_TO_BITS(t, MUTEX_TYPE_SHIFT, MUTEX_TYPE_LEN)

#define  MUTEX_TYPE_BITS_NORMAL      MUTEX_TYPE_TO_BITS(MUTEX_TYPE_NORMAL)
#define  MUTEX_TYPE_BITS_RECURSIVE   MUTEX_TYPE_TO_BITS(MUTEX_TYPE_RECURSIVE)
#define  MUTEX_TYPE_BITS_ERRORCHECK  MUTEX_TYPE_TO_BITS(MUTEX_TYPE_ERRORCHECK)

/* Mutex owner field:
 *
 * This is only used for recursive and errorcheck mutexes. It holds the
 * tid of the owning thread. We use 16 bits to represent tid here,
 * so the highest tid is 65535. There is a test to check /proc/sys/kernel/pid_max
 * to make sure it will not exceed our limit.
 */
#define  MUTEX_OWNER_SHIFT     16
#define  MUTEX_OWNER_LEN       16

#define  MUTEX_OWNER_FROM_BITS(v)    FIELD_FROM_BITS(v,MUTEX_OWNER_SHIFT,MUTEX_OWNER_LEN)
#define  MUTEX_OWNER_TO_BITS(v)      FIELD_TO_BITS(v,MUTEX_OWNER_SHIFT,MUTEX_OWNER_LEN)

/* Convenience macros.
 *
 * These are used to form or modify the bit pattern of a given mutex value
 */



/* a mutex attribute holds the following fields
 *
 * bits:     name       description
 * 0-3       type       type of mutex
 * 4         shared     process-shared flag
 */
#define  MUTEXATTR_TYPE_MASK   0x000f
#define  MUTEXATTR_SHARED_MASK 0x0010


int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    *attr = PTHREAD_MUTEX_DEFAULT;
    return 0;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    *attr = -1;
    return 0;
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type_p)
{
    int type = (*attr & MUTEXATTR_TYPE_MASK);

    if (type < PTHREAD_MUTEX_NORMAL || type > PTHREAD_MUTEX_ERRORCHECK) {
        return EINVAL;
    }

    *type_p = type;
    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    if (type < PTHREAD_MUTEX_NORMAL || type > PTHREAD_MUTEX_ERRORCHECK ) {
        return EINVAL;
    }

    *attr = (*attr & ~MUTEXATTR_TYPE_MASK) | type;
    return 0;
}

/* process-shared mutexes are not supported at the moment */

int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int  pshared)
{
    switch (pshared) {
    case PTHREAD_PROCESS_PRIVATE:
        *attr &= ~MUTEXATTR_SHARED_MASK;
        return 0;

    case PTHREAD_PROCESS_SHARED:
        /* our current implementation of pthread actually supports shared
         * mutexes but won't cleanup if a process dies with the mutex held.
         * Nevertheless, it's better than nothing. Shared mutexes are used
         * by surfaceflinger and audioflinger.
         */
        *attr |= MUTEXATTR_SHARED_MASK;
        return 0;
    }
    return EINVAL;
}

int pthread_mutexattr_getpshared(const pthread_mutexattr_t* attr, int* pshared) {
    *pshared = (*attr & MUTEXATTR_SHARED_MASK) ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE;
    return 0;
}

static inline atomic_int* get_mutex_value_pointer(pthread_mutex_t* mutex) {
    static_assert(sizeof(atomic_int) == sizeof(mutex->value),
                  "mutex->value should actually be atomic_int in implementation.");

    // We prefer casting to atomic_int instead of declaring mutex->value to be atomic_int directly.
    // Because using the second method pollutes pthread.h, and causes an error when compiling libcxx.
    return reinterpret_cast<atomic_int*>(&mutex->value);
}

int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr) {
    atomic_int* mutex_value_ptr = get_mutex_value_pointer(mutex);

    if (__predict_true(attr == NULL)) {
        atomic_init(mutex_value_ptr, MUTEX_TYPE_BITS_NORMAL);
        return 0;
    }

    int value = 0;
    if ((*attr & MUTEXATTR_SHARED_MASK) != 0) {
        value |= MUTEX_SHARED_MASK;
    }

    switch (*attr & MUTEXATTR_TYPE_MASK) {
    case PTHREAD_MUTEX_NORMAL:
        value |= MUTEX_TYPE_BITS_NORMAL;
        break;
    case PTHREAD_MUTEX_RECURSIVE:
        value |= MUTEX_TYPE_BITS_RECURSIVE;
        break;
    case PTHREAD_MUTEX_ERRORCHECK:
        value |= MUTEX_TYPE_BITS_ERRORCHECK;
        break;
    default:
        return EINVAL;
    }

    atomic_init(mutex_value_ptr, value);
    return 0;
}

static inline int __pthread_normal_mutex_trylock(atomic_int* mutex_value_ptr, int shared) {
    const int unlocked           = shared | MUTEX_STATE_BITS_UNLOCKED;
    const int locked_uncontended = shared | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;

    int mvalue = unlocked;
    if (__predict_true(atomic_compare_exchange_strong_explicit(mutex_value_ptr, &mvalue,
                                                locked_uncontended,
                                                memory_order_acquire,
                                                memory_order_relaxed))) {
        return 0;
    }
    return EBUSY;
}

/*
 * Lock a mutex of type NORMAL.
 *
 * As noted above, there are three states:
 *   0 (unlocked, no contention)
 *   1 (locked, no contention)
 *   2 (locked, contention)
 *
 * Non-recursive mutexes don't use the thread-id or counter fields, and the
 * "type" value is zero, so the only bits that will be set are the ones in
 * the lock state field.
 */
static inline int __pthread_normal_mutex_lock(atomic_int* mutex_value_ptr, int shared,
                                              const timespec* abs_timeout_or_null, clockid_t clock) {
    if (__predict_true(__normal_mutex_trylock(mutex_value_ptr, shared) == 0)) {
        return 0;
    }

    ScopedTrace trace("Contending for pthread mutex");

    const int unlocked           = shared | MUTEX_STATE_BITS_UNLOCKED;
    const int locked_contended = shared | MUTEX_STATE_BITS_LOCKED_CONTENDED;

    // We want to go to sleep until the mutex is available, which requires
    // promoting it to locked_contended. We need to swap in the new state
    // value and then wait until somebody wakes us up.
    // An atomic_exchange is used to compete with other threads for the lock.
    // If it returns unlocked, we have acquired the lock, otherwise another
    // thread still holds the lock and we should wait again.
    // If lock is acquired, an acquire fence is needed to make all memory accesses
    // made by other threads visible to the current CPU.
    while (atomic_exchange_explicit(mutex_value_ptr, locked_contended,
                                    memory_order_acquire) != unlocked) {
        timespec ts;
        timespec* rel_timeout = NULL;
        if (abs_timeout_or_null != NULL) {
            rel_timeout = &ts;
            if (!timespec_from_absolute_timespec(*rel_timeout, *abs_timeout_or_null, clock)) {
                return ETIMEDOUT;
            }
        }
        if (__futex_wait_ex(mutex_value_ptr, shared, locked_contended, rel_timeout) == -ETIMEDOUT) {
            return ETIMEDOUT;
        }
    }
    return 0;
}

/*
 * Release a mutex of type NORMAL.  The caller is responsible for determining
 * that we are in fact the owner of this lock.
 */
static inline void __pthread_normal_mutex_unlock(atomic_int* mutex_value_ptr, int shared) {
    const int unlocked         = shared | MUTEX_STATE_BITS_UNLOCKED;
    const int locked_contended = shared | MUTEX_STATE_BITS_LOCKED_CONTENDED;

    // We use an atomic_exchange to release the lock. If locked_contended state
    // is returned, some threads is waiting for the lock and we need to wake up
    // one of them.
    // A release fence is required to make previous stores visible to next
    // lock owner threads.
    if (atomic_exchange_explicit(mutex_value_ptr, unlocked,
                                 memory_order_release) == locked_contended) {
        // Wake up one waiting thread. We don't know which thread will be
        // woken or when it'll start executing -- futexes make no guarantees
        // here. There may not even be a thread waiting.
        //
        // The newly-woken thread will replace the unlocked state we just set above
        // with locked_contended state, which means that when it eventually releases
        // the mutex it will also call FUTEX_WAKE. This results in one extra wake
        // call whenever a lock is contended, but let us avoid forgetting anyone
        // without requiring us to track the number of sleepers.
        //
        // It's possible for another thread to sneak in and grab the lock between
        // the exchange above and the wake call below. If the new thread is "slow"
        // and holds the lock for a while, we'll wake up a sleeper, which will swap
        // in locked_uncontended state and then go back to sleep since the lock is
        // still held. If the new thread is "fast", running to completion before
        // we call wake, the thread we eventually wake will find an unlocked mutex
        // and will execute. Either way we have correct behavior and nobody is
        // orphaned on the wait queue.
        __futex_wake_ex(mutex_value_ptr, shared, 1);
    }
}

/* This common inlined function is used to increment the counter of a recursive mutex.
 *
 * If the counter overflows, it will return EAGAIN.
 * Otherwise, it atomically increments the counter and returns 0.
 *
 */
static inline int __recursive_increment(atomic_int* mutex_value_ptr, int mvalue) {
    // Detect recursive lock overflow and return EAGAIN.
    // This is safe because only the owner thread can modify the
    // counter bits in the mutex value.
    if (MUTEX_COUNTER_BITS_WILL_OVERFLOW(mvalue)) {
        return EAGAIN;
    }

    // We own the mutex, but other threads are able to change the lower bits
    // (e.g. promoting it to "contended"), so we need to use an atomic exchange
    // loop to update the counter. The counter will not overflow in the loop,
    // as only the owner thread can change it.
    // The mutex is still locked, so we don't need a release fence.
    atomic_fetch_add_explicit(mutex_value_ptr, MUTEX_COUNTER_BITS_ONE, memory_order_relaxed);
    return 0;
}

static int __pthread_mutex_lock_with_timeout(pthread_mutex_t* mutex,
                                           const timespec* abs_timeout_or_null, clockid_t clock) {
    atomic_int* mutex_value_ptr = get_mutex_value_pointer(mutex);

    int mvalue, mtype, tid, shared;

    mvalue = atomic_load_explicit(mutex_value_ptr, memory_order_relaxed);
    mtype = (mvalue & MUTEX_TYPE_MASK);
    shared = (mvalue & MUTEX_SHARED_MASK);

    // Handle common case first.
    if ( __predict_true(mtype == MUTEX_TYPE_BITS_NORMAL) ) {
        return __pthread_normal_mutex_lock(mutex_value_ptr, shared, abs_timeout_or_null, clock);
    }

    // Do we already own this recursive or error-check mutex?
    tid = __get_thread()->tid;
    if (tid == MUTEX_OWNER_FROM_BITS(mvalue)) {
        if (mtype == MUTEX_TYPE_BITS_ERRORCHECK) {
            return EDEADLK;
        }
        return __recursive_increment(mutex_value_ptr, mvalue);
    }

    const int unlocked           = mtype | shared | MUTEX_STATE_BITS_UNLOCKED;
    const int locked_uncontended = mtype | shared | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;
    const int locked_contended   = mtype | shared | MUTEX_STATE_BITS_LOCKED_CONTENDED;

    // First, if the mutex is unlocked, try to quickly acquire it.
    // In the optimistic case where this works, set the state to locked_uncontended.
    if (mvalue == unlocked) {
        int newval = MUTEX_OWNER_TO_BITS(tid) | locked_uncontended;
        // If exchanged successfully, an acquire fence is required to make
        // all memory accesses made by other threads visible to the current CPU.
        if (__predict_true(atomic_compare_exchange_strong_explicit(mutex_value_ptr, &mvalue,
                           newval, memory_order_acquire, memory_order_relaxed))) {
            return 0;
        }
    }

    ScopedTrace trace("Contending for pthread mutex");

    while (true) {
        if (mvalue == unlocked) {
            // NOTE: We put the state to locked_contended since we _know_ there
            // is contention when we are in this loop. This ensures all waiters
            // will be unlocked.

            int newval = MUTEX_OWNER_TO_BITS(tid) | locked_contended;
            // If exchanged successfully, an acquire fence is required to make
            // all memory accesses made by other threads visible to the current CPU.
            if (__predict_true(atomic_compare_exchange_weak_explicit(mutex_value_ptr,
                                                                     &mvalue, newval,
                                                                     memory_order_acquire,
                                                                     memory_order_relaxed))) {
                return 0;
            }
            continue;
        } else if (MUTEX_STATE_BITS_IS_LOCKED_UNCONTENDED(mvalue)) {
            // We should set it to locked_contended beforing going to sleep. This can make
            // sure waiters will be woken up eventually.

            int newval = MUTEX_STATE_BITS_FLIP_CONTENTION(mvalue);
            if (__predict_false(!atomic_compare_exchange_weak_explicit(mutex_value_ptr,
                                                                       &mvalue, newval,
                                                                       memory_order_relaxed,
                                                                       memory_order_relaxed))) {
                continue;
            }
            mvalue = newval;
        }

        // We are in locked_contended state, sleep until someone wakes us up.
        timespec ts;
        timespec* rel_timeout = NULL;
        if (abs_timeout_or_null != NULL) {
            rel_timeout = &ts;
            if (!timespec_from_absolute_timespec(*rel_timeout, *abs_timeout_or_null, clock)) {
                return ETIMEDOUT;
            }
        }
        if (__futex_wait_ex(mutex_value_ptr, shared, mvalue, rel_timeout) == -ETIMEDOUT) {
            return ETIMEDOUT;
        }
        mvalue = atomic_load_explicit(mutex_value_ptr, memory_order_relaxed);
    }
}

int pthread_mutex_lock(pthread_mutex_t* mutex) {
    atomic_int* mutex_value_ptr = get_mutex_value_pointer(mutex);

    int mvalue = atomic_load_explicit(mutex_value_ptr, memory_order_relaxed);
    int mtype = (mvalue & MUTEX_TYPE_MASK);
    int shared = (mvalue & MUTEX_SHARED_MASK);
    // Avoid slowing down fast path of normal mutex lock operation.
    if (__predict_true(mtype == MUTEX_TYPE_BITS_NORMAL)) {
      if (__predict_true(__pthread_normal_mutex_trylock(mutex_value_ptr, shared) == 0)) {
        return 0;
      }
    }
    return __pthread_mutex_lock_with_timeout(mutex, NULL, 0);
}

int pthread_mutex_unlock(pthread_mutex_t* mutex) {
    atomic_int* mutex_value_ptr = get_mutex_value_pointer(mutex);

    int mvalue, mtype, tid, shared;

    mvalue = atomic_load_explicit(mutex_value_ptr, memory_order_relaxed);
    mtype  = (mvalue & MUTEX_TYPE_MASK);
    shared = (mvalue & MUTEX_SHARED_MASK);

    // Handle common case first.
    if (__predict_true(mtype == MUTEX_TYPE_BITS_NORMAL)) {
        __pthread_normal_mutex_unlock(mutex_value_ptr, shared);
        return 0;
    }

    // Do we already own this recursive or error-check mutex?
    tid = __get_thread()->tid;
    if ( tid != MUTEX_OWNER_FROM_BITS(mvalue) )
        return EPERM;

    // If the counter is > 0, we can simply decrement it atomically.
    // Since other threads can mutate the lower state bits (and only the
    // lower state bits), use a compare_exchange loop to do it.
    if (!MUTEX_COUNTER_BITS_IS_ZERO(mvalue)) {
        // We still own the mutex, so a release fence is not needed.
        atomic_fetch_sub_explicit(mutex_value_ptr, MUTEX_COUNTER_BITS_ONE, memory_order_relaxed);
        return 0;
    }

    // The counter is 0, so we'are going to unlock the mutex by resetting its
    // state to unlocked, we need to perform a atomic_exchange inorder to read
    // the current state, which will be locked_contended if there may have waiters
    // to awake.
    // A release fence is required to make previous stores visible to next
    // lock owner threads.
    const int unlocked = mtype | shared | MUTEX_STATE_BITS_UNLOCKED;
    mvalue = atomic_exchange_explicit(mutex_value_ptr, unlocked, memory_order_release);
    if (MUTEX_STATE_BITS_IS_LOCKED_CONTENDED(mvalue)) {
        __futex_wake_ex(mutex_value_ptr, shared, 1);
    }

    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t* mutex) {
    atomic_int* mutex_value_ptr = get_mutex_value_pointer(mutex);

    int mvalue = atomic_load_explicit(mutex_value_ptr, memory_order_relaxed);
    int mtype  = (mvalue & MUTEX_TYPE_MASK);
    int shared = (mvalue & MUTEX_SHARED_MASK);

    const int unlocked           = mtype | shared | MUTEX_STATE_BITS_UNLOCKED;
    const int locked_uncontended = mtype | shared | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;

    // Handle common case first.
    if (__predict_true(mtype == MUTEX_TYPE_BITS_NORMAL)) {
        return __pthread_normal_mutex_trylock(mutex_value_ptr, shared);
    }

    // Do we already own this recursive or error-check mutex?
    pid_t tid = __get_thread()->tid;
    if (tid == MUTEX_OWNER_FROM_BITS(mvalue)) {
        if (mtype == MUTEX_TYPE_BITS_ERRORCHECK) {
            return EBUSY;
        }
        return __recursive_increment(mutex_value_ptr, mvalue);
    }

    // Same as pthread_mutex_lock, except that we don't want to wait, and
    // the only operation that can succeed is a single compare_exchange to acquire the
    // lock if it is released / not owned by anyone. No need for a complex loop.
    // If exchanged successfully, an acquire fence is required to make
    // all memory accesses made by other threads visible to the current CPU.
    mvalue = unlocked;
    int newval = MUTEX_OWNER_TO_BITS(tid) | locked_uncontended;
    if (__predict_true(atomic_compare_exchange_strong_explicit(mutex_value_ptr, &mvalue, newval,
                                                               memory_order_acquire,
                                                               memory_order_relaxed))) {
        return 0;
    }
    return EBUSY;
}

#if !defined(__LP64__)
extern "C" int pthread_mutex_lock_timeout_np(pthread_mutex_t* mutex, unsigned ms) {
    timespec abs_timeout;
    clock_gettime(CLOCK_MONOTONIC, &abs_timeout);
    abs_timeout.tv_sec  += ms / 1000;
    abs_timeout.tv_nsec += (ms % 1000) * 1000000;
    if (abs_timeout.tv_nsec >= NS_PER_S) {
        abs_timeout.tv_sec++;
        abs_timeout.tv_nsec -= NS_PER_S;
    }

    int error = __pthread_mutex_lock_with_timeout(mutex, &abs_timeout, CLOCK_MONOTONIC);
    if (error == ETIMEDOUT) {
        error = EBUSY;
    }
    return error;
}
#endif

int pthread_mutex_timedlock(pthread_mutex_t* mutex, const timespec* abs_timeout) {
    return __pthread_mutex_lock_with_timeout(mutex, abs_timeout, CLOCK_REALTIME);
}

int pthread_mutex_destroy(pthread_mutex_t* mutex) {
    // Use trylock to ensure that the mutex is valid and not already locked.
    int error = pthread_mutex_trylock(mutex);
    if (error != 0) {
        return error;
    }

    atomic_int* mutex_value_ptr = get_mutex_value_pointer(mutex);
    atomic_store_explicit(mutex_value_ptr, 0xdead10cc, memory_order_relaxed);
    return 0;
}
