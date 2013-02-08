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

extern int __isinf(double); /* isinf.c */
int (isinf)(double a1) { return __isinf(a1); }

/*
 * The BSD "long double" functions are broken when sizeof(long double) == sizeof(double).
 * Android works around those cases by replacing the broken functions with our own trivial stubs
 * that call the regular "double" function.
 */

int __fpclassifyl(long double a1) { return __fpclassifyd(a1); }
int __isfinitel(long double a1) { return __isfinite(a1); }
int __isinfl(long double a1) { return __isinf(a1); }
int __isnanl(long double a1) { return isnan(a1); }
int __isnormall(long double a1) { return __isnormal(a1); }
int __signbitl(long double a1) { return __signbit(a1); }

long double acoshl(long double a1) { return acosh(a1); }
long double asinhl(long double a1) { return asinh(a1); }
long double atanhl(long double a1) { return atanh(a1); }
long double cbrtl(long double a1) { return cbrt(a1); }
long double copysignl(long double a1, long double a2) { return copysign(a1, a2); }
long double coshl(long double a1) { return cosh(a1); }
long double cosl(long double a1) { return cos(a1); }
long double erfcl(long double a1) { return erfc(a1); }
long double erfl(long double a1) { return erf(a1); }
long double expm1l(long double a1) { return expm1(a1); }
long double fabsl(long double a1) { return fabs(a1); }
long double fmaxl(long double a1, long double a2) { return fmax(a1, a2); }
long double fmodl(long double a1, long double a2) { return fmod(a1, a2); }
long double fminl(long double a1, long double a2) { return fmin(a1, a2); }
int ilogbl(long double a1) { return ilogb(a1); }
long double lgammal(long double a1) { return lgamma(a1); }
long long llrintl(long double a1) { return llrint(a1); }
long double log10l(long double a1) { return log10(a1); }
long double log1pl(long double a1) { return log1p(a1); }
long double log2l(long double a1) { return log2(a1); }
long double logbl(long double a1) { return logb(a1); }
long double logl(long double a1) { return log(a1); }
long lrintl(long double a1) { return lrint(a1); }
long long llroundl(long double a1) { return llround(a1); }
long lroundl(long double a1) { return lround(a1); }
long double modfl(long double a1, long double* a2) { return modf(a1, (double*) a2); }
long double powl(long double a1, long double a2) { return pow(a1, a2); }
long double rintl(long double a1) { return rint(a1); }
long double roundl(long double a1) { return round(a1); }
long double scalbnl(long double a1, int a2) { return scalbn(a1, a2); }
long double significandl(long double a1) { return significand(a1); }
long double sinhl(long double a1) { return sinh(a1); }
long double sinl(long double a1) { return sin(a1); }
long double sqrtl(long double a1) { return sqrt(a1); }
long double tanhl(long double a1) { return tanh(a1); }
long double tanl(long double a1) { return tan(a1); }
long double tgammal(long double a1) { return tgamma(a1); }
