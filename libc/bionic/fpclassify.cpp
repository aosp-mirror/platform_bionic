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

#include <math.h>

// Legacy cruft from before we had builtin implementations of the standard macros.
// No longer declared in our <math.h>.

extern "C" int __fpclassifyd(double d) {
  return fpclassify(d);
}
__strong_alias(__fpclassify, __fpclassifyd); // glibc uses __fpclassify, BSD __fpclassifyd.

extern "C" int __fpclassifyf(float f) {
  return fpclassify(f);
}

extern "C" int __isinf(double d) {
  return isinf(d);
}
__strong_alias(isinf, __isinf);

extern "C" int __isinff(float f) {
  return isinf(f);
}
__strong_alias(isinff, __isinff);

extern "C" int __isnan(double d) {
  return isnan(d);
}
__strong_alias(isnan, __isnan);

extern "C" int __isnanf(float f) {
  return isnan(f);
}
__strong_alias(isnanf, __isnanf);

extern "C" int __isfinite(double d) {
  return isfinite(d);
}
__strong_alias(isfinite, __isfinite);

extern "C" int __isfinitef(float f) {
  return isfinite(f);
}
__strong_alias(isfinitef, __isfinitef);

extern "C" int __isnormal(double d) {
  return isnormal(d);
}
__strong_alias(isnormal, __isnormal);

extern "C" int __isnormalf(float f) {
  return isnormal(f);
}
__strong_alias(isnormalf, __isnormalf);

extern "C" int __fpclassifyl(long double ld) {
  return fpclassify(ld);
}

extern "C" int __isinfl(long double ld) {
  return isinf(ld);
}

extern "C" int __isnanl(long double ld) {
  return isnan(ld);
}

extern "C" int __isfinitel(long double ld) {
  return isfinite(ld);
}

extern "C" int __isnormall(long double ld) {
  return isnormal(ld);
}

__strong_alias(isinfl, __isinfl);
__strong_alias(isnanl, __isnanl);
__strong_alias(isfinitel, __isfinitel);
__strong_alias(isnormall, __isnormall);
