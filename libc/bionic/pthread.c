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
#include <sys/atomics.h>
#include <sys/mman.h>
#include <unistd.h>

#include "bionic_atomic_inline.h"
#include "bionic_futex.h"
#include "bionic_pthread.h"
#include "bionic_tls.h"
#include "pthread_internal.h"
#include "thread_private.h"

extern void pthread_debug_mutex_lock_check(pthread_mutex_t *mutex);
extern void pthread_debug_mutex_unlock_check(pthread_mutex_t *mutex);

extern void _exit_with_stack_teardown(void * stackBase, int stackSize, int retCode);
extern void _exit_thread(int  retCode);

int  __futex_wake_ex(volatile void *ftx, int pshared, int val)
{
    return __futex_syscall3(ftx, pshared ? FUTEX_WAKE : FUTEX_WAKE_PRIVATE, val);
}

int  __futex_wait_ex(volatile void *ftx, int pshared, int val, const struct timespec *timeout)
{
    return __futex_syscall4(ftx, pshared ? FUTEX_WAIT : FUTEX_WAIT_PRIVATE, val, timeout);
}

/* CAVEAT: our implementation of pthread_cleanup_push/pop doesn't support C++ exceptions
 *         and thread cancelation
 */

void __pthread_cleanup_push( __pthread_cleanup_t*      c,
                             __pthread_cleanup_func_t  routine,
                             void*                     arg )
{
    pthread_internal_t*  thread = __get_thread();

    c->__cleanup_routine  = routine;
    c->__cleanup_arg      = arg;
    c->__cleanup_prev     = thread->cleanup_stack;
    thread->cleanup_stack = c;
}

void __pthread_cleanup_pop( __pthread_cleanup_t*  c, int  execute )
{
    pthread_internal_t*  thread = __get_thread();

    thread->cleanup_stack = c->__cleanup_prev;
    if (execute)
        c->__cleanup_routine(c->__cleanup_arg);
}

void pthread_exit(void * retval)
{
    pthread_internal_t*  thread     = __get_thread();
    void*                stack_base = thread->attr.stack_base;
    int                  stack_size = thread->attr.stack_size;
    int                  user_stack = (thread->attr.flags & PTHREAD_ATTR_FLAG_USER_STACK) != 0;
    sigset_t mask;

    // call the cleanup handlers first
    while (thread->cleanup_stack) {
        __pthread_cleanup_t*  c = thread->cleanup_stack;
        thread->cleanup_stack   = c->__cleanup_prev;
        c->__cleanup_routine(c->__cleanup_arg);
    }

    // call the TLS destructors, it is important to do that before removing this
    // thread from the global list. this will ensure that if someone else deletes
    // a TLS key, the corresponding value will be set to NULL in this thread's TLS
    // space (see pthread_key_delete)
    pthread_key_clean_all();

    if (thread->alternate_signal_stack != NULL) {
      // Tell the kernel to stop using the alternate signal stack.
      stack_t ss;
      ss.ss_sp = NULL;
      ss.ss_flags = SS_DISABLE;
      sigaltstack(&ss, NULL);

      // Free it.
      munmap(thread->alternate_signal_stack, SIGSTKSZ);
      thread->alternate_signal_stack = NULL;
    }

    // if the thread is detached, destroy the pthread_internal_t
    // otherwise, keep it in memory and signal any joiners.
    pthread_mutex_lock(&gThreadListLock);
    if (thread->attr.flags & PTHREAD_ATTR_FLAG_DETACHED) {
        _pthread_internal_remove_locked(thread);
    } else {
       /* make sure that the thread struct doesn't have stale pointers to a stack that
        * will be unmapped after the exit call below.
        */
        if (!user_stack) {
            thread->attr.stack_base = NULL;
            thread->attr.stack_size = 0;
            thread->tls = NULL;
        }

       /* Indicate that the thread has exited for joining threads. */
        thread->attr.flags |= PTHREAD_ATTR_FLAG_ZOMBIE;
        thread->return_value = retval;

       /* Signal the joining thread if present. */
        if (thread->attr.flags & PTHREAD_ATTR_FLAG_JOINED) {
            pthread_cond_signal(&thread->join_cond);
        }
    }
    pthread_mutex_unlock(&gThreadListLock);

    sigfillset(&mask);
    sigdelset(&mask, SIGSEGV);
    (void)sigprocmask(SIG_SETMASK, &mask, (sigset_t *)NULL);

    // destroy the thread stack
    if (user_stack)
        _exit_thread((int)retval);
    else
        _exit_with_stack_teardown(stack_base, stack_size, (int)retval);
}

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

