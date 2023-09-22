/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <signal.h>
#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <thread>

#include <android-base/macros.h>
#include <android-base/threads.h>

#include <gtest/gtest.h>

#include "SignalUtils.h"
#include "utils.h"

using namespace std::chrono_literals;

#if defined(ANDROID_HOST_MUSL)
// Musl doesn't export __SIGRTMIN and __SIGRTMAX, #define
// them here.
#define __SIGRTMIN 32
#define __SIGRTMAX 64
#endif

static int SIGNAL_MIN() {
  return 1; // Signals start at 1 (SIGHUP), not 0.
}

template <typename SigSetT>
static int SIGNAL_MAX(SigSetT* set) {
  return sizeof(*set) * 8;
}

template <typename SigSetT>
static void TestSigSet1(int (fn)(SigSetT*)) {
  // nullptr sigset_t*/sigset64_t*.
  SigSetT* set_ptr = nullptr;
  errno = 0;
  ASSERT_EQ(-1, fn(set_ptr));
  ASSERT_ERRNO(EINVAL);

  // Non-nullptr.
  SigSetT set = {};
  errno = 0;
  ASSERT_EQ(0, fn(&set));
  ASSERT_ERRNO(0);
}

template <typename SigSetT>
static void TestSigSet2(int (fn)(SigSetT*, int)) {
  // nullptr sigset_t*/sigset64_t*.
  SigSetT* set_ptr = nullptr;
  errno = 0;
  ASSERT_EQ(-1, fn(set_ptr, SIGSEGV));
  ASSERT_ERRNO(EINVAL);

  SigSetT set = {};

  // Bad signal number: too small.
  errno = 0;
  ASSERT_EQ(-1, fn(&set, 0));
  ASSERT_ERRNO(EINVAL);

  // Bad signal number: too high.
  errno = 0;
  ASSERT_EQ(-1, fn(&set, SIGNAL_MAX(&set) + 1));
  ASSERT_ERRNO(EINVAL);

  // Good signal numbers, low and high ends of range.
  errno = 0;
  ASSERT_EQ(0, fn(&set, SIGNAL_MIN()));
  ASSERT_ERRNO(0);
  ASSERT_EQ(0, fn(&set, SIGNAL_MAX(&set)));
  ASSERT_ERRNO(0);
}

TEST(signal, sigaddset_invalid) {
  TestSigSet2(sigaddset);
}

TEST(signal, sigaddset64_invalid) {
#if defined(__BIONIC__)
  TestSigSet2(sigaddset64);
#endif
}

TEST(signal, sigdelset_invalid) {
  TestSigSet2(sigdelset);
}

TEST(signal, sigdelset64_invalid) {
#if defined(__BIONIC__)
  TestSigSet2(sigdelset64);
#endif
}

TEST(signal, sigemptyset_invalid) {
  TestSigSet1(sigemptyset);
}

TEST(signal, sigemptyset64_invalid) {
#if defined(__BIONIC__)
  TestSigSet1(sigemptyset64);
#endif
}

TEST(signal, sigfillset_invalid) {
  TestSigSet1(sigfillset);
}

TEST(signal, sigfillset64_invalid) {
#if defined(__BIONIC__)
  TestSigSet1(sigfillset64);
#endif
}

TEST(signal, sigismember_invalid) {
  TestSigSet2(sigismember);
}

TEST(signal, sigismember64_invalid) {
#if defined(__BIONIC__)
  TestSigSet2(sigismember64);
#endif
}

TEST(signal, raise_invalid) {
  errno = 0;
  ASSERT_EQ(-1, raise(-1));
  ASSERT_ERRNO(EINVAL);
}

static void raise_in_signal_handler_helper(int signal_number) {
  ASSERT_EQ(SIGALRM, signal_number);
  static int count = 0;
  if (++count == 1) {
    raise(SIGALRM);
  }
}

TEST(signal, raise_in_signal_handler) {
  ScopedSignalHandler ssh(SIGALRM, raise_in_signal_handler_helper);
  raise(SIGALRM);
}

static int g_sigsuspend_signal_handler_call_count = 0;

