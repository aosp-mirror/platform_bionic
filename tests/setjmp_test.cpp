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
#include <sys/syscall.h>
#include <unistd.h>

#include <android-base/silent_death_test.h>
#include <android-base/test_utils.h>

#include "SignalUtils.h"

using setjmp_DeathTest = SilentDeathTest;

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
#if defined(__BIONIC__)
    // TIMER_SIGNAL.
    sigaddset64(&ss, __SIGRTMIN);
#endif
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

#if defined(__arm__) || defined(__aarch64__)
// arm and arm64 have the same callee save fp registers (8-15),
// but use different instructions for accessing them.
#if defined(__arm__)
#define SET_FREG(n, v) asm volatile("vmov.f64 d"#n ", #"#v : : : "d"#n)
#define GET_FREG(n) ({ double _r; asm volatile("fcpyd %P0, d"#n : "=w"(_r) : :); _r;})
#define CLEAR_FREG(n) asm volatile("vmov.i64 d"#n ", #0x0" : : : "d"#n)
#elif defined(__aarch64__)
#define SET_FREG(n, v) asm volatile("fmov d"#n ", "#v : : : "d"#n)
#define GET_FREG(n) ({ double _r; asm volatile("fmov %0, d"#n : "=r"(_r) : :); _r; })
#define CLEAR_FREG(n) asm volatile("fmov d"#n ", xzr" : : : "d"#n)
#endif
#define SET_FREGS \
  SET_FREG(8, 8.0); SET_FREG(9, 9.0); SET_FREG(10, 10.0); SET_FREG(11, 11.0); \
  SET_FREG(12, 12.0); SET_FREG(13, 13.0); SET_FREG(14, 14.0); SET_FREG(15, 15.0)
#define CLEAR_FREGS \
  CLEAR_FREG(8); CLEAR_FREG(9); CLEAR_FREG(10); CLEAR_FREG(11); \
  CLEAR_FREG(12); CLEAR_FREG(13); CLEAR_FREG(14); CLEAR_FREG(15)
#define CHECK_FREGS \
  EXPECT_EQ(8.0, GET_FREG(8)); EXPECT_EQ(9.0, GET_FREG(9)); \
  EXPECT_EQ(10.0, GET_FREG(10)); EXPECT_EQ(11.0, GET_FREG(11)); \
  EXPECT_EQ(12.0, GET_FREG(12)); EXPECT_EQ(13.0, GET_FREG(13)); \
  EXPECT_EQ(14.0, GET_FREG(14)); EXPECT_EQ(15.0, GET_FREG(15))

#elif defined(__riscv)
// riscv64 has callee save registers fs0-fs11.
// TODO: use Zfa to get 1.0 rather than the one_p trick.
#define SET_FREGS \
  double one = 1, *one_p = &one; \
  asm volatile("fmv.d.x fs0, zero ; fld fs1, (%0) ; \
                fadd.d fs2, fs1, fs1 ; fadd.d fs3, fs2, fs1 ; \
                fadd.d fs4, fs3, fs1 ; fadd.d fs5, fs4, fs1 ; \
                fadd.d fs6, fs5, fs1 ; fadd.d fs7, fs6, fs1 ; \
                fadd.d fs8, fs7, fs1 ; fadd.d fs9, fs8, fs1 ; \
                fadd.d fs10, fs9, fs1 ; fadd.d fs11, fs10, fs1" \
               : \
               : "r"(one_p) \
               : "fs0", "fs1", "fs2", "fs3", "fs4", "fs5", \
                  "fs6", "fs7", "fs8", "fs9", "fs10", "fs11")
#define CLEAR_FREGS \
  asm volatile("fmv.d.x fs0, zero ; fmv.d.x fs1, zero ; \
                fmv.d.x fs2, zero ; fmv.d.x fs3, zero ; \
                fmv.d.x fs4, zero ; fmv.d.x fs5, zero ; \
                fmv.d.x fs6, zero ; fmv.d.x fs7, zero ; \
                fmv.d.x fs8, zero ; fmv.d.x fs9, zero ; \
                fmv.d.x fs10, zero ; fmv.d.x fs11, zero" \
               : : : "fs0", "fs1", "fs2", "fs3", "fs4", "fs5", \
                     "fs6", "fs7", "fs8", "fs9", "fs10", "fs11")