#define  MUTEX_STATE_FROM_BITS(v)    FIELD_FROM_BITS(v, MUTEX_STATE_SHIFT, MUTEX_STATE_LEN)
#define  MUTEX_STATE_TO_BITS(v)      FIELD_TO_BITS(v, MUTEX_STATE_SHIFT, MUTEX_STATE_LEN)

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
#define  MUTEX_COUNTER_BITS_ONE      FIELD_TO_BITS(1,MUTEX_COUNTER_SHIFT,MUTEX_COUNTER_LEN)

/* Returns true iff the counter is 0 */
#define  MUTEX_COUNTER_BITS_ARE_ZERO(v)  (((v) & MUTEX_COUNTER_MASK) == 0)

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
 * tid of the owning thread. Note that this works because the Linux
 * kernel _only_ uses 16-bit values for tids.
 *
 * More specifically, it will wrap to 10000 when it reaches over 32768 for
 * application processes. You can check this by running the following inside
 * an adb shell session:
 *
    OLDPID=$$;
    while true; do
    NEWPID=$(sh -c 'echo $$')
    if [ "$NEWPID" -gt 32768 ]; then
        echo "AARGH: new PID $NEWPID is too high!"
        exit 1
    fi
    if [ "$NEWPID" -lt "$OLDPID" ]; then
        echo "****** Wrapping from PID $OLDPID to $NEWPID. *******"
    else
        echo -n "$NEWPID!"
    fi
    OLDPID=$NEWPID
    done

 * Note that you can run the same example on a desktop Linux system,
 * the wrapping will also happen at 32768, but will go back to 300 instead.
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
    if (attr) {
        *attr = PTHREAD_MUTEX_DEFAULT;
        return 0;
    } else {
        return EINVAL;
    }
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    if (attr) {
        *attr = -1;
        return 0;
    } else {
        return EINVAL;
    }
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type)
{
    if (attr) {
        int  atype = (*attr & MUTEXATTR_TYPE_MASK);

         if (atype >= PTHREAD_MUTEX_NORMAL &&
             atype <= PTHREAD_MUTEX_ERRORCHECK) {
            *type = atype;
            return 0;
        }
    }
    return EINVAL;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    if (attr && type >= PTHREAD_MUTEX_NORMAL &&
                type <= PTHREAD_MUTEX_ERRORCHECK ) {
        *attr = (*attr & ~MUTEXATTR_TYPE_MASK) | type;
        return 0;
    }
    return EINVAL;
}

/* process-shared mutexes are not supported at the moment */

int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int  pshared)
{
    if (!attr)
        return EINVAL;

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

int pthread_mutexattr_getpshared(pthread_mutexattr_t *attr, int *pshared)
{
    if (!attr || !pshared)
        return EINVAL;

    *pshared = (*attr & MUTEXATTR_SHARED_MASK) ? PTHREAD_PROCESS_SHARED
                                               : PTHREAD_PROCESS_PRIVATE;
    return 0;
}

int pthread_mutex_init(pthread_mutex_t *mutex,
                       const pthread_mutexattr_t *attr)
{
    int value = 0;

    if (mutex == NULL)
        return EINVAL;

    if (__predict_true(attr == NULL)) {
        mutex->value = MUTEX_TYPE_BITS_NORMAL;
        return 0;
    }

    if ((*attr & MUTEXATTR_SHARED_MASK) != 0)
        value |= MUTEX_SHARED_MASK;

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

    mutex->value = value;
    return 0;
}


/*
 * Lock a non-recursive mutex.
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
static __inline__ void
_normal_lock(pthread_mutex_t*  mutex, int shared)
{
    /* convenience shortcuts */
    const int unlocked           = shared | MUTEX_STATE_BITS_UNLOCKED;
    const int locked_uncontended = shared | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;
    /*
     * The common case is an unlocked mutex, so we begin by trying to
     * change the lock's state from 0 (UNLOCKED) to 1 (LOCKED).
     * __bionic_cmpxchg() returns 0 if it made the swap successfully.
     * If the result is nonzero, this lock is already held by another thread.
     */
    if (__bionic_cmpxchg(unlocked, locked_uncontended, &mutex->value) != 0) {
        const int locked_contended = shared | MUTEX_STATE_BITS_LOCKED_CONTENDED;
        /*
         * We want to go to sleep until the mutex is available, which
         * requires promoting it to state 2 (CONTENDED). We need to
         * swap in the new state value and then wait until somebody wakes us up.
         *
         * __bionic_swap() returns the previous value.  We swap 2 in and
         * see if we got zero back; if so, we have acquired the lock.  If
         * not, another thread still holds the lock and we wait again.
         *
         * The second argument to the __futex_wait() call is compared
         * against the current value.  If it doesn't match, __futex_wait()
         * returns immediately (otherwise, it sleeps for a time specified
         * by the third argument; 0 means sleep forever).  This ensures
         * that the mutex is in state 2 when we go to sleep on it, which
         * guarantees a wake-up call.
         */
        while (__bionic_swap(locked_contended, &mutex->value) != unlocked)
            __futex_wait_ex(&mutex->value, shared, locked_contended, 0);
    }
    ANDROID_MEMBAR_FULL();
}