TEST(signal, sigsuspend_sigpending) {
  SignalMaskRestorer smr;

  // Block SIGALRM.
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGALRM, nullptr));

  ScopedSignalHandler ssh(SIGALRM, [](int) { ++g_sigsuspend_signal_handler_call_count; });

  // There should be no pending signals.
  sigset_t pending;
  sigemptyset(&pending);
  ASSERT_EQ(0, sigpending(&pending));
  for (int i = SIGNAL_MIN(); i <= SIGNAL_MAX(&pending); ++i) {
    EXPECT_FALSE(sigismember(&pending, i)) << i;
  }

  // Raise SIGALRM and check our signal handler wasn't called.
  raise(SIGALRM);
  ASSERT_EQ(0, g_sigsuspend_signal_handler_call_count);

  // We should now have a pending SIGALRM but nothing else.
  sigemptyset(&pending);
  ASSERT_EQ(0, sigpending(&pending));
  for (int i = SIGNAL_MIN(); i <= SIGNAL_MAX(&pending); ++i) {
    EXPECT_EQ((i == SIGALRM), sigismember(&pending, i));
  }

  // Use sigsuspend to block everything except SIGALRM...
  sigset_t not_SIGALRM;
  sigfillset(&not_SIGALRM);
  sigdelset(&not_SIGALRM, SIGALRM);
  ASSERT_EQ(-1, sigsuspend(&not_SIGALRM));
  ASSERT_ERRNO(EINTR);
  // ...and check that we now receive our pending SIGALRM.
  ASSERT_EQ(1, g_sigsuspend_signal_handler_call_count);
}

static int g_sigsuspend64_signal_handler_call_count = 0;

TEST(signal, sigsuspend64_sigpending64) {
  SignalMaskRestorer smr;

  // Block SIGRTMIN.
  sigset64_t just_SIGRTMIN;
  sigemptyset64(&just_SIGRTMIN);
  sigaddset64(&just_SIGRTMIN, SIGRTMIN);
  ASSERT_EQ(0, sigprocmask64(SIG_BLOCK, &just_SIGRTMIN, nullptr));

  ScopedSignalHandler ssh(SIGRTMIN, [](int) { ++g_sigsuspend64_signal_handler_call_count; });

  // There should be no pending signals.
  sigset64_t pending;
  sigemptyset64(&pending);
  ASSERT_EQ(0, sigpending64(&pending));
  for (int i = SIGNAL_MIN(); i <= SIGNAL_MAX(&pending); ++i) {
    EXPECT_FALSE(sigismember64(&pending, i)) << i;
  }

  // Raise SIGRTMIN and check our signal handler wasn't called.
  raise(SIGRTMIN);
  ASSERT_EQ(0, g_sigsuspend64_signal_handler_call_count);

  // We should now have a pending SIGRTMIN but nothing else.
  sigemptyset64(&pending);
  ASSERT_EQ(0, sigpending64(&pending));
  for (int i = SIGNAL_MIN(); i <= SIGNAL_MAX(&pending); ++i) {
    EXPECT_EQ((i == SIGRTMIN), sigismember64(&pending, i));
  }

  // Use sigsuspend64 to block everything except SIGRTMIN...
  sigset64_t not_SIGRTMIN;
  sigfillset64(&not_SIGRTMIN);
  sigdelset64(&not_SIGRTMIN, SIGRTMIN);
  ASSERT_EQ(-1, sigsuspend64(&not_SIGRTMIN));
  ASSERT_ERRNO(EINTR);
  // ...and check that we now receive our pending SIGRTMIN.
  ASSERT_EQ(1, g_sigsuspend64_signal_handler_call_count);
}

