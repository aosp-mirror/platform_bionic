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

#if defined(__BIONIC_LP32_USE_LONG_DOUBLE)
#define COMPLEX_TEST complex_h_force_long_double
#else
#define COMPLEX_TEST complex_h
#endif

// This file is compiled against both glibc and bionic, and our complex.h
// depends on bionic-specific macros, so hack around that.
#include <sys/cdefs.h>
#if !defined(__INTRODUCED_IN)
#define __INTRODUCED_IN(x)
#define __INTRODUCED_IN_32(x)
#define __INTRODUCED_IN_64(x)
#define __RENAME_LDBL(a,b,c)
#endif

// libc++ actively gets in the way of including <complex.h> from C++, so we
// have to be naughty.
#include "../libc/include/complex.h"

// (libc++ also seems to have really bad implementations of its own that ignore
// the intricacies of floating point math.)
// http://llvm.org/bugs/show_bug.cgi?id=21504

#include <math.h> // For M_PI_2/M_PI_2l.

// Prettify gtest Complex printing.
#include <iostream>
namespace testing {
namespace internal {
inline void PrintTo(const double _Complex& c, std::ostream* os) {
  *os << "(" << creal(c) << "," << cimag(c) << "i)";
}
inline void PrintTo(const float _Complex& c, std::ostream* os) {
  *os << "(" << crealf(c) << "," << cimagf(c) << "i)";
}
inline void PrintTo(const long double _Complex& c, std::ostream* os) {
  *os << "(" << creall(c) << "," << cimagl(c) << "i)";
}
}
}

// Macro 'I' defined in complex.h conflicts with gtest.h.
#pragma push_macro("I")
#undef I
#include <gtest/gtest.h>
#pragma pop_macro("I")

TEST(COMPLEX_TEST, cabs) {
  ASSERT_EQ(0.0, cabs(0));
}

TEST(COMPLEX_TEST, cabsf) {
  ASSERT_EQ(0.0, cabsf(0));
}

TEST(COMPLEX_TEST, cabsl) {
  ASSERT_EQ(0.0, cabsl(0));
}

TEST(COMPLEX_TEST, cacos) {
  ASSERT_EQ(M_PI_2, cacos(0.0));
}

TEST(COMPLEX_TEST, cacosf) {
  ASSERT_EQ(static_cast<float>(M_PI_2), cacosf(0.0));
}

TEST(COMPLEX_TEST, cacosl) {
  ASSERT_EQ(M_PI_2l, cacosl(0.0));
}

TEST(COMPLEX_TEST, cacosh) {
  ASSERT_EQ(0.0, cacosh(1.0));
}

TEST(COMPLEX_TEST, cacoshl) {
  ASSERT_EQ(0.0, cacoshl(1.0));
}

TEST(COMPLEX_TEST, cacoshf) {
  ASSERT_EQ(0.0, cacoshf(1.0));
}

TEST(COMPLEX_TEST, carg) {
  ASSERT_EQ(0.0, carg(0));
}

TEST(COMPLEX_TEST, cargf) {
  ASSERT_EQ(0.0, cargf(0));
}

TEST(COMPLEX_TEST, cargl) {
  ASSERT_EQ(0.0, cargl(0));
}

TEST(COMPLEX_TEST, casin) {
  ASSERT_EQ(0.0, casin(0));
}

TEST(COMPLEX_TEST, casinf) {
  ASSERT_EQ(0.0, casinf(0));
}

TEST(COMPLEX_TEST, casinl) {
  ASSERT_EQ(0.0, casinl(0));
}

TEST(COMPLEX_TEST, casinh) {
  ASSERT_EQ(0.0, casinh(0));
}

TEST(COMPLEX_TEST, casinhf) {
  ASSERT_EQ(0.0, casinhf(0));
}

TEST(COMPLEX_TEST, casinhl) {
  ASSERT_EQ(0.0, casinhl(0));
}

TEST(COMPLEX_TEST, catan) {
  ASSERT_EQ(0.0, catan(0));
}