/*
 * Release a non-recursive mutex.  The caller is responsible for determining
 * that we are in fact the owner of this lock.
 */
static __inline__ void
_normal_unlock(pthread_mutex_t*  mutex, int shared)
{
    ANDROID_MEMBAR_FULL();

    /*
     * The mutex state will be 1 or (rarely) 2.  We use an atomic decrement
     * to release the lock.  __bionic_atomic_dec() returns the previous value;
     * if it wasn't 1 we have to do some additional work.
     */
    if (__bionic_atomic_dec(&mutex->value) != (shared|MUTEX_STATE_BITS_LOCKED_UNCONTENDED)) {
        /*
         * Start by releasing the lock.  The decrement changed it from
         * "contended lock" to "uncontended lock", which means we still
         * hold it, and anybody who tries to sneak in will push it back
         * to state 2.
         *
         * Once we set it to zero the lock is up for grabs.  We follow
         * this with a __futex_wake() to ensure that one of the waiting
         * threads has a chance to grab it.
         *
         * This doesn't cause a race with the swap/wait pair in
         * _normal_lock(), because the __futex_wait() call there will
         * return immediately if the mutex value isn't 2.
         */
        mutex->value = shared;

        /*
         * Wake up one waiting thread.  We don't know which thread will be
         * woken or when it'll start executing -- futexes make no guarantees
         * here.  There may not even be a thread waiting.
         *
         * The newly-woken thread will replace the 0 we just set above
         * with 2, which means that when it eventually releases the mutex
         * it will also call FUTEX_WAKE.  This results in one extra wake
         * call whenever a lock is contended, but lets us avoid forgetting
         * anyone without requiring us to track the number of sleepers.
         *
         * It's possible for another thread to sneak in and grab the lock
         * between the zero assignment above and the wake call below.  If
         * the new thread is "slow" and holds the lock for a while, we'll
         * wake up a sleeper, which will swap in a 2 and then go back to
         * sleep since the lock is still held.  If the new thread is "fast",
         * running to completion before we call wake, the thread we
         * eventually wake will find an unlocked mutex and will execute.
         * Either way we have correct behavior and nobody is orphaned on
         * the wait queue.
         */
        __futex_wake_ex(&mutex->value, shared, 1);
    }
}

/* This common inlined function is used to increment the counter of an
 * errorcheck or recursive mutex.
 *
 * For errorcheck mutexes, it will return EDEADLK
 * If the counter overflows, it will return EAGAIN
 * Otherwise, it atomically increments the counter and returns 0
 * after providing an acquire barrier.
 *
 * mtype is the current mutex type
 * mvalue is the current mutex value (already loaded)
 * mutex pointers to the mutex.
 */
static __inline__ __attribute__((always_inline)) int
_recursive_increment(pthread_mutex_t* mutex, int mvalue, int mtype)
{
    if (mtype == MUTEX_TYPE_BITS_ERRORCHECK) {
        /* trying to re-lock a mutex we already acquired */
        return EDEADLK;
    }

    /* Detect recursive lock overflow and return EAGAIN.
     * This is safe because only the owner thread can modify the
     * counter bits in the mutex value.
     */
    if (MUTEX_COUNTER_BITS_WILL_OVERFLOW(mvalue)) {
        return EAGAIN;
    }

    /* We own the mutex, but other threads are able to change
     * the lower bits (e.g. promoting it to "contended"), so we
     * need to use an atomic cmpxchg loop to update the counter.
     */
    for (;;) {
        /* increment counter, overflow was already checked */
        int newval = mvalue + MUTEX_COUNTER_BITS_ONE;
        if (__predict_true(__bionic_cmpxchg(mvalue, newval, &mutex->value) == 0)) {
            /* mutex is still locked, not need for a memory barrier */
            return 0;
        }
        /* the value was changed, this happens when another thread changes
         * the lower state bits from 1 to 2 to indicate contention. This
         * cannot change the counter, so simply reload and try again.
         */
        mvalue = mutex->value;
    }
}

