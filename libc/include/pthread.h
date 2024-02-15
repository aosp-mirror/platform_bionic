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

#pragma once

/**
 * @file pthread.h
 * @brief POSIX threads.
 */

#include <limits.h>
#include <bits/page_size.h>
#include <bits/pthread_types.h>
#include <sched.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <time.h>

__BEGIN_DECLS

enum {
  PTHREAD_MUTEX_NORMAL = 0,
  PTHREAD_MUTEX_RECURSIVE = 1,
  PTHREAD_MUTEX_ERRORCHECK = 2,

  PTHREAD_MUTEX_ERRORCHECK_NP = PTHREAD_MUTEX_ERRORCHECK,
  PTHREAD_MUTEX_RECURSIVE_NP  = PTHREAD_MUTEX_RECURSIVE,

  PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL
};

#define PTHREAD_MUTEX_INITIALIZER { { ((PTHREAD_MUTEX_NORMAL & 3) << 14) } }
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP { { ((PTHREAD_MUTEX_RECURSIVE & 3) << 14) } }
#define PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP { { ((PTHREAD_MUTEX_ERRORCHECK & 3) << 14) } }

#define PTHREAD_COND_INITIALIZER  { { 0 } }
#define PTHREAD_COND_INITIALIZER_MONOTONIC_NP  { { 1 << 1 } }

#define PTHREAD_RWLOCK_INITIALIZER  { { 0 } }

enum {
  PTHREAD_RWLOCK_PREFER_READER_NP = 0,
  PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP = 1,
};

#define PTHREAD_ONCE_INIT 0

#if __ANDROID_API__ >= 24
#define PTHREAD_BARRIER_SERIAL_THREAD (-1)
#endif

#if defined(__LP64__)
#define PTHREAD_STACK_MIN 16384
#else
#define PTHREAD_STACK_MIN 8192
#endif

#define PTHREAD_CREATE_DETACHED 1
#define PTHREAD_CREATE_JOINABLE 0

#define PTHREAD_EXPLICIT_SCHED 0
#define PTHREAD_INHERIT_SCHED 1

#define PTHREAD_PRIO_NONE 0
#define PTHREAD_PRIO_INHERIT 1

#define PTHREAD_PROCESS_PRIVATE 0
#define PTHREAD_PROCESS_SHARED 1

#define PTHREAD_SCOPE_SYSTEM 0
#define PTHREAD_SCOPE_PROCESS 1

int pthread_atfork(void (* _Nullable __prepare)(void), void (* _Nullable __parent)(void), void (* _Nullable __child)(void));

int pthread_attr_destroy(pthread_attr_t* _Nonnull __attr);
int pthread_attr_getdetachstate(const pthread_attr_t* _Nonnull __attr, int* _Nonnull __state);
int pthread_attr_getguardsize(const pthread_attr_t* _Nonnull __attr, size_t* _Nonnull __size);
int pthread_attr_getinheritsched(const pthread_attr_t* _Nonnull __attr, int* _Nonnull __flag) __INTRODUCED_IN(28);
int pthread_attr_getschedparam(const pthread_attr_t* _Nonnull __attr, struct sched_param* _Nonnull __param);
int pthread_attr_getschedpolicy(const pthread_attr_t* _Nonnull __attr, int* _Nonnull __policy);
int pthread_attr_getscope(const pthread_attr_t* _Nonnull __attr, int* _Nonnull __scope);
int pthread_attr_getstack(const pthread_attr_t* _Nonnull __attr, void* _Nullable * _Nonnull __addr, size_t* _Nonnull __size);
int pthread_attr_getstacksize(const pthread_attr_t* _Nonnull __attr, size_t* _Nonnull __size);
int pthread_attr_init(pthread_attr_t* _Nonnull __attr);
int pthread_attr_setdetachstate(pthread_attr_t* _Nonnull __attr, int __state);
int pthread_attr_setguardsize(pthread_attr_t* _Nonnull __attr, size_t __size);
int pthread_attr_setinheritsched(pthread_attr_t* _Nonnull __attr, int __flag) __INTRODUCED_IN(28);
int pthread_attr_setschedparam(pthread_attr_t* _Nonnull __attr, const struct sched_param* _Nonnull __param);
int pthread_attr_setschedpolicy(pthread_attr_t* _Nonnull __attr, int __policy);
int pthread_attr_setscope(pthread_attr_t* _Nonnull __attr, int __scope);
int pthread_attr_setstack(pthread_attr_t* _Nonnull __attr, void* _Nonnull __addr, size_t __size);
int pthread_attr_setstacksize(pthread_attr_t* _Nonnull __addr, size_t __size);

