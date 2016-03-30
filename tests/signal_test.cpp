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
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include "ScopedSignalHandler.h"

static size_t SIGNAL_MIN() {
  return 1; // Signals start at 1 (SIGHUP), not 0.
}

static size_t SIGNAL_MAX() {
  size_t result = SIGRTMAX;

#if defined(__BIONIC__) && !defined(__mips__) && !defined(__LP64__)
  // 32-bit bionic's sigset_t is too small for ARM and x86: 32 bits instead of 64.
  // This means you can't refer to any of the real-time signals.
  // See http://b/3038348 and http://b/5828899.
  result = 32;
#else
  // Otherwise, C libraries should be perfectly capable of using their largest signal.
  if (sizeof(sigset_t) * 8 < static_cast<size_t>(SIGRTMAX)) {
    abort();
  }
#endif

  return result;
}

template <typename Fn>
static void TestSigSet1(Fn fn) {
  // NULL sigset_t*.
  sigset_t* set_ptr = NULL;
  errno = 0;
  ASSERT_EQ(-1, fn(set_ptr));
  ASSERT_EQ(EINVAL, errno);

  // Non-NULL.
  sigset_t set;
  errno = 0;
  ASSERT_EQ(0, fn(&set));
  ASSERT_EQ(0, errno);
}

template <typename Fn>
static void TestSigSet2(Fn fn) {
  // NULL sigset_t*.
  sigset_t* set_ptr = NULL;
  errno = 0;
  ASSERT_EQ(-1, fn(set_ptr, SIGSEGV));
  ASSERT_EQ(EINVAL, errno);

  sigset_t set;
  sigemptyset(&set);

  // Bad signal number: too small.
  errno = 0;
  ASSERT_EQ(-1, fn(&set, 0));
  ASSERT_EQ(EINVAL, errno);

  // Bad signal number: too high.
  errno = 0;
  ASSERT_EQ(-1, fn(&set, SIGNAL_MAX() + 1));
  ASSERT_EQ(EINVAL, errno);

  // Good signal numbers, low and high ends of range.
  errno = 0;
  ASSERT_EQ(0, fn(&set, SIGNAL_MIN()));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(0, fn(&set, SIGNAL_MAX()));
  ASSERT_EQ(0, errno);
}

TEST(signal, sigismember_invalid) {
  TestSigSet2(sigismember);
}

TEST(signal, sigaddset_invalid) {
  TestSigSet2(sigaddset);
}

TEST(signal, sigdelset_invalid) {
  TestSigSet2(sigdelset);
}

TEST(signal, sigemptyset_invalid) {
  TestSigSet1(sigemptyset);
}

TEST(signal, sigfillset_invalid) {
  TestSigSet1(sigfillset);
}

TEST(signal, raise_invalid) {
  errno = 0;
  ASSERT_EQ(-1, raise(-1));
  ASSERT_EQ(EINVAL, errno);
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

static void HandleSIGALRM(int signal_number) {
  ASSERT_EQ(SIGALRM, signal_number);
}

TEST(signal, sigwait) {
  ScopedSignalHandler ssh(SIGALRM, HandleSIGALRM);

  sigset_t wait_set;
  sigemptyset(&wait_set);
  sigaddset(&wait_set, SIGALRM);

  alarm(1);

  int received_signal;
  errno = 0;
  ASSERT_EQ(0, sigwait(&wait_set, &received_signal));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(SIGALRM, received_signal);
}

static int g_sigsuspend_test_helper_call_count = 0;

static void SigSuspendTestHelper(int) {
  ++g_sigsuspend_test_helper_call_count;
}

TEST(signal, sigsuspend_sigpending) {
  // Block SIGALRM.
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  sigset_t original_set;
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGALRM, &original_set));

  ScopedSignalHandler ssh(SIGALRM, SigSuspendTestHelper);

  // There should be no pending signals.
  sigset_t pending;
  sigemptyset(&pending);
  ASSERT_EQ(0, sigpending(&pending));
  for (size_t i = SIGNAL_MIN(); i <= SIGNAL_MAX(); ++i) {
    EXPECT_FALSE(sigismember(&pending, i)) << i;
  }

  // Raise SIGALRM and check our signal handler wasn't called.
  raise(SIGALRM);
  ASSERT_EQ(0, g_sigsuspend_test_helper_call_count);

  // We should now have a pending SIGALRM but nothing else.
  sigemptyset(&pending);
  ASSERT_EQ(0, sigpending(&pending));
  for (size_t i = SIGNAL_MIN(); i <= SIGNAL_MAX(); ++i) {
    EXPECT_EQ((i == SIGALRM), sigismember(&pending, i));
  }

  // Use sigsuspend to block everything except SIGALRM...
  sigset_t not_SIGALRM;
  sigfillset(&not_SIGALRM);
  sigdelset(&not_SIGALRM, SIGALRM);
  ASSERT_EQ(-1, sigsuspend(&not_SIGALRM));
  ASSERT_EQ(EINTR, errno);
  // ...and check that we now receive our pending SIGALRM.
  ASSERT_EQ(1, g_sigsuspend_test_helper_call_count);

  // Restore the original set.
  ASSERT_EQ(0, sigprocmask(SIG_SETMASK, &original_set, NULL));
}

