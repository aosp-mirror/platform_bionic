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

// libc++ actively gets in the way of including <complex.h> from C++, so we
// have to be naughty.
#include <../libc/include/complex.h>

// (libc++ also seems to have really bad implementations of its own that ignore
// the intricacies of floating point math.)
// http://llvm.org/bugs/show_bug.cgi?id=21504

#include <math.h> // For M_PI_2/M_PI_2l.

// Note that gtest doesn't support complex numbers, so the output from
// assertion failures is misleading/useless (at best you'll only see the real
// part).
// TODO: find out why gtest doesn't use these; until then they're only useful
// for manual printf^Woperator<< debugging.
#include <iostream>
std::ostream& operator<<(std::ostream& os, const double _Complex c) {
  os << "(" << creal(c) << "," << cimag(c) << "i)";
 return os;
}
std::ostream& operator<<(std::ostream& os, const float _Complex c) {
  os << "(" << crealf(c) << "," << cimagf(c) << "i)";
  return os;
}
std::ostream& operator<<(std::ostream& os, const long double _Complex c) {
  os << "(" << creall(c) << "," << cimagl(c) << "i)";
  return os;
}

TEST(complex, cabs) {
  ASSERT_EQ(0.0, cabs(0));
}

TEST(complex, cabsf) {
  ASSERT_EQ(0.0, cabsf(0));
}

TEST(complex, cabsl) {
  ASSERT_EQ(0.0, cabsl(0));
}

TEST(complex, cacos) {
  ASSERT_EQ(M_PI_2, cacos(0.0));
}

TEST(complex, cacosf) {
  ASSERT_EQ(static_cast<float>(M_PI_2), cacosf(0.0));
}

TEST(complex, cacosl) {
  ASSERT_EQ(M_PI_2l, cacosl(0.0));
}

TEST(complex, cacosh) {
  ASSERT_EQ(0.0, cacosh(1.0));
}

TEST(complex, cacoshl) {
  ASSERT_EQ(0.0, cacoshl(1.0));
}

TEST(complex, cacoshf) {
  ASSERT_EQ(0.0, cacoshf(1.0));
}

TEST(complex, carg) {
  ASSERT_EQ(0.0, carg(0));
}

TEST(complex, cargf) {
  ASSERT_EQ(0.0, cargf(0));
}

TEST(complex, cargl) {
  ASSERT_EQ(0.0, cargl(0));
}

TEST(complex, casin) {
  ASSERT_EQ(0.0, casin(0));
}

TEST(complex, casinf) {
  ASSERT_EQ(0.0, casinf(0));
}

TEST(complex, casinl) {
  ASSERT_EQ(0.0, casinl(0));
}

TEST(complex, casinh) {
  ASSERT_EQ(0.0, casinh(0));
}

TEST(complex, casinhf) {
  ASSERT_EQ(0.0, casinhf(0));
}

TEST(complex, casinhl) {
  ASSERT_EQ(0.0, casinhl(0));
}

TEST(complex, catan) {
  ASSERT_EQ(0.0, catan(0));
}

TEST(complex, catanf) {
  ASSERT_EQ(0.0, catanf(0));
}

TEST(complex, catanl) {
  ASSERT_EQ(0.0, catanl(0));
}

TEST(complex, catanh) {
  ASSERT_EQ(0.0, catanh(0));
}

TEST(complex, catanhf) {
  ASSERT_EQ(0.0, catanhf(0));
}

TEST(complex, catanhl) {
  ASSERT_EQ(0.0, catanhl(0));
}

TEST(complex, ccos) {
  ASSERT_EQ(1.0, ccos(0));
}

TEST(complex, ccosf) {
  ASSERT_EQ(1.0, ccosf(0));
}

TEST(complex, ccosl) {
  ASSERT_EQ(1.0, ccosl(0));
}

TEST(complex, ccosh) {
  ASSERT_EQ(1.0, ccosh(0));
}

TEST(complex, ccoshf) {
  ASSERT_EQ(1.0, ccoshf(0));
}

TEST(complex, ccoshl) {
  ASSERT_EQ(1.0, ccoshl(0));
}

TEST(complex, cexp) {
  ASSERT_EQ(1.0, cexp(0));
}

TEST(complex, cexpf) {
  ASSERT_EQ(1.0, cexpf(0));
}

TEST(complex, cexpl) {
  ASSERT_EQ(1.0, cexpl(0));
}

TEST(complex, cimag) {
  ASSERT_EQ(0.0, cimag(0));
}

TEST(complex, cimagf) {
  ASSERT_EQ(0.0f, cimagf(0));
}

TEST(complex, cimagl) {
  ASSERT_EQ(0.0, cimagl(0));
}

TEST(complex, clog) {
  ASSERT_EQ(0.0, clog(1.0));
}

TEST(complex, clogf) {
  ASSERT_EQ(0.0f, clogf(1.0f));
}

TEST(complex, clogl) {
  ASSERT_EQ(0.0L, clogl(1.0L));
}

TEST(complex, conj) {
  ASSERT_EQ(0.0, conj(0));
}

TEST(complex, conjf) {
  ASSERT_EQ(0.0f, conjf(0));
}

TEST(complex, conjl) {
  ASSERT_EQ(0.0, conjl(0));
}

TEST(complex, cpow) {
  ASSERT_EQ(8.0, cpow(2.0, 3.0));
}

TEST(complex, cpowf) {
  ASSERT_EQ(8.0f, cpowf(2.0f, 3.0f));
}

TEST(complex, cpowl) {
  ASSERT_EQ(8.0L, cpowl(2.0L, 3.0L));
}

TEST(complex, cproj) {
  ASSERT_EQ(0.0, cproj(0));
}

TEST(complex, cprojf) {
  ASSERT_EQ(0.0f, cprojf(0));
}

TEST(complex, cprojl) {
  ASSERT_EQ(0.0, cprojl(0));
}

TEST(complex, creal) {
  ASSERT_EQ(2.0, creal(2.0 + 3.0I));
}

TEST(complex, crealf) {
  ASSERT_EQ(2.0f, crealf(2.0f + 3.0fI));
}

TEST(complex, creall) {
  ASSERT_EQ(2.0, creall(2.0L + 3.0LI));
}

TEST(complex, csin) {
  ASSERT_EQ(0.0, csin(0));
}

TEST(complex, csinf) {
  ASSERT_EQ(0.0, csinf(0));
}

TEST(complex, csinl) {
  ASSERT_EQ(0.0, csinl(0));
}

TEST(complex, csinh) {
  ASSERT_EQ(0.0, csinh(0));
}

TEST(complex, csinhf) {
  ASSERT_EQ(0.0, csinhf(0));
}

TEST(complex, csinhl) {
  ASSERT_EQ(0.0, csinhl(0));
}

TEST(complex, csqrt) {
  ASSERT_EQ(0.0, csqrt(0));
}

TEST(complex, csqrtf) {
  ASSERT_EQ(0.0f, csqrt(0));
}

TEST(complex, csqrtl) {
  ASSERT_EQ(0.0, csqrtl(0));
}

TEST(complex, ctan) {
  ASSERT_EQ(0.0, ctan(0));
}

TEST(complex, ctanf) {
  ASSERT_EQ(0.0, ctanf(0));
}

TEST(complex, ctanl) {
  ASSERT_EQ(0.0, ctanl(0));
}

TEST(complex, ctanh) {
  ASSERT_EQ(0.0, ctanh(0));
}

TEST(complex, ctanhf) {
  ASSERT_EQ(0.0, ctanhf(0));
}

TEST(complex, ctanhl) {
  ASSERT_EQ(0.0, ctanhl(0));
}