int pthread_condattr_destroy(pthread_condattr_t* _Nonnull __attr);
int pthread_condattr_getclock(const pthread_condattr_t* _Nonnull __attr, clockid_t* _Nonnull __clock);
int pthread_condattr_getpshared(const pthread_condattr_t* _Nonnull __attr, int* _Nonnull __shared);
int pthread_condattr_init(pthread_condattr_t* _Nonnull __attr);
int pthread_condattr_setclock(pthread_condattr_t* _Nonnull __attr, clockid_t __clock);
int pthread_condattr_setpshared(pthread_condattr_t* _Nonnull __attr, int __shared);

int pthread_cond_broadcast(pthread_cond_t* _Nonnull __cond);
int pthread_cond_clockwait(pthread_cond_t* _Nonnull __cond, pthread_mutex_t* _Nonnull __mutex, clockid_t __clock,
                           const struct timespec* _Nullable __timeout) __INTRODUCED_IN(30);
int pthread_cond_destroy(pthread_cond_t* _Nonnull __cond);
int pthread_cond_init(pthread_cond_t* _Nonnull __cond, const pthread_condattr_t* _Nullable __attr);
int pthread_cond_signal(pthread_cond_t* _Nonnull __cond);
int pthread_cond_timedwait(pthread_cond_t* _Nonnull __cond, pthread_mutex_t* _Nonnull __mutex, const struct timespec* _Nullable __timeout);
/*
 * Condition variables use CLOCK_REALTIME by default for their timeouts, however that is
 * typically inappropriate, since that clock can change dramatically, causing the timeout to
 * either expire earlier or much later than intended.
 * Condition variables have an initialization option to use CLOCK_MONOTONIC, and in addition,
 * Android provides pthread_cond_timedwait_monotonic_np to use CLOCK_MONOTONIC on a condition
 * variable for this single wait no matter how it was initialized.
 * Note that pthread_cond_clockwait() allows specifying an arbitrary clock and has superseded this
 * function.
 */
int pthread_cond_timedwait_monotonic_np(pthread_cond_t* _Nonnull __cond, pthread_mutex_t* _Nonnull __mutex,
                                        const struct timespec* _Nullable __timeout) __INTRODUCED_IN_64(28);
int pthread_cond_wait(pthread_cond_t* _Nonnull __cond, pthread_mutex_t* _Nonnull __mutex);

#if defined(__clang__)
/*
 * Disable -Wbuiltin-requires-header because clang confuses this declaration with the one defined in
 * "llvm/tools/clang/include/clang/Basic/Builtins.def", which did not define any formal arguments.
 * It seems to be an upstream bug and the fix (https://reviews.llvm.org/D58531) is still under
 * review. Thus, let's disable the warning for this function declaration.
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbuiltin-requires-header"
#endif
int pthread_create(pthread_t* _Nonnull __pthread_ptr, pthread_attr_t const* _Nullable __attr, void* _Nonnull (* _Nonnull __start_routine)(void* _Nonnull), void* _Nullable);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

int pthread_detach(pthread_t __pthread);
void pthread_exit(void* _Nullable __return_value) __noreturn;

int pthread_equal(pthread_t __lhs, pthread_t __rhs);

int pthread_getattr_np(pthread_t __pthread, pthread_attr_t* _Nonnull __attr);

int pthread_getcpuclockid(pthread_t __pthread, clockid_t* _Nonnull __clock);

void* _Nullable pthread_getspecific(pthread_key_t __key);

pid_t pthread_gettid_np(pthread_t __pthread);

int pthread_join(pthread_t __pthread, void* _Nullable * _Nullable __return_value_ptr);

int pthread_key_create(pthread_key_t* _Nonnull __key_ptr, void (* _Nullable __key_destructor)(void* _Nullable));
int pthread_key_delete(pthread_key_t __key);

int pthread_mutexattr_destroy(pthread_mutexattr_t* _Nonnull __attr);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t* _Nonnull __attr, int* _Nonnull __shared);
int pthread_mutexattr_gettype(const pthread_mutexattr_t* _Nonnull __attr, int* _Nonnull __type);
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t* _Nonnull __attr, int* _Nonnull __protocol) __INTRODUCED_IN(28);
int pthread_mutexattr_init(pthread_mutexattr_t* _Nonnull __attr);
int pthread_mutexattr_setpshared(pthread_mutexattr_t* _Nonnull __attr, int __shared);
int pthread_mutexattr_settype(pthread_mutexattr_t* _Nonnull __attr, int __type);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t* _Nonnull __attr, int __protocol) __INTRODUCED_IN(28);

int pthread_mutex_clocklock(pthread_mutex_t* _Nonnull __mutex, clockid_t __clock,
                            const struct timespec* _Nullable __abstime) __INTRODUCED_IN(30);
int pthread_mutex_destroy(pthread_mutex_t* _Nonnull __mutex);
int pthread_mutex_init(pthread_mutex_t* _Nonnull __mutex, const pthread_mutexattr_t* _Nullable __attr);
int pthread_mutex_lock(pthread_mutex_t* _Nonnull __mutex);
int pthread_mutex_timedlock(pthread_mutex_t* _Nonnull __mutex, const struct timespec* _Nullable __timeout);

/*
 * POSIX historically only supported using pthread_mutex_timedlock() with CLOCK_REALTIME, however
 * that is typically inappropriate, since that clock can change dramatically, causing the timeout to
 * either expire earlier or much later than intended.
 * This function is added to use a timespec based on CLOCK_MONOTONIC that does not suffer
 * from this issue.
 * Note that pthread_mutex_clocklock() allows specifying an arbitrary clock and has superseded this
 * function.
 */