static void EmptySignalHandler(int) {}
static void EmptySignalAction(int, siginfo_t*, void*) {}

TEST(signal, sigaction) {
  // Both bionic and glibc set SA_RESTORER when talking to the kernel on arm,
  // arm64, x86, and x86-64. The version of glibc we're using also doesn't
  // define SA_RESTORER, but luckily it's the same value everywhere, and mips
  // doesn't use the bit for anything.
  static const unsigned sa_restorer = 0x4000000;

  // See what's currently set for SIGALRM.
  struct sigaction original_sa;
  memset(&original_sa, 0, sizeof(original_sa));
  ASSERT_EQ(0, sigaction(SIGALRM, NULL, &original_sa));
  ASSERT_TRUE(original_sa.sa_handler == NULL);
  ASSERT_TRUE(original_sa.sa_sigaction == NULL);
  ASSERT_EQ(0U, original_sa.sa_flags & ~sa_restorer);

  // Set a traditional sa_handler signal handler.
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sigaddset(&sa.sa_mask, SIGALRM);
  sa.sa_flags = SA_ONSTACK;
  sa.sa_handler = EmptySignalHandler;
  ASSERT_EQ(0, sigaction(SIGALRM, &sa, NULL));

  // Check that we can read it back.
  memset(&sa, 0, sizeof(sa));
  ASSERT_EQ(0, sigaction(SIGALRM, NULL, &sa));
  ASSERT_TRUE(sa.sa_handler == EmptySignalHandler);
  ASSERT_TRUE((void*) sa.sa_sigaction == (void*) sa.sa_handler);
  ASSERT_EQ(static_cast<unsigned>(SA_ONSTACK), sa.sa_flags & ~sa_restorer);

  // Set a new-style sa_sigaction signal handler.
  memset(&sa, 0, sizeof(sa));
  sigaddset(&sa.sa_mask, SIGALRM);
  sa.sa_flags = SA_ONSTACK | SA_SIGINFO;
  sa.sa_sigaction = EmptySignalAction;
  ASSERT_EQ(0, sigaction(SIGALRM, &sa, NULL));

  // Check that we can read it back.
  memset(&sa, 0, sizeof(sa));
  ASSERT_EQ(0, sigaction(SIGALRM, NULL, &sa));
  ASSERT_TRUE(sa.sa_sigaction == EmptySignalAction);
  ASSERT_TRUE((void*) sa.sa_sigaction == (void*) sa.sa_handler);
  ASSERT_EQ(static_cast<unsigned>(SA_ONSTACK | SA_SIGINFO), sa.sa_flags & ~sa_restorer);

  // Put everything back how it was.
  ASSERT_EQ(0, sigaction(SIGALRM, &original_sa, NULL));
}

