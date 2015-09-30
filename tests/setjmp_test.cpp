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

// Two distinct signal sets, pipu
struct SigSets {
  SigSets() : one(MakeSigSet(0)), two(MakeSigSet(1)) {
  }

  static sigset_t MakeSigSet(int offset) {
    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGUSR1 + offset);
#if defined(__LP64__)
    // For arm and x86, sigset_t was too small for the RT signals.
    // For mips, sigset_t was large enough but jmp_buf wasn't.
    sigaddset(&ss, SIGRTMIN + offset);
#endif
    return ss;
  }

  sigset_t one;
  sigset_t two;
  sigset_t original;
};

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
  SigSets ss;
  sigprocmask(SIG_SETMASK, &ss.one, &ss.original);
  jmp_buf jb;
  if (_setjmp(jb) == 0) {
    sigprocmask(SIG_SETMASK, &ss.two, NULL);
    _longjmp(jb, 1);
    FAIL(); // Unreachable.
  } else {
    AssertSigmaskEquals(ss.two);
  }
  sigprocmask(SIG_SETMASK, &ss.original, NULL);
}

TEST(setjmp, setjmp_signal_mask) {
  // setjmp/longjmp do save/restore the signal mask on bionic, but not on glibc.
  // This is a BSD versus System V historical accident. POSIX leaves the
  // behavior unspecified, so any code that cares needs to use sigsetjmp.
  SigSets ss;
  sigprocmask(SIG_SETMASK, &ss.one, &ss.original);
  jmp_buf jb;
  if (setjmp(jb) == 0) {
    sigprocmask(SIG_SETMASK, &ss.two, NULL);
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
  sigprocmask(SIG_SETMASK, &ss.original, NULL);
}

TEST(setjmp, sigsetjmp_0_signal_mask) {
  // sigsetjmp(0)/siglongjmp do not save/restore the signal mask.
  SigSets ss;
  sigprocmask(SIG_SETMASK, &ss.one, &ss.original);
  sigjmp_buf sjb;
  if (sigsetjmp(sjb, 0) == 0) {
    sigprocmask(SIG_SETMASK, &ss.two, NULL);
    siglongjmp(sjb, 1);
    FAIL(); // Unreachable.
  } else {
    AssertSigmaskEquals(ss.two);
  }
  sigprocmask(SIG_SETMASK, &ss.original, NULL);
}

TEST(setjmp, sigsetjmp_1_signal_mask) {
  // sigsetjmp(1)/siglongjmp does save/restore the signal mask.
  SigSets ss;
  sigprocmask(SIG_SETMASK, &ss.one, &ss.original);
  sigjmp_buf sjb;
  if (sigsetjmp(sjb, 1) == 0) {
    sigprocmask(SIG_SETMASK, &ss.two, NULL);
    siglongjmp(sjb, 1);
    FAIL(); // Unreachable.
  } else {
    AssertSigmaskEquals(ss.one);
  }
  sigprocmask(SIG_SETMASK, &ss.original, NULL);
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
#define __JB_SIGFLAG 7
#elif defined(__x86_64)
#define __JB_SIGFLAG 8
#endif

TEST(setjmp, setjmp_cookie) {
#if !defined(__mips__)
  jmp_buf jb;
  int value = setjmp(jb);
  ASSERT_EQ(0, value);

  long* sigflag = reinterpret_cast<long*>(jb) + __JB_SIGFLAG;

  // Make sure there's actually a cookie.
  EXPECT_NE(0, *sigflag & ~1);

  // Wipe it out
  *sigflag &= 1;
  EXPECT_DEATH(longjmp(jb, 0), "");
#endif
}
