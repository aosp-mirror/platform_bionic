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

#include "utils.h"

#include <fenv.h>
#include <stdint.h>

static void TestRounding(float expectation1, float expectation2) {
  // volatile to prevent compiler optimizations.
  volatile float f = 1.968750f;
  volatile float m = 0x1.0p23f;
  volatile float x = f + m;
  ASSERT_FLOAT_EQ(expectation1, x);
  x = x - m;
  ASSERT_EQ(expectation2, x);
}

static void DivideByZero() {
  // volatile to prevent compiler optimizations.
  volatile float zero = 0.0f;
  volatile float result __attribute__((unused)) = 123.0f / zero;
}

TEST(fenv, fesetround_fegetround_FE_TONEAREST) {
  fesetround(FE_TONEAREST);
  ASSERT_EQ(FE_TONEAREST, fegetround());
  TestRounding(8388610.0f, 2.0f);
}

TEST(fenv, fesetround_fegetround_FE_TOWARDZERO) {
  fesetround(FE_TOWARDZERO);
  ASSERT_EQ(FE_TOWARDZERO, fegetround());
  TestRounding(8388609.0f, 1.0f);
}

TEST(fenv, fesetround_fegetround_FE_UPWARD) {
  fesetround(FE_UPWARD);
  ASSERT_EQ(FE_UPWARD, fegetround());
  TestRounding(8388610.0f, 2.0f);
}

TEST(fenv, fesetround_fegetround_FE_DOWNWARD) {
  fesetround(FE_DOWNWARD);
  ASSERT_EQ(FE_DOWNWARD, fegetround());
  TestRounding(8388609.0f, 1.0f);
}

TEST(fenv, feclearexcept_fetestexcept) {
  // Clearing clears.
  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));

  // Dividing by zero sets FE_DIVBYZERO.
  DivideByZero();
  int raised = fetestexcept(FE_DIVBYZERO | FE_OVERFLOW);
  ASSERT_TRUE((raised & FE_OVERFLOW) == 0);
  ASSERT_TRUE((raised & FE_DIVBYZERO) != 0);

  // Clearing an unset bit is a no-op.
  feclearexcept(FE_OVERFLOW);
  ASSERT_TRUE((raised & FE_OVERFLOW) == 0);
  ASSERT_TRUE((raised & FE_DIVBYZERO) != 0);

  // Clearing a set bit works.
  feclearexcept(FE_DIVBYZERO);
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));
}

TEST(fenv, FE_DFL_ENV_macro) {
  ASSERT_EQ(0, fesetenv(FE_DFL_ENV));
}

TEST(fenv, feraiseexcept) {
  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));

  ASSERT_EQ(0, feraiseexcept(FE_DIVBYZERO | FE_OVERFLOW));
  ASSERT_EQ(FE_DIVBYZERO | FE_OVERFLOW, fetestexcept(FE_ALL_EXCEPT));
}

TEST(fenv, fegetenv_fesetenv) {
  // Set FE_OVERFLOW only.
  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));
  ASSERT_EQ(0, feraiseexcept(FE_OVERFLOW));

  // fegetenv (unlike feholdexcept) leaves the current state untouched...
  fenv_t state;
  ASSERT_EQ(0, fegetenv(&state));
  ASSERT_EQ(FE_OVERFLOW, fetestexcept(FE_ALL_EXCEPT));

  // Dividing by zero sets the appropriate flag...
  DivideByZero();
  ASSERT_EQ(FE_DIVBYZERO | FE_OVERFLOW, fetestexcept(FE_ALL_EXCEPT));

  // And fesetenv (unlike feupdateenv) clobbers that to return to where
  // we started.
  ASSERT_EQ(0, fesetenv(&state));
  ASSERT_EQ(FE_OVERFLOW, fetestexcept(FE_ALL_EXCEPT));
}

