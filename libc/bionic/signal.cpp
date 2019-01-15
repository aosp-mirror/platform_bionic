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
#include <sys/epoll.h>
#include <sys/signalfd.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "private/ErrnoRestorer.h"
#include "private/SigSetConverter.h"
#include "private/sigrtmin.h"

extern "C" int __rt_sigpending(const sigset64_t*, size_t);
extern "C" int ___rt_sigqueueinfo(pid_t, int, siginfo_t*);
extern "C" int __rt_sigsuspend(const sigset64_t*, size_t);
extern "C" int __rt_sigtimedwait(const sigset64_t*, siginfo_t*, const timespec*, size_t);

int pthread_sigmask(int how, const sigset_t* new_set, sigset_t* old_set) {
  ErrnoRestorer errno_restorer;
  return (sigprocmask(how, new_set, old_set) == -1) ? errno : 0;
}

int pthread_sigmask64(int how, const sigset64_t* new_set, sigset64_t* old_set) {
  ErrnoRestorer errno_restorer;
  return (sigprocmask64(how, new_set, old_set) == -1) ? errno : 0;
}

template <typename SigSetT>
int SigAddSet(SigSetT* set, int sig) {
  int bit = sig - 1; // Signal numbers start at 1, but bit positions start at 0.
  unsigned long* local_set = reinterpret_cast<unsigned long*>(set);
  if (set == nullptr || bit < 0 || bit >= static_cast<int>(8*sizeof(*set))) {
    errno = EINVAL;
    return -1;
  }
  local_set[bit / LONG_BIT] |= 1UL << (bit % LONG_BIT);
  return 0;
}

int sigaddset(sigset_t* set, int sig) {
  return SigAddSet(set, sig);
}

int sigaddset64(sigset64_t* set, int sig) {
  return SigAddSet(set, sig);
}

// This isn't in our header files, but is exposed on all architectures.
extern "C" int sigblock(int mask) {
  SigSetConverter in, out;
  sigemptyset(&in.sigset);
  in.bsd = mask;
  if (sigprocmask(SIG_BLOCK, &in.sigset, &out.sigset) == -1) return -1;
  return out.bsd;
}

template <typename SigSetT>
int SigDelSet(SigSetT* set, int sig) {
  int bit = sig - 1; // Signal numbers start at 1, but bit positions start at 0.
  unsigned long* local_set = reinterpret_cast<unsigned long*>(set);
  if (set == nullptr || bit < 0 || bit >= static_cast<int>(8*sizeof(*set))) {
    errno = EINVAL;
    return -1;
  }
  local_set[bit / LONG_BIT] &= ~(1UL << (bit % LONG_BIT));
  return 0;
}

int sigdelset(sigset_t* set, int sig) {
  return SigDelSet(set, sig);
}

int sigdelset64(sigset64_t* set, int sig) {
  return SigDelSet(set, sig);
}

template <typename SigSetT>
int SigEmptySet(SigSetT* set) {
  if (set == nullptr) {
    errno = EINVAL;
    return -1;
  }
  memset(set, 0, sizeof(*set));
  return 0;
}

int sigemptyset(sigset_t* set) {
  return SigEmptySet(set);
}

int sigemptyset64(sigset64_t* set) {
  return SigEmptySet(set);
}

template <typename SigSetT>
int SigFillSet(SigSetT* set) {
  if (set == nullptr) {
    errno = EINVAL;
    return -1;
  }
  memset(set, 0xff, sizeof(*set));
  return 0;
}

int sigfillset(sigset_t* set) {
  return SigFillSet(set);
}

int sigfillset64(sigset64_t* set) {
  return SigFillSet(set);
}

int sighold(int sig) {
  sigset64_t set = {};
  if (sigaddset64(&set, sig) == -1) return -1;
  return sigprocmask64(SIG_BLOCK, &set, nullptr);
}

int sigignore(int sig) {
  struct sigaction64 sa = { .sa_handler = SIG_IGN };
  return sigaction64(sig, &sa, nullptr);
}

int siginterrupt(int sig, int flag) {
  struct sigaction64 act;
  sigaction64(sig, nullptr, &act);
  if (flag) {
    act.sa_flags &= ~SA_RESTART;
  } else {
    act.sa_flags |= SA_RESTART;
  }
  return sigaction64(sig, &act, nullptr);
}

template <typename SigSetT>
int SigIsMember(const SigSetT* set, int sig) {
  int bit = sig - 1; // Signal numbers start at 1, but bit positions start at 0.
  const unsigned long* local_set = reinterpret_cast<const unsigned long*>(set);
  if (set == nullptr || bit < 0 || bit >= static_cast<int>(8*sizeof(*set))) {
    errno = EINVAL;
    return -1;
  }
  return static_cast<int>((local_set[bit / LONG_BIT] >> (bit % LONG_BIT)) & 1);
}

int sigismember(const sigset_t* set, int sig) {
  return SigIsMember(set, sig);
}

