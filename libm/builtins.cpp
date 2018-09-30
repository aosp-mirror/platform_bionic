/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <math.h>

#include "fpmath.h"

double fabs(double x) {
#if __arm__
  // Both Clang and GCC insist on moving r0/r1 into a double register
  // and using fabs where bit-twiddling would be a better choice.
  // They get fabsf right, but we need to be careful in fabsl too.
  IEEEd2bits u;
  u.d = x;
  u.bits.sign = 0;
  return u.d;
#else
  return __builtin_fabs(x);
#endif
}

float fabsf(float x) {
  return __builtin_fabsf(x);
}

#if defined(__LP64__)
long double fabsl(long double x) { return __builtin_fabsl(x); }
#else
long double fabsl(long double x) {
  // Don't use __builtin_fabs here because of ARM. (See fabs above.)
  return fabs(x);
}
#endif

#if defined(__aarch64__)
float ceilf(float x) { return __builtin_ceilf(x); }
double ceil(double x) { return __builtin_ceil(x); }

float floorf(float x) { return __builtin_floorf(x); }
double floor(double x) { return __builtin_floor(x); }

float fmaf(float x, float y, float z) { return __builtin_fmaf(x, y, z); }
double fma(double x, double y, double z) { return __builtin_fma(x, y, z); }

float fmaxf(float x, float y) { return __builtin_fmaxf(x, y); }
double fmax(double x, double y) { return __builtin_fmax(x, y); }

float fminf(float x, float y) { return __builtin_fminf(x, y); }
double fmin(double x, double y) { return __builtin_fmin(x, y); }

float rintf(float x) { return __builtin_rintf(x); }
double rint(double x) { return __builtin_rint(x); }

float roundf(float x) { return __builtin_roundf(x); }
double round(double x) { return __builtin_round(x); }

float truncf(float x) { return __builtin_truncf(x); }
double trunc(double x) { return __builtin_trunc(x); }
#endif