template <typename SigActionT, typename SigSetT>
static void TestSigAction(int (sigaction_fn)(int, const SigActionT*, SigActionT*),
                          int (sigaddset_fn)(SigSetT*, int),
                          int sig) {
  // Both bionic and glibc set SA_RESTORER when talking to the kernel on arm,
  // arm64, x86, and x86-64. The version of glibc we're using also doesn't
  // define SA_RESTORER, but luckily it's the same value everywhere.
  static const unsigned sa_restorer = 0x4000000;

  // See what's currently set for this signal.
  SigActionT original_sa = {};
  ASSERT_EQ(0, sigaction_fn(sig, nullptr, &original_sa));
  ASSERT_TRUE(original_sa.sa_handler == nullptr);
  ASSERT_TRUE(original_sa.sa_sigaction == nullptr);
  ASSERT_EQ(0U, original_sa.sa_flags & ~sa_restorer);
#ifdef SA_RESTORER
  ASSERT_EQ(bool(original_sa.sa_flags & sa_restorer), bool(original_sa.sa_restorer));
#endif

  // Set a traditional sa_handler signal handler.
  auto no_op_signal_handler = [](int) {};
  SigActionT sa = {};
  sigaddset_fn(&sa.sa_mask, sig);
  sa.sa_flags = SA_ONSTACK;
  sa.sa_handler = no_op_signal_handler;
  ASSERT_EQ(0, sigaction_fn(sig, &sa, nullptr));

  // Check that we can read it back.
  sa = {};
  ASSERT_EQ(0, sigaction_fn(sig, nullptr, &sa));
  ASSERT_TRUE(sa.sa_handler == no_op_signal_handler);
  ASSERT_TRUE((void*) sa.sa_sigaction == (void*) sa.sa_handler);
  ASSERT_EQ(static_cast<unsigned>(SA_ONSTACK), sa.sa_flags & ~sa_restorer);
#ifdef SA_RESTORER
  ASSERT_EQ(bool(sa.sa_flags & sa_restorer), bool(sa.sa_restorer));
#endif

  // Set a new-style sa_sigaction signal handler.
  auto no_op_sigaction = [](int, siginfo_t*, void*) {};
  sa = {};
  sigaddset_fn(&sa.sa_mask, sig);
  sa.sa_flags = SA_ONSTACK | SA_SIGINFO;
  sa.sa_sigaction = no_op_sigaction;
  ASSERT_EQ(0, sigaction_fn(sig, &sa, nullptr));

  // Check that we can read it back.
  sa = {};
  ASSERT_EQ(0, sigaction_fn(sig, nullptr, &sa));
  ASSERT_TRUE(sa.sa_sigaction == no_op_sigaction);
  ASSERT_TRUE((void*) sa.sa_sigaction == (void*) sa.sa_handler);
  ASSERT_EQ(static_cast<unsigned>(SA_ONSTACK | SA_SIGINFO), sa.sa_flags & ~sa_restorer);
#ifdef SA_RESTORER
  ASSERT_EQ(bool(sa.sa_flags & sa_restorer), bool(sa.sa_restorer));
#endif

  // Put everything back how it was.
  ASSERT_EQ(0, sigaction_fn(sig, &original_sa, nullptr));
}

TEST(signal, sigaction) {
  TestSigAction(sigaction, sigaddset, SIGALRM);
}

TEST(signal, sigaction64_SIGRTMIN) {
  TestSigAction(sigaction64, sigaddset64, SIGRTMIN);
}

static void ClearSignalMask() {
  uint64_t sigset = 0;
  SignalSetAdd(&sigset, __SIGRTMIN);
  if (syscall(__NR_rt_sigprocmask, SIG_SETMASK, &sigset, nullptr, sizeof(sigset)) != 0) {
    abort();
  }
}

static void FillSignalMask() {
  uint64_t sigset = ~0ULL;
  for (int signo = __SIGRTMIN + 1; signo < SIGRTMIN; ++signo) {
    SignalSetDel(&sigset, signo);
  }
  if (syscall(__NR_rt_sigprocmask, SIG_SETMASK, &sigset, nullptr, sizeof(sigset)) != 0) {
    abort();
  }
}

static uint64_t GetSignalMask() {
  uint64_t sigset;
  if (syscall(__NR_rt_sigprocmask, SIG_SETMASK, nullptr, &sigset, sizeof(sigset)) != 0) {
    abort();
  }
  return sigset;
}

static void TestSignalMaskFiltered(uint64_t sigset) {
#if defined(__BIONIC__)
  for (int signo = __SIGRTMIN; signo < SIGRTMIN; ++signo) {
    bool signal_blocked = sigset & (1ULL << (signo - 1));
    if (signo == __SIGRTMIN) {
      // TIMER_SIGNAL must be blocked.
      EXPECT_EQ(true, signal_blocked) << "signal " << signo;
    } else {
      // The other reserved signals must not be blocked.
      EXPECT_EQ(false, signal_blocked) << "signal " << signo;
    }
  }
#else
  UNUSED(sigset);
#endif
}

static void TestSignalMaskFunction(std::function<void()> fn) {
  ClearSignalMask();
  fn();
  TestSignalMaskFiltered(GetSignalMask());
}

TEST(signal, sigaction_filter) {
  ClearSignalMask();
  static uint64_t sigset;
  struct sigaction sa = {};
  sa.sa_handler = [](int) { sigset = GetSignalMask(); };
  sa.sa_flags = SA_ONSTACK | SA_NODEFER;
  sigfillset(&sa.sa_mask);
  sigaction(SIGUSR1, &sa, nullptr);
  raise(SIGUSR1);

  // On LP32, struct sigaction::sa_mask is only 32-bits wide.
  unsigned long expected_sigset = ~0UL;

  // SIGKILL and SIGSTOP are always blocked.
  expected_sigset &= ~(1UL << (SIGKILL - 1));
  expected_sigset &= ~(1UL << (SIGSTOP - 1));

  ASSERT_EQ(static_cast<uint64_t>(expected_sigset), sigset);
}