int sigismember64(const sigset64_t* set, int sig) {
  return SigIsMember(set, sig);
}

__LIBC_HIDDEN__ sighandler_t _signal(int sig, sighandler_t handler, int flags) {
  struct sigaction64 sa = { .sa_handler = handler, .sa_flags = flags };
  return (sigaction64(sig, &sa, &sa) == -1) ? SIG_ERR : sa.sa_handler;
}

sighandler_t signal(int sig, sighandler_t handler) {
  return _signal(sig, handler, SA_RESTART);
}

int sigpause(int sig) {
  sigset64_t set = {};
  if (sigprocmask64(SIG_SETMASK, nullptr, &set) == -1 || sigdelset64(&set, sig) == -1) return -1;
  return sigsuspend64(&set);
}

int sigpending(sigset_t* bionic_set) {
  SigSetConverter set = {};
  set.sigset = *bionic_set;
  if (__rt_sigpending(&set.sigset64, sizeof(set.sigset64)) == -1) return -1;
  *bionic_set = set.sigset;
  return 0;
}

int sigpending64(sigset64_t* set) {
  return __rt_sigpending(set, sizeof(*set));
}

int sigqueue(pid_t pid, int sig, const sigval value) {
  siginfo_t info;
  memset(&info, 0, sizeof(siginfo_t));
  info.si_signo = sig;
  info.si_code = SI_QUEUE;
  info.si_pid = getpid();
  info.si_uid = getuid();
  info.si_value = value;
  return ___rt_sigqueueinfo(pid, sig, &info);
}

int sigrelse(int sig) {
  sigset64_t set = {};
  if (sigaddset64(&set, sig) == -1) return -1;
  return sigprocmask64(SIG_UNBLOCK, &set, nullptr);
}

sighandler_t sigset(int sig, sighandler_t disp) {
  struct sigaction64 new_sa;
  if (disp != SIG_HOLD) new_sa = { .sa_handler = disp };

  struct sigaction64 old_sa;
  if (sigaction64(sig, (disp == SIG_HOLD) ? nullptr : &new_sa, &old_sa) == -1) {
    return SIG_ERR;
  }

  sigset64_t new_mask = {};
  sigaddset64(&new_mask, sig);
  sigset64_t old_mask;
  if (sigprocmask64(disp == SIG_HOLD ? SIG_BLOCK : SIG_UNBLOCK, &new_mask, &old_mask) == -1) {
    return SIG_ERR;
  }

  return sigismember64(&old_mask, sig) ? SIG_HOLD : old_sa.sa_handler;
}

// This isn't in our header files, but is exposed on all architectures.
extern "C" int sigsetmask(int mask) {
  SigSetConverter in, out;
  sigemptyset(&in.sigset);
  in.bsd = mask;
  if (sigprocmask(SIG_SETMASK, &in.sigset, &out.sigset) == -1) return -1;
  return out.bsd;
}

int sigsuspend(const sigset_t* bionic_set) {
  SigSetConverter set = {};
  set.sigset = *bionic_set;
  return sigsuspend64(&set.sigset64);
}

int sigsuspend64(const sigset64_t* set) {
  sigset64_t mutable_set;
  sigset64_t* mutable_set_ptr = nullptr;
  if (set) {
    mutable_set = filter_reserved_signals(*set, SIG_SETMASK);
    mutable_set_ptr = &mutable_set;
  }
  return __rt_sigsuspend(mutable_set_ptr, sizeof(*set));
}

int sigtimedwait(const sigset_t* bionic_set, siginfo_t* info, const timespec* timeout) {
  SigSetConverter set = {};
  set.sigset = *bionic_set;
  return sigtimedwait64(&set.sigset64, info, timeout);
}

int sigtimedwait64(const sigset64_t* set, siginfo_t* info, const timespec* timeout) {
  sigset64_t mutable_set;
  sigset64_t* mutable_set_ptr = nullptr;
  if (set) {
    mutable_set = filter_reserved_signals(*set, SIG_SETMASK);
    mutable_set_ptr = &mutable_set;
  }
  return __rt_sigtimedwait(mutable_set_ptr, info, timeout, sizeof(*set));
}

int sigwait(const sigset_t* bionic_set, int* sig) {
  SigSetConverter set = {};
  set.sigset = *bionic_set;
  return sigwait64(&set.sigset64, sig);
}

int sigwait64(const sigset64_t* set, int* sig) {
  while (true) {
    // __rt_sigtimedwait can return EAGAIN or EINTR, we need to loop
    // around them since sigwait is only allowed to return EINVAL.
    int result = sigtimedwait64(set, nullptr, nullptr);
    if (result >= 0) {
      *sig = result;
      return 0;
    }
    if (errno != EAGAIN && errno != EINTR) return errno;
  }
}

int sigwaitinfo(const sigset_t* set, siginfo_t* info) {
  return sigtimedwait(set, info, nullptr);
}

int sigwaitinfo64(const sigset64_t* set, siginfo_t* info) {
  return sigtimedwait64(set, info, nullptr);
}