__LIBC_HIDDEN__
int pthread_mutex_lock_impl(pthread_mutex_t *mutex)
{
    int mvalue, mtype, tid, shared;

    if (__predict_false(mutex == NULL))
        return EINVAL;

    mvalue = mutex->value;
    mtype = (mvalue & MUTEX_TYPE_MASK);
    shared = (mvalue & MUTEX_SHARED_MASK);

    /* Handle normal case first */
    if ( __predict_true(mtype == MUTEX_TYPE_BITS_NORMAL) ) {
        _normal_lock(mutex, shared);
        return 0;
    }

    /* Do we already own this recursive or error-check mutex ? */
    tid = __get_thread()->tid;
    if ( tid == MUTEX_OWNER_FROM_BITS(mvalue) )
        return _recursive_increment(mutex, mvalue, mtype);

    /* Add in shared state to avoid extra 'or' operations below */
    mtype |= shared;

    /* First, if the mutex is unlocked, try to quickly acquire it.
     * In the optimistic case where this works, set the state to 1 to
     * indicate locked with no contention */
    if (mvalue == mtype) {
        int newval = MUTEX_OWNER_TO_BITS(tid) | mtype | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;
        if (__bionic_cmpxchg(mvalue, newval, &mutex->value) == 0) {
            ANDROID_MEMBAR_FULL();
            return 0;
        }
        /* argh, the value changed, reload before entering the loop */
        mvalue = mutex->value;
    }

    for (;;) {
        int newval;

        /* if the mutex is unlocked, its value should be 'mtype' and
         * we try to acquire it by setting its owner and state atomically.
         * NOTE: We put the state to 2 since we _know_ there is contention
         * when we are in this loop. This ensures all waiters will be
         * unlocked.
         */
        if (mvalue == mtype) {
            newval = MUTEX_OWNER_TO_BITS(tid) | mtype | MUTEX_STATE_BITS_LOCKED_CONTENDED;
            /* TODO: Change this to __bionic_cmpxchg_acquire when we
             *        implement it to get rid of the explicit memory
             *        barrier below.
             */
            if (__predict_false(__bionic_cmpxchg(mvalue, newval, &mutex->value) != 0)) {
                mvalue = mutex->value;
                continue;
            }
            ANDROID_MEMBAR_FULL();
            return 0;
        }

        /* the mutex is already locked by another thread, if its state is 1
         * we will change it to 2 to indicate contention. */
        if (MUTEX_STATE_BITS_IS_LOCKED_UNCONTENDED(mvalue)) {
            newval = MUTEX_STATE_BITS_FLIP_CONTENTION(mvalue); /* locked state 1 => state 2 */
            if (__predict_false(__bionic_cmpxchg(mvalue, newval, &mutex->value) != 0)) {
                mvalue = mutex->value;
                continue;
            }
            mvalue = newval;
        }

        /* wait until the mutex is unlocked */
        __futex_wait_ex(&mutex->value, shared, mvalue, NULL);

        mvalue = mutex->value;
    }
    /* NOTREACHED */
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    int err = pthread_mutex_lock_impl(mutex);
#ifdef PTHREAD_DEBUG
    if (PTHREAD_DEBUG_ENABLED) {
        if (!err) {
            pthread_debug_mutex_lock_check(mutex);
        }
    }
#endif
    return err;
}

__LIBC_HIDDEN__
int pthread_mutex_unlock_impl(pthread_mutex_t *mutex)
{
    int mvalue, mtype, tid, shared;

    if (__predict_false(mutex == NULL))
        return EINVAL;

    mvalue = mutex->value;
    mtype  = (mvalue & MUTEX_TYPE_MASK);
    shared = (mvalue & MUTEX_SHARED_MASK);

    /* Handle common case first */
    if (__predict_true(mtype == MUTEX_TYPE_BITS_NORMAL)) {
        _normal_unlock(mutex, shared);
        return 0;
    }

    /* Do we already own this recursive or error-check mutex ? */
    tid = __get_thread()->tid;
    if ( tid != MUTEX_OWNER_FROM_BITS(mvalue) )
        return EPERM;

    /* If the counter is > 0, we can simply decrement it atomically.
     * Since other threads can mutate the lower state bits (and only the
     * lower state bits), use a cmpxchg to do it.
     */
    if (!MUTEX_COUNTER_BITS_IS_ZERO(mvalue)) {
        for (;;) {
            int newval = mvalue - MUTEX_COUNTER_BITS_ONE;
            if (__predict_true(__bionic_cmpxchg(mvalue, newval, &mutex->value) == 0)) {
                /* success: we still own the mutex, so no memory barrier */
                return 0;
            }
            /* the value changed, so reload and loop */
            mvalue = mutex->value;
        }
    }

    /* the counter is 0, so we're going to unlock the mutex by resetting
     * its value to 'unlocked'. We need to perform a swap in order
     * to read the current state, which will be 2 if there are waiters
     * to awake.
     *
     * TODO: Change this to __bionic_swap_release when we implement it
     *        to get rid of the explicit memory barrier below.
     */
    ANDROID_MEMBAR_FULL();  /* RELEASE BARRIER */
    mvalue = __bionic_swap(mtype | shared | MUTEX_STATE_BITS_UNLOCKED, &mutex->value);

    /* Wake one waiting thread, if any */
    if (MUTEX_STATE_BITS_IS_LOCKED_CONTENDED(mvalue)) {
        __futex_wake_ex(&mutex->value, shared, 1);
    }
    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
#ifdef PTHREAD_DEBUG
    if (PTHREAD_DEBUG_ENABLED) {
        pthread_debug_mutex_unlock_check(mutex);
    }
#endif
    return pthread_mutex_unlock_impl(mutex);
}

