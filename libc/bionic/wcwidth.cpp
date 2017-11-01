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

#include <wchar.h>

#include "private/icu.h"

int wcwidth(wchar_t wc) {
  // Fast-path ASCII.
  if (wc >= 0x20 && wc < 0x7f) return 1;

  // ASCII NUL is a special case.
  if (wc == 0) return 0;

  // C0.
  if (wc < ' ' || (wc >= 0x7f && wc <= 0xa0)) return -1;

  // Now for the i18n part. This isn't defined or standardized, so a lot of the choices are
  // pretty arbitrary. See https://www.cl.cam.ac.uk/~mgk25/ucs/wcwidth.c for more details.

  // Fancy unicode control characters?
  switch (__icu_charType(wc)) {
   case -1:
    // No icu4c available; give up.
    return -1;
   case U_CONTROL_CHAR:
    return -1;
   case U_NON_SPACING_MARK:
   case U_ENCLOSING_MARK:
   case U_FORMAT_CHAR:
    return 0;
  }
  if (__icu_hasBinaryProperty(wc, UCHAR_DEFAULT_IGNORABLE_CODE_POINT, nullptr)) return 0;

  // Medial and final jamo render as zero width when used correctly.
  switch (__icu_getIntPropertyValue(wc, UCHAR_HANGUL_SYLLABLE_TYPE)) {
   case U_HST_VOWEL_JAMO:
   case U_HST_TRAILING_JAMO:
    return 0;
   case U_HST_LEADING_JAMO:
   case U_HST_LV_SYLLABLE:
   case U_HST_LVT_SYLLABLE:
    return 2;
  }

  if (wc >= 0x3248 && wc <= 0x4dff) {
    // Circled two-digit CJK "speed sign" numbers. EastAsianWidth is ambiguous,
    // but wide makes more sense.
    if (wc <= 0x324f) return 2;
    // Hexagrams. EastAsianWidth is neutral, but wide seems better.
    if (wc >= 0x4dc0) return 2;
  }

  // The EastAsianWidth property is at least defined by the Unicode standard!
  switch (__icu_getIntPropertyValue(wc, UCHAR_EAST_ASIAN_WIDTH)) {
   case U_EA_AMBIGUOUS:
   case U_EA_HALFWIDTH:
   case U_EA_NARROW:
   case U_EA_NEUTRAL:
    return 1;
   case U_EA_FULLWIDTH:
   case U_EA_WIDE:
    return 2;
  }

  return 0;
}
