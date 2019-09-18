/*
 * Copyright (C) 2013 The Android Open Source Project
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

#define _GNU_SOURCE
#include <float.h>
#include <math.h>

#if !defined(__LP64__)

// The BSD "long double" functions are broken when sizeof(long double) == sizeof(double).
// Android works around those cases by replacing the broken functions with our own trivial stubs
// that call the regular "double" function.

long double copysignl(long double a1, long double a2) { return copysign(a1, a2); }
long double fmaxl(long double a1, long double a2) { return fmax(a1, a2); }
long double fmodl(long double a1, long double a2) { return fmod(a1, a2); }
long double fminl(long double a1, long double a2) { return fmin(a1, a2); }
int ilogbl(long double a1) { return ilogb(a1); }
long long llrintl(long double a1) { return llrint(a1); }
#if !defined(__i386__) // x86 has an assembler lrint/lrintl.
long lrintl(long double a1) { return lrint(a1); }
#endif
long long llroundl(long double a1) { return llround(a1); }
long lroundl(long double a1) { return lround(a1); }
long double modfl(long double a1, long double* a2) { double i; double f = modf(a1, &i); *a2 = i; return f; }
float nexttowardf(float a1, long double a2) { return nextafterf(a1, (float) a2); }
long double roundl(long double a1) { return round(a1); }
void sincosl(long double x, long double* s, long double* c) { sincos(x, (double*) s, (double*) c); }

#endif // __LP64__

// FreeBSD doesn't have an ld128 implementation of tgammal, so both LP32 and LP64 need this.
long double tgammal(long double x) { return tgamma(x); }