__LIBC_HIDDEN__
int pthread_mutex_trylock_impl(pthread_mutex_t *mutex)
{
    int mvalue, mtype, tid, shared;

    if (__predict_false(mutex == NULL))
        return EINVAL;

    mvalue = mutex->value;
    mtype  = (mvalue & MUTEX_TYPE_MASK);
    shared = (mvalue & MUTEX_SHARED_MASK);

    /* Handle common case first */
    if ( __predict_true(mtype == MUTEX_TYPE_BITS_NORMAL) )
    {
        if (__bionic_cmpxchg(shared|MUTEX_STATE_BITS_UNLOCKED,
                             shared|MUTEX_STATE_BITS_LOCKED_UNCONTENDED,
                             &mutex->value) == 0) {
            ANDROID_MEMBAR_FULL();
            return 0;
        }

        return EBUSY;
    }

    /* Do we already own this recursive or error-check mutex ? */
    tid = __get_thread()->tid;
    if ( tid == MUTEX_OWNER_FROM_BITS(mvalue) )
        return _recursive_increment(mutex, mvalue, mtype);

    /* Same as pthread_mutex_lock, except that we don't want to wait, and
     * the only operation that can succeed is a single cmpxchg to acquire the
     * lock if it is released / not owned by anyone. No need for a complex loop.
     */
    mtype |= shared | MUTEX_STATE_BITS_UNLOCKED;
    mvalue = MUTEX_OWNER_TO_BITS(tid) | mtype | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;

    if (__predict_true(__bionic_cmpxchg(mtype, mvalue, &mutex->value) == 0)) {
        ANDROID_MEMBAR_FULL();
        return 0;
    }

    return EBUSY;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    int err = pthread_mutex_trylock_impl(mutex);
#ifdef PTHREAD_DEBUG
    if (PTHREAD_DEBUG_ENABLED) {
        if (!err) {
            pthread_debug_mutex_lock_check(mutex);
        }
    }
#endif
    return err;
}

/* initialize 'ts' with the difference between 'abstime' and the current time
 * according to 'clock'. Returns -1 if abstime already expired, or 0 otherwise.
 */
static int
__timespec_to_absolute(struct timespec*  ts, const struct timespec*  abstime, clockid_t  clock)
{
    clock_gettime(clock, ts);
    ts->tv_sec  = abstime->tv_sec - ts->tv_sec;
    ts->tv_nsec = abstime->tv_nsec - ts->tv_nsec;
    if (ts->tv_nsec < 0) {
        ts->tv_sec--;
        ts->tv_nsec += 1000000000;
    }
    if ((ts->tv_nsec < 0) || (ts->tv_sec < 0))
        return -1;

    return 0;
}

/* initialize 'abstime' to the current time according to 'clock' plus 'msecs'
 * milliseconds.
 */
static void
__timespec_to_relative_msec(struct timespec*  abstime, unsigned  msecs, clockid_t  clock)
{
    clock_gettime(clock, abstime);
    abstime->tv_sec  += msecs/1000;
    abstime->tv_nsec += (msecs%1000)*1000000;
    if (abstime->tv_nsec >= 1000000000) {
        abstime->tv_sec++;
        abstime->tv_nsec -= 1000000000;
    }
}

