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

#if defined(__arm__) && (__ARM_ARCH <= 7)
// armv7 arm32 has no instructions to implement these builtins,
// so we include the msun source in the .bp file instead.
#else
double ceil(double x) { return __builtin_ceil(x); }
float ceilf(float x) { return __builtin_ceilf(x); }
#if defined(__ILP32__)
__weak_reference(ceil, ceill);
#endif
#endif

double copysign(double x, double y) { return __builtin_copysign(x, y); }
float copysignf(float x, float y) { return __builtin_copysignf(x, y); }
long double copysignl(long double x, long double y) { return __builtin_copysignl(x, y); }

double fabs(double x) { return __builtin_fabs(x); }
float fabsf(float x) { return __builtin_fabsf(x); }
long double fabsl(long double x) { return __builtin_fabsl(x); }

#if defined(__arm__) && (__ARM_ARCH <= 7)
// armv7 arm32 has no instructions to implement these builtins,
// so we include the msun source in the .bp file instead.
#else
double floor(double x) { return __builtin_floor(x); }
float floorf(float x) { return __builtin_floorf(x); }
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

#if defined(__aarch64__) || defined(__riscv) || defined(__i386__) || defined(__x86_64__)
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

#if defined(__arm__) && (__ARM_ARCH <= 7)
// armv7 arm32 has no instructions to implement these builtins,
// so we include the msun source in the .bp file instead.
#else
double rint(double x) { return __builtin_rint(x); }
float rintf(float x) { return __builtin_rintf(x); }
#if defined(__ILP32__)
__weak_reference(rint, rintl);
#endif
#endif

#if defined(__aarch64__) || defined(__riscv)
double round(double x) { return __builtin_round(x); }
float roundf(float x) { return __builtin_roundf(x); }
#endif

double sqrt(double x) { return __builtin_sqrt(x); }
float sqrtf(float x) { return __builtin_sqrtf(x); }
#if defined(__ILP32__)
__weak_reference(sqrt, sqrtl);
#endif

#if defined(__arm__) && (__ARM_ARCH <= 7)
// armv7 arm32 has no instructions to implement these builtins,
// so we include the msun source in the .bp file instead.
#else
double trunc(double x) { return __builtin_trunc(x); }
float truncf(float x) { return __builtin_truncf(x); }
#if defined(__ILP32__)
__weak_reference(trunc, truncl);
#endif
#endif