TEST(fenv, feholdexcept_feupdateenv) {
  // Set FE_OVERFLOW only.
  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));
  ASSERT_EQ(0, feraiseexcept(FE_OVERFLOW));

  // feholdexcept (unlike fegetenv) clears everything...
  fenv_t state;
  ASSERT_EQ(0, feholdexcept(&state));
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));

  // Dividing by zero sets the appropriate flag...
  DivideByZero();
  ASSERT_EQ(FE_DIVBYZERO, fetestexcept(FE_ALL_EXCEPT));

  // And feupdateenv (unlike fesetenv) merges what we started with
  // (FE_OVERFLOW) with what we now have (FE_DIVBYZERO).
  ASSERT_EQ(0, feupdateenv(&state));
  ASSERT_EQ(FE_DIVBYZERO | FE_OVERFLOW, fetestexcept(FE_ALL_EXCEPT));
}

TEST(fenv, fegetexceptflag_fesetexceptflag) {
  // Set three flags.
  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(0, feraiseexcept(FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW));
  ASSERT_EQ(FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW, fetestexcept(FE_ALL_EXCEPT));

  fexcept_t all; // FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW
  fexcept_t two; // FE_OVERFLOW | FE_UNDERFLOW
  ASSERT_EQ(0, fegetexceptflag(&all, FE_ALL_EXCEPT));
  ASSERT_EQ(0, fegetexceptflag(&two, FE_OVERFLOW | FE_UNDERFLOW));

  // Check we can restore all.
  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(0, fesetexceptflag(&all, FE_ALL_EXCEPT));
  ASSERT_EQ(FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW, fetestexcept(FE_ALL_EXCEPT));

  // Check that `two` only stored a subset.
  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(0, fesetexceptflag(&two, FE_ALL_EXCEPT));
  ASSERT_EQ(FE_OVERFLOW | FE_UNDERFLOW, fetestexcept(FE_ALL_EXCEPT));

  // Check that we can restore a single flag.
  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(0, fesetexceptflag(&all, FE_DIVBYZERO));
  ASSERT_EQ(FE_DIVBYZERO, fetestexcept(FE_ALL_EXCEPT));

  // Check that we can restore a subset of flags.
  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(0, fesetexceptflag(&all, FE_OVERFLOW | FE_UNDERFLOW));
  ASSERT_EQ(FE_OVERFLOW | FE_UNDERFLOW, fetestexcept(FE_ALL_EXCEPT));
}

TEST(fenv, fedisableexcept_fegetexcept) {
  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));

  // No SIGFPE please...
  ASSERT_EQ(0, fedisableexcept(FE_ALL_EXCEPT));
  ASSERT_EQ(0, fegetexcept());
  ASSERT_EQ(0, feraiseexcept(FE_INVALID));
  ASSERT_EQ(FE_INVALID, fetestexcept(FE_ALL_EXCEPT));
}

TEST(fenv, feenableexcept_fegetexcept) {
#if defined(__aarch64__) || defined(__arm__)
  // ARM doesn't support this. They used to if you go back far enough, but it was removed in
  // the Cortex-A8 between r3p1 and r3p2.
  ASSERT_EQ(-1, feenableexcept(FE_INVALID));
  ASSERT_EQ(0, fegetexcept());
  ASSERT_EQ(-1, feenableexcept(FE_DIVBYZERO));
  ASSERT_EQ(0, fegetexcept());
  ASSERT_EQ(-1, feenableexcept(FE_OVERFLOW));
  ASSERT_EQ(0, fegetexcept());
  ASSERT_EQ(-1, feenableexcept(FE_UNDERFLOW));
  ASSERT_EQ(0, fegetexcept());
  ASSERT_EQ(-1, feenableexcept(FE_INEXACT));
  ASSERT_EQ(0, fegetexcept());
  ASSERT_EQ(-1, feenableexcept(FE_DENORMAL));
  ASSERT_EQ(0, fegetexcept());
#else
  // We can't recover from SIGFPE, so sacrifice a child...
  pid_t pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    feclearexcept(FE_ALL_EXCEPT);
    ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));
    ASSERT_EQ(0, feenableexcept(FE_INVALID));
    ASSERT_EQ(FE_INVALID, fegetexcept());
    ASSERT_EQ(0, feraiseexcept(FE_INVALID));
    _exit(123);
  }

  AssertChildExited(pid, -SIGFPE);
#endif
}
