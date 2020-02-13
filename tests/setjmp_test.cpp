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

#include "SignalUtils.h"

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

// Two distinct signal sets.
struct SigSets {
  SigSets() : one(MakeSigSet(0)), two(MakeSigSet(1)) {
  }

  static sigset64_t MakeSigSet(int offset) {
    sigset64_t ss;
    sigemptyset64(&ss);
    sigaddset64(&ss, SIGUSR1 + offset);
    // TIMER_SIGNAL.
    sigaddset64(&ss, __SIGRTMIN);
    sigaddset64(&ss, SIGRTMIN + offset);
    return ss;
  }

  sigset64_t one;
  sigset64_t two;
};

void AssertSigmaskEquals(const sigset64_t& expected) {
  sigset64_t actual;
  sigprocmask64(SIG_SETMASK, nullptr, &actual);
  size_t end = sizeof(expected) * 8;
  for (size_t i = 1; i <= end; ++i) {
    EXPECT_EQ(sigismember64(&expected, i), sigismember64(&actual, i)) << i;
  }
}

TEST(setjmp, _setjmp_signal_mask) {
  SignalMaskRestorer smr;

  // _setjmp/_longjmp do not save/restore the signal mask.
  SigSets ss;
  sigprocmask64(SIG_SETMASK, &ss.one, nullptr);
  jmp_buf jb;
  if (_setjmp(jb) == 0) {
    sigprocmask64(SIG_SETMASK, &ss.two, nullptr);
    _longjmp(jb, 1);
    FAIL(); // Unreachable.
  } else {
    AssertSigmaskEquals(ss.two);
  }
}

TEST(setjmp, setjmp_signal_mask) {
  SignalMaskRestorer smr;

  // setjmp/longjmp do save/restore the signal mask on bionic, but not on glibc.
  // This is a BSD versus System V historical accident. POSIX leaves the
  // behavior unspecified, so any code that cares needs to use sigsetjmp.
  SigSets ss;
  sigprocmask64(SIG_SETMASK, &ss.one, nullptr);
  jmp_buf jb;
  if (setjmp(jb) == 0) {
    sigprocmask64(SIG_SETMASK, &ss.two, nullptr);
    longjmp(jb, 1);
    FAIL(); // Unreachable.
  } else {
#if defined(__BIONIC__)
    // bionic behaves like BSD and does save/restore the signal mask.
    AssertSigmaskEquals(ss.one);
#else
    // glibc behaves like System V and doesn't save/restore the signal mask.
    AssertSigmaskEquals(ss.two);
#endif
  }
}

TEST(setjmp, sigsetjmp_0_signal_mask) {
  SignalMaskRestorer smr;

  // sigsetjmp(0)/siglongjmp do not save/restore the signal mask.
  SigSets ss;
  sigprocmask64(SIG_SETMASK, &ss.one, nullptr);
  sigjmp_buf sjb;
  if (sigsetjmp(sjb, 0) == 0) {
    sigprocmask64(SIG_SETMASK, &ss.two, nullptr);
    siglongjmp(sjb, 1);
    FAIL(); // Unreachable.
  } else {
    AssertSigmaskEquals(ss.two);
  }
}

TEST(setjmp, sigsetjmp_1_signal_mask) {
  SignalMaskRestorer smr;

  // sigsetjmp(1)/siglongjmp does save/restore the signal mask.
  SigSets ss;
  sigprocmask64(SIG_SETMASK, &ss.one, nullptr);
  sigjmp_buf sjb;
  if (sigsetjmp(sjb, 1) == 0) {
    sigprocmask64(SIG_SETMASK, &ss.two, nullptr);
    siglongjmp(sjb, 1);
    FAIL(); // Unreachable.
  } else {
    AssertSigmaskEquals(ss.one);
  }
}

#if defined(__aarch64__)
#define SET_FREG(n, v) asm volatile("fmov d"#n ", "#v : : : "d"#n)
#define CLEAR_FREG(n) asm volatile("fmov d"#n ", xzr" : : : "d"#n)
#define SET_FREGS \
  SET_FREG(8, 8.0); SET_FREG(9, 9.0); SET_FREG(10, 10.0); SET_FREG(11, 11.0); \
  SET_FREG(12, 12.0); SET_FREG(13, 13.0); SET_FREG(14, 14.0); SET_FREG(15, 15.0);
