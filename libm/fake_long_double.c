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

#include <float.h>
#include <math.h>

/*
 * On LP64 sizeof(long double) > sizeof(double) so functions which fall back
 * to their double variants lose precision. Emit a warning whenever something
 * links against such functions.
 * On LP32 sizeof(long double) == sizeof(double) so we don't need to warn.
 */
#ifdef __LP64__
#define WARN_IMPRECISE(x) \
  __warn_references(x, # x " has lower than advertised precision");
#else
#define WARN_IMPRECISE(x)
#endif //__LP64__

#define DECLARE_IMPRECISE(f) \
  long double f ## l(long double v) { return f(v); } \
  WARN_IMPRECISE(x)

DECLARE_IMPRECISE(cosh);
DECLARE_IMPRECISE(erfc);
DECLARE_IMPRECISE(erf);
DECLARE_IMPRECISE(lgamma);
DECLARE_IMPRECISE(sinh);
DECLARE_IMPRECISE(tanh);
DECLARE_IMPRECISE(tgamma);
DECLARE_IMPRECISE(significand);

long double powl(long double a1, long double a2) { return pow(a1, a2); }
WARN_IMPRECISE(powl)

#ifndef __LP64__
/*
 * The BSD "long double" functions are broken when sizeof(long double) == sizeof(double).
 * Android works around those cases by replacing the broken functions with our own trivial stubs
 * that call the regular "double" function.
 */

int __isfinitel(long double a1) { return __isfinite(a1); }
int __isnormall(long double a1) { return __isnormal(a1); }
int __signbitl(long double a1) { return __signbit(a1); }

long double acoshl(long double a1) { return acosh(a1); }
long double acosl(long double a1) { return acos(a1); }
long double asinhl(long double a1) { return asinh(a1); }
long double asinl(long double a1) { return asin(a1); }
long double atan2l(long double a1, long double a2) { return atan2(a1, a2); }
long double atanhl(long double a1) { return atanh(a1); }
long double atanl(long double a1) { return atan(a1); }
long double cbrtl(long double a1) { return cbrt(a1); }
long double ceill(long double a1) { return ceil(a1); }
long double copysignl(long double a1, long double a2) { return copysign(a1, a2); }

long double cosl(long double a1) { return cos(a1); }

long double exp2l(long double a1) { return exp2(a1); }
long double expl(long double a1) { return exp(a1); }
long double expm1l(long double a1) { return expm1(a1); }
long double fabsl(long double a1) { return fabs(a1); }
long double floorl(long double a1) { return floor(a1); }
long double fmal(long double a1, long double a2, long double a3) { return fma(a1, a2, a3); }
long double fmaxl(long double a1, long double a2) { return fmax(a1, a2); }
long double fmodl(long double a1, long double a2) { return fmod(a1, a2); }
long double fminl(long double a1, long double a2) { return fmin(a1, a2); }
long double frexpl(long double a1, int* a2) { return frexp(a1, a2); }
long double hypotl(long double a1, long double a2) { return hypot(a1, a2); }
int ilogbl(long double a1) { return ilogb(a1); }
long double ldexpl(long double a1, int a2) { return ldexp(a1, a2); }
long long llrintl(long double a1) { return llrint(a1); }
long double log10l(long double a1) { return log10(a1); }
long double log1pl(long double a1) { return log1p(a1); }
long double log2l(long double a1) { return log2(a1); }
long double logbl(long double a1) { return logb(a1); }
long double logl(long double a1) { return log(a1); }
long lrintl(long double a1) { return lrint(a1); }
long long llroundl(long double a1) { return llround(a1); }
long lroundl(long double a1) { return lround(a1); }
long double modfl(long double a1, long double* a2) { double i; double f = modf(a1, &i); *a2 = i; return f; }
long double nanl(const char* a1) { return nan(a1); }
long double nextafterl(long double a1, long double a2) { return nextafter(a1, a2); }
long double remainderl(long double a1, long double a2) { return remainder(a1, a2); }
long double remquol(long double a1, long double a2, int* a3) { return remquo(a1, a2, a3); }
long double rintl(long double a1) { return rint(a1); }
long double roundl(long double a1) { return round(a1); }
long double scalbnl(long double a1, int a2) { return scalbn(a1, a2); }
long double sinl(long double a1) { return sin(a1); }
long double sqrtl(long double a1) { return sqrt(a1); }
long double tanl(long double a1) { return tan(a1); }
long double truncl(long double a1) { return trunc(a1); }

#endif // __LP64__