TEST(signal, sigaction64_filter) {
  ClearSignalMask();
  static uint64_t sigset;
  struct sigaction64 sa = {};
  sa.sa_handler = [](int) { sigset = GetSignalMask(); };
  sa.sa_flags = SA_ONSTACK | SA_NODEFER;
  sigfillset64(&sa.sa_mask);
  sigaction64(SIGUSR1, &sa, nullptr);
  raise(SIGUSR1);

  uint64_t expected_sigset = ~0ULL;

  // SIGKILL and SIGSTOP are always blocked.
  expected_sigset &= ~(1ULL << (SIGKILL - 1));
  expected_sigset &= ~(1ULL << (SIGSTOP - 1));

  ASSERT_EQ(expected_sigset, sigset);
}

TEST(signal, sigprocmask_setmask_filter) {
  TestSignalMaskFunction([]() {
    ClearSignalMask();
    sigset_t sigset_libc;
    sigfillset(&sigset_libc);
    ASSERT_EQ(0, sigprocmask(SIG_SETMASK, &sigset_libc, nullptr));
  });
}

TEST(signal, sigprocmask64_setmask_filter) {
  TestSignalMaskFunction([]() {
    ClearSignalMask();
    sigset64_t sigset_libc;
    sigfillset64(&sigset_libc);
    ASSERT_EQ(0, sigprocmask64(SIG_SETMASK, &sigset_libc, nullptr));
  });
}

TEST(signal, pthread_sigmask_setmask_filter) {
  TestSignalMaskFunction([]() {
    ClearSignalMask();
    sigset_t sigset_libc;
    sigfillset(&sigset_libc);
    ASSERT_EQ(0, pthread_sigmask(SIG_SETMASK, &sigset_libc, nullptr));
  });
}

TEST(signal, pthread_sigmask64_setmask_filter) {
  TestSignalMaskFunction([]() {
    ClearSignalMask();
    sigset64_t sigset_libc;
    sigfillset64(&sigset_libc);
    ASSERT_EQ(0, pthread_sigmask64(SIG_SETMASK, &sigset_libc, nullptr));
  });
}

TEST(signal, sigprocmask_block_filter) {
  TestSignalMaskFunction([]() {
    ClearSignalMask();
    sigset_t sigset_libc;
    sigfillset(&sigset_libc);
    ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &sigset_libc, nullptr));
  });
}

TEST(signal, sigprocmask64_block_filter) {
  TestSignalMaskFunction([]() {
    ClearSignalMask();
    sigset64_t sigset_libc;
    sigfillset64(&sigset_libc);
    ASSERT_EQ(0, sigprocmask64(SIG_BLOCK, &sigset_libc, nullptr));
  });
}

TEST(signal, pthread_sigmask_block_filter) {
  TestSignalMaskFunction([]() {
    ClearSignalMask();
    sigset_t sigset_libc;
    sigfillset(&sigset_libc);
    ASSERT_EQ(0, pthread_sigmask(SIG_BLOCK, &sigset_libc, nullptr));
  });
}

TEST(signal, pthread_sigmask64_block_filter) {
  TestSignalMaskFunction([]() {
    ClearSignalMask();
    sigset64_t sigset_libc;
    sigfillset64(&sigset_libc);
    ASSERT_EQ(0, pthread_sigmask64(SIG_BLOCK, &sigset_libc, nullptr));
  });
}

TEST(signal, sigprocmask_unblock_filter) {
  TestSignalMaskFunction([]() {
    FillSignalMask();
    sigset_t sigset_libc;
    sigfillset(&sigset_libc);
    ASSERT_EQ(0, sigprocmask(SIG_UNBLOCK, &sigset_libc, nullptr));
  });
}

TEST(signal, sigprocmask64_unblock_filter) {
  TestSignalMaskFunction([]() {
    FillSignalMask();
    sigset64_t sigset_libc;
    sigfillset64(&sigset_libc);
    ASSERT_EQ(0, sigprocmask64(SIG_UNBLOCK, &sigset_libc, nullptr));
  });
}

TEST(signal, pthread_sigmask_unblock_filter) {
  TestSignalMaskFunction([]() {
    FillSignalMask();
    sigset_t sigset_libc;
    sigfillset(&sigset_libc);
    ASSERT_EQ(0, pthread_sigmask(SIG_UNBLOCK, &sigset_libc, nullptr));
  });
}

TEST(signal, pthread_sigmask64_unblock_filter) {
  TestSignalMaskFunction([]() {
    FillSignalMask();
    sigset64_t sigset_libc;
    sigfillset64(&sigset_libc);
    ASSERT_EQ(0, pthread_sigmask64(SIG_UNBLOCK, &sigset_libc, nullptr));
  });
}

