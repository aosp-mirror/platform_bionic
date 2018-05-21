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

#include <tgmath.h>

#include "header_checks.h"

#define TGMATH(f_) f_(f1); f_(d1); f_(ld1);
#define TGMATHC(f_) f_(f1); f_(d1); f_(ld1); f_(fc1); f_(dc1); f_(ldc1);
#define TGMATHCONLY(f_) f_(fc1); f_(dc1); f_(ldc1);
#define TGMATH2(f_) f_(f1, f2); f_(d1, d2); f_(ld1, ld2);
#define TGMATH2C(f_) f_(f1, f2); f_(d1, d2); f_(ld1, ld2); f_(fc1, fc2); f_(dc1, dc2); f_(ldc1, ldc2);
#define TGMATH3(f_) f_(f1, f2, f3); f_(d1, d2, d3); f_(ld1, ld2, ld3);

static void tgmath_h() {
  float f1, f2, f3;
  f1 = f2 = f3 = 0;
  float complex fc1, fc2, fc3;
  fc1 = fc2 = fc3 = 0;
  double d1, d2, d3;
  d1 = d2 = d3 = 0;
  double complex dc1, dc2, dc3;
  dc1 = dc2 = dc3 = 0;
  long double ld1, ld2, ld3;
  ld1 = ld2 = ld3 = 0;
  long double complex ldc1, ldc2, ldc3;
  ldc1 = ldc2 = ldc3 = 0;
  int i = 0;
  long l = 0;

  TGMATHC(acos);
  TGMATHC(asin);
  TGMATHC(atan);
  TGMATHC(acosh);
  TGMATHC(asinh);
  TGMATHC(atanh);
  TGMATHC(cos);
  TGMATHC(sin);
  TGMATHC(tan);
  TGMATHC(cosh);
  TGMATHC(sinh);
  TGMATHC(tanh);
  TGMATHC(exp);
  TGMATHC(log);
  TGMATH2C(pow);
  TGMATHC(sqrt);
  TGMATHC(fabs);

  TGMATH2(atan2);
  TGMATH(cbrt);
  TGMATH(ceil);
  TGMATH2(copysign);
  TGMATH(erf);
  TGMATH(erfc);
  TGMATH(exp2);
  TGMATH(expm1);
  TGMATH2(fdim);
  TGMATH(floor);
  TGMATH3(fma);
  TGMATH2(fmax);
  TGMATH2(fmin);
  TGMATH2(fmod);
  frexp(f1, &i); frexp(d1, &i); frexp(ld1, &i);
  TGMATH2(hypot);
  TGMATH(ilogb);
  ldexp(f1, i); ldexp(d1, i); ldexp(ld1, i);
  TGMATH(lgamma);
  TGMATH(llrint);
  TGMATH(llround);
  TGMATH(log10);
  TGMATH(log1p);
  TGMATH(log2);
  TGMATH(logb);
  TGMATH(lrint);
  TGMATH(lround);
  TGMATH(nearbyint);
  TGMATH2(nextafter);
  TGMATH2(nexttoward);
  TGMATH2(remainder);
  remquo(f1, f2, &i); remquo(d1, d2, &i); remquo(ld1, ld2, &i);
  TGMATH(rint);
  TGMATH(round);
  scalbln(f1, l); scalbln(d1, l); scalbln(ld1, l);
  scalbn(f1, i); scalbn(d1, i); scalbn(ld1, i);
  TGMATH(tgamma);
  TGMATH(trunc);

  TGMATHCONLY(carg);
  TGMATHCONLY(cimag);
  TGMATHCONLY(conj);
  TGMATHCONLY(cproj);
  TGMATHCONLY(creal);
}
