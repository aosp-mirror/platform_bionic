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
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "private/kernel_sigset_t.h"

extern "C" int ___rt_sigqueueinfo(pid_t, int, siginfo_t*);
extern "C" int __rt_sigtimedwait(const sigset_t*, siginfo_t*, const timespec*, size_t);

int sigaddset(sigset_t* set, int signum) {
  int bit = signum - 1; // Signal numbers start at 1, but bit positions start at 0.
  unsigned long* local_set = reinterpret_cast<unsigned long*>(set);
  if (set == NULL || bit < 0 || bit >= static_cast<int>(8*sizeof(sigset_t))) {
    errno = EINVAL;
    return -1;
  }
  local_set[bit / LONG_BIT] |= 1UL << (bit % LONG_BIT);
  return 0;
}

// This isn't in our header files, but is exposed on all architectures.
extern "C" int sigblock(int mask) {
  union {
    int mask;
    sigset_t set;
  } in, out;

  sigemptyset(&in.set);
  in.mask = mask;

  if (sigprocmask(SIG_BLOCK, &in.set, &out.set) == -1) return -1;
  return out.mask;
}

int sigdelset(sigset_t* set, int signum) {
  int bit = signum - 1; // Signal numbers start at 1, but bit positions start at 0.
  unsigned long* local_set = reinterpret_cast<unsigned long*>(set);
  if (set == NULL || bit < 0 || bit >= static_cast<int>(8*sizeof(sigset_t))) {
    errno = EINVAL;
    return -1;
  }
  local_set[bit / LONG_BIT] &= ~(1UL << (bit % LONG_BIT));
  return 0;
}

int sigemptyset(sigset_t* set) {
  if (set == NULL) {
    errno = EINVAL;
    return -1;
  }
  memset(set, 0, sizeof(sigset_t));
  return 0;
}

int sigfillset(sigset_t* set) {
  if (set == NULL) {
    errno = EINVAL;
    return -1;
  }
  memset(set, ~0, sizeof(sigset_t));
  return 0;
}

int sighold(int sig) {
  kernel_sigset_t set;
  set.clear();
  if (!set.set(sig)) return -1;
  return __rt_sigprocmask(SIG_BLOCK, &set, nullptr, sizeof(set));
}

int sigignore(int sig) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  if (sigemptyset(&sa.sa_mask) == -1) return -1;
  sa.sa_handler = SIG_IGN;
  return sigaction(sig, &sa, nullptr);
}

int siginterrupt(int sig, int flag) {
  struct sigaction act;
  sigaction(sig, nullptr, &act);
  if (flag) {
    act.sa_flags &= ~SA_RESTART;
  } else {
    act.sa_flags |= SA_RESTART;
  }
  return sigaction(sig, &act, nullptr);
}

int sigismember(const sigset_t* set, int signum) {
  int bit = signum - 1; // Signal numbers start at 1, but bit positions start at 0.
  const unsigned long* local_set = reinterpret_cast<const unsigned long*>(set);
  if (set == NULL || bit < 0 || bit >= static_cast<int>(8*sizeof(sigset_t))) {
    errno = EINVAL;
    return -1;
  }
  return static_cast<int>((local_set[bit / LONG_BIT] >> (bit % LONG_BIT)) & 1);
}

__LIBC_HIDDEN__ sighandler_t _signal(int signum, sighandler_t handler, int flags) {
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = handler;
  sa.sa_flags = flags;

  if (sigaction(signum, &sa, &sa) == -1) {
    return SIG_ERR;
  }

  return sa.sa_handler;
}

sighandler_t signal(int signum, sighandler_t handler) {
  return _signal(signum, handler, SA_RESTART);
}

int sigpause(int sig) {
  kernel_sigset_t set;
  set.clear();
  if (__rt_sigprocmask(SIG_SETMASK, nullptr, &set, sizeof(set)) == -1) return -1;
  if (!set.clear(sig)) return -1;
  return __rt_sigsuspend(&set, sizeof(set));
}

