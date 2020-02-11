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
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <unistd.h>

#include "pthread_internal.h"

#include "private/bionic_constants.h"
#include "private/bionic_fortify.h"
#include "private/bionic_futex.h"
#include "private/bionic_systrace.h"
#include "private/bionic_time_conversions.h"
#include "private/bionic_tls.h"

/* a mutex attribute holds the following fields
 *
 * bits:     name       description
 * 0-3       type       type of mutex
 * 4         shared     process-shared flag
 * 5         protocol   whether it is a priority inherit mutex.
 */
#define  MUTEXATTR_TYPE_MASK   0x000f
#define  MUTEXATTR_SHARED_MASK 0x0010
#define MUTEXATTR_PROTOCOL_MASK 0x0020

#define MUTEXATTR_PROTOCOL_SHIFT 5

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

int pthread_mutexattr_setprotocol(pthread_mutexattr_t* attr, int protocol) {
    if (protocol != PTHREAD_PRIO_NONE && protocol != PTHREAD_PRIO_INHERIT) {
        return EINVAL;
    }
    *attr = (*attr & ~MUTEXATTR_PROTOCOL_MASK) | (protocol << MUTEXATTR_PROTOCOL_SHIFT);
    return 0;
}

int pthread_mutexattr_getprotocol(const pthread_mutexattr_t* attr, int* protocol) {
    *protocol = (*attr & MUTEXATTR_PROTOCOL_MASK) >> MUTEXATTR_PROTOCOL_SHIFT;
    return 0;
}

// Priority Inheritance mutex implementation
struct PIMutex {
  // mutex type, can be 0 (normal), 1 (recursive), 2 (errorcheck), constant during lifetime
  uint8_t type;
  // process-shared flag, constant during lifetime
  bool shared;
  // <number of times a thread holding a recursive PI mutex> - 1
  uint16_t counter;
  // owner_tid is read/written by both userspace code and kernel code. It includes three fields:
  // FUTEX_WAITERS, FUTEX_OWNER_DIED and FUTEX_TID_MASK.
  atomic_int owner_tid;
};

static inline __always_inline int PIMutexTryLock(PIMutex& mutex) {
    pid_t tid = __get_thread()->tid;
    // Handle common case first.
    int old_owner = 0;
    if (__predict_true(atomic_compare_exchange_strong_explicit(&mutex.owner_tid,
                                                               &old_owner, tid,
                                                               memory_order_acquire,
                                                               memory_order_relaxed))) {
        return 0;
    }
    if (tid == (old_owner & FUTEX_TID_MASK)) {
        // We already own this mutex.
        if (mutex.type == PTHREAD_MUTEX_NORMAL) {
            return EBUSY;
        }
        if (mutex.type == PTHREAD_MUTEX_ERRORCHECK) {
            return EDEADLK;
        }
        if (mutex.counter == 0xffff) {
            return EAGAIN;
        }
        mutex.counter++;
        return 0;
    }
    return EBUSY;
}

// Inlining this function in pthread_mutex_lock() adds the cost of stack frame instructions on
// ARM/ARM64, which increases at most 20 percent overhead. So make it noinline.
static int  __attribute__((noinline)) PIMutexTimedLock(PIMutex& mutex,
                                                       bool use_realtime_clock,
                                                       const timespec* abs_timeout) {
    int ret = PIMutexTryLock(mutex);
    if (__predict_true(ret == 0)) {
        return 0;
    }
    if (ret == EBUSY) {
        ScopedTrace trace("Contending for pthread mutex");
        ret = -__futex_pi_lock_ex(&mutex.owner_tid, mutex.shared, use_realtime_clock, abs_timeout);
    }
    return ret;
}

static int PIMutexUnlock(PIMutex& mutex) {
    pid_t tid = __get_thread()->tid;
    int old_owner = tid;
    // Handle common case first.
    if (__predict_true(mutex.type == PTHREAD_MUTEX_NORMAL)) {
        if (__predict_true(atomic_compare_exchange_strong_explicit(&mutex.owner_tid,
                                                                   &old_owner, 0,
                                                                   memory_order_release,
                                                                   memory_order_relaxed))) {
            return 0;
        }
    } else {
        old_owner = atomic_load_explicit(&mutex.owner_tid, memory_order_relaxed);
    }

    if (tid != (old_owner & FUTEX_TID_MASK)) {
        // The mutex can only be unlocked by the thread who owns it.
        return EPERM;
    }
    if (mutex.type == PTHREAD_MUTEX_RECURSIVE) {
        if (mutex.counter != 0u) {
            --mutex.counter;
            return 0;
        }
    }
    if (old_owner == tid) {
        // No thread is waiting.
        if (__predict_true(atomic_compare_exchange_strong_explicit(&mutex.owner_tid,
                                                                   &old_owner, 0,
                                                                   memory_order_release,
                                                                   memory_order_relaxed))) {
            return 0;
        }
    }
    return -__futex_pi_unlock(&mutex.owner_tid, mutex.shared);
}

