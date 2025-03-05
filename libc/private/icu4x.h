/*
 * Copyright (C) 2016 The Android Open Source Project
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

#pragma once

#include <ctype.h>
#include <stdint.h>
#include <wchar.h>

enum UCharCategory {
  U_NON_SPACING_MARK = 6,
  U_ENCLOSING_MARK = 7,
  U_DECIMAL_NUMBER = 9,
  U_CONTROL_CHAR = 15,
  U_FORMAT_CHAR = 16,
  U_DASH_PUNCTUATION = 19,
  U_OTHER_PUNCTUATION = 23,
};

enum UEastAsianWidth {
  U_EA_NEUTRAL,
  U_EA_AMBIGUOUS,
  U_EA_HALFWIDTH,
  U_EA_FULLWIDTH,
  U_EA_NARROW,
  U_EA_WIDE,
};

enum UHangulSyllableType {
  U_HST_NOT_APPLICABLE,
  U_HST_LEADING_JAMO,
  U_HST_VOWEL_JAMO,
  U_HST_TRAILING_JAMO,
  U_HST_LV_SYLLABLE,
  U_HST_LVT_SYLLABLE,
};

__BEGIN_DECLS

uint8_t __icu4x_bionic_general_category(uint32_t cp);
uint8_t __icu4x_bionic_east_asian_width(uint32_t cp);
uint8_t __icu4x_bionic_hangul_syllable_type(uint32_t cp);

bool __icu4x_bionic_is_alphabetic(uint32_t cp);
bool __icu4x_bionic_is_default_ignorable_code_point(uint32_t cp);
bool __icu4x_bionic_is_lowercase(uint32_t cp);
bool __icu4x_bionic_is_alnum(uint32_t cp);
bool __icu4x_bionic_is_blank(uint32_t cp);
bool __icu4x_bionic_is_graph(uint32_t cp);
bool __icu4x_bionic_is_print(uint32_t cp);
bool __icu4x_bionic_is_xdigit(uint32_t cp);
bool __icu4x_bionic_is_white_space(uint32_t cp);
bool __icu4x_bionic_is_uppercase(uint32_t cp);

uint32_t __icu4x_bionic_to_upper(uint32_t ch);
uint32_t __icu4x_bionic_to_lower(uint32_t ch);

__END_DECLS
