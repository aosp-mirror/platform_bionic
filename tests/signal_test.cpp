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

#include <gtest/gtest.h>

#include <errno.h>
#include <signal.h>

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

class ScopedSignalHandler {
 public:
  ScopedSignalHandler(int signal_number, void (*handler)(int)) : signal_number_(signal_number) {
    sigemptyset(&action_.sa_mask);
    action_.sa_flags = 0;
    action_.sa_handler = handler;
    sigaction(signal_number_, &action_, &old_action_);
  }

  ~ScopedSignalHandler() {
    sigaction(signal_number_, &old_action_, NULL);
  }

 private:
  struct sigaction action_;
  struct sigaction old_action_;
  const int signal_number_;
};

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

static int gSigSuspendTestHelperCallCount = 0;

static void SigSuspendTestHelper(int) {
  ++gSigSuspendTestHelperCallCount;
}

TEST(signal, sigsuspend_sigpending) {
  ScopedSignalHandler ssh(SIGALRM, SigSuspendTestHelper);

  // Block SIGALRM.
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  sigset_t original_set;
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGALRM, &original_set));

  // There should be no pending signals.
  sigset_t pending;
  sigemptyset(&pending);
  ASSERT_EQ(0, sigpending(&pending));
  for (size_t i = SIGNAL_MIN(); i <= SIGNAL_MAX(); ++i) {
    EXPECT_FALSE(sigismember(&pending, i)) << i;
  }

  // Raise SIGALRM and check our signal handler wasn't called.
  raise(SIGALRM);
  ASSERT_EQ(0, gSigSuspendTestHelperCallCount);

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
  ASSERT_EQ(1, gSigSuspendTestHelperCallCount);

  // Restore the original set.
  assert(0 == sigprocmask(SIG_SETMASK, &original_set, NULL));
}
