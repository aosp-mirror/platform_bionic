/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <sys/types.h>

#include <math.h>
#include <machine/ieee.h>

// These aren't declared in our <math.h>.
extern "C" int __isinf(double);
extern "C" int __isnan(double);

union float_u {
  float f;
  ieee_single bits;
};

union double_u {
  double d;
  ieee_double bits;
};

int __fpclassifyd(double d) {
  double_u u;
  u.d = d;
  if (u.bits.dbl_exp == 0) {
    return ((u.bits.dbl_fracl | u.bits.dbl_frach) == 0) ? FP_ZERO : FP_SUBNORMAL;
  }
  if (u.bits.dbl_exp == DBL_EXP_INFNAN) {
    return ((u.bits.dbl_fracl | u.bits.dbl_frach) == 0) ? FP_INFINITE : FP_NAN;
  }
  return FP_NORMAL;
}
__strong_alias(__fpclassify, __fpclassifyd); // glibc uses __fpclassify, BSD __fpclassifyd.

int __fpclassifyf(float f) {
  float_u u;
  u.f = f;
  if (u.bits.sng_exp == 0) {
    return (u.bits.sng_frac == 0) ? FP_ZERO : FP_SUBNORMAL;
  }
  if (u.bits.sng_exp == SNG_EXP_INFNAN) {
    return (u.bits.sng_frac == 0) ? FP_INFINITE : FP_NAN;
  }
  return FP_NORMAL;
}

int __isinf(double d) {
  return (__fpclassifyd(d) == FP_INFINITE);
}
__strong_alias(isinf, __isinf);

int __isinff(float f) {
  return (__fpclassifyf(f) == FP_INFINITE);
}
__strong_alias(isinff, __isinff);

int __isnan(double d) {
  return (__fpclassifyd(d) == FP_NAN);
}
__strong_alias(isnan, __isnan);

int __isnanf(float f) {
  return (__fpclassifyf(f) == FP_NAN);
}
__strong_alias(isnanf, __isnanf);

#if __LP64__

// LP64 uses 128-bit long doubles.

union long_double_u {
  long double ld;
  ieee_ext bits;
};

#define zero_frac(b) ((b.ext_fracl | b.ext_fraclm | b.ext_frachm | b.ext_frach) == 0)

int __fpclassifyl(long double ld) {
  long_double_u u;
  u.ld = ld;
  if (u.bits.ext_exp == 0) {
    return zero_frac(u.bits) ? FP_ZERO : FP_SUBNORMAL;
  }
  if (u.bits.ext_exp == EXT_EXP_INFNAN) {
    return zero_frac(u.bits) ? FP_INFINITE : FP_NAN;
  }
  return FP_NORMAL;
}

int __isinfl(long double ld) {
  return (__fpclassifyl(ld) == FP_INFINITE);
}

int __isnanl(long double ld) {
  return (__fpclassifyl(ld) == FP_NAN);
}

#else

// LP32 uses double as long double.

__strong_alias(__fpclassifyl, __fpclassify);
__strong_alias(__isinfl, __isinf);
__strong_alias(__isnanl, __isnan);

#endif

__strong_alias(isinfl, __isinfl);
__strong_alias(isnanl, __isnanl);