int pthread_mutex_timedlock_monotonic_np(pthread_mutex_t* _Nonnull __mutex, const struct timespec* _Nullable __timeout)
    __INTRODUCED_IN(28);
int pthread_mutex_trylock(pthread_mutex_t* _Nonnull __mutex);
int pthread_mutex_unlock(pthread_mutex_t* _Nonnull __mutex);

int pthread_once(pthread_once_t* _Nonnull __once, void (* _Nonnull __init_routine)(void));

int pthread_rwlockattr_init(pthread_rwlockattr_t* _Nonnull __attr);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t* _Nonnull __attr);
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t* _Nonnull __attr, int* _Nonnull __shared);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t* _Nonnull __attr, int __shared);
int pthread_rwlockattr_getkind_np(const pthread_rwlockattr_t* _Nonnull __attr, int* _Nonnull __kind)
  __INTRODUCED_IN(23);
int pthread_rwlockattr_setkind_np(pthread_rwlockattr_t* _Nonnull __attr, int __kind) __INTRODUCED_IN(23);

int pthread_rwlock_clockrdlock(pthread_rwlock_t* _Nonnull __rwlock, clockid_t __clock,
                               const struct timespec* _Nullable __timeout) __INTRODUCED_IN(30);
int pthread_rwlock_clockwrlock(pthread_rwlock_t* _Nonnull __rwlock, clockid_t __clock,
                               const struct timespec* _Nullable __timeout) __INTRODUCED_IN(30);
int pthread_rwlock_destroy(pthread_rwlock_t* _Nonnull __rwlock);
int pthread_rwlock_init(pthread_rwlock_t* _Nonnull __rwlock, const pthread_rwlockattr_t* _Nullable __attr);
int pthread_rwlock_rdlock(pthread_rwlock_t* _Nonnull __rwlock);
int pthread_rwlock_timedrdlock(pthread_rwlock_t* _Nonnull __rwlock, const struct timespec* _Nullable __timeout);
/* See the comment on pthread_mutex_timedlock_monotonic_np for usage of this function. */
int pthread_rwlock_timedrdlock_monotonic_np(pthread_rwlock_t* _Nonnull __rwlock,
                                            const struct timespec* _Nullable __timeout) __INTRODUCED_IN(28);
int pthread_rwlock_timedwrlock(pthread_rwlock_t* _Nonnull __rwlock, const struct timespec* _Nullable __timeout);
/* See the comment on pthread_mutex_timedlock_monotonic_np for usage of this function. */
int pthread_rwlock_timedwrlock_monotonic_np(pthread_rwlock_t* _Nonnull __rwlock,
                                            const struct timespec* _Nullable __timeout) __INTRODUCED_IN(28);
int pthread_rwlock_tryrdlock(pthread_rwlock_t* _Nonnull __rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t* _Nonnull __rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t* _Nonnull __rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t* _Nonnull __rwlock);

#if __ANDROID_API__ >= 24
int pthread_barrierattr_init(pthread_barrierattr_t* _Nonnull __attr) __INTRODUCED_IN(24);
int pthread_barrierattr_destroy(pthread_barrierattr_t* _Nonnull __attr) __INTRODUCED_IN(24);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t* _Nonnull __attr, int* _Nonnull __shared) __INTRODUCED_IN(24);
int pthread_barrierattr_setpshared(pthread_barrierattr_t* _Nonnull __attr, int __shared) __INTRODUCED_IN(24);
#endif