static int PIMutexDestroy(PIMutex& mutex) {
    // The mutex should be in unlocked state (owner_tid == 0) when destroyed.
    // Store 0xffffffff to make the mutex unusable.
    int old_owner = 0;
    if (atomic_compare_exchange_strong_explicit(&mutex.owner_tid, &old_owner, 0xffffffff,
                                                memory_order_relaxed, memory_order_relaxed)) {
        return 0;
    }
    return EBUSY;
}

#if !defined(__LP64__)

namespace PIMutexAllocator {
// pthread_mutex_t has only 4 bytes in 32-bit programs, which are not enough to hold PIMutex.
// So we use malloc to allocate PIMutexes and use 16-bit of pthread_mutex_t as indexes to find
// the allocated PIMutexes. This allows at most 65536 PI mutexes.
// When calling operations like pthread_mutex_lock/unlock, the 16-bit index is mapped to the
// corresponding PIMutex. To make the map operation fast, we use a lockless mapping method:
//   Once a PIMutex is allocated, all the data used to map index to the PIMutex isn't changed until
//   it is destroyed.
// Below are the data structures:
//   // struct Node contains a PIMutex.
//   typedef Node NodeArray[256];
//   typedef NodeArray* NodeArrayP;
//   NodeArrayP nodes[256];
//
// A 16-bit index is mapped to Node as below:
//   (*nodes[index >> 8])[index & 0xff]
//
// Also use a free list to allow O(1) finding recycled PIMutexes.

union Node {
    PIMutex mutex;
    int next_free_id;  // If not -1, refer to the next node in the free PIMutex list.
};
typedef Node NodeArray[256];
typedef NodeArray* NodeArrayP;

// lock_ protects below items.
static Lock lock;
static NodeArrayP* nodes;
static int next_to_alloc_id;
static int first_free_id = -1;  // If not -1, refer to the first node in the free PIMutex list.

static inline __always_inline Node& IdToNode(int id) {
    return (*nodes[id >> 8])[id & 0xff];
}

static inline __always_inline PIMutex& IdToPIMutex(int id) {
    return IdToNode(id).mutex;
}

static int AllocIdLocked() {
    if (first_free_id != -1) {
        int result = first_free_id;
        first_free_id = IdToNode(result).next_free_id;
        return result;
    }
    if (next_to_alloc_id >= 0x10000) {
        return -1;
    }
    int array_pos = next_to_alloc_id >> 8;
    int node_pos = next_to_alloc_id & 0xff;
    if (node_pos == 0) {
        if (array_pos == 0) {
            nodes = static_cast<NodeArray**>(calloc(256, sizeof(NodeArray*)));
            if (nodes == nullptr) {
                return -1;
            }
        }
        nodes[array_pos] = static_cast<NodeArray*>(malloc(sizeof(NodeArray)));
        if (nodes[array_pos] == nullptr) {
            return -1;
        }
    }
    return next_to_alloc_id++;
}

// If succeed, return an id referring to a PIMutex, otherwise return -1.
// A valid id is in range [0, 0xffff].
static int AllocId() {
    lock.lock();
    int result = AllocIdLocked();
    lock.unlock();
    if (result != -1) {
        memset(&IdToPIMutex(result), 0, sizeof(PIMutex));
    }
    return result;
}

static void FreeId(int id) {
    lock.lock();
    IdToNode(id).next_free_id = first_free_id;
    first_free_id = id;
    lock.unlock();
}

}  // namespace PIMutexAllocator

#endif  // !defined(__LP64__)


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

/* Convenience macros.
 *
 * These are used to form or modify the bit pattern of a given mutex value
 */

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

#define  MUTEX_STATE_UNLOCKED            0   /* must be 0 to match PTHREAD_MUTEX_INITIALIZER */
#define  MUTEX_STATE_LOCKED_UNCONTENDED  1   /* must be 1 due to atomic dec in unlock operation */
#define  MUTEX_STATE_LOCKED_CONTENDED    2   /* must be 1 + LOCKED_UNCONTENDED due to atomic dec */

#define  MUTEX_STATE_BITS_UNLOCKED            MUTEX_STATE_TO_BITS(MUTEX_STATE_UNLOCKED)
#define  MUTEX_STATE_BITS_LOCKED_UNCONTENDED  MUTEX_STATE_TO_BITS(MUTEX_STATE_LOCKED_UNCONTENDED)
#define  MUTEX_STATE_BITS_LOCKED_CONTENDED    MUTEX_STATE_TO_BITS(MUTEX_STATE_LOCKED_CONTENDED)