__LIBC_HIDDEN__
int pthread_mutex_lock_timeout_np_impl(pthread_mutex_t *mutex, unsigned msecs)
{
    clockid_t        clock = CLOCK_MONOTONIC;
    struct timespec  abstime;
    struct timespec  ts;
    int               mvalue, mtype, tid, shared;

    /* compute absolute expiration time */
    __timespec_to_relative_msec(&abstime, msecs, clock);

    if (__predict_false(mutex == NULL))
        return EINVAL;

    mvalue = mutex->value;
    mtype  = (mvalue & MUTEX_TYPE_MASK);
    shared = (mvalue & MUTEX_SHARED_MASK);

    /* Handle common case first */
    if ( __predict_true(mtype == MUTEX_TYPE_BITS_NORMAL) )
    {
        const int unlocked           = shared | MUTEX_STATE_BITS_UNLOCKED;
        const int locked_uncontended = shared | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;
        const int locked_contended   = shared | MUTEX_STATE_BITS_LOCKED_CONTENDED;

        /* fast path for uncontended lock. Note: MUTEX_TYPE_BITS_NORMAL is 0 */
        if (__bionic_cmpxchg(unlocked, locked_uncontended, &mutex->value) == 0) {
            ANDROID_MEMBAR_FULL();
            return 0;
        }

        /* loop while needed */
        while (__bionic_swap(locked_contended, &mutex->value) != unlocked) {
            if (__timespec_to_absolute(&ts, &abstime, clock) < 0)
                return EBUSY;

            __futex_wait_ex(&mutex->value, shared, locked_contended, &ts);
        }
        ANDROID_MEMBAR_FULL();
        return 0;
    }

    /* Do we already own this recursive or error-check mutex ? */
    tid = __get_thread()->tid;
    if ( tid == MUTEX_OWNER_FROM_BITS(mvalue) )
        return _recursive_increment(mutex, mvalue, mtype);

    /* the following implements the same loop than pthread_mutex_lock_impl
     * but adds checks to ensure that the operation never exceeds the
     * absolute expiration time.
     */
    mtype |= shared;

    /* first try a quick lock */
    if (mvalue == mtype) {
        mvalue = MUTEX_OWNER_TO_BITS(tid) | mtype | MUTEX_STATE_BITS_LOCKED_UNCONTENDED;
        if (__predict_true(__bionic_cmpxchg(mtype, mvalue, &mutex->value) == 0)) {
            ANDROID_MEMBAR_FULL();
            return 0;
        }
        mvalue = mutex->value;
    }

    for (;;) {
        struct timespec ts;

        /* if the value is 'unlocked', try to acquire it directly */
        /* NOTE: put state to 2 since we know there is contention */
        if (mvalue == mtype) /* unlocked */ {
            mvalue = MUTEX_OWNER_TO_BITS(tid) | mtype | MUTEX_STATE_BITS_LOCKED_CONTENDED;
            if (__bionic_cmpxchg(mtype, mvalue, &mutex->value) == 0) {
                ANDROID_MEMBAR_FULL();
                return 0;
            }
            /* the value changed before we could lock it. We need to check
             * the time to avoid livelocks, reload the value, then loop again. */
            if (__timespec_to_absolute(&ts, &abstime, clock) < 0)
                return EBUSY;

            mvalue = mutex->value;
            continue;
        }

        /* The value is locked. If 'uncontended', try to switch its state
         * to 'contented' to ensure we get woken up later. */
        if (MUTEX_STATE_BITS_IS_LOCKED_UNCONTENDED(mvalue)) {
            int newval = MUTEX_STATE_BITS_FLIP_CONTENTION(mvalue);
            if (__bionic_cmpxchg(mvalue, newval, &mutex->value) != 0) {
                /* this failed because the value changed, reload it */
                mvalue = mutex->value;
            } else {
                /* this succeeded, update mvalue */
                mvalue = newval;
            }
        }

        /* check time and update 'ts' */
        if (__timespec_to_absolute(&ts, &abstime, clock) < 0)
            return EBUSY;

        /* Only wait to be woken up if the state is '2', otherwise we'll
         * simply loop right now. This can happen when the second cmpxchg
         * in our loop failed because the mutex was unlocked by another
         * thread.
         */
        if (MUTEX_STATE_BITS_IS_LOCKED_CONTENDED(mvalue)) {
            if (__futex_wait_ex(&mutex->value, shared, mvalue, &ts) == ETIMEDOUT) {
                return EBUSY;
            }
            mvalue = mutex->value;
        }
    }
    /* NOTREACHED */
}