TEST(COMPLEX_TEST, catanf) {
  ASSERT_EQ(0.0, catanf(0));
}

TEST(COMPLEX_TEST, catanl) {
  ASSERT_EQ(0.0, catanl(0));
}

TEST(COMPLEX_TEST, catanh) {
  ASSERT_EQ(0.0, catanh(0));
}

TEST(COMPLEX_TEST, catanhf) {
  ASSERT_EQ(0.0, catanhf(0));
}

TEST(COMPLEX_TEST, catanhl) {
  ASSERT_EQ(0.0, catanhl(0));
}

TEST(COMPLEX_TEST, ccos) {
  ASSERT_EQ(1.0, ccos(0));
}

TEST(COMPLEX_TEST, ccosf) {
  ASSERT_EQ(1.0, ccosf(0));
}

TEST(COMPLEX_TEST, ccosl) {
  ASSERT_EQ(1.0, ccosl(0));
}

TEST(COMPLEX_TEST, ccosh) {
  ASSERT_EQ(1.0, ccosh(0));
}

TEST(COMPLEX_TEST, ccoshf) {
  ASSERT_EQ(1.0, ccoshf(0));
}

TEST(COMPLEX_TEST, ccoshl) {
  ASSERT_EQ(1.0, ccoshl(0));
}

TEST(COMPLEX_TEST, cexp) {
  ASSERT_EQ(1.0, cexp(0));
}

TEST(COMPLEX_TEST, cexpf) {
  ASSERT_EQ(1.0, cexpf(0));
}

TEST(COMPLEX_TEST, cexpl) {
  ASSERT_EQ(1.0, cexpl(0));
}

TEST(COMPLEX_TEST, cimag) {
  ASSERT_EQ(0.0, cimag(0));
}

TEST(COMPLEX_TEST, cimagf) {
  ASSERT_EQ(0.0f, cimagf(0));
}

TEST(COMPLEX_TEST, cimagl) {
  ASSERT_EQ(0.0, cimagl(0));
}

TEST(COMPLEX_TEST, clog) {
  ASSERT_EQ(0.0, clog(1.0));
}

TEST(COMPLEX_TEST, clogf) {
  ASSERT_EQ(0.0f, clogf(1.0f));
}

TEST(COMPLEX_TEST, clogl) {
  ASSERT_EQ(0.0L, clogl(1.0L));
}

TEST(COMPLEX_TEST, conj) {
  ASSERT_EQ(0.0, conj(0));
}

TEST(COMPLEX_TEST, conjf) {
  ASSERT_EQ(0.0f, conjf(0));
}

TEST(COMPLEX_TEST, conjl) {
  ASSERT_EQ(0.0, conjl(0));
}

TEST(COMPLEX_TEST, cpow) {
  ASSERT_EQ(8.0, cpow(2.0, 3.0));
}

TEST(COMPLEX_TEST, cpowf) {
  ASSERT_EQ(8.0f, cpowf(2.0f, 3.0f));
}

TEST(COMPLEX_TEST, cpowl) {
  ASSERT_EQ(8.0L, cpowl(2.0L, 3.0L));
}

TEST(COMPLEX_TEST, cproj) {
  ASSERT_EQ(0.0, cproj(0));
}

TEST(COMPLEX_TEST, cprojf) {
  ASSERT_EQ(0.0f, cprojf(0));
}

TEST(COMPLEX_TEST, cprojl) {
  ASSERT_EQ(0.0, cprojl(0));
}

TEST(COMPLEX_TEST, creal) {
  ASSERT_EQ(2.0, creal(2.0 + 3.0I));
}

TEST(COMPLEX_TEST, crealf) {
  ASSERT_EQ(2.0f, crealf(2.0f + 3.0fI));
}

TEST(COMPLEX_TEST, creall) {
  ASSERT_EQ(2.0, creall(2.0L + 3.0LI));
}

TEST(COMPLEX_TEST, csin) {
  ASSERT_EQ(0.0, csin(0));
}

