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

double fabs(double x) { return __builtin_fabs(x); }
float fabsf(float x) { return __builtin_fabsf(x); }
long double fabsl(long double x) { return __builtin_fabsl(x); }

#if defined(__aarch64__) || defined(__riscv) || defined(__i386__) || defined(__x86_64__)
float ceilf(float x) { return __builtin_ceilf(x); }
double ceil(double x) { return __builtin_ceil(x); }
#if defined(__ILP32__)
__weak_reference(ceil, ceill);
#endif
#endif

double copysign(double x, double y) { return __builtin_copysign(x, y); }
float copysignf(float x, float y) { return __builtin_copysignf(x, y); }
long double copysignl(long double x, long double y) { return __builtin_copysignl(x, y); }

#if defined(__arm__) && (__ARM_ARCH < 8)
// armv8 arm32 has a single-instruction implementation for these, but
// armv7 arm32 doesn't, so __builtin_ doesn't work for arm32.
#include "math_private.h"
namespace s_floor {
#include "upstream-freebsd/lib/msun/src/s_floor.c"
}
namespace s_floorf {
#include "upstream-freebsd/lib/msun/src/s_floorf.c"
}
float floorf(float x) { return s_floorf::floorf(x); }
double floor(double x) { return s_floor::floor(x); }
#else
float floorf(float x) { return __builtin_floorf(x); }
double floor(double x) { return __builtin_floor(x); }
#if defined(__ILP32__)
__weak_reference(floor, floorl);
#endif
#endif

#if defined(__aarch64__) || defined(__riscv)
float fmaf(float x, float y, float z) { return __builtin_fmaf(x, y, z); }
double fma(double x, double y, double z) { return __builtin_fma(x, y, z); }

float fmaxf(float x, float y) { return __builtin_fmaxf(x, y); }
double fmax(double x, double y) { return __builtin_fmax(x, y); }

float fminf(float x, float y) { return __builtin_fminf(x, y); }
double fmin(double x, double y) { return __builtin_fmin(x, y); }
#endif

#if defined(__aarch64__) || defined(__riscv)
long lrint(double x) { return __builtin_lrint(x); }
long lrintf(float x) { return __builtin_lrintf(x); }
long long llrint(double x) { return __builtin_llrint(x); }
long long llrintf(float x) { return __builtin_llrintf(x); }
#endif

#if defined(__aarch64__) || defined(__riscv)
long lround(double x) { return __builtin_lround(x); }
long lroundf(float x) { return __builtin_lroundf(x); }
long long llround(double x) { return __builtin_llround(x); }
long long llroundf(float x) { return __builtin_llroundf(x); }
#endif

#if defined(__aarch64__) || defined(__riscv) || defined(__i386__) || defined(__x86_64__)
float rintf(float x) { return __builtin_rintf(x); }
double rint(double x) { return __builtin_rint(x); }
#if defined(__ILP32__)
__weak_reference(rint, rintl);
#endif
#endif

#if defined(__aarch64__) || defined(__riscv)
float roundf(float x) { return __builtin_roundf(x); }
double round(double x) { return __builtin_round(x); }
#endif

float sqrtf(float x) { return __builtin_sqrtf(x); }
double sqrt(double x) { return __builtin_sqrt(x); }
#if defined(__ILP32__)
__weak_reference(sqrt, sqrtl);
#endif

#if defined(__aarch64__) || defined(__riscv) || defined(__i386__) || defined(__x86_64__)
float truncf(float x) { return __builtin_truncf(x); }
double trunc(double x) { return __builtin_trunc(x); }
#if defined(__ILP32__)
__weak_reference(trunc, truncl);
#endif
#endif
