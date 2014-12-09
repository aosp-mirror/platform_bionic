/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <setjmp.h>
#include <stdlib.h>

TEST(setjmp, setjmp_smoke) {
  int value;
  jmp_buf jb;
  if ((value = setjmp(jb)) == 0) {
    longjmp(jb, 123);
    FAIL(); // Unreachable.
  } else {
    ASSERT_EQ(123, value);
  }
}

TEST(setjmp, _setjmp_smoke) {
  int value;
  jmp_buf jb;
  if ((value = _setjmp(jb)) == 0) {
    _longjmp(jb, 456);
    FAIL(); // Unreachable.
  } else {
    ASSERT_EQ(456, value);
  }
}

TEST(setjmp, sigsetjmp_0_smoke) {
  int value;
  sigjmp_buf jb;
  if ((value = sigsetjmp(jb, 0)) == 0) {
    siglongjmp(jb, 789);
    FAIL(); // Unreachable.
  } else {
    ASSERT_EQ(789, value);
  }
}

TEST(setjmp, sigsetjmp_1_smoke) {
  int value;
  sigjmp_buf jb;
  if ((value = sigsetjmp(jb, 0)) == 0) {
    siglongjmp(jb, 0xabc);
    FAIL(); // Unreachable.
  } else {
    ASSERT_EQ(0xabc, value);
  }
}

static sigset_t SigSetOf(int signal, int rt_signal = 0) {
  sigset_t ss;
  sigemptyset(&ss);
  sigaddset(&ss, signal);
  if (rt_signal != 0) {
    sigaddset(&ss, rt_signal);
  }
  return ss;
}

void AssertSigmaskEquals(const sigset_t& expected) {
  sigset_t actual;
  sigprocmask(0 /* ignored */, NULL, &actual);
  size_t end = sizeof(sigset_t) * 8;
  for (size_t i = 1; i <= end; ++i) {
    EXPECT_EQ(sigismember(&expected, i), sigismember(&actual, i)) << i;
  }
}

TEST(setjmp, _setjmp_signal_mask) {
  // _setjmp/_longjmp do not save/restore the signal mask.
  sigset_t ss1(SigSetOf(SIGUSR1, SIGRTMIN + 8));
  sigset_t ss2(SigSetOf(SIGUSR2, SIGRTMIN + 9));
  sigset_t original_set;
  sigprocmask(SIG_SETMASK, &ss1, &original_set);
  jmp_buf jb;
  if (_setjmp(jb) == 0) {
    sigprocmask(SIG_SETMASK, &ss2, NULL);
    _longjmp(jb, 1);
    FAIL(); // Unreachable.
  } else {
    AssertSigmaskEquals(ss2);
  }
  sigprocmask(SIG_SETMASK, &original_set, NULL);
}

TEST(setjmp, setjmp_signal_mask) {
  // setjmp/longjmp do save/restore the signal mask on bionic, but not on glibc.
  // This is a BSD versus System V historical accident. POSIX leaves the
  // behavior unspecified, so any code that cares needs to use sigsetjmp.
  sigset_t ss1(SigSetOf(SIGUSR1, SIGRTMIN + 8));
  sigset_t ss2(SigSetOf(SIGUSR2, SIGRTMIN + 9));
  sigset_t original_set;
  sigprocmask(SIG_SETMASK, &ss1, &original_set);
  jmp_buf jb;
  if (setjmp(jb) == 0) {
    sigprocmask(SIG_SETMASK, &ss2, NULL);
    longjmp(jb, 1);
    FAIL(); // Unreachable.
  } else {
#if defined(__BIONIC__)
    // bionic behaves like BSD and does save/restore the signal mask.
    AssertSigmaskEquals(ss1);
#else
    // glibc behaves like System V and doesn't save/restore the signal mask.
    AssertSigmaskEquals(ss2);
#endif
  }
  sigprocmask(SIG_SETMASK, &original_set, NULL);
}

TEST(setjmp, sigsetjmp_0_signal_mask) {
  // sigsetjmp(0)/siglongjmp do not save/restore the signal mask.
  sigset_t ss1(SigSetOf(SIGUSR1, SIGRTMIN + 8));
  sigset_t ss2(SigSetOf(SIGUSR2, SIGRTMIN + 9));
  sigset_t original_set;
  sigprocmask(SIG_SETMASK, &ss1, &original_set);
  sigjmp_buf sjb;
  if (sigsetjmp(sjb, 0) == 0) {
    sigprocmask(SIG_SETMASK, &ss2, NULL);
    siglongjmp(sjb, 1);
    FAIL(); // Unreachable.
  } else {
    AssertSigmaskEquals(ss2);
  }
  sigprocmask(SIG_SETMASK, &original_set, NULL);
}

TEST(setjmp, sigsetjmp_1_signal_mask) {
  // sigsetjmp(1)/siglongjmp does save/restore the signal mask.
  sigset_t ss1(SigSetOf(SIGUSR1, SIGRTMIN + 8));
  sigset_t ss2(SigSetOf(SIGUSR2, SIGRTMIN + 9));
  sigset_t original_set;
  sigprocmask(SIG_SETMASK, &ss1, &original_set);
  sigjmp_buf sjb;
  if (sigsetjmp(sjb, 1) == 0) {
    sigprocmask(SIG_SETMASK, &ss2, NULL);
    siglongjmp(sjb, 1);
    FAIL(); // Unreachable.
  } else {
    AssertSigmaskEquals(ss1);
  }
  sigprocmask(SIG_SETMASK, &original_set, NULL);
}