TEST(COMPLEX_TEST, csinf) {
  ASSERT_EQ(0.0, csinf(0));
}

TEST(COMPLEX_TEST, csinl) {
  ASSERT_EQ(0.0, csinl(0));
}

TEST(COMPLEX_TEST, csinh) {
  ASSERT_EQ(0.0, csinh(0));
}

TEST(COMPLEX_TEST, csinhf) {
  ASSERT_EQ(0.0, csinhf(0));
}

TEST(COMPLEX_TEST, csinhl) {
  ASSERT_EQ(0.0, csinhl(0));
}

TEST(COMPLEX_TEST, csqrt) {
  ASSERT_EQ(0.0, csqrt(0));
}

TEST(COMPLEX_TEST, csqrtf) {
  ASSERT_EQ(0.0f, csqrtf(0));
}

TEST(COMPLEX_TEST, csqrtl) {
  ASSERT_EQ(0.0, csqrtl(0));
}

TEST(COMPLEX_TEST, ctan) {
  ASSERT_EQ(0.0, ctan(0));
}

TEST(COMPLEX_TEST, ctanf) {
  ASSERT_EQ(0.0, ctanf(0));
}

TEST(COMPLEX_TEST, ctanl) {
  ASSERT_EQ(0.0, ctanl(0));
}

TEST(COMPLEX_TEST, ctanh) {
  ASSERT_EQ(0.0, ctanh(0));

  double complex z;

  // If z is NaN+0i, the result is NaN+0i.
  z = ctanh(nan("") + 0i);
  ASSERT_TRUE(isnan(creal(z)));
  ASSERT_EQ(0.0, cimag(z));

  // If z is NaN+yi, the result is NaN+NaNi.
  z = ctanh(nan("") + 2.0i);
  ASSERT_TRUE(isnan(creal(z)));
  ASSERT_TRUE(isnan(cimag(z)));

  // If z is NaN+NaNi, the result is NaN+NaNi.
  z = ctanh(nan("") + nan("") * I);
  ASSERT_TRUE(isnan(creal(z)));
  ASSERT_TRUE(isnan(cimag(z)));
}

TEST(COMPLEX_TEST, ctanhf) {
  ASSERT_EQ(0.0f, ctanhf(0.0f));

  float complex z;

  // If z is NaN+0i, the result is NaN+0i.
  z = ctanhf(nanf("") + 0.0fi);
  ASSERT_TRUE(isnan(crealf(z)));
  ASSERT_EQ(0.0f, cimagf(z));

  // If z is NaN+yi, the result is NaN+NaNi.
  z = ctanhf(nanf("") + 2.0fi);
  ASSERT_TRUE(isnan(crealf(z)));
  ASSERT_TRUE(isnan(cimagf(z)));

  // If z is NaN+NaNi, the result is NaN+NaNi.
  z = ctanhf(nanf("") + nanf("") * I);
  ASSERT_TRUE(isnan(crealf(z)));
  ASSERT_TRUE(isnan(cimagf(z)));
}

TEST(COMPLEX_TEST, ctanhl) {
  ASSERT_EQ(0.0L, ctanhl(0.0L));

  long double complex z;

  // If z is NaN+0i, the result is NaN+0i.
  z = ctanhl(nanl("") + 0.0Li);
  ASSERT_TRUE(isnan(creall(z)));
  // TODO: this case is currently broken in the netbsd ctanhl.
  // ASSERT_EQ(0.0L, cimagl(z));

  // If z is NaN+yi, the result is NaN+NaNi.
  z = ctanhl(nanl("") + 2.0Li);
  ASSERT_TRUE(isnan(creall(z)));
  ASSERT_TRUE(isnan(cimagl(z)));

  // If z is NaN+NaNi, the result is NaN+NaNi.
  z = ctanhl(nanl("") + nanl("") * I);
  ASSERT_TRUE(isnan(creall(z)));
  ASSERT_TRUE(isnan(cimagl(z)));
}
