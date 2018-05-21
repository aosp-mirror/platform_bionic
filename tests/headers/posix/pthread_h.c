/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "header_checks.h"

static void pthread_h() {
  MACRO(PTHREAD_BARRIER_SERIAL_THREAD);

#if !defined(__BIONIC__) // No thread cancellation on Android.
  MACRO(PTHREAD_CANCEL_ASYNCHRONOUS);
  MACRO(PTHREAD_CANCEL_ENABLE);
  MACRO(PTHREAD_CANCEL_DEFERRED);
  MACRO(PTHREAD_CANCEL_DISABLE);
  MACRO(PTHREAD_CANCELED);
#endif

  MACRO(PTHREAD_CREATE_DETACHED);
  MACRO(PTHREAD_CREATE_JOINABLE);
  MACRO(PTHREAD_EXPLICIT_SCHED);
  MACRO(PTHREAD_INHERIT_SCHED);
  MACRO(PTHREAD_MUTEX_DEFAULT);
  MACRO(PTHREAD_MUTEX_ERRORCHECK);
  MACRO(PTHREAD_MUTEX_NORMAL);
  MACRO(PTHREAD_MUTEX_RECURSIVE);

#if !defined(__BIONIC__) // No robust mutexes on Android.
  MACRO(PTHREAD_MUTEX_ROBUST);
  MACRO(PTHREAD_MUTEX_STALLED);
#endif

  MACRO(PTHREAD_ONCE_INIT);

  MACRO(PTHREAD_PRIO_INHERIT);
  MACRO(PTHREAD_PRIO_NONE);
#if !defined(__BIONIC__)
  MACRO(PTHREAD_PRIO_PROTECT);
#endif

  MACRO(PTHREAD_PROCESS_SHARED);
  MACRO(PTHREAD_PROCESS_PRIVATE);
  MACRO(PTHREAD_SCOPE_PROCESS);
  MACRO(PTHREAD_SCOPE_SYSTEM);

  pthread_cond_t c0 = PTHREAD_COND_INITIALIZER;
  pthread_mutex_t m0 = PTHREAD_MUTEX_INITIALIZER;
  pthread_rwlock_t rw0 = PTHREAD_RWLOCK_INITIALIZER;

  TYPE(pthread_attr_t);
  TYPE(pthread_barrier_t);
  TYPE(pthread_barrierattr_t);
  TYPE(pthread_cond_t);
  TYPE(pthread_condattr_t);
  TYPE(pthread_key_t);
  TYPE(pthread_mutex_t);
  TYPE(pthread_mutexattr_t);
  TYPE(pthread_once_t);
  TYPE(pthread_rwlock_t);
  TYPE(pthread_rwlockattr_t);
  TYPE(pthread_spinlock_t);
  TYPE(pthread_t);

  FUNCTION(pthread_atfork, int (*f)(void (*)(void), void (*)(void), void (*)(void)));
  FUNCTION(pthread_attr_destroy, int (*f)(pthread_attr_t*));
  FUNCTION(pthread_attr_getdetachstate, int (*f)(const pthread_attr_t*, int*));
  FUNCTION(pthread_attr_getguardsize, int (*f)(const pthread_attr_t*, size_t*));
  FUNCTION(pthread_attr_getinheritsched, int (*f)(const pthread_attr_t*, int*));
  FUNCTION(pthread_attr_getschedparam, int (*f)(const pthread_attr_t*, struct sched_param*));
  FUNCTION(pthread_attr_getschedpolicy, int (*f)(const pthread_attr_t*, int*));
  FUNCTION(pthread_attr_getscope, int (*f)(const pthread_attr_t*, int*));
  FUNCTION(pthread_attr_getstack, int (*f)(const pthread_attr_t*, void**, size_t*));
  FUNCTION(pthread_attr_getstacksize, int (*f)(const pthread_attr_t*, size_t*));
  FUNCTION(pthread_attr_init, int (*f)(pthread_attr_t*));
  FUNCTION(pthread_attr_setdetachstate, int (*f)(pthread_attr_t*, int));
  FUNCTION(pthread_attr_setguardsize, int (*f)(pthread_attr_t*, size_t));
  FUNCTION(pthread_attr_setinheritsched, int (*f)(pthread_attr_t*, int));
  FUNCTION(pthread_attr_setschedparam, int (*f)(pthread_attr_t*, const struct sched_param*));
  FUNCTION(pthread_attr_setschedpolicy, int (*f)(pthread_attr_t*, int));
  FUNCTION(pthread_attr_setscope, int (*f)(pthread_attr_t*, int));
  FUNCTION(pthread_attr_setstack, int (*f)(pthread_attr_t*, void*, size_t));
  FUNCTION(pthread_attr_setstacksize, int (*f)(pthread_attr_t*, size_t));
  FUNCTION(pthread_barrier_destroy, int (*f)(pthread_barrier_t*));
  FUNCTION(pthread_barrier_init, int (*f)(pthread_barrier_t*, const pthread_barrierattr_t*, unsigned));
  FUNCTION(pthread_barrier_wait, int (*f)(pthread_barrier_t*));
  FUNCTION(pthread_barrierattr_destroy, int (*f)(pthread_barrierattr_t*));
  FUNCTION(pthread_barrierattr_getpshared, int (*f)(const pthread_barrierattr_t*, int*));
  FUNCTION(pthread_barrierattr_init, int (*f)(pthread_barrierattr_t*));
  FUNCTION(pthread_barrierattr_setpshared, int (*f)(pthread_barrierattr_t*, int));
#if !defined(__BIONIC__) // No thread cancellation on Android.
  FUNCTION(pthread_cancel, int (*f)(pthread_t));
#endif
  FUNCTION(pthread_cond_broadcast, int (*f)(pthread_cond_t*));
  FUNCTION(pthread_cond_destroy, int (*f)(pthread_cond_t*));
  FUNCTION(pthread_cond_init, int (*f)(pthread_cond_t*, const pthread_condattr_t*));
  FUNCTION(pthread_cond_signal, int (*f)(pthread_cond_t*));
  FUNCTION(pthread_cond_timedwait, int (*f)(pthread_cond_t*, pthread_mutex_t*, const struct timespec*));
  FUNCTION(pthread_cond_wait, int (*f)(pthread_cond_t*, pthread_mutex_t*));
  FUNCTION(pthread_condattr_destroy, int (*f)(pthread_condattr_t*));
  FUNCTION(pthread_condattr_getclock, int (*f)(const pthread_condattr_t*, clockid_t*));
  FUNCTION(pthread_condattr_getpshared, int (*f)(const pthread_condattr_t*, int*));
  FUNCTION(pthread_condattr_init, int (*f)(pthread_condattr_t*));
  FUNCTION(pthread_condattr_setclock, int (*f)(pthread_condattr_t*, clockid_t));
  FUNCTION(pthread_condattr_setpshared, int (*f)(pthread_condattr_t*, int));
  FUNCTION(pthread_create, int (*f)(pthread_t*, const pthread_attr_t*, void* (*)(void*), void*));
  FUNCTION(pthread_detach, int (*f)(pthread_t));
  FUNCTION(pthread_equal, int (*f)(pthread_t, pthread_t));
  FUNCTION(pthread_exit, void (*f)(void*));
#if !defined(__BIONIC__) // Marked obsolescent.
  FUNCTION(pthread_getconcurrency, int (*f)(void));
#endif
  FUNCTION(pthread_getcpuclockid, int (*f)(pthread_t, clockid_t*));
  FUNCTION(pthread_getschedparam, int (*f)(pthread_t, int*, struct sched_param*));
  FUNCTION(pthread_getspecific, void* (*f)(pthread_key_t));
  FUNCTION(pthread_join, int (*f)(pthread_t, void**));
  FUNCTION(pthread_key_create, int (*f)(pthread_key_t*, void (*)(void*)));
  FUNCTION(pthread_key_delete, int (*f)(pthread_key_t));
#if !defined(__BIONIC__) // No robust mutexes on Android.
  FUNCTION(pthread_mutex_consistent, int (*f)(pthread_mutex_t*));
#endif
  FUNCTION(pthread_mutex_destroy, int (*f)(pthread_mutex_t*));
#if !defined(__BIONIC__) // No robust mutexes on Android.
  FUNCTION(pthread_mutex_getprioceiling, int (*f)(const pthread_mutex_t*, int*));
#endif
  FUNCTION(pthread_mutex_init, int (*f)(pthread_mutex_t*, const pthread_mutexattr_t*));
  FUNCTION(pthread_mutex_lock, int (*f)(pthread_mutex_t*));
#if !defined(__BIONIC__) // No robust mutexes on Android.
  FUNCTION(pthread_mutex_setprioceiling, int (*f)(pthread_mutex_t*, int, int*));
#endif
  FUNCTION(pthread_mutex_timedlock, int (*f)(pthread_mutex_t*, const struct timespec*));
  FUNCTION(pthread_mutex_trylock, int (*f)(pthread_mutex_t*));
  FUNCTION(pthread_mutex_unlock, int (*f)(pthread_mutex_t*));
  FUNCTION(pthread_mutexattr_destroy, int (*f)(pthread_mutexattr_t*));
#if !defined(__BIONIC__) // No robust mutexes on Android.
  FUNCTION(pthread_mutexattr_getprioceiling, int (*f)(const pthread_mutexattr_t*, int*));
#endif
  FUNCTION(pthread_mutexattr_getprotocol, int (*f)(const pthread_mutexattr_t*, int*));
  FUNCTION(pthread_mutexattr_getpshared, int (*f)(const pthread_mutexattr_t*, int*));
#if !defined(__BIONIC__) // No robust mutexes on Android.
  FUNCTION(pthread_mutexattr_getrobust, int (*f)(const pthread_mutexattr_t*, int*));
#endif
  FUNCTION(pthread_mutexattr_gettype, int (*f)(const pthread_mutexattr_t*, int*));
  FUNCTION(pthread_mutexattr_init, int (*f)(pthread_mutexattr_t*));
#if !defined(__BIONIC__) // No robust mutexes on Android.
  FUNCTION(pthread_mutexattr_setprioceiling, int (*f)(pthread_mutexattr_t*, int));
#endif
  FUNCTION(pthread_mutexattr_setprotocol, int (*f)(pthread_mutexattr_t*, int));
  FUNCTION(pthread_mutexattr_setpshared, int (*f)(pthread_mutexattr_t*, int));
#if !defined(__BIONIC__) // No robust mutexes on Android.
  FUNCTION(pthread_mutexattr_setrobust, int (*f)(pthread_mutexattr_t*, int));
#endif
  FUNCTION(pthread_mutexattr_settype, int (*f)(pthread_mutexattr_t*, int));
  FUNCTION(pthread_once, int (*f)(pthread_once_t*, void (*)(void)));
  FUNCTION(pthread_rwlock_destroy, int (*f)(pthread_rwlock_t*));
  FUNCTION(pthread_rwlock_init, int (*f)(pthread_rwlock_t*, const pthread_rwlockattr_t*));
  FUNCTION(pthread_rwlock_rdlock, int (*f)(pthread_rwlock_t*));
  FUNCTION(pthread_rwlock_timedrdlock, int (*f)(pthread_rwlock_t*, const struct timespec*));
  FUNCTION(pthread_rwlock_timedwrlock, int (*f)(pthread_rwlock_t*, const struct timespec*));
  FUNCTION(pthread_rwlock_tryrdlock, int (*f)(pthread_rwlock_t*));
  FUNCTION(pthread_rwlock_trywrlock, int (*f)(pthread_rwlock_t*));
  FUNCTION(pthread_rwlock_unlock, int (*f)(pthread_rwlock_t*));
  FUNCTION(pthread_rwlock_wrlock, int (*f)(pthread_rwlock_t*));
  FUNCTION(pthread_rwlockattr_destroy, int (*f)(pthread_rwlockattr_t*));
  FUNCTION(pthread_rwlockattr_getpshared, int (*f)(const pthread_rwlockattr_t*, int*));
  FUNCTION(pthread_rwlockattr_init, int (*f)(pthread_rwlockattr_t*));
  FUNCTION(pthread_rwlockattr_setpshared, int (*f)(pthread_rwlockattr_t*, int));
  FUNCTION(pthread_self, pthread_t (*f)(void));
#if !defined(__BIONIC__) // No thread cancellation on Android.
  FUNCTION(pthread_setcancelstate, int (*f)(int, int*));
  FUNCTION(pthread_setcanceltype, int (*f)(int, int*));
#endif
#if !defined(__BIONIC__) // Marked obsolescent.
  FUNCTION(pthread_setconcurrency, int (*f)(int));
#endif
  FUNCTION(pthread_setschedparam, int (*f)(pthread_t, int, const struct sched_param*));
  FUNCTION(pthread_setschedprio, int (*f)(pthread_t, int));
  FUNCTION(pthread_setspecific, int (*f)(pthread_key_t, const void*));
  FUNCTION(pthread_spin_destroy, int (*f)(pthread_spinlock_t*));
  FUNCTION(pthread_spin_init, int (*f)(pthread_spinlock_t*, int));
  FUNCTION(pthread_spin_lock, int (*f)(pthread_spinlock_t*));
  FUNCTION(pthread_spin_trylock, int (*f)(pthread_spinlock_t*));
  FUNCTION(pthread_spin_unlock, int (*f)(pthread_spinlock_t*));
#if !defined(__BIONIC__) // No thread cancellation on Android.
  FUNCTION(pthread_testcancel, void (*f)(void));
#endif

#if !defined(pthread_cleanup_pop)
#error pthread_cleanup_pop
#endif
#if !defined(pthread_cleanup_push)
#error pthread_cleanup_push
#endif
}

// POSIX: "Inclusion of the <pthread.h> header shall make symbols defined in the
// headers <sched.h> and <time.h> visible."

#define DO_NOT_INCLUDE_SCHED_H
#include "sched_h.c"
#define DO_NOT_INCLUDE_TIME_H
#include "time_h.c"
