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