// Return true iff the mutex is unlocked.
#define MUTEX_STATE_BITS_IS_UNLOCKED(v) (((v) & MUTEX_STATE_MASK) == MUTEX_STATE_BITS_UNLOCKED)

// Return true iff the mutex is locked with no waiters.
#define MUTEX_STATE_BITS_IS_LOCKED_UNCONTENDED(v)  (((v) & MUTEX_STATE_MASK) == MUTEX_STATE_BITS_LOCKED_UNCONTENDED)

// return true iff the mutex is locked with maybe waiters.
#define MUTEX_STATE_BITS_IS_LOCKED_CONTENDED(v)   (((v) & MUTEX_STATE_MASK) == MUTEX_STATE_BITS_LOCKED_CONTENDED)

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
 * We support normal, recursive and errorcheck mutexes.
 */
#define  MUTEX_TYPE_SHIFT      14
#define  MUTEX_TYPE_LEN        2
#define  MUTEX_TYPE_MASK       FIELD_MASK(MUTEX_TYPE_SHIFT,MUTEX_TYPE_LEN)

#define  MUTEX_TYPE_TO_BITS(t)       FIELD_TO_BITS(t, MUTEX_TYPE_SHIFT, MUTEX_TYPE_LEN)

#define  MUTEX_TYPE_BITS_NORMAL      MUTEX_TYPE_TO_BITS(PTHREAD_MUTEX_NORMAL)
#define  MUTEX_TYPE_BITS_RECURSIVE   MUTEX_TYPE_TO_BITS(PTHREAD_MUTEX_RECURSIVE)
#define  MUTEX_TYPE_BITS_ERRORCHECK  MUTEX_TYPE_TO_BITS(PTHREAD_MUTEX_ERRORCHECK)
// Use a special mutex type to mark priority inheritance mutexes.
#define  PI_MUTEX_STATE     MUTEX_TYPE_TO_BITS(3)

// For a PI mutex, it includes below fields:
//   Atomic(uint16_t) state;
//   PIMutex pi_mutex;  // uint16_t pi_mutex_id in 32-bit programs
//
//   state holds the following fields:
//
//   bits:   name    description
//   15-14   type    mutex type, should be 3
//   13-0    padding should be 0
//
//   pi_mutex holds the state of a PI mutex.
//   pi_mutex_id holds an integer to find the state of a PI mutex.
//
// For a Non-PI mutex, it includes below fields:
//   Atomic(uint16_t) state;
//   atomic_int owner_tid;  // Atomic(uint16_t) in 32-bit programs
//
//   state holds the following fields:
//
//   bits:     name     description
//   15-14     type     mutex type, can be 0 (normal), 1 (recursive), 2 (errorcheck)
//   13        shared   process-shared flag
//   12-2      counter  <number of times a thread holding a recursive Non-PI mutex> - 1
//   1-0       state    lock state (0, 1 or 2)
//
//   bits 15-13 are constant during the lifetime of the mutex.
//
//   owner_tid is used only in recursive and errorcheck Non-PI mutexes to hold the mutex owner
//   thread id.
//
// PI mutexes and Non-PI mutexes are distinguished by checking type field in state.
#if defined(__LP64__)
struct pthread_mutex_internal_t {
    _Atomic(uint16_t) state;
    uint16_t __pad;
    union {
        atomic_int owner_tid;
        PIMutex pi_mutex;
    };
    char __reserved[28];

    PIMutex& ToPIMutex() {
        return pi_mutex;
    }

    void FreePIMutex() {
    }
} __attribute__((aligned(4)));

#else
struct pthread_mutex_internal_t {
    _Atomic(uint16_t) state;
    union {
        _Atomic(uint16_t) owner_tid;
        uint16_t pi_mutex_id;
    };

    PIMutex& ToPIMutex() {
        return PIMutexAllocator::IdToPIMutex(pi_mutex_id);
    }

    void FreePIMutex() {
        PIMutexAllocator::FreeId(pi_mutex_id);
    }
} __attribute__((aligned(4)));
#endif

static_assert(sizeof(pthread_mutex_t) == sizeof(pthread_mutex_internal_t),
              "pthread_mutex_t should actually be pthread_mutex_internal_t in implementation.");

// For binary compatibility with old version of pthread_mutex_t, we can't use more strict alignment
// than 4-byte alignment.
static_assert(alignof(pthread_mutex_t) == 4,
              "pthread_mutex_t should fulfill the alignment of pthread_mutex_internal_t.");

