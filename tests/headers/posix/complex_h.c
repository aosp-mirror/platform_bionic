// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <complex.h>

#include "header_checks.h"

#if !defined(complex)
#error complex
#endif
#if !defined(_Complex_I)
#error _Complex_I
#endif

#if 0 // No libc actually implements this.
#if !defined(imaginary)
#error imaginary
#endif
#if !defined(_Imaginary_I)
#error _Imaginary_I
#endif
#endif

#if !defined(I)
#error I
#endif

static void complex_h() {
  FUNCTION(cabs, double (*f)(double complex));
  FUNCTION(cabsf, float (*f)(float complex));
  FUNCTION(cabsl, long double (*f)(long double complex));

  FUNCTION(cacos, double complex (*f)(double complex));
  FUNCTION(cacosf, float complex (*f)(float complex));
  FUNCTION(cacosl, long double complex (*f)(long double complex));

  FUNCTION(cacosh, double complex (*f)(double complex));
  FUNCTION(cacoshf, float complex (*f)(float complex));
  FUNCTION(cacoshl, long double complex (*f)(long double complex));

  FUNCTION(carg, double (*f)(double complex));
  FUNCTION(cargf, float (*f)(float complex));
  FUNCTION(cargl, long double (*f)(long double complex));

  FUNCTION(casin, double complex (*f)(double complex));
  FUNCTION(casinf, float complex (*f)(float complex));
  FUNCTION(casinl, long double complex (*f)(long double complex));

  FUNCTION(casinh, double complex (*f)(double complex));
  FUNCTION(casinhf, float complex (*f)(float complex));
  FUNCTION(casinhl, long double complex (*f)(long double complex));

  FUNCTION(catan, double complex (*f)(double complex));
  FUNCTION(catanf, float complex (*f)(float complex));
  FUNCTION(catanl, long double complex (*f)(long double complex));

  FUNCTION(catanh, double complex (*f)(double complex));
  FUNCTION(catanhf, float complex (*f)(float complex));
  FUNCTION(catanhl, long double complex (*f)(long double complex));

  FUNCTION(ccos, double complex (*f)(double complex));
  FUNCTION(ccosf, float complex (*f)(float complex));
  FUNCTION(ccosl, long double complex (*f)(long double complex));

  FUNCTION(ccosh, double complex (*f)(double complex));
  FUNCTION(ccoshf, float complex (*f)(float complex));
  FUNCTION(ccoshl, long double complex (*f)(long double complex));

  FUNCTION(cexp, double complex (*f)(double complex));
  FUNCTION(cexpf, float complex (*f)(float complex));
  FUNCTION(cexpl, long double complex (*f)(long double complex));

  FUNCTION(cimag, double (*f)(double complex));
  FUNCTION(cimagf, float (*f)(float complex));
  FUNCTION(cimagl, long double (*f)(long double complex));

  FUNCTION(clog, double complex (*f)(double complex));
  FUNCTION(clogf, float complex (*f)(float complex));
  FUNCTION(clogl, long double complex (*f)(long double complex));

  FUNCTION(conj, double complex (*f)(double complex));
  FUNCTION(conjf, float complex (*f)(float complex));
  FUNCTION(conjl, long double complex (*f)(long double complex));

  FUNCTION(cpow, double complex (*f)(double complex, double complex));
  FUNCTION(cpowf, float complex (*f)(float complex, float complex));
  FUNCTION(cpowl, long double complex (*f)(long double complex, long double complex));

  FUNCTION(cproj, double complex (*f)(double complex));
  FUNCTION(cprojf, float complex (*f)(float complex));
  FUNCTION(cprojl, long double complex (*f)(long double complex));

  FUNCTION(creal, double (*f)(double complex));
  FUNCTION(crealf, float (*f)(float complex));
  FUNCTION(creall, long double (*f)(long double complex));

  FUNCTION(csin, double complex (*f)(double complex));
  FUNCTION(csinf, float complex (*f)(float complex));
  FUNCTION(csinl, long double complex (*f)(long double complex));

  FUNCTION(csinh, double complex (*f)(double complex));
  FUNCTION(csinhf, float complex (*f)(float complex));
  FUNCTION(csinhl, long double complex (*f)(long double complex));

  FUNCTION(csqrt, double complex (*f)(double complex));
  FUNCTION(csqrtf, float complex (*f)(float complex));
  FUNCTION(csqrtl, long double complex (*f)(long double complex));

  FUNCTION(ctan, double complex (*f)(double complex));
  FUNCTION(ctanf, float complex (*f)(float complex));
  FUNCTION(ctanl, long double complex (*f)(long double complex));

  FUNCTION(ctanh, double complex (*f)(double complex));
  FUNCTION(ctanhf, float complex (*f)(float complex));
  FUNCTION(ctanhl, long double complex (*f)(long double complex));
}