int pthread_mutex_lock_timeout_np(pthread_mutex_t *mutex, unsigned msecs)
{
    int err = pthread_mutex_lock_timeout_np_impl(mutex, msecs);
#ifdef PTHREAD_DEBUG
    if (PTHREAD_DEBUG_ENABLED) {
        if (!err) {
            pthread_debug_mutex_lock_check(mutex);
        }
    }
#endif
    return err;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    int ret;

    /* use trylock to ensure that the mutex value is
     * valid and is not already locked. */
    ret = pthread_mutex_trylock_impl(mutex);
    if (ret != 0)
        return ret;

    mutex->value = 0xdead10cc;
    return 0;
}



int pthread_condattr_init(pthread_condattr_t *attr)
{
    if (attr == NULL)
        return EINVAL;

    *attr = PTHREAD_PROCESS_PRIVATE;
    return 0;
}

int pthread_condattr_getpshared(pthread_condattr_t *attr, int *pshared)
{
    if (attr == NULL || pshared == NULL)
        return EINVAL;

    *pshared = *attr;
    return 0;
}

int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared)
{
    if (attr == NULL)
        return EINVAL;

    if (pshared != PTHREAD_PROCESS_SHARED &&
        pshared != PTHREAD_PROCESS_PRIVATE)
        return EINVAL;

    *attr = pshared;
    return 0;
}

int pthread_condattr_destroy(pthread_condattr_t *attr)
{
    if (attr == NULL)
        return EINVAL;

    *attr = 0xdeada11d;
    return 0;
}

/* We use one bit in condition variable values as the 'shared' flag
 * The rest is a counter.
 */
#define COND_SHARED_MASK        0x0001
#define COND_COUNTER_INCREMENT  0x0002
#define COND_COUNTER_MASK       (~COND_SHARED_MASK)

#define COND_IS_SHARED(c)  (((c)->value & COND_SHARED_MASK) != 0)

/* XXX *technically* there is a race condition that could allow
 * XXX a signal to be missed.  If thread A is preempted in _wait()
 * XXX after unlocking the mutex and before waiting, and if other
 * XXX threads call signal or broadcast UINT_MAX/2 times (exactly),
 * XXX before thread A is scheduled again and calls futex_wait(),
 * XXX then the signal will be lost.
 */

int pthread_cond_init(pthread_cond_t *cond,
                      const pthread_condattr_t *attr)
{
    if (cond == NULL)
        return EINVAL;

    cond->value = 0;

    if (attr != NULL && *attr == PTHREAD_PROCESS_SHARED)
        cond->value |= COND_SHARED_MASK;

    return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
    if (cond == NULL)
        return EINVAL;

    cond->value = 0xdeadc04d;
    return 0;
}

/* This function is used by pthread_cond_broadcast and
 * pthread_cond_signal to atomically decrement the counter
 * then wake-up 'counter' threads.
 */
static int
__pthread_cond_pulse(pthread_cond_t *cond, int  counter)
{
    long flags;

    if (__predict_false(cond == NULL))
        return EINVAL;

    flags = (cond->value & ~COND_COUNTER_MASK);
    for (;;) {
        long oldval = cond->value;
        long newval = ((oldval - COND_COUNTER_INCREMENT) & COND_COUNTER_MASK)
                      | flags;
        if (__bionic_cmpxchg(oldval, newval, &cond->value) == 0)
            break;
    }

    /*
     * Ensure that all memory accesses previously made by this thread are
     * visible to the woken thread(s).  On the other side, the "wait"
     * code will issue any necessary barriers when locking the mutex.
     *
     * This may not strictly be necessary -- if the caller follows
     * recommended practice and holds the mutex before signaling the cond
     * var, the mutex ops will provide correct semantics.  If they don't
     * hold the mutex, they're subject to race conditions anyway.
     */
    ANDROID_MEMBAR_FULL();

    __futex_wake_ex(&cond->value, COND_IS_SHARED(cond), counter);
    return 0;
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
    return __pthread_cond_pulse(cond, INT_MAX);
}

int pthread_cond_signal(pthread_cond_t *cond)
{
    return __pthread_cond_pulse(cond, 1);
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    return pthread_cond_timedwait(cond, mutex, NULL);
}

int __pthread_cond_timedwait_relative(pthread_cond_t *cond,
                                      pthread_mutex_t * mutex,
                                      const struct timespec *reltime)
{
    int  status;
    int  oldvalue = cond->value;

    pthread_mutex_unlock(mutex);
    status = __futex_wait_ex(&cond->value, COND_IS_SHARED(cond), oldvalue, reltime);
    pthread_mutex_lock(mutex);

    if (status == (-ETIMEDOUT)) return ETIMEDOUT;
    return 0;
}