#define CLEAR_FREGS \
  CLEAR_FREG(8); CLEAR_FREG(9); CLEAR_FREG(10); CLEAR_FREG(11); \
  CLEAR_FREG(12); CLEAR_FREG(13); CLEAR_FREG(14); CLEAR_FREG(15);
#define GET_FREG(n) ({ double _r; asm volatile("fmov %0, d"#n : "=r"(_r) : :); _r; })
#define CHECK_FREGS \
    EXPECT_EQ(8.0, GET_FREG(8)); EXPECT_EQ(9.0, GET_FREG(9)); \
    EXPECT_EQ(10.0, GET_FREG(10)); EXPECT_EQ(11.0, GET_FREG(11)); \
    EXPECT_EQ(12.0, GET_FREG(12)); EXPECT_EQ(13.0, GET_FREG(13)); \
    EXPECT_EQ(14.0, GET_FREG(14)); EXPECT_EQ(15.0, GET_FREG(15));
#elif defined(__arm__)
#define SET_FREG(n, v) \
  ({ const double _v{v}; asm volatile("fcpyd d"#n ", %P0" : : "w"(_v) : "d"#n); })
#define SET_FREGS \
  SET_FREG(8, 8); SET_FREG(9, 9); SET_FREG(10, 10); SET_FREG(11, 11); \
  SET_FREG(12, 12); SET_FREG(13, 13); SET_FREG(14, 14); SET_FREG(15, 15);
#define CLEAR_FREGS \
  SET_FREG(8, 0); SET_FREG(9, 0); SET_FREG(10, 0); SET_FREG(11, 0); \
  SET_FREG(12, 0); SET_FREG(13, 0); SET_FREG(14, 0); SET_FREG(15, 0);
#define GET_FREG(n) ({ double _r; asm volatile("fcpyd %P0, d"#n : "=w"(_r) : :); _r;})
#define CHECK_FREGS \
    EXPECT_EQ(8.0, GET_FREG(8)); EXPECT_EQ(9.0, GET_FREG(9)); \
    EXPECT_EQ(10.0, GET_FREG(10)); EXPECT_EQ(11.0, GET_FREG(11)); \
    EXPECT_EQ(12.0, GET_FREG(12)); EXPECT_EQ(13.0, GET_FREG(13)); \
    EXPECT_EQ(14.0, GET_FREG(14)); EXPECT_EQ(15.0, GET_FREG(15));
#else
/* The other architectures don't save/restore fp registers. */
#define SET_FREGS
#define CLEAR_FREGS
#define CHECK_FREGS
#endif

TEST(setjmp, setjmp_fp_registers) {
  int value;
  jmp_buf jb;
  SET_FREGS;
  if ((value = setjmp(jb)) == 0) {
    CLEAR_FREGS;
    longjmp(jb, 123);
    FAIL(); // Unreachable.
  } else {
    ASSERT_EQ(123, value);
    CHECK_FREGS;
  }
}

#if defined(__arm__)
#define __JB_SIGFLAG 0
#elif defined(__aarch64__)
#define __JB_SIGFLAG 0
#elif defined(__i386__)
#define __JB_SIGFLAG 8
#elif defined(__x86_64)
#define __JB_SIGFLAG 8
#endif

TEST(setjmp, setjmp_cookie) {
  jmp_buf jb;
  int value = setjmp(jb);
  ASSERT_EQ(0, value);

  long* sigflag = reinterpret_cast<long*>(jb) + __JB_SIGFLAG;

  // Make sure there's actually a cookie.
  EXPECT_NE(0, *sigflag & ~1);

  // Wipe it out
  *sigflag &= 1;
  EXPECT_DEATH(longjmp(jb, 0), "");
}

TEST(setjmp, setjmp_cookie_checksum) {
  jmp_buf jb;
  int value = setjmp(jb);

  if (value == 0) {
    // Flip a bit.
    reinterpret_cast<long*>(jb)[1] ^= 1;

    EXPECT_DEATH(longjmp(jb, 1), "checksum mismatch");
  } else {
    fprintf(stderr, "setjmp_cookie_checksum: longjmp succeeded?");
  }
}

__attribute__((noinline)) void call_longjmp(jmp_buf buf) {
  longjmp(buf, 123);
}

TEST(setjmp, setjmp_stack) {
  jmp_buf buf;
  int value = setjmp(buf);
  if (value == 0) call_longjmp(buf);
  EXPECT_EQ(123, value);
}
