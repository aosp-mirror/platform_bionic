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

#include <errno.h>
#include <malloc.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "private/bionic_lock.h"

// System calls.
extern "C" int __rt_sigprocmask(int, const sigset64_t*, sigset64_t*, size_t);
extern "C" int __rt_sigtimedwait(const sigset64_t*, siginfo_t*, const timespec*, size_t);
extern "C" int __timer_create(clockid_t, sigevent*, __kernel_timer_t*);
extern "C" int __timer_delete(__kernel_timer_t);
extern "C" int __timer_getoverrun(__kernel_timer_t);
extern "C" int __timer_gettime(__kernel_timer_t, itimerspec*);
extern "C" int __timer_settime(__kernel_timer_t, int, const itimerspec*, itimerspec*);

// Most POSIX timers are handled directly by the kernel. We translate SIGEV_THREAD timers
// into SIGEV_THREAD_ID timers so the kernel handles all the time-related stuff and we just
// need to worry about running user code on a thread.

// We can't use SIGALRM because too many other C library functions throw that around, and since
// they don't send to a specific thread, all threads are eligible to handle the signal and we can
// end up with one of our POSIX timer threads handling it (meaning that the intended recipient
// doesn't). glibc uses SIGRTMIN for its POSIX timer implementation, so in the absence of any
// reason to use anything else, we use that too.
static const int TIMER_SIGNAL = (__SIGRTMIN + 0);

struct PosixTimer {
  __kernel_timer_t kernel_timer_id;

  int sigev_notify;

  // The fields below are only needed for a SIGEV_THREAD timer.
  Lock startup_handshake_lock;
  pthread_t callback_thread;
  void (*callback)(sigval_t);
  sigval_t callback_argument;
  atomic_bool deleted;  // Set when the timer is deleted, to prevent further calling of callback.
};

static __kernel_timer_t to_kernel_timer_id(timer_t timer) {
  return reinterpret_cast<PosixTimer*>(timer)->kernel_timer_id;
}

static void* __timer_thread_start(void* arg) {
  PosixTimer* timer = reinterpret_cast<PosixTimer*>(arg);

  // Check that our parent managed to create the kernel timer and bail if not...
  timer->startup_handshake_lock.lock();
  if (timer->kernel_timer_id == -1) {
    free(timer);
    return nullptr;
  }

  // Give ourselves a specific meaningful name now we have a kernel timer.
  char name[16]; // 16 is the kernel-imposed limit.
  snprintf(name, sizeof(name), "POSIX timer %d", to_kernel_timer_id(timer));
  pthread_setname_np(timer->callback_thread, name);

  sigset64_t sigset = {};
  sigaddset64(&sigset, TIMER_SIGNAL);

  while (true) {
    // Wait for a signal...
    siginfo_t si = {};
    if (__rt_sigtimedwait(&sigset, &si, nullptr, sizeof(sigset)) == -1) continue;

    if (si.si_code == SI_TIMER) {
      // This signal was sent because a timer fired, so call the callback.

      // All events to the callback will be ignored when the timer is deleted.
      if (atomic_load(&timer->deleted) == true) {
        continue;
      }
      timer->callback(timer->callback_argument);
    } else if (si.si_code == SI_TKILL) {
      // This signal was sent because someone wants us to exit.
      free(timer);
      return nullptr;
    }
  }
}

static void __timer_thread_stop(PosixTimer* timer) {
  atomic_store(&timer->deleted, true);
  pthread_kill(timer->callback_thread, TIMER_SIGNAL);
}