#define GET_FREG(n) ({ double _r; asm volatile("fmv.d %0, fs"#n : "=f"(_r) : :); _r; })
#define CHECK_FREGS \
  EXPECT_EQ(0.0, GET_FREG(0)); EXPECT_EQ(1.0, GET_FREG(1)); \
  EXPECT_EQ(2.0, GET_FREG(2)); EXPECT_EQ(3.0, GET_FREG(3)); \
  EXPECT_EQ(4.0, GET_FREG(4)); EXPECT_EQ(5.0, GET_FREG(5)); \
  EXPECT_EQ(6.0, GET_FREG(6)); EXPECT_EQ(7.0, GET_FREG(7)); \
  EXPECT_EQ(8.0, GET_FREG(8)); EXPECT_EQ(9.0, GET_FREG(9)); \
  EXPECT_EQ(10.0, GET_FREG(10)); EXPECT_EQ(11.0, GET_FREG(11))

#else
// x86 and x86-64 don't save/restore fp registers.
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
#define JB_SIGFLAG_OFFSET 0
#elif defined(__aarch64__)
#define JB_SIGFLAG_OFFSET 0
#elif defined(__i386__)
#define JB_SIGFLAG_OFFSET 8
#elif defined(__riscv)
#define JB_SIGFLAG_OFFSET 0
#elif defined(__x86_64)
#define JB_SIGFLAG_OFFSET 8
#endif

TEST_F(setjmp_DeathTest, setjmp_cookie) {
  jmp_buf jb;
  int value = setjmp(jb);
  ASSERT_EQ(0, value);

  long* sigflag = reinterpret_cast<long*>(jb) + JB_SIGFLAG_OFFSET;

  // Make sure there's actually a cookie.
  EXPECT_NE(0, *sigflag & ~1);

  // Wipe it out
  *sigflag &= 1;
  EXPECT_DEATH(longjmp(jb, 0), "");
}

TEST_F(setjmp_DeathTest, setjmp_cookie_checksum) {
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

TEST(setjmp, bug_152210274) {
  // Ensure that we never have a mangled value in the stack pointer.
#if defined(__BIONIC__)
  struct sigaction sa = {.sa_flags = SA_SIGINFO, .sa_sigaction = [](int, siginfo_t*, void*) {}};
  ASSERT_EQ(0, sigaction(SIGPROF, &sa, 0));

  constexpr size_t kNumThreads = 20;

  // Start a bunch of threads calling setjmp/longjmp.
  auto jumper = [](void* arg) -> void* {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPROF);
    pthread_sigmask(SIG_UNBLOCK, &set, nullptr);

    jmp_buf buf;
    for (size_t count = 0; count < 100000; ++count) {
      if (setjmp(buf) != 0) {
        perror("setjmp");
        abort();
      }
      // This will never be true, but the compiler doesn't know that, so the
      // setjmp won't be removed by DCE. With HWASan/MTE this also acts as a
      // kind of enforcement that the threads are done before leaving the test.
      if (*static_cast<size_t*>(arg) != 123) longjmp(buf, 1);
    }
    return nullptr;
  };
  pthread_t threads[kNumThreads];
  pid_t tids[kNumThreads] = {};
  size_t var = 123;
  for (size_t i = 0; i < kNumThreads; ++i) {
    ASSERT_EQ(0, pthread_create(&threads[i], nullptr, jumper, &var));
    tids[i] = pthread_gettid_np(threads[i]);
  }

  // Start the interrupter thread.
  auto interrupter = [](void* arg) -> void* {
    pid_t* tids = static_cast<pid_t*>(arg);
    for (size_t count = 0; count < 1000; ++count) {
      for (size_t i = 0; i < kNumThreads; i++) {
        if (tgkill(getpid(), tids[i], SIGPROF) == -1 && errno != ESRCH) {
          perror("tgkill failed");
          abort();
        }
      }
      usleep(100);
    }
    return nullptr;
  };
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, interrupter, tids));
  pthread_join(t, nullptr);
  for (size_t i = 0; i < kNumThreads; i++) {
    pthread_join(threads[i], nullptr);
  }
#else
  GTEST_SKIP() << "tests uses functions not in glibc";
#endif
}