int sigpending(sigset_t* bionic_set) {
  kernel_sigset_t set;
  int result = __rt_sigpending(&set, sizeof(set));
  if (result != -1) {
    *bionic_set = set.bionic;
  }
  return result;
}

int sigprocmask(int how, const sigset_t* bionic_new_set, sigset_t* bionic_old_set) {
  kernel_sigset_t new_set;
  kernel_sigset_t* new_set_ptr = NULL;
  if (bionic_new_set != NULL) {
    new_set.set(bionic_new_set);
    new_set_ptr = &new_set;
  }

  kernel_sigset_t old_set;
  if (__rt_sigprocmask(how, new_set_ptr, &old_set, sizeof(old_set)) == -1) {
    return -1;
  }

  if (bionic_old_set != NULL) {
    *bionic_old_set = old_set.bionic;
  }

  return 0;
}

int sigqueue(pid_t pid, int signo, const sigval value) {
  siginfo_t info;
  memset(&info, 0, sizeof(siginfo_t));
  info.si_signo = signo;
  info.si_code = SI_QUEUE;
  info.si_pid = getpid();
  info.si_uid = getuid();
  info.si_value = value;

  return ___rt_sigqueueinfo(pid, signo, &info);
}

int sigrelse(int sig) {
  kernel_sigset_t set;
  set.clear();
  if (!set.set(sig)) return -1;
  return __rt_sigprocmask(SIG_UNBLOCK, &set, nullptr, sizeof(set));
}

sighandler_t sigset(int sig, sighandler_t disp) {
  struct sigaction new_sa;
  if (disp != SIG_HOLD) {
    memset(&new_sa, 0, sizeof(new_sa));
    new_sa.sa_handler = disp;
    sigemptyset(&new_sa.sa_mask);
  }

  struct sigaction old_sa;
  if (sigaction(sig, (disp == SIG_HOLD) ? nullptr : &new_sa, &old_sa) == -1) {
    return SIG_ERR;
  }

  kernel_sigset_t new_mask{sig};
  kernel_sigset_t old_mask;
  if (__rt_sigprocmask(disp == SIG_HOLD ? SIG_BLOCK : SIG_UNBLOCK, &new_mask, &old_mask,
                       sizeof(new_mask)) == -1) {
    return SIG_ERR;
  }

  return old_mask.is_set(sig) ? SIG_HOLD : old_sa.sa_handler;
}

// This isn't in our header files, but is exposed on all architectures.
extern "C" int sigsetmask(int mask) {
  union {
    int mask;
    sigset_t set;
  } in, out;

  sigemptyset(&in.set);
  in.mask = mask;

  if (sigprocmask(SIG_SETMASK, &in.set, &out.set) == -1) return -1;
  return out.mask;
}

int sigsuspend(const sigset_t* bionic_set) {
  kernel_sigset_t set(bionic_set);
  return __rt_sigsuspend(&set, sizeof(set));
}

int sigtimedwait(const sigset_t* set, siginfo_t* info, const timespec* timeout) {
  kernel_sigset_t sigset(set);
  return __rt_sigtimedwait(sigset.get(), info, timeout, sizeof(sigset));
}

int sigwait(const sigset_t* set, int* sig) {
  kernel_sigset_t sigset(set);
  while (true) {
    // __rt_sigtimedwait can return EAGAIN or EINTR, we need to loop
    // around them since sigwait is only allowed to return EINVAL.
    int result = __rt_sigtimedwait(sigset.get(), NULL, NULL, sizeof(sigset));
    if (result >= 0) {
      *sig = result;
      return 0;
    }

    if (errno != EAGAIN && errno != EINTR) {
      return errno;
    }
  }
}

int sigwaitinfo(const sigset_t* set, siginfo_t* info) {
  return sigtimedwait(set, info, NULL);
}
