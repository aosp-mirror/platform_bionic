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

#include <locale.h>

#include "header_checks.h"

static void locale_h() {
  TYPE(struct lconv);
  STRUCT_MEMBER(struct lconv, char*, currency_symbol);
  STRUCT_MEMBER(struct lconv, char*, decimal_point);
  STRUCT_MEMBER(struct lconv, char, frac_digits);
  STRUCT_MEMBER(struct lconv, char*, grouping);
  STRUCT_MEMBER(struct lconv, char*, int_curr_symbol);
  STRUCT_MEMBER(struct lconv, char, int_frac_digits);
  STRUCT_MEMBER(struct lconv, char, int_n_cs_precedes);
  STRUCT_MEMBER(struct lconv, char, int_n_sep_by_space);
  STRUCT_MEMBER(struct lconv, char, int_n_sign_posn);
  STRUCT_MEMBER(struct lconv, char, int_p_cs_precedes);
  STRUCT_MEMBER(struct lconv, char, int_p_sep_by_space);
  STRUCT_MEMBER(struct lconv, char, int_p_sign_posn);
  STRUCT_MEMBER(struct lconv, char*, mon_decimal_point);
  STRUCT_MEMBER(struct lconv, char*, mon_grouping);
  STRUCT_MEMBER(struct lconv, char*, mon_thousands_sep);
  STRUCT_MEMBER(struct lconv, char*, negative_sign);
  STRUCT_MEMBER(struct lconv, char, n_cs_precedes);
  STRUCT_MEMBER(struct lconv, char, n_sep_by_space);
  STRUCT_MEMBER(struct lconv, char, n_sign_posn);
  STRUCT_MEMBER(struct lconv, char*, positive_sign);
  STRUCT_MEMBER(struct lconv, char, p_cs_precedes);
  STRUCT_MEMBER(struct lconv, char, p_sep_by_space);
  STRUCT_MEMBER(struct lconv, char, p_sign_posn);
  STRUCT_MEMBER(struct lconv, char*, thousands_sep);

  MACRO(NULL);

  MACRO(LC_ALL);
  MACRO(LC_COLLATE);
  MACRO(LC_CTYPE);
  MACRO(LC_MONETARY);
  MACRO(LC_NUMERIC);
  MACRO(LC_TIME);

  MACRO(LC_COLLATE_MASK);
  MACRO(LC_CTYPE_MASK);
  MACRO(LC_MESSAGES_MASK);
  MACRO(LC_MONETARY_MASK);
  MACRO(LC_NUMERIC_MASK);
  MACRO(LC_TIME_MASK);
  MACRO(LC_ALL_MASK);

  MACRO_TYPE(locale_t, LC_GLOBAL_LOCALE);
  TYPE(locale_t);

  FUNCTION(duplocale, locale_t (*f)(locale_t));
  FUNCTION(freelocale, void (*f)(locale_t));
  FUNCTION(localeconv, struct lconv* (*f)(void));
  FUNCTION(newlocale, locale_t (*f)(int, const char*, locale_t));
  FUNCTION(setlocale, char* (*f)(int, const char*));
  FUNCTION(uselocale, locale_t (*f)(locale_t));
}
