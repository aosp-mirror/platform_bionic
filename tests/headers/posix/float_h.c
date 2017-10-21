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

#include <float.h>

#include "header_checks.h"

static void float_h() {
  int flt_rounds = FLT_ROUNDS;

  MACRO(FLT_EVAL_METHOD);

  MACRO(FLT_RADIX);
  MACRO(FLT_MANT_DIG);
  MACRO(DBL_MANT_DIG);
  MACRO(LDBL_MANT_DIG);
  MACRO(DECIMAL_DIG);
  MACRO(FLT_DIG);
  MACRO(DBL_DIG);
  MACRO(LDBL_DIG);
  MACRO(FLT_MIN_EXP);
  MACRO(DBL_MIN_EXP);
  MACRO(LDBL_MIN_EXP);
  MACRO(FLT_MIN_10_EXP);
  MACRO(DBL_MIN_10_EXP);
  MACRO(LDBL_MIN_10_EXP);
  MACRO(FLT_MAX_EXP);
  MACRO(DBL_MAX_EXP);
  MACRO(LDBL_MAX_EXP);
  MACRO(FLT_MAX_10_EXP);
  MACRO(DBL_MAX_10_EXP);
  MACRO(LDBL_MAX_10_EXP);
  MACRO(FLT_MAX);
  MACRO(DBL_MAX);
  MACRO(LDBL_MAX);
  MACRO(FLT_EPSILON);
  MACRO(DBL_EPSILON);
  MACRO(LDBL_EPSILON);
  MACRO(FLT_MIN);
  MACRO(DBL_MIN);
  MACRO(LDBL_MIN);
}