TEST(signal, sys_signame) {
#if defined(__BIONIC__)
  ASSERT_TRUE(sys_signame[0] == NULL);
  ASSERT_STREQ("HUP", sys_signame[SIGHUP]);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

TEST(signal, sys_siglist) {
  ASSERT_TRUE(sys_siglist[0] == NULL);
  ASSERT_STREQ("Hangup", sys_siglist[SIGHUP]);
}

TEST(signal, limits) {
  // This comes from the kernel.
  ASSERT_EQ(32, __SIGRTMIN);

  // We reserve a non-zero number at the bottom for ourselves.
  ASSERT_GT(SIGRTMIN, __SIGRTMIN);

  // MIPS has more signals than everyone else.
#if defined(__mips__)
  ASSERT_EQ(128, __SIGRTMAX);
#else
  ASSERT_EQ(64, __SIGRTMAX);
#endif

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
  sigval_t sigval;
  sigval.sival_int = 1;
  errno = 0;
  ASSERT_EQ(0, sigqueue(getpid(), SIGALRM, sigval));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(1, g_sigqueue_signal_handler_call_count);
}

TEST(signal, sigwaitinfo) {
  // Block SIGALRM.
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  sigset_t original_set;
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGALRM, &original_set));

  // Raise SIGALRM.
  sigval_t sigval;
  sigval.sival_int = 1;
  ASSERT_EQ(0, sigqueue(getpid(), SIGALRM, sigval));

  // Get pending SIGALRM.
  siginfo_t info;
  errno = 0;
  ASSERT_EQ(SIGALRM, sigwaitinfo(&just_SIGALRM, &info));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(SIGALRM, info.si_signo);
  ASSERT_EQ(1, info.si_value.sival_int);

  ASSERT_EQ(0, sigprocmask(SIG_SETMASK, &original_set, NULL));
}

TEST(signal, sigtimedwait) {
  // Block SIGALRM.
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  sigset_t original_set;
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGALRM, &original_set));

  // Raise SIGALRM.
  sigval_t sigval;
  sigval.sival_int = 1;
  ASSERT_EQ(0, sigqueue(getpid(), SIGALRM, sigval));

  // Get pending SIGALRM.
  siginfo_t info;
  struct timespec timeout;
  timeout.tv_sec = 2;
  timeout.tv_nsec = 0;
  errno = 0;
  ASSERT_EQ(SIGALRM, sigtimedwait(&just_SIGALRM, &info, &timeout));
  ASSERT_EQ(0, errno);

  ASSERT_EQ(0, sigprocmask(SIG_SETMASK, &original_set, NULL));
}

static int64_t NanoTime() {
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return static_cast<int64_t>(t.tv_sec) * 1000000000LL + t.tv_nsec;
}

TEST(signal, sigtimedwait_timeout) {
  // Block SIGALRM.
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  sigset_t original_set;
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGALRM, &original_set));

  // Wait timeout.
  int64_t start_time = NanoTime();
  siginfo_t info;
  struct timespec timeout;
  timeout.tv_sec = 0;
  timeout.tv_nsec = 1000000;
  errno = 0;
  ASSERT_EQ(-1, sigtimedwait(&just_SIGALRM, &info, &timeout));
  ASSERT_EQ(EAGAIN, errno);
  ASSERT_GE(NanoTime() - start_time, 1000000);

  ASSERT_EQ(0, sigprocmask(SIG_SETMASK, &original_set, NULL));
}

#if defined(__BIONIC__)
TEST(signal, rt_tgsigqueueinfo) {
  // Test whether rt_tgsigqueueinfo allows sending arbitrary si_code values to self.
  // If this fails, your kernel needs commit 66dd34a to be backported.
  static constexpr char error_msg[] =
    "\nPlease ensure that the following kernel patch has been applied:\n"
    "* https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/commit/?id=66dd34ad31e5963d72a700ec3f2449291d322921\n";
  static siginfo received;

  struct sigaction handler;
  memset(&handler, 0, sizeof(handler));
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

#if defined(__arm__) || defined(__aarch64__) || defined(__i386__) || defined(__x86_64__)
TEST(signal, sigset_size) {
  // The setjmp implementations for ARM, AArch64, x86, and x86_64 assume that sigset_t can fit in a
  // long. This is true because ARM and x86 have broken rt signal support, and AArch64 and x86_64
  // both have a SIGRTMAX defined as 64.
  static_assert(sizeof(sigset_t) <= sizeof(long), "sigset_t doesn't fit in a long");
}

#endif
#endif
