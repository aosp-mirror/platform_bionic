/*
 * Copyright (C) 2008 The Android Open Source Project
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
#include <pthread.h>
#include <stdlib.h>

static pthread_once_t locale_once = PTHREAD_ONCE_INIT;
static lconv locale;

static void __locale_init() {
  locale.decimal_point = const_cast<char*>(".");

  char* not_available = const_cast<char*>("");
  locale.thousands_sep = not_available;
  locale.grouping = not_available;
  locale.int_curr_symbol = not_available;
  locale.currency_symbol = not_available;
  locale.mon_decimal_point = not_available;
  locale.mon_thousands_sep = not_available;
  locale.mon_grouping = not_available;
  locale.positive_sign = not_available;
  locale.negative_sign = not_available;

  locale.int_frac_digits = CHAR_MAX;
  locale.frac_digits = CHAR_MAX;
  locale.p_cs_precedes = CHAR_MAX;
  locale.p_sep_by_space = CHAR_MAX;
  locale.n_cs_precedes = CHAR_MAX;
  locale.n_sep_by_space = CHAR_MAX;
  locale.p_sign_posn = CHAR_MAX;
  locale.n_sign_posn = CHAR_MAX;
  locale.int_p_cs_precedes = CHAR_MAX;
  locale.int_p_sep_by_space = CHAR_MAX;
  locale.int_n_cs_precedes = CHAR_MAX;
  locale.int_n_sep_by_space = CHAR_MAX;
  locale.int_p_sign_posn = CHAR_MAX;
  locale.int_n_sign_posn = CHAR_MAX;
}

lconv* localeconv() {
  pthread_once(&locale_once, __locale_init);
  return &locale;
}

// setlocale(3) always fails on bionic.
char* setlocale(int /*category*/, char const* /*locale*/) {
  return NULL;
}
