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

#include <wctype.h>

#include "header_checks.h"

static void wctype_h() {
  TYPE(wint_t);
  TYPE(wctrans_t);
  TYPE(wctype_t);
  TYPE(locale_t);

  MACRO(WEOF);

  FUNCTION(iswalnum, int (*f)(wint_t));
  FUNCTION(iswalnum_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswalpha, int (*f)(wint_t));
  FUNCTION(iswalpha_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswblank, int (*f)(wint_t));
  FUNCTION(iswblank_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswcntrl, int (*f)(wint_t));
  FUNCTION(iswcntrl_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswctype, int (*f)(wint_t, wctype_t));
  FUNCTION(iswctype_l, int (*f)(wint_t, wctype_t, locale_t));
  FUNCTION(iswdigit, int (*f)(wint_t));
  FUNCTION(iswdigit_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswgraph, int (*f)(wint_t));
  FUNCTION(iswgraph_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswlower, int (*f)(wint_t));
  FUNCTION(iswlower_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswprint, int (*f)(wint_t));
  FUNCTION(iswprint_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswpunct, int (*f)(wint_t));
  FUNCTION(iswpunct_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswspace, int (*f)(wint_t));
  FUNCTION(iswspace_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswupper, int (*f)(wint_t));
  FUNCTION(iswupper_l, int (*f)(wint_t, locale_t));
  FUNCTION(iswxdigit, int (*f)(wint_t));
  FUNCTION(iswxdigit_l, int (*f)(wint_t, locale_t));
  FUNCTION(towctrans, wint_t (*f)(wint_t, wctrans_t));
  FUNCTION(towctrans_l, wint_t (*f)(wint_t, wctrans_t, locale_t));
  FUNCTION(towlower, wint_t (*f)(wint_t));
  FUNCTION(towlower_l, wint_t (*f)(wint_t, locale_t));
  FUNCTION(towupper, wint_t (*f)(wint_t));
  FUNCTION(towupper_l, wint_t (*f)(wint_t, locale_t));
  FUNCTION(wctrans, wctrans_t (*f)(const char*));
  FUNCTION(wctrans_l, wctrans_t (*f)(const char*, locale_t));
  FUNCTION(wctype, wctype_t (*f)(const char*));
  FUNCTION(wctype_l, wctype_t (*f)(const char*, locale_t));
}