static inline pthread_mutex_internal_t* __get_internal_mutex(pthread_mutex_t* mutex_interface) {
  return reinterpret_cast<pthread_mutex_internal_t*>(mutex_interface);
}

int pthread_mutex_init(pthread_mutex_t* mutex_interface, const pthread_mutexattr_t* attr) {
    pthread_mutex_internal_t* mutex = __get_internal_mutex(mutex_interface);

    memset(mutex, 0, sizeof(pthread_mutex_internal_t));

    if (__predict_true(attr == nullptr)) {
        atomic_init(&mutex->state, MUTEX_TYPE_BITS_NORMAL);
        return 0;
    }

    uint16_t state = 0;
    if ((*attr & MUTEXATTR_SHARED_MASK) != 0) {
        state |= MUTEX_SHARED_MASK;
    }

    switch (*attr & MUTEXATTR_TYPE_MASK) {
    case PTHREAD_MUTEX_NORMAL:
      state |= MUTEX_TYPE_BITS_NORMAL;
      break;
    case PTHREAD_MUTEX_RECURSIVE:
      state |= MUTEX_TYPE_BITS_RECURSIVE;
      break;
    case PTHREAD_MUTEX_ERRORCHECK:
      state |= MUTEX_TYPE_BITS_ERRORCHECK;
      break;
    default:
        return EINVAL;
    }

    if (((*attr & MUTEXATTR_PROTOCOL_MASK) >> MUTEXATTR_PROTOCOL_SHIFT) == PTHREAD_PRIO_INHERIT) {
#if !defined(__LP64__)
        if (state & MUTEX_SHARED_MASK) {
            return EINVAL;
        }
        int id = PIMutexAllocator::AllocId();
        if (id == -1) {
            return ENOMEM;
        }
        mutex->pi_mutex_id = id;
#endif
        atomic_init(&mutex->state, PI_MUTEX_STATE);
        PIMutex& pi_mutex = mutex->ToPIMutex();
        pi_mutex.type = *attr & MUTEXATTR_TYPE_MASK;
        pi_mutex.shared = (*attr & MUTEXATTR_SHARED_MASK) != 0;
    } else {
        atomic_init(&mutex->state, state);
        atomic_init(&mutex->owner_tid, 0);
    }
    return 0;
}