// glibc filters out signals via sigfillset, not the actual underlying functions.
TEST(signal, sigset_filter) {
#if defined(__BIONIC__)
  TestSignalMaskFunction([]() {
    for (int i = 1; i <= 64; ++i) {
      sigset(i, SIG_HOLD);
    }
  });
#endif
}

TEST(signal, sighold_filter) {
#if defined(__BIONIC__)
  TestSignalMaskFunction([]() {
    for (int i = 1; i <= 64; ++i) {
      sighold(i);
    }
  });
#endif
}

#if defined(__BIONIC__) && !defined(__riscv)
// Not exposed via headers, but the symbols are available if you declare them yourself.
extern "C" int sigblock(int);
extern "C" int sigsetmask(int);
#define HAVE_SIGBLOCK_SIGSETMASK
#endif

TEST(signal, sigblock_filter) {
#if defined(HAVE_SIGBLOCK_SIGSETMASK)
  TestSignalMaskFunction([]() {
    sigblock(~0U);
  });
#endif
}

TEST(signal, sigsetmask_filter) {
#if defined(HAVE_SIGBLOCK_SIGSETMASK)
  TestSignalMaskFunction([]() {
    sigsetmask(~0U);
  });
#endif
}

TEST(signal, sys_signame) {
#if defined(__BIONIC__)
  ASSERT_TRUE(sys_signame[0] == nullptr);
  ASSERT_STREQ("HUP", sys_signame[SIGHUP]);
#else
  GTEST_SKIP() << "glibc doesn't have sys_signame";
#endif
}

TEST(signal, sys_siglist) {
#if !defined(ANDROID_HOST_MUSL)
  ASSERT_TRUE(sys_siglist[0] == nullptr);
  ASSERT_STREQ("Hangup", sys_siglist[SIGHUP]);
#else
  GTEST_SKIP() << "musl doesn't have sys_siglist";
#endif
}

TEST(signal, limits) {
  // These come from the kernel.
  ASSERT_EQ(32, __SIGRTMIN);
  ASSERT_EQ(64, __SIGRTMAX);

  // We reserve a non-zero number at the bottom for ourselves.
  ASSERT_GT(SIGRTMIN, __SIGRTMIN);

  // We don't currently reserve any at the top.
  ASSERT_EQ(SIGRTMAX, __SIGRTMAX);
}

static int g_sigqueue_signal_handler_call_count = 0;

static void SigqueueSignalHandler(int signum, siginfo_t* info, void*) {
  ASSERT_EQ(SIGALRM, signum);
  ASSERT_EQ(SIGALRM, info->si_signo);
  ASSERT_EQ(SI_QUEUE, info->si_code);
  ASSERT_EQ(1, info->si_value.sival_int);
  ++g_sigqueue_signal_handler_call_count;
}

TEST(signal, sigqueue) {
  ScopedSignalHandler ssh(SIGALRM, SigqueueSignalHandler, SA_SIGINFO);
  sigval sigval = {.sival_int = 1};
  errno = 0;
  ASSERT_EQ(0, sigqueue(getpid(), SIGALRM, sigval));
  ASSERT_ERRNO(0);
  ASSERT_EQ(1, g_sigqueue_signal_handler_call_count);
}

TEST(signal, pthread_sigqueue_self) {
#if !defined(ANDROID_HOST_MUSL)
  ScopedSignalHandler ssh(SIGALRM, SigqueueSignalHandler, SA_SIGINFO);
  sigval sigval = {.sival_int = 1};
  errno = 0;
  ASSERT_EQ(0, pthread_sigqueue(pthread_self(), SIGALRM, sigval));
  ASSERT_ERRNO(0);
  ASSERT_EQ(1, g_sigqueue_signal_handler_call_count);
#else
  GTEST_SKIP() << "musl doesn't have pthread_sigqueue";
#endif
}

TEST(signal, pthread_sigqueue_other) {
#if !defined(ANDROID_HOST_MUSL)
  ScopedSignalHandler ssh(SIGALRM, SigqueueSignalHandler, SA_SIGINFO);
  sigval sigval = {.sival_int = 1};

  sigset_t mask;
  sigfillset(&mask);
  pthread_sigmask(SIG_SETMASK, &mask, nullptr);
  pthread_t thread;
  int rc = pthread_create(&thread, nullptr,
                          [](void*) -> void* {
                            sigset_t mask;
                            sigemptyset(&mask);
                            sigsuspend(&mask);
                            return nullptr;
                          },
                          nullptr);
  ASSERT_EQ(0, rc);

  errno = 0;
  ASSERT_EQ(0, pthread_sigqueue(thread, SIGALRM, sigval));
  ASSERT_ERRNO(0);
  pthread_join(thread, nullptr);
  ASSERT_EQ(1, g_sigqueue_signal_handler_call_count);
#else
  GTEST_SKIP() << "musl doesn't have pthread_sigqueue";
#endif
}