// https://pubs.opengroup.org/onlinepubs/9799919799.2024edition/functions/timer_create.html
int timer_create(clockid_t clock_id, sigevent* evp, timer_t* timer_id) {
  PosixTimer* timer = reinterpret_cast<PosixTimer*>(malloc(sizeof(PosixTimer)));
  if (timer == nullptr) {
    return -1;
  }

  timer->kernel_timer_id = -1;
  timer->sigev_notify = (evp == nullptr) ? SIGEV_SIGNAL : evp->sigev_notify;

  // If not a SIGEV_THREAD timer, the kernel can handle it without our help.
  if (timer->sigev_notify != SIGEV_THREAD) {
    if (__timer_create(clock_id, evp, &timer->kernel_timer_id) == -1) {
      free(timer);
      return -1;
    }

    *timer_id = timer;
    return 0;
  }

  // Otherwise, this must be SIGEV_THREAD timer...
  timer->callback = evp->sigev_notify_function;
  timer->callback_argument = evp->sigev_value;
  atomic_store_explicit(&timer->deleted, false, memory_order_relaxed);

  // Check arguments that the kernel doesn't care about but we do.
  if (timer->callback == nullptr) {
    free(timer);
    errno = EINVAL;
    return -1;
  }

  // Create this timer's thread.
  pthread_attr_t thread_attributes;
  if (evp->sigev_notify_attributes == nullptr) {
    pthread_attr_init(&thread_attributes);
  } else {
    thread_attributes = *reinterpret_cast<pthread_attr_t*>(evp->sigev_notify_attributes);
  }
  pthread_attr_setdetachstate(&thread_attributes, PTHREAD_CREATE_DETACHED);

  // We start the thread with TIMER_SIGNAL blocked by blocking the signal here and letting it
  // inherit. If it tried to block the signal itself, there would be a race.
  sigset64_t sigset = {};
  sigaddset64(&sigset, TIMER_SIGNAL);
  sigset64_t old_sigset;

  // Prevent the child thread from running until the timer has been created.
  timer->startup_handshake_lock.init(false);
  timer->startup_handshake_lock.lock();

  // Use __rt_sigprocmask instead of sigprocmask64 to avoid filtering out TIMER_SIGNAL.
  __rt_sigprocmask(SIG_BLOCK, &sigset, &old_sigset, sizeof(sigset));

  int rc = pthread_create(&timer->callback_thread, &thread_attributes, __timer_thread_start, timer);

  __rt_sigprocmask(SIG_SETMASK, &old_sigset, nullptr, sizeof(old_sigset));

  if (rc != 0) {
    free(timer);
    errno = rc;
    return -1;
  }

  // Try to create the kernel timer.
  sigevent se = *evp;
  se.sigev_signo = TIMER_SIGNAL;
  se.sigev_notify = SIGEV_THREAD_ID;
  se.sigev_notify_thread_id = pthread_gettid_np(timer->callback_thread);
  rc = __timer_create(clock_id, &se, &timer->kernel_timer_id);

  // Let the child run (whether we created the kernel timer or not).
  timer->startup_handshake_lock.unlock();
  // If __timer_create(2) failed, the child will kill itself and free the
  // timer struct, so we just need to exit.
  if (rc == -1) {
    return -1;
  }

  *timer_id = timer;
  return 0;
}

// https://pubs.opengroup.org/onlinepubs/9799919799.2024edition/functions/timer_delete.html
int timer_delete(timer_t id) {
  int rc = __timer_delete(to_kernel_timer_id(id));
  if (rc == -1) {
    return -1;
  }

  PosixTimer* timer = reinterpret_cast<PosixTimer*>(id);
  if (timer->sigev_notify == SIGEV_THREAD) {
    // Stopping the timer's thread frees the timer data when it's safe.
    __timer_thread_stop(timer);
  } else {
    // For timers without threads, we can just free right away.
    free(timer);
  }

  return 0;
}

// https://pubs.opengroup.org/onlinepubs/9799919799.2024edition/functions/timer_gettime.html
int timer_gettime(timer_t id, itimerspec* ts) {
  return __timer_gettime(to_kernel_timer_id(id), ts);
}

// https://pubs.opengroup.org/onlinepubs/9799919799.2024edition/functions/timer_settime.html
// When using timer_settime to disarm a repeatable SIGEV_THREAD timer with a very small
// period (like below 1ms), the kernel may continue to send events to the callback thread
// for a few extra times. This behavior is fine because in POSIX standard: The effect of
// disarming or resetting a timer with pending expiration notifications is unspecified.
int timer_settime(timer_t id, int flags, const itimerspec* ts, itimerspec* ots) {
  PosixTimer* timer= reinterpret_cast<PosixTimer*>(id);
  return __timer_settime(timer->kernel_timer_id, flags, ts, ots);
}

// https://pubs.opengroup.org/onlinepubs/9799919799.2024edition/functions/timer_getoverrun.html
int timer_getoverrun(timer_t id) {
  return __timer_getoverrun(to_kernel_timer_id(id));
}