// namespace for Non-PI mutex routines.
namespace NonPI {

static inline __always_inline int NormalMutexTryLock(pthread_mutex_internal_t* mutex,
                                                     uint16_t shared) {
    const uint16_t unlocked           = shared | MUTEX_STATE_BITS_UNLOCKED;
    const uint16_t locked_uncontended = shared | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;

    uint16_t old_state = unlocked;
    if (__predict_true(atomic_compare_exchange_strong_explicit(&mutex->state, &old_state,
                         locked_uncontended, memory_order_acquire, memory_order_relaxed))) {
        return 0;
    }
    return EBUSY;
}

/*
 * Lock a normal Non-PI mutex.
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
static inline __always_inline int NormalMutexLock(pthread_mutex_internal_t* mutex,
                                                  uint16_t shared,
                                                  bool use_realtime_clock,
                                                  const timespec* abs_timeout_or_null) {
    if (__predict_true(NormalMutexTryLock(mutex, shared) == 0)) {
        return 0;
    }
    int result = check_timespec(abs_timeout_or_null, true);
    if (result != 0) {
        return result;
    }

    ScopedTrace trace("Contending for pthread mutex");

    const uint16_t unlocked           = shared | MUTEX_STATE_BITS_UNLOCKED;
    const uint16_t locked_contended = shared | MUTEX_STATE_BITS_LOCKED_CONTENDED;

    // We want to go to sleep until the mutex is available, which requires
    // promoting it to locked_contended. We need to swap in the new state
    // and then wait until somebody wakes us up.
    // An atomic_exchange is used to compete with other threads for the lock.
    // If it returns unlocked, we have acquired the lock, otherwise another
    // thread still holds the lock and we should wait again.
    // If lock is acquired, an acquire fence is needed to make all memory accesses
    // made by other threads visible to the current CPU.
    while (atomic_exchange_explicit(&mutex->state, locked_contended,
                                    memory_order_acquire) != unlocked) {
        if (__futex_wait_ex(&mutex->state, shared, locked_contended, use_realtime_clock,
                            abs_timeout_or_null) == -ETIMEDOUT) {
            return ETIMEDOUT;
        }
    }
    return 0;
}

/*
 * Release a normal Non-PI mutex.  The caller is responsible for determining
 * that we are in fact the owner of this lock.
 */
static inline __always_inline void NormalMutexUnlock(pthread_mutex_internal_t* mutex,
                                                     uint16_t shared) {
    const uint16_t unlocked         = shared | MUTEX_STATE_BITS_UNLOCKED;
    const uint16_t locked_contended = shared | MUTEX_STATE_BITS_LOCKED_CONTENDED;

    // We use an atomic_exchange to release the lock. If locked_contended state
    // is returned, some threads is waiting for the lock and we need to wake up
    // one of them.
    // A release fence is required to make previous stores visible to next
    // lock owner threads.
    if (atomic_exchange_explicit(&mutex->state, unlocked,
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
        //
        // The pthread_mutex_internal_t object may have been deallocated between the
        // atomic exchange and the wake call. In that case, this wake call could
        // target unmapped memory or memory used by an otherwise unrelated futex
        // operation. Even if the kernel avoids spurious futex wakeups from its
        // point of view, this wake call could trigger a spurious wakeup in any
        // futex accessible from this process. References:
        //  - https://lkml.org/lkml/2014/11/27/472
        //  - http://austingroupbugs.net/view.php?id=811#c2267
        __futex_wake_ex(&mutex->state, shared, 1);
    }
}

/* This common inlined function is used to increment the counter of a recursive Non-PI mutex.
 *
 * If the counter overflows, it will return EAGAIN.
 * Otherwise, it atomically increments the counter and returns 0.
 *
 */
static inline __always_inline int RecursiveIncrement(pthread_mutex_internal_t* mutex,
                                                     uint16_t old_state) {
    // Detect recursive lock overflow and return EAGAIN.
    // This is safe because only the owner thread can modify the
    // counter bits in the mutex value.
    if (MUTEX_COUNTER_BITS_WILL_OVERFLOW(old_state)) {
        return EAGAIN;
    }

    // Other threads are able to change the lower bits (e.g. promoting it to "contended"),
    // but the mutex counter will not overflow. So we use atomic_fetch_add operation here.
    // The mutex is already locked by current thread, so we don't need an acquire fence.
    atomic_fetch_add_explicit(&mutex->state, MUTEX_COUNTER_BITS_ONE, memory_order_relaxed);
    return 0;
}

// Wait on a recursive or errorcheck Non-PI mutex.
static inline __always_inline int RecursiveOrErrorcheckMutexWait(pthread_mutex_internal_t* mutex,
                                                                 uint16_t shared,
                                                                 uint16_t old_state,
                                                                 bool use_realtime_clock,
                                                                 const timespec* abs_timeout) {
// __futex_wait always waits on a 32-bit value. But state is 16-bit. For a normal mutex, the owner_tid
// field in mutex is not used. On 64-bit devices, the __pad field in mutex is not used.
// But when a recursive or errorcheck mutex is used on 32-bit devices, we need to add the
// owner_tid value in the value argument for __futex_wait, otherwise we may always get EAGAIN error.

#if defined(__LP64__)
  return __futex_wait_ex(&mutex->state, shared, old_state, use_realtime_clock, abs_timeout);

#else
  // This implementation works only when the layout of pthread_mutex_internal_t matches below expectation.
  // And it is based on the assumption that Android is always in little-endian devices.
  static_assert(offsetof(pthread_mutex_internal_t, state) == 0, "");
  static_assert(offsetof(pthread_mutex_internal_t, owner_tid) == 2, "");

  uint32_t owner_tid = atomic_load_explicit(&mutex->owner_tid, memory_order_relaxed);
  return __futex_wait_ex(&mutex->state, shared, (owner_tid << 16) | old_state,
                         use_realtime_clock, abs_timeout);
#endif
}

// Lock a Non-PI mutex.
static int MutexLockWithTimeout(pthread_mutex_internal_t* mutex, bool use_realtime_clock,
                                const timespec* abs_timeout_or_null) {
    uint16_t old_state = atomic_load_explicit(&mutex->state, memory_order_relaxed);
    uint16_t mtype = (old_state & MUTEX_TYPE_MASK);
    uint16_t shared = (old_state & MUTEX_SHARED_MASK);

    // Handle common case first.
    if ( __predict_true(mtype == MUTEX_TYPE_BITS_NORMAL) ) {
        return NormalMutexLock(mutex, shared, use_realtime_clock, abs_timeout_or_null);
    }

    // Do we already own this recursive or error-check mutex?
    pid_t tid = __get_thread()->tid;
    if (tid == atomic_load_explicit(&mutex->owner_tid, memory_order_relaxed)) {
        if (mtype == MUTEX_TYPE_BITS_ERRORCHECK) {
            return EDEADLK;
        }
        return RecursiveIncrement(mutex, old_state);
    }

    const uint16_t unlocked           = mtype | shared | MUTEX_STATE_BITS_UNLOCKED;
    const uint16_t locked_uncontended = mtype | shared | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;
    const uint16_t locked_contended   = mtype | shared | MUTEX_STATE_BITS_LOCKED_CONTENDED;

    // First, if the mutex is unlocked, try to quickly acquire it.
    // In the optimistic case where this works, set the state to locked_uncontended.
    if (old_state == unlocked) {
        // If exchanged successfully, an acquire fence is required to make
        // all memory accesses made by other threads visible to the current CPU.
        if (__predict_true(atomic_compare_exchange_strong_explicit(&mutex->state, &old_state,
                             locked_uncontended, memory_order_acquire, memory_order_relaxed))) {
            atomic_store_explicit(&mutex->owner_tid, tid, memory_order_relaxed);
            return 0;
        }
    }

    ScopedTrace trace("Contending for pthread mutex");

    while (true) {
        if (old_state == unlocked) {
            // NOTE: We put the state to locked_contended since we _know_ there
            // is contention when we are in this loop. This ensures all waiters
            // will be unlocked.

            // If exchanged successfully, an acquire fence is required to make
            // all memory accesses made by other threads visible to the current CPU.
            if (__predict_true(atomic_compare_exchange_weak_explicit(&mutex->state,
                                                                     &old_state, locked_contended,
                                                                     memory_order_acquire,
                                                                     memory_order_relaxed))) {
                atomic_store_explicit(&mutex->owner_tid, tid, memory_order_relaxed);
                return 0;
            }
            continue;
        } else if (MUTEX_STATE_BITS_IS_LOCKED_UNCONTENDED(old_state)) {
            // We should set it to locked_contended beforing going to sleep. This can make
            // sure waiters will be woken up eventually.

            int new_state = MUTEX_STATE_BITS_FLIP_CONTENTION(old_state);
            if (__predict_false(!atomic_compare_exchange_weak_explicit(&mutex->state,
                                                                       &old_state, new_state,
                                                                       memory_order_relaxed,
                                                                       memory_order_relaxed))) {
                continue;
            }
            old_state = new_state;
        }

        int result = check_timespec(abs_timeout_or_null, true);
        if (result != 0) {
            return result;
        }
        // We are in locked_contended state, sleep until someone wakes us up.
        if (RecursiveOrErrorcheckMutexWait(mutex, shared, old_state, use_realtime_clock,
                                           abs_timeout_or_null) == -ETIMEDOUT) {
            return ETIMEDOUT;
        }
        old_state = atomic_load_explicit(&mutex->state, memory_order_relaxed);
    }
}

}  // namespace NonPI

static inline __always_inline bool IsMutexDestroyed(uint16_t mutex_state) {
    return mutex_state == 0xffff;
}

// Inlining this function in pthread_mutex_lock() adds the cost of stack frame instructions on
// ARM64. So make it noinline.
static int __attribute__((noinline)) HandleUsingDestroyedMutex(pthread_mutex_t* mutex,
                                                               const char* function_name) {
    if (android_get_application_target_sdk_version() >= 28) {
        __fortify_fatal("%s called on a destroyed mutex (%p)", function_name, mutex);
    }
    return EBUSY;
}

int pthread_mutex_lock(pthread_mutex_t* mutex_interface) {
#if !defined(__LP64__)
    // Some apps depend on being able to pass NULL as a mutex and get EINVAL
    // back. Don't need to worry about it for LP64 since the ABI is brand new,
    // but keep compatibility for LP32. http://b/19995172.
    if (mutex_interface == nullptr) {
        return EINVAL;
    }
#endif

    pthread_mutex_internal_t* mutex = __get_internal_mutex(mutex_interface);
    uint16_t old_state = atomic_load_explicit(&mutex->state, memory_order_relaxed);
    uint16_t mtype = (old_state & MUTEX_TYPE_MASK);
    // Avoid slowing down fast path of normal mutex lock operation.
    if (__predict_true(mtype == MUTEX_TYPE_BITS_NORMAL)) {
        uint16_t shared = (old_state & MUTEX_SHARED_MASK);
        if (__predict_true(NonPI::NormalMutexTryLock(mutex, shared) == 0)) {
            return 0;
        }
    }
    if (old_state == PI_MUTEX_STATE) {
        PIMutex& m = mutex->ToPIMutex();
        // Handle common case first.
        if (__predict_true(PIMutexTryLock(m) == 0)) {
            return 0;
        }
        return PIMutexTimedLock(mutex->ToPIMutex(), false, nullptr);
    }
    if (__predict_false(IsMutexDestroyed(old_state))) {
        return HandleUsingDestroyedMutex(mutex_interface, __FUNCTION__);
    }
    return NonPI::MutexLockWithTimeout(mutex, false, nullptr);
}

int pthread_mutex_unlock(pthread_mutex_t* mutex_interface) {
#if !defined(__LP64__)
    // Some apps depend on being able to pass NULL as a mutex and get EINVAL
    // back. Don't need to worry about it for LP64 since the ABI is brand new,
    // but keep compatibility for LP32. http://b/19995172.
    if (mutex_interface == nullptr) {
        return EINVAL;
    }
#endif

    pthread_mutex_internal_t* mutex = __get_internal_mutex(mutex_interface);
    uint16_t old_state = atomic_load_explicit(&mutex->state, memory_order_relaxed);
    uint16_t mtype  = (old_state & MUTEX_TYPE_MASK);
    uint16_t shared = (old_state & MUTEX_SHARED_MASK);

    // Handle common case first.
    if (__predict_true(mtype == MUTEX_TYPE_BITS_NORMAL)) {
        NonPI::NormalMutexUnlock(mutex, shared);
        return 0;
    }
    if (old_state == PI_MUTEX_STATE) {
        return PIMutexUnlock(mutex->ToPIMutex());
    }
    if (__predict_false(IsMutexDestroyed(old_state))) {
        return HandleUsingDestroyedMutex(mutex_interface, __FUNCTION__);
    }

    // Do we already own this recursive or error-check mutex?
    pid_t tid = __get_thread()->tid;
    if ( tid != atomic_load_explicit(&mutex->owner_tid, memory_order_relaxed) ) {
        return EPERM;
    }

    // If the counter is > 0, we can simply decrement it atomically.
    // Since other threads can mutate the lower state bits (and only the
    // lower state bits), use a compare_exchange loop to do it.
    if (!MUTEX_COUNTER_BITS_IS_ZERO(old_state)) {
        // We still own the mutex, so a release fence is not needed.
        atomic_fetch_sub_explicit(&mutex->state, MUTEX_COUNTER_BITS_ONE, memory_order_relaxed);
        return 0;
    }

    // The counter is 0, so we'are going to unlock the mutex by resetting its
    // state to unlocked, we need to perform a atomic_exchange inorder to read
    // the current state, which will be locked_contended if there may have waiters
    // to awake.
    // A release fence is required to make previous stores visible to next
    // lock owner threads.
    atomic_store_explicit(&mutex->owner_tid, 0, memory_order_relaxed);
    const uint16_t unlocked = mtype | shared | MUTEX_STATE_BITS_UNLOCKED;
    old_state = atomic_exchange_explicit(&mutex->state, unlocked, memory_order_release);
    if (MUTEX_STATE_BITS_IS_LOCKED_CONTENDED(old_state)) {
        __futex_wake_ex(&mutex->state, shared, 1);
    }

    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t* mutex_interface) {
    pthread_mutex_internal_t* mutex = __get_internal_mutex(mutex_interface);

    uint16_t old_state = atomic_load_explicit(&mutex->state, memory_order_relaxed);
    uint16_t mtype  = (old_state & MUTEX_TYPE_MASK);

    // Handle common case first.
    if (__predict_true(mtype == MUTEX_TYPE_BITS_NORMAL)) {
        uint16_t shared = (old_state & MUTEX_SHARED_MASK);
        return NonPI::NormalMutexTryLock(mutex, shared);
    }
    if (old_state == PI_MUTEX_STATE) {
        return PIMutexTryLock(mutex->ToPIMutex());
    }
    if (__predict_false(IsMutexDestroyed(old_state))) {
        return HandleUsingDestroyedMutex(mutex_interface, __FUNCTION__);
    }

    // Do we already own this recursive or error-check mutex?
    pid_t tid = __get_thread()->tid;
    if (tid == atomic_load_explicit(&mutex->owner_tid, memory_order_relaxed)) {
        if (mtype == MUTEX_TYPE_BITS_ERRORCHECK) {
            return EBUSY;
        }
        return NonPI::RecursiveIncrement(mutex, old_state);
    }

    uint16_t shared = (old_state & MUTEX_SHARED_MASK);
    const uint16_t unlocked           = mtype | shared | MUTEX_STATE_BITS_UNLOCKED;
    const uint16_t locked_uncontended = mtype | shared | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;

    // Same as pthread_mutex_lock, except that we don't want to wait, and
    // the only operation that can succeed is a single compare_exchange to acquire the
    // lock if it is released / not owned by anyone. No need for a complex loop.
    // If exchanged successfully, an acquire fence is required to make
    // all memory accesses made by other threads visible to the current CPU.
    old_state = unlocked;
    if (__predict_true(atomic_compare_exchange_strong_explicit(&mutex->state, &old_state,
                                                               locked_uncontended,
                                                               memory_order_acquire,
                                                               memory_order_relaxed))) {
        atomic_store_explicit(&mutex->owner_tid, tid, memory_order_relaxed);
        return 0;
    }
    return EBUSY;
}

#if !defined(__LP64__)
extern "C" int pthread_mutex_lock_timeout_np(pthread_mutex_t* mutex_interface, unsigned ms) {
    timespec ts;
    timespec_from_ms(ts, ms);
    timespec abs_timeout;
    absolute_timespec_from_timespec(abs_timeout, ts, CLOCK_MONOTONIC);
    int error = NonPI::MutexLockWithTimeout(__get_internal_mutex(mutex_interface), false,
                                            &abs_timeout);
    if (error == ETIMEDOUT) {
        error = EBUSY;
    }
    return error;
}
#endif

static int __pthread_mutex_timedlock(pthread_mutex_t* mutex_interface, bool use_realtime_clock,
                                     const timespec* abs_timeout, const char* function) {
    pthread_mutex_internal_t* mutex = __get_internal_mutex(mutex_interface);
    uint16_t old_state = atomic_load_explicit(&mutex->state, memory_order_relaxed);
    uint16_t mtype = (old_state & MUTEX_TYPE_MASK);
    // Handle common case first.
    if (__predict_true(mtype == MUTEX_TYPE_BITS_NORMAL)) {
        uint16_t shared = (old_state & MUTEX_SHARED_MASK);
        if (__predict_true(NonPI::NormalMutexTryLock(mutex, shared) == 0)) {
            return 0;
        }
    }
    if (old_state == PI_MUTEX_STATE) {
        return PIMutexTimedLock(mutex->ToPIMutex(), use_realtime_clock, abs_timeout);
    }
    if (__predict_false(IsMutexDestroyed(old_state))) {
        return HandleUsingDestroyedMutex(mutex_interface, function);
    }
    return NonPI::MutexLockWithTimeout(mutex, use_realtime_clock, abs_timeout);
}

int pthread_mutex_timedlock(pthread_mutex_t* mutex_interface, const struct timespec* abs_timeout) {
    return __pthread_mutex_timedlock(mutex_interface, true, abs_timeout, __FUNCTION__);
}

int pthread_mutex_timedlock_monotonic_np(pthread_mutex_t* mutex_interface,
                                         const struct timespec* abs_timeout) {
    return __pthread_mutex_timedlock(mutex_interface, false, abs_timeout, __FUNCTION__);
}

int pthread_mutex_clocklock(pthread_mutex_t* mutex_interface, clockid_t clock,
                            const struct timespec* abs_timeout) {
  switch (clock) {
    case CLOCK_MONOTONIC:
      return __pthread_mutex_timedlock(mutex_interface, false, abs_timeout, __FUNCTION__);
    case CLOCK_REALTIME:
      return __pthread_mutex_timedlock(mutex_interface, true, abs_timeout, __FUNCTION__);
    default: {
      pthread_mutex_internal_t* mutex = __get_internal_mutex(mutex_interface);
      uint16_t old_state = atomic_load_explicit(&mutex->state, memory_order_relaxed);
      if (IsMutexDestroyed(old_state)) {
        return HandleUsingDestroyedMutex(mutex_interface, __FUNCTION__);
      }
      return EINVAL;
    }
  }
}

int pthread_mutex_destroy(pthread_mutex_t* mutex_interface) {
    pthread_mutex_internal_t* mutex = __get_internal_mutex(mutex_interface);
    uint16_t old_state = atomic_load_explicit(&mutex->state, memory_order_relaxed);
    if (__predict_false(IsMutexDestroyed(old_state))) {
        return HandleUsingDestroyedMutex(mutex_interface, __FUNCTION__);
    }
    if (old_state == PI_MUTEX_STATE) {
        int result = PIMutexDestroy(mutex->ToPIMutex());
        if (result == 0) {
            mutex->FreePIMutex();
            atomic_store(&mutex->state, 0xffff);
        }
        return result;
    }
    // Store 0xffff to make the mutex unusable. Although POSIX standard says it is undefined
    // behavior to destroy a locked mutex, we prefer not to change mutex->state in that situation.
    if (MUTEX_STATE_BITS_IS_UNLOCKED(old_state) &&
        atomic_compare_exchange_strong_explicit(&mutex->state, &old_state, 0xffff,
                                                memory_order_relaxed, memory_order_relaxed)) {
      return 0;
    }
    return EBUSY;
}
