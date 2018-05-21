/*
 * Copyright (C) 2017 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <math.h>

#include "header_checks.h"

static void math_h() {
  TYPE(float_t);
  TYPE(double_t);

#if !defined(fpclassify)
#error fpclassify
#endif
#if !defined(isfinite)
#error isfinite
#endif
#if !defined(isgreater)
#error isgreater
#endif
#if !defined(isgreaterequal)
#error isgreaterequal
#endif
#if !defined(isinf)
#error isinf
#endif
#if !defined(isless)
#error isless
#endif
#if !defined(islessequal)
#error islessequal
#endif
#if !defined(islessgreater)
#error islessgreater
#endif
#if !defined(isnan)
#error isnan
#endif
#if !defined(isnormal)
#error isnormal
#endif
#if !defined(isunordered)
#error isunordered
#endif
#if !defined(signbit)
#error signbit
#endif

  MACRO(M_E);
  MACRO(M_LOG2E);
  MACRO(M_LOG10E);
  MACRO(M_LN2);
  MACRO(M_LN10);
  MACRO(M_PI);
  MACRO(M_PI_2);
  MACRO(M_PI_4);
  MACRO(M_1_PI);
  MACRO(M_2_PI);
  MACRO(M_2_SQRTPI);
  MACRO(M_SQRT2);
  MACRO(M_SQRT1_2);

  MACRO(MAXFLOAT);

  MACRO(HUGE_VAL);
  MACRO(HUGE_VALF);
  MACRO(HUGE_VALL);
  MACRO(INFINITY);
  MACRO(NAN);

  MACRO(FP_INFINITE);
  MACRO(FP_NAN);
  MACRO(FP_NORMAL);
  MACRO(FP_SUBNORMAL);
  MACRO(FP_ZERO);

#if defined(FP_FAST_FMA) && FP_FAST_FMA != 1
#error FP_FAST_FMA
#endif
#if defined(FP_FAST_FMAF) && FP_FAST_FMAF != 1
#error FP_FAST_FMAF
#endif
#if defined(FP_FAST_FMAL) && FP_FAST_FMAL != 1
#error FP_FAST_FMAL
#endif

  MACRO(FP_ILOGB0);
  MACRO(FP_ILOGBNAN);

  MACRO_VALUE(MATH_ERRNO, 1);
  MACRO_VALUE(MATH_ERREXCEPT, 2);

#if !defined(math_errhandling)
#error math_errhandling
#endif
  MACRO_TYPE(int, math_errhandling);

  FUNCTION(acos, double (*f)(double));
  FUNCTION(acosf, float (*f)(float));
  FUNCTION(acosh, double (*f)(double));
  FUNCTION(acoshf, float (*f)(float));
  FUNCTION(acoshl, long double (*f)(long double));
  FUNCTION(acosl, long double (*f)(long double));

  FUNCTION(asin, double (*f)(double));
  FUNCTION(asinf, float (*f)(float));
  FUNCTION(asinh, double (*f)(double));
  FUNCTION(asinhf, float (*f)(float));
  FUNCTION(asinhl, long double (*f)(long double));
  FUNCTION(asinl, long double (*f)(long double));

  FUNCTION(atan, double (*f)(double));
  FUNCTION(atan2, double (*f)(double, double));
  FUNCTION(atan2f, float (*f)(float, float));
  FUNCTION(atan2l, long double (*f)(long double, long double));
  FUNCTION(atanf, float (*f)(float));
  FUNCTION(atanh, double (*f)(double));
  FUNCTION(atanhf, float (*f)(float));
  FUNCTION(atanhl, long double (*f)(long double));
  FUNCTION(atanl, long double (*f)(long double));

  FUNCTION(cbrt, double (*f)(double));
  FUNCTION(cbrtf, float (*f)(float));
  FUNCTION(cbrtl, long double (*f)(long double));

  FUNCTION(ceil, double (*f)(double));
  FUNCTION(ceilf, float (*f)(float));
  FUNCTION(ceill, long double (*f)(long double));

  FUNCTION(copysign, double (*f)(double, double));
  FUNCTION(copysignf, float (*f)(float, float));
  FUNCTION(copysignl, long double (*f)(long double, long double));

  FUNCTION(cos, double (*f)(double));
  FUNCTION(cosf, float (*f)(float));
  FUNCTION(cosh, double (*f)(double));
  FUNCTION(coshf, float (*f)(float));
  FUNCTION(coshl, long double (*f)(long double));
  FUNCTION(cosl, long double (*f)(long double));

  FUNCTION(erf, double (*f)(double));
  FUNCTION(erfc, double (*f)(double));
  FUNCTION(erfcf, float (*f)(float));
  FUNCTION(erfcl, long double (*f)(long double));
  FUNCTION(erff, float (*f)(float));
  FUNCTION(erfl, long double (*f)(long double));

  FUNCTION(exp, double (*f)(double));
  FUNCTION(exp2, double (*f)(double));
  FUNCTION(exp2f, float (*f)(float));
  FUNCTION(exp2l, long double (*f)(long double));
  FUNCTION(expf, float (*f)(float));
  FUNCTION(expl, long double (*f)(long double));
  FUNCTION(expm1, double (*f)(double));
  FUNCTION(expm1f, float (*f)(float));
  FUNCTION(expm1l, long double (*f)(long double));

  FUNCTION(fabs, double (*f)(double));
  FUNCTION(fabsf, float (*f)(float));
  FUNCTION(fabsl, long double (*f)(long double));

  FUNCTION(fdim, double (*f)(double, double));
  FUNCTION(fdimf, float (*f)(float, float));
  FUNCTION(fdiml, long double (*f)(long double, long double));

  FUNCTION(floor, double (*f)(double));
  FUNCTION(floorf, float (*f)(float));
  FUNCTION(floorl, long double (*f)(long double));

  FUNCTION(fma, double (*f)(double, double, double));
  FUNCTION(fmaf, float (*f)(float, float, float));
  FUNCTION(fmal, long double (*f)(long double, long double, long double));

  FUNCTION(fmax, double (*f)(double, double));
  FUNCTION(fmaxf, float (*f)(float, float));
  FUNCTION(fmaxl, long double (*f)(long double, long double));

  FUNCTION(fmin, double (*f)(double, double));
  FUNCTION(fminf, float (*f)(float, float));
  FUNCTION(fminl, long double (*f)(long double, long double));

  FUNCTION(fmod, double (*f)(double, double));
  FUNCTION(fmodf, float (*f)(float, float));
  FUNCTION(fmodl, long double (*f)(long double, long double));

  FUNCTION(frexp, double (*f)(double, int*));
  FUNCTION(frexpf, float (*f)(float, int*));
  FUNCTION(frexpl, long double (*f)(long double, int*));

  FUNCTION(hypot, double (*f)(double, double));
  FUNCTION(hypotf, float (*f)(float, float));
  FUNCTION(hypotl, long double (*f)(long double, long double));

  FUNCTION(ilogb, int (*f)(double));
  FUNCTION(ilogbf, int (*f)(float));
  FUNCTION(ilogbl, int (*f)(long double));

  FUNCTION(j0, double (*f)(double));
  FUNCTION(j1, double (*f)(double));
  FUNCTION(jn, double (*f)(int, double));

  FUNCTION(ldexp, double (*f)(double, int));
  FUNCTION(ldexpf, float (*f)(float, int));
  FUNCTION(ldexpl, long double (*f)(long double, int));

  FUNCTION(lgamma, double (*f)(double));
  FUNCTION(lgammaf, float (*f)(float));
  FUNCTION(lgammal, long double (*f)(long double));

  FUNCTION(llrint, long long (*f)(double));
  FUNCTION(llrintf, long long (*f)(float));
  FUNCTION(llrintl, long long (*f)(long double));

  FUNCTION(llround, long long (*f)(double));
  FUNCTION(llroundf, long long (*f)(float));
  FUNCTION(llroundl, long long (*f)(long double));

  FUNCTION(log, double (*f)(double));
  FUNCTION(log10, double (*f)(double));
  FUNCTION(log10f, float (*f)(float));
  FUNCTION(log10l, long double (*f)(long double));
  FUNCTION(log1p, double (*f)(double));
  FUNCTION(log1pf, float (*f)(float));
  FUNCTION(log1pl, long double (*f)(long double));
  FUNCTION(log2, double (*f)(double));
  FUNCTION(log2f, float (*f)(float));
  FUNCTION(log2l, long double (*f)(long double));
  FUNCTION(logb, double (*f)(double));
  FUNCTION(logbf, float (*f)(float));
  FUNCTION(logbl, long double (*f)(long double));
  FUNCTION(logf, float (*f)(float));
  FUNCTION(logl, long double (*f)(long double));

  FUNCTION(lrint, long (*f)(double));
  FUNCTION(lrintf, long (*f)(float));
  FUNCTION(lrintl, long (*f)(long double));

  FUNCTION(lround, long (*f)(double));
  FUNCTION(lroundf, long (*f)(float));
  FUNCTION(lroundl, long (*f)(long double));

  FUNCTION(modf, double (*f)(double, double*));
  FUNCTION(modff, float (*f)(float, float*));
  FUNCTION(modfl, long double (*f)(long double, long double*));

  FUNCTION(nan, double (*f)(const char*));
  FUNCTION(nanf, float (*f)(const char*));
  FUNCTION(nanl, long double (*f)(const char*));

  FUNCTION(nearbyint, double (*f)(double));
  FUNCTION(nearbyintf, float (*f)(float));
  FUNCTION(nearbyintl, long double (*f)(long double));

  FUNCTION(nextafter, double (*f)(double, double));
  FUNCTION(nextafterf, float (*f)(float, float));
  FUNCTION(nextafterl, long double (*f)(long double, long double));

  FUNCTION(nexttoward, double (*f)(double, long double));
  FUNCTION(nexttowardf, float (*f)(float, long double));
  FUNCTION(nexttowardl, long double (*f)(long double, long double));

  FUNCTION(pow, double (*f)(double, double));
  FUNCTION(powf, float (*f)(float, float));
  FUNCTION(powl, long double (*f)(long double, long double));

  FUNCTION(remainder, double (*f)(double, double));
  FUNCTION(remainderf, float (*f)(float, float));
  FUNCTION(remainderl, long double (*f)(long double, long double));

  FUNCTION(remquo, double (*f)(double, double, int*));
  FUNCTION(remquof, float (*f)(float, float, int*));
  FUNCTION(remquol, long double (*f)(long double, long double, int*));

  FUNCTION(rint, double (*f)(double));
  FUNCTION(rintf, float (*f)(float));
  FUNCTION(rintl, long double (*f)(long double));

  FUNCTION(round, double (*f)(double));
  FUNCTION(roundf, float (*f)(float));
  FUNCTION(roundl, long double (*f)(long double));

  FUNCTION(scalbln, double (*f)(double, long));
  FUNCTION(scalblnf, float (*f)(float, long));
  FUNCTION(scalblnl, long double (*f)(long double, long));

  FUNCTION(scalbn, double (*f)(double, int));
  FUNCTION(scalbnf, float (*f)(float, int));
  FUNCTION(scalbnl, long double (*f)(long double, int));

  FUNCTION(sin, double (*f)(double));
  FUNCTION(sinf, float (*f)(float));
  FUNCTION(sinh, double (*f)(double));
  FUNCTION(sinhf, float (*f)(float));
  FUNCTION(sinhl, long double (*f)(long double));
  FUNCTION(sinl, long double (*f)(long double));

  FUNCTION(sqrt, double (*f)(double));
  FUNCTION(sqrtf, float (*f)(float));
  FUNCTION(sqrtl, long double (*f)(long double));

  FUNCTION(tan, double (*f)(double));
  FUNCTION(tanf, float (*f)(float));
  FUNCTION(tanh, double (*f)(double));
  FUNCTION(tanhf, float (*f)(float));
  FUNCTION(tanhl, long double (*f)(long double));
  FUNCTION(tanl, long double (*f)(long double));

  FUNCTION(tgamma, double (*f)(double));
  FUNCTION(tgammaf, float (*f)(float));
  FUNCTION(tgammal, long double (*f)(long double));

  FUNCTION(trunc, double (*f)(double));
  FUNCTION(truncf, float (*f)(float));
  FUNCTION(truncl, long double (*f)(long double));

  FUNCTION(y0, double (*f)(double));
  FUNCTION(y1, double (*f)(double));
  FUNCTION(yn, double (*f)(int, double));

  int s = signgam;
}
