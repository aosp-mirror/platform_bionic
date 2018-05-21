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

#include <fenv.h>

#include "header_checks.h"

static void fenv_h() {
  TYPE(fenv_t);
  TYPE(fexcept_t);

  MACRO(FE_DIVBYZERO);
  MACRO(FE_INEXACT);
  MACRO(FE_INVALID);
  MACRO(FE_OVERFLOW);
  MACRO(FE_UNDERFLOW);

  MACRO(FE_ALL_EXCEPT);

  MACRO(FE_DOWNWARD);
  MACRO(FE_TONEAREST);
  MACRO(FE_TOWARDZERO);
  MACRO(FE_UPWARD);

  const fenv_t* fe_dfl_env = FE_DFL_ENV;

  FUNCTION(feclearexcept, int (*f)(int));
  FUNCTION(fegetenv, int (*f)(fenv_t*));
  FUNCTION(fegetexceptflag, int (*f)(fexcept_t*, int));
  FUNCTION(fegetround, int (*f)(void));
  FUNCTION(feholdexcept, int (*f)(fenv_t*));
  FUNCTION(feraiseexcept, int (*f)(int));
  FUNCTION(fesetenv, int (*f)(const fenv_t*));
  FUNCTION(fesetexceptflag, int (*f)(const fexcept_t*, int));
  FUNCTION(fesetround, int (*f)(int));
  FUNCTION(fetestexcept, int (*f)(int));
  FUNCTION(feupdateenv, int (*f)(const fenv_t*));
}