TEST(signal, sigwait_SIGALRM) {
  SignalMaskRestorer smr;

  // Block SIGALRM.
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGALRM, nullptr));

  // Raise SIGALRM.
  sigval sigval = {.sival_int = 1};
  ASSERT_EQ(0, sigqueue(getpid(), SIGALRM, sigval));

  // Get pending SIGALRM.
  int sig;
  ASSERT_EQ(0, sigwait(&just_SIGALRM, &sig));
  ASSERT_EQ(SIGALRM, sig);
}

TEST(signal, sigwait64_SIGRTMIN) {
  SignalMaskRestorer smr;

  // Block SIGRTMIN.
  sigset64_t just_SIGRTMIN;
  sigemptyset64(&just_SIGRTMIN);
  sigaddset64(&just_SIGRTMIN, SIGRTMIN);
  ASSERT_EQ(0, sigprocmask64(SIG_BLOCK, &just_SIGRTMIN, nullptr));

  // Raise SIGRTMIN.
  sigval sigval = {.sival_int = 1};
  ASSERT_EQ(0, sigqueue(getpid(), SIGRTMIN, sigval));

  // Get pending SIGRTMIN.
  int sig;
  ASSERT_EQ(0, sigwait64(&just_SIGRTMIN, &sig));
  ASSERT_EQ(SIGRTMIN, sig);
}

TEST(signal, sigwaitinfo) {
  SignalMaskRestorer smr;

  // Block SIGALRM.
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGALRM, nullptr));

  // Raise SIGALRM.
  sigval sigval = {.sival_int = 1};
  ASSERT_EQ(0, sigqueue(getpid(), SIGALRM, sigval));

  // Get pending SIGALRM.
  siginfo_t info;
  errno = 0;
  ASSERT_EQ(SIGALRM, sigwaitinfo(&just_SIGALRM, &info));
  ASSERT_ERRNO(0);
  ASSERT_EQ(SIGALRM, info.si_signo);
  ASSERT_EQ(1, info.si_value.sival_int);
}

TEST(signal, sigwaitinfo64_SIGRTMIN) {
  SignalMaskRestorer smr;

  // Block SIGRTMIN.
  sigset64_t just_SIGRTMIN;
  sigemptyset64(&just_SIGRTMIN);
  sigaddset64(&just_SIGRTMIN, SIGRTMIN);
  ASSERT_EQ(0, sigprocmask64(SIG_BLOCK, &just_SIGRTMIN, nullptr));

  // Raise SIGRTMIN.
  sigval sigval = {.sival_int = 1};
  ASSERT_EQ(0, sigqueue(getpid(), SIGRTMIN, sigval));

  // Get pending SIGRTMIN.
  siginfo_t info;
  errno = 0;
  ASSERT_EQ(SIGRTMIN, sigwaitinfo64(&just_SIGRTMIN, &info));
  ASSERT_ERRNO(0);
  ASSERT_EQ(SIGRTMIN, info.si_signo);
  ASSERT_EQ(1, info.si_value.sival_int);
}

TEST(signal, sigtimedwait) {
  SignalMaskRestorer smr;

  // Block SIGALRM.
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGALRM, nullptr));

  // Raise SIGALRM.
  sigval sigval = { .sival_int = 1 };
  ASSERT_EQ(0, sigqueue(getpid(), SIGALRM, sigval));

  // Get pending SIGALRM.
  siginfo_t info;
  timespec timeout = { .tv_sec = 2, .tv_nsec = 0 };
  errno = 0;
  ASSERT_EQ(SIGALRM, sigtimedwait(&just_SIGALRM, &info, &timeout));
  ASSERT_ERRNO(0);
}

TEST(signal, sigtimedwait64_SIGRTMIN) {
  SignalMaskRestorer smr;

  // Block SIGRTMIN.
  sigset64_t just_SIGRTMIN;
  sigemptyset64(&just_SIGRTMIN);
  sigaddset64(&just_SIGRTMIN, SIGRTMIN);
  ASSERT_EQ(0, sigprocmask64(SIG_BLOCK, &just_SIGRTMIN, nullptr));

  // Raise SIGALRM.
  sigval sigval = { .sival_int = 1 };
  ASSERT_EQ(0, sigqueue(getpid(), SIGRTMIN, sigval));

  // Get pending SIGALRM.
  siginfo_t info;
  timespec timeout = { .tv_sec = 2, .tv_nsec = 0 };
  errno = 0;
  ASSERT_EQ(SIGRTMIN, sigtimedwait64(&just_SIGRTMIN, &info, &timeout));
  ASSERT_ERRNO(0);
}