#if __ANDROID_API__ >= 24
int pthread_barrier_init(pthread_barrier_t* _Nonnull __barrier, const pthread_barrierattr_t* _Nullable __attr, unsigned __count) __INTRODUCED_IN(24);
int pthread_barrier_destroy(pthread_barrier_t* _Nonnull __barrier) __INTRODUCED_IN(24);
int pthread_barrier_wait(pthread_barrier_t* _Nonnull __barrier) __INTRODUCED_IN(24);
#endif

#if __ANDROID_API__ >= 24
int pthread_spin_destroy(pthread_spinlock_t* _Nonnull __spinlock) __INTRODUCED_IN(24);
int pthread_spin_init(pthread_spinlock_t* _Nonnull __spinlock, int __shared) __INTRODUCED_IN(24);
int pthread_spin_lock(pthread_spinlock_t* _Nonnull __spinlock) __INTRODUCED_IN(24);
int pthread_spin_trylock(pthread_spinlock_t* _Nonnull __spinlock) __INTRODUCED_IN(24);
int pthread_spin_unlock(pthread_spinlock_t* _Nonnull __spinlock) __INTRODUCED_IN(24);
#endif

pthread_t pthread_self(void) __attribute_const__;

#if defined(__USE_GNU)
int pthread_getname_np(pthread_t __pthread, char* _Nonnull __buf, size_t __n) __INTRODUCED_IN(26);
#endif
/* TODO: this should be __USE_GNU too. */
int pthread_setname_np(pthread_t __pthread, const char* _Nonnull __name);

/**
 * [pthread_setschedparam(3)](https://man7.org/linux/man-pages/man3/pthread_setschedparam.3.html)
 * sets the scheduler policy and parameters of the given thread.
 *
 * This call is not useful to applications on Android, because they don't
 * have permission to set their scheduling policy, and the only priority
 * for their policy is 0 anyway. If you only need to set your scheduling
 * priority, see setpriority() instead.
 *
 * Returns 0 on success and returns an error number on failure.
 */
int pthread_setschedparam(pthread_t __pthread, int __policy, const struct sched_param* _Nonnull __param);

/**
 * [pthread_getschedparam(3)](https://man7.org/linux/man-pages/man3/pthread_getschedparam.3.html)
 * gets the scheduler policy and parameters of the given thread.
 *
 * Returns 0 on success and returns an error number on failure.
 */
int pthread_getschedparam(pthread_t __pthread, int* _Nonnull __policy, struct sched_param* _Nonnull __param);

/**
 * [pthread_setschedprio(3)](https://man7.org/linux/man-pages/man3/pthread_setschedprio.3.html)
 * sets the scheduler priority of the given thread.
 *
 * This call is not useful to applications on Android, because they don't
 * have permission to set their scheduling policy, and the only priority
 * for their policy is 0 anyway. If you only need to set your scheduling
 * priority, see setpriority() instead.
 *
 * Returns 0 on success and returns an error number on failure.
 *
 * Available since API level 28.
 */
int pthread_setschedprio(pthread_t __pthread, int __priority) __INTRODUCED_IN(28);

int pthread_setspecific(pthread_key_t __key, const void* _Nullable __value);

typedef void (* _Nullable __pthread_cleanup_func_t)(void* _Nullable);

typedef struct __pthread_cleanup_t {
  struct __pthread_cleanup_t*   _Nullable __cleanup_prev;
  __pthread_cleanup_func_t      _Nullable __cleanup_routine;
  void*                         _Nullable __cleanup_arg;
} __pthread_cleanup_t;

void __pthread_cleanup_push(__pthread_cleanup_t* _Nonnull c, __pthread_cleanup_func_t _Nullable, void* _Nullable);
void __pthread_cleanup_pop(__pthread_cleanup_t* _Nonnull, int);

/* Believe or not, the definitions of pthread_cleanup_push and
 * pthread_cleanup_pop below are correct. Posix states that these
 * can be implemented as macros that might introduce opening and
 * closing braces, and that using setjmp/longjmp/return/break/continue
 * between them results in undefined behavior.
 */
#define  pthread_cleanup_push(routine, arg)                      \
    do {                                                         \
        __pthread_cleanup_t  __cleanup;                          \
        __pthread_cleanup_push( &__cleanup, (routine), (arg) );  \

#define  pthread_cleanup_pop(execute)                  \
        __pthread_cleanup_pop( &__cleanup, (execute)); \
    } while (0);                                       \

__END_DECLS