int __pthread_cond_timedwait(pthread_cond_t *cond,
                             pthread_mutex_t * mutex,
                             const struct timespec *abstime,
                             clockid_t clock)
{
    struct timespec ts;
    struct timespec * tsp;

    if (abstime != NULL) {
        if (__timespec_to_absolute(&ts, abstime, clock) < 0)
            return ETIMEDOUT;
        tsp = &ts;
    } else {
        tsp = NULL;
    }

    return __pthread_cond_timedwait_relative(cond, mutex, tsp);
}

int pthread_cond_timedwait(pthread_cond_t *cond,
                           pthread_mutex_t * mutex,
                           const struct timespec *abstime)
{
    return __pthread_cond_timedwait(cond, mutex, abstime, CLOCK_REALTIME);
}


/* this one exists only for backward binary compatibility */
int pthread_cond_timedwait_monotonic(pthread_cond_t *cond,
                                     pthread_mutex_t * mutex,
                                     const struct timespec *abstime)
{
    return __pthread_cond_timedwait(cond, mutex, abstime, CLOCK_MONOTONIC);
}

int pthread_cond_timedwait_monotonic_np(pthread_cond_t *cond,
                                     pthread_mutex_t * mutex,
                                     const struct timespec *abstime)
{
    return __pthread_cond_timedwait(cond, mutex, abstime, CLOCK_MONOTONIC);
}

int pthread_cond_timedwait_relative_np(pthread_cond_t *cond,
                                      pthread_mutex_t * mutex,
                                      const struct timespec *reltime)
{
    return __pthread_cond_timedwait_relative(cond, mutex, reltime);
}

int pthread_cond_timeout_np(pthread_cond_t *cond,
                            pthread_mutex_t * mutex,
                            unsigned msecs)
{
    struct timespec ts;

    ts.tv_sec = msecs / 1000;
    ts.tv_nsec = (msecs % 1000) * 1000000;

    return __pthread_cond_timedwait_relative(cond, mutex, &ts);
}


/* NOTE: this implementation doesn't support a init function that throws a C++ exception
 *       or calls fork()
 */
int pthread_once( pthread_once_t*  once_control,  void (*init_routine)(void) )
{
    volatile pthread_once_t* ocptr = once_control;

    /* PTHREAD_ONCE_INIT is 0, we use the following bit flags
     *
     *   bit 0 set  -> initialization is under way
     *   bit 1 set  -> initialization is complete
     */
#define ONCE_INITIALIZING           (1 << 0)
#define ONCE_COMPLETED              (1 << 1)

    /* First check if the once is already initialized. This will be the common
    * case and we want to make this as fast as possible. Note that this still
    * requires a load_acquire operation here to ensure that all the
    * stores performed by the initialization function are observable on
    * this CPU after we exit.
    */
    if (__predict_true((*ocptr & ONCE_COMPLETED) != 0)) {
        ANDROID_MEMBAR_FULL();
        return 0;
    }

    for (;;) {
        /* Try to atomically set the INITIALIZING flag.
         * This requires a cmpxchg loop, and we may need
         * to exit prematurely if we detect that
         * COMPLETED is now set.
         */
        int32_t  oldval, newval;

        do {
            oldval = *ocptr;
            if ((oldval & ONCE_COMPLETED) != 0)
                break;

            newval = oldval | ONCE_INITIALIZING;
        } while (__bionic_cmpxchg(oldval, newval, ocptr) != 0);

        if ((oldval & ONCE_COMPLETED) != 0) {
            /* We detected that COMPLETED was set while in our loop */
            ANDROID_MEMBAR_FULL();
            return 0;
        }

        if ((oldval & ONCE_INITIALIZING) == 0) {
            /* We got there first, we can jump out of the loop to
             * handle the initialization */
            break;
        }

        /* Another thread is running the initialization and hasn't completed
         * yet, so wait for it, then try again. */
        __futex_wait_ex(ocptr, 0, oldval, NULL);
    }

    /* call the initialization function. */
    (*init_routine)();

    /* Do a store_release indicating that initialization is complete */
    ANDROID_MEMBAR_FULL();
    *ocptr = ONCE_COMPLETED;

    /* Wake up any waiters, if any */
    __futex_wake_ex(ocptr, 0, INT_MAX);

    return 0;
}

pid_t __pthread_gettid(pthread_t thid) {
  pthread_internal_t* thread = (pthread_internal_t*) thid;
  return thread->tid;
}

int __pthread_settid(pthread_t thid, pid_t tid) {
  if (thid == 0) {
      return EINVAL;
  }

  pthread_internal_t* thread = (pthread_internal_t*) thid;
  thread->tid = tid;

  return 0;
}