TEST(signal, sigtimedwait_timeout) {
  // Block SIGALRM.
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  sigset_t original_set;
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGALRM, &original_set));

  // Wait timeout.
  auto t0 = std::chrono::steady_clock::now();
  siginfo_t info;
  timespec timeout = { .tv_sec = 0, .tv_nsec = 1000000 };
  errno = 0;
  ASSERT_EQ(-1, sigtimedwait(&just_SIGALRM, &info, &timeout));
  ASSERT_ERRNO(EAGAIN);
  auto t1 = std::chrono::steady_clock::now();
  ASSERT_GE(t1-t0, 1000000ns);

  ASSERT_EQ(0, sigprocmask(SIG_SETMASK, &original_set, nullptr));
}

#if defined(__BIONIC__)
TEST(signal, rt_tgsigqueueinfo) {
  // Test whether rt_tgsigqueueinfo allows sending arbitrary si_code values to self.
  // If this fails, your kernel needs commit 66dd34a to be backported.
  static constexpr char error_msg[] =
    "\nPlease ensure that the following kernel patch has been applied:\n"
    "* https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/commit/?id=66dd34ad31e5963d72a700ec3f2449291d322921\n";
  static siginfo received;

  struct sigaction handler = {};
  handler.sa_sigaction = [](int, siginfo_t* siginfo, void*) { received = *siginfo; };
  handler.sa_flags = SA_SIGINFO;

  ASSERT_EQ(0, sigaction(SIGUSR1, &handler, nullptr));

  siginfo sent;
  memset(&sent, 0, sizeof(sent));

  sent.si_code = SI_TKILL;
  ASSERT_EQ(0, syscall(SYS_rt_tgsigqueueinfo, getpid(), gettid(), SIGUSR1, &sent))
    << "rt_tgsigqueueinfo failed: " << strerror(errno) << error_msg;
  ASSERT_EQ(sent.si_code, received.si_code) << "rt_tgsigqueueinfo modified si_code, expected "
                                            << sent.si_code << ", received " << received.si_code
                                            << error_msg;

  sent.si_code = SI_USER;
  ASSERT_EQ(0, syscall(SYS_rt_tgsigqueueinfo, getpid(), gettid(), SIGUSR1, &sent))
    << "rt_tgsigqueueinfo failed: " << strerror(errno) << error_msg;
  ASSERT_EQ(sent.si_code, received.si_code) << "rt_tgsigqueueinfo modified si_code, expected "
                                            << sent.si_code << ", received " << received.si_code
                                            << error_msg;
}
#endif

TEST(signal, sigset_size) {
  // The setjmp implementations assume that sigset_t can fit in a long.
  // This is true because the 32-bit ABIs have broken rt signal support,
  // but the 64-bit ABIs both have a SIGRTMAX defined as 64.
#if defined(__BIONIC__)
  static_assert(sizeof(sigset_t) <= sizeof(long), "sigset_t doesn't fit in a long");
#endif
  static_assert(sizeof(sigset64_t)*8 >= 64, "sigset64_t too small for real-time signals");
}

TEST(signal, sigignore_EINVAL) {
  errno = 0;
  ASSERT_EQ(-1, sigignore(99999));
  ASSERT_ERRNO(EINVAL);
}

TEST(signal, sigignore) {
  errno = 0;
  EXPECT_EQ(-1, sigignore(SIGKILL));
  EXPECT_ERRNO(EINVAL);

  errno = 0;
  EXPECT_EQ(-1, sigignore(SIGSTOP));
  EXPECT_ERRNO(EINVAL);

  ScopedSignalHandler sigalrm{SIGALRM};
  ASSERT_EQ(0, sigignore(SIGALRM));

  struct sigaction sa;
  ASSERT_EQ(0, sigaction(SIGALRM, nullptr, &sa));
  EXPECT_EQ(SIG_IGN, sa.sa_handler);
}

TEST(signal, sighold_EINVAL) {
  errno = 0;
  ASSERT_EQ(-1, sighold(99999));
  ASSERT_ERRNO(EINVAL);
}

TEST(signal, sigpause_EINVAL) {
  errno = 0;
  ASSERT_EQ(-1, sigpause(99999));
  ASSERT_ERRNO(EINVAL);
}

TEST(signal, sigrelse_EINVAL) {
  errno = 0;
  ASSERT_EQ(-1, sigpause(99999));
  ASSERT_ERRNO(EINVAL);
}

