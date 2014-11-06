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
// have to declare the complex math functions ourselves.
// (libc++ also seems to have really bad implementations of its own that ignore
// the intricacies of floating point math.)
// http://llvm.org/bugs/show_bug.cgi?id=21504

#include <math.h> // For M_PI.

extern "C" double cabs(double _Complex);
TEST(complex, cabs) {
  ASSERT_EQ(0.0, cabs(0));
}

extern "C" float cabsf(float _Complex);
TEST(complex, cabsf) {
  ASSERT_EQ(0.0, cabsf(0));
}

extern "C" long double cabsl(long double _Complex);
TEST(complex, cabsl) {
  ASSERT_EQ(0.0, cabsl(0));
}

extern "C" double _Complex cacos(double _Complex);
TEST(complex, cacos) {
  ASSERT_EQ(M_PI/2.0, cacos(0.0));
}

extern "C" float _Complex cacosf(float _Complex);
TEST(complex, cacosf) {
  ASSERT_EQ(static_cast<float>(M_PI)/2.0f, cacosf(0.0));
}

extern "C" double _Complex cacosh(double _Complex);
TEST(complex, cacosh) {
  ASSERT_EQ(0.0, cacosh(1.0));
}

extern "C" float _Complex cacoshf(float _Complex);
TEST(complex, cacoshf) {
  ASSERT_EQ(0.0, cacoshf(1.0));
}

extern "C" double carg(double _Complex);
TEST(complex, carg) {
  ASSERT_EQ(0.0, carg(0));
}

extern "C" float cargf(float _Complex);
TEST(complex, cargf) {
  ASSERT_EQ(0.0, cargf(0));
}

extern "C" long double cargl(long double _Complex);
TEST(complex, cargl) {
  ASSERT_EQ(0.0, cargl(0));
}

extern "C" double _Complex casin(double _Complex);
TEST(complex, casin) {
  ASSERT_EQ(0.0, casin(0));
}

extern "C" float _Complex casinf(float _Complex);
TEST(complex, casinf) {
  ASSERT_EQ(0.0, casinf(0));
}

extern "C" double _Complex casinh(double _Complex);
TEST(complex, casinh) {
  ASSERT_EQ(0.0, casinh(0));
}

extern "C" float _Complex casinhf(float _Complex);
TEST(complex, casinhf) {
  ASSERT_EQ(0.0, casinhf(0));
}

extern "C" double _Complex catan(double _Complex);
TEST(complex, catan) {
  ASSERT_EQ(0.0, catan(0));
}

extern "C" float _Complex catanf(float _Complex);
TEST(complex, catanf) {
  ASSERT_EQ(0.0, catanf(0));
}

extern "C" double _Complex catanh(double _Complex);
TEST(complex, catanh) {
  ASSERT_EQ(0.0, catanh(0));
}

extern "C" float _Complex catanhf(float _Complex);
TEST(complex, catanhf) {
  ASSERT_EQ(0.0, catanhf(0));
}

extern "C" double _Complex ccos(double _Complex);
TEST(complex, ccos) {
  ASSERT_EQ(1.0, ccos(0));
}

extern "C" float _Complex ccosf(float _Complex);
TEST(complex, ccosf) {
  ASSERT_EQ(1.0, ccosf(0));
}

extern "C" double _Complex ccosh(double _Complex);
TEST(complex, ccosh) {
  ASSERT_EQ(1.0, ccosh(0));
}

extern "C" float _Complex ccoshf(float _Complex);
TEST(complex, ccoshf) {
  ASSERT_EQ(1.0, ccoshf(0));
}

extern "C" double _Complex cexp(double _Complex);
TEST(complex, cexp) {
  ASSERT_EQ(1.0, cexp(0));
}

extern "C" float _Complex cexpf(float _Complex);
TEST(complex, cexpf) {
  ASSERT_EQ(1.0, cexpf(0));
}

extern "C" double cimag(double _Complex);
TEST(complex, cimag) {
  ASSERT_EQ(0.0, cimag(0));
}

extern "C" float cimagf(float _Complex);
TEST(complex, cimagf) {
  ASSERT_EQ(0.0f, cimagf(0));
}

extern "C" long double cimagl(long double _Complex);
TEST(complex, cimagl) {
  ASSERT_EQ(0.0, cimagl(0));
}

extern "C" double _Complex conj(double _Complex);
TEST(complex, conj) {
  ASSERT_EQ(0.0, conj(0));
}

extern "C" float _Complex conjf(float _Complex);
TEST(complex, conjf) {
  ASSERT_EQ(0.0f, conjf(0));
}

extern "C" long double _Complex conjl(long double _Complex);
TEST(complex, conjl) {
  ASSERT_EQ(0.0, conjl(0));
}

extern "C" double _Complex cproj(double _Complex);
TEST(complex, cproj) {
  ASSERT_EQ(0.0, cproj(0));
}

extern "C" float _Complex cprojf(float _Complex);
TEST(complex, cprojf) {
  ASSERT_EQ(0.0f, cprojf(0));
}

extern "C" long double _Complex cprojl(long double _Complex);
TEST(complex, cprojl) {
  ASSERT_EQ(0.0, cprojl(0));
}

extern "C" double creal(double _Complex);
TEST(complex, creal) {
  ASSERT_EQ(0.0, creal(0));
}

extern "C" float crealf(float _Complex);
TEST(complex, crealf) {
  ASSERT_EQ(0.0f, crealf(0));
}

extern "C" long double creall(long double _Complex);
TEST(complex, creall) {
  ASSERT_EQ(0.0, creall(0));
}

extern "C" double _Complex csin(double _Complex);
TEST(complex, csin) {
  ASSERT_EQ(0.0, csin(0));
}

extern "C" float _Complex csinf(float _Complex);
TEST(complex, csinf) {
  ASSERT_EQ(0.0, csinf(0));
}

extern "C" double _Complex csinh(double _Complex);
TEST(complex, csinh) {
  ASSERT_EQ(0.0, csinh(0));
}

extern "C" float _Complex csinhf(float _Complex);
TEST(complex, csinhf) {
  ASSERT_EQ(0.0, csinhf(0));
}

extern "C" double _Complex csqrt(double _Complex);
TEST(complex, csqrt) {
  ASSERT_EQ(0.0, csqrt(0));
}

extern "C" float _Complex csqrtf(float _Complex);
TEST(complex, csqrtf) {
  ASSERT_EQ(0.0f, csqrt(0));
}

extern "C" long double _Complex csqrtl(long double _Complex);
TEST(complex, csqrtl) {
  ASSERT_EQ(0.0, csqrtl(0));
}

extern "C" double _Complex ctan(double _Complex);
TEST(complex, ctan) {
  ASSERT_EQ(0.0, ctan(0));
}

extern "C" float _Complex ctanf(float _Complex);
TEST(complex, ctanf) {
  ASSERT_EQ(0.0, ctanf(0));
}

extern "C" double _Complex ctanh(double _Complex);
TEST(complex, ctanh) {
  ASSERT_EQ(0.0, ctanh(0));
}

extern "C" float _Complex ctanhf(float _Complex);
TEST(complex, ctanhf) {
  ASSERT_EQ(0.0, ctanhf(0));
}