static void TestSigholdSigpauseSigrelse(int sig) {
  static int signal_handler_call_count = 0;
  ScopedSignalHandler ssh{sig, [](int) { signal_handler_call_count++; }};
  SignalMaskRestorer mask_restorer;
  sigset_t set;

  // sighold(SIGALRM/SIGRTMIN) should add SIGALRM/SIGRTMIN to the signal mask ...
  ASSERT_EQ(0, sighold(sig));
  ASSERT_EQ(0, sigprocmask(SIG_SETMASK, nullptr, &set));
  EXPECT_TRUE(sigismember(&set, sig));

  // ... preventing our SIGALRM/SIGRTMIN handler from running ...
  raise(sig);
  ASSERT_EQ(0, signal_handler_call_count);
  // ... until sigpause(SIGALRM/SIGRTMIN) temporarily unblocks it.
  ASSERT_EQ(-1, sigpause(sig));
  ASSERT_ERRNO(EINTR);
  ASSERT_EQ(1, signal_handler_call_count);

  if (sig >= SIGRTMIN && sizeof(void*) == 8) {
    // But sigpause(SIGALRM/SIGRTMIN) shouldn't permanently unblock SIGALRM/SIGRTMIN.
    ASSERT_EQ(0, sigprocmask(SIG_SETMASK, nullptr, &set));
    EXPECT_TRUE(sigismember(&set, sig));

    // Whereas sigrelse(SIGALRM/SIGRTMIN) should.
    ASSERT_EQ(0, sigrelse(sig));
    ASSERT_EQ(0, sigprocmask(SIG_SETMASK, nullptr, &set));
    EXPECT_FALSE(sigismember(&set, sig));
  } else {
    // sigismember won't work for SIGRTMIN on LP32.
  }
}

TEST(signal, sighold_sigpause_sigrelse) {
  TestSigholdSigpauseSigrelse(SIGALRM);
}

TEST(signal, sighold_sigpause_sigrelse_RT) {
  TestSigholdSigpauseSigrelse(SIGRTMIN);
}

TEST(signal, sigset_EINVAL) {
  errno = 0;
  ASSERT_EQ(SIG_ERR, sigset(99999, SIG_DFL));
  ASSERT_ERRNO(EINVAL);
}

TEST(signal, sigset_RT) {
  static int signal_handler_call_count = 0;
  auto signal_handler = [](int) { signal_handler_call_count++; };
  ScopedSignalHandler ssh{SIGRTMIN, signal_handler};
  SignalMaskRestorer mask_restorer;

  ASSERT_EQ(signal_handler, sigset(SIGRTMIN, SIG_HOLD));
#if defined(__LP64__)
  sigset_t set;
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, nullptr, &set));
  ASSERT_TRUE(sigismember(&set, SIGRTMIN));
#endif

  ASSERT_EQ(SIG_HOLD, sigset(SIGRTMIN, signal_handler));
  ASSERT_EQ(signal_handler, sigset(SIGRTMIN, signal_handler));
  ASSERT_EQ(0, signal_handler_call_count);
  raise(SIGRTMIN);
  ASSERT_EQ(1, signal_handler_call_count);
}

TEST(signal, sigset) {
  static int signal_handler_call_count = 0;
  auto signal_handler = [](int) { signal_handler_call_count++; };
  ScopedSignalHandler ssh{SIGALRM, signal_handler};
  SignalMaskRestorer mask_restorer;

  ASSERT_EQ(0, signal_handler_call_count);
  raise(SIGALRM);
  ASSERT_EQ(1, signal_handler_call_count);

  // Block SIGALRM so the next sigset(SIGARLM) call will return SIG_HOLD.
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGALRM);
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &set, nullptr));

  sigemptyset(&set);
  ASSERT_EQ(SIG_HOLD, sigset(SIGALRM, signal_handler));
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, nullptr, &set));
  EXPECT_FALSE(sigismember(&set, SIGALRM));

  ASSERT_EQ(signal_handler, sigset(SIGALRM, SIG_IGN));
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, nullptr, &set));
  EXPECT_FALSE(sigismember(&set, SIGALRM));

  ASSERT_EQ(SIG_IGN, sigset(SIGALRM, SIG_DFL));
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, nullptr, &set));
  EXPECT_FALSE(sigismember(&set, SIGALRM));

  ASSERT_EQ(SIG_DFL, sigset(SIGALRM, SIG_HOLD));
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, nullptr, &set));
  EXPECT_TRUE(sigismember(&set, SIGALRM));
}

TEST(signal, killpg_EINVAL) {
  // POSIX leaves pgrp <= 1 undefined, but glibc fails with EINVAL for < 0
  // and passes 0 through to kill(2).
  errno = 0;
  ASSERT_EQ(-1, killpg(-1, SIGKILL));
  ASSERT_ERRNO(EINVAL);
}
