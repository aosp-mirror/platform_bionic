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

#include <wctype.h>

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "bionic/macros.h"
#include "private/icu.h"

enum {
  WC_TYPE_INVALID = 0,
  WC_TYPE_ALNUM,
  WC_TYPE_ALPHA,
  WC_TYPE_BLANK,
  WC_TYPE_CNTRL,
  WC_TYPE_DIGIT,
  WC_TYPE_GRAPH,
  WC_TYPE_LOWER,
  WC_TYPE_PRINT,
  WC_TYPE_PUNCT,
  WC_TYPE_SPACE,
  WC_TYPE_UPPER,
  WC_TYPE_XDIGIT,
  WC_TYPE_MAX
};

static u_hasBinaryProperty_t __find_u_hasBinaryProperty() {
  static auto u_hasBinaryProperty =
      reinterpret_cast<u_hasBinaryProperty_t>(__find_icu_symbol("u_hasBinaryProperty"));
  return u_hasBinaryProperty;
}

#define DO_ISW(icu_constant, narrow_fn) \
  u_hasBinaryProperty_t u_hasBinaryProperty; \
  if (__predict_true(wc < 0x80) || \
      !(u_hasBinaryProperty = __find_u_hasBinaryProperty())) { \
    return narrow_fn(wc); \
  } \
  return u_hasBinaryProperty(wc, icu_constant); \

int iswalnum(wint_t wc) { DO_ISW(UCHAR_POSIX_ALNUM, isalnum); }
__strong_alias(iswalnum_l, iswalnum);
int iswalpha(wint_t wc) { DO_ISW(UCHAR_ALPHABETIC, isalpha); }
__strong_alias(iswalpha_l, iswalpha);
int iswblank(wint_t wc) { DO_ISW(UCHAR_POSIX_BLANK, isblank); }
__strong_alias(iswblank_l, iswblank);
int iswgraph(wint_t wc) { DO_ISW(UCHAR_POSIX_GRAPH, isgraph); }
__strong_alias(iswgraph_l, iswgraph);
int iswlower(wint_t wc) { DO_ISW(UCHAR_LOWERCASE, islower); }
__strong_alias(iswlower_l, iswlower);
int iswprint(wint_t wc) { DO_ISW(UCHAR_POSIX_PRINT, isprint); }
__strong_alias(iswprint_l, iswprint);
int iswspace(wint_t wc) { DO_ISW(UCHAR_WHITE_SPACE, isspace); }
__strong_alias(iswspace_l, iswspace);
int iswupper(wint_t wc) { DO_ISW(UCHAR_UPPERCASE, isupper); }
__strong_alias(iswupper_l, iswupper);
int iswxdigit(wint_t wc) { DO_ISW(UCHAR_POSIX_XDIGIT, isxdigit); }
__strong_alias(iswxdigit_l, iswxdigit);

int iswcntrl(wint_t wc) {
  if (wc < 0x80) return iscntrl(wc);
  typedef int8_t (*FnT)(UChar32);
  static auto u_charType = reinterpret_cast<FnT>(__find_icu_symbol("u_charType"));
  return u_charType ? (u_charType(wc) == U_CONTROL_CHAR) : iscntrl(wc);
}
__strong_alias(iswcntrl_l, iswcntrl);

int iswdigit(wint_t wc) {
  if (wc < 0x80) return isdigit(wc);
  typedef UBool (*FnT)(UChar32);
  static auto u_isdigit = reinterpret_cast<FnT>(__find_icu_symbol("u_isdigit"));
  return u_isdigit ? u_isdigit(wc) : isdigit(wc);
}
__strong_alias(iswdigit_l, iswdigit);

int iswpunct(wint_t wc) {
  if (wc < 0x80) return ispunct(wc);
  typedef UBool (*FnT)(UChar32);
  static auto u_ispunct = reinterpret_cast<FnT>(__find_icu_symbol("u_ispunct"));
  return u_ispunct ? u_ispunct(wc) : ispunct(wc);
}
__strong_alias(iswpunct_l, iswpunct);

int iswctype(wint_t wc, wctype_t char_class) {
  if (char_class < WC_TYPE_ALNUM || char_class > WC_TYPE_XDIGIT) return 0;
  static int (*fns[])(wint_t) = {
    iswalnum, iswalpha, iswblank, iswcntrl, iswdigit, iswgraph,
    iswlower, iswprint, iswpunct, iswspace, iswupper, iswxdigit
  };
  return fns[char_class - WC_TYPE_ALNUM](wc);
}
__strong_alias(iswctype_l, iswctype);

wint_t towlower(wint_t wc) {
  if (wc < 0x80) return tolower(wc);

  typedef UChar32 (*FnT)(UChar32);
  static auto u_tolower = reinterpret_cast<FnT>(__find_icu_symbol("u_tolower"));
  return u_tolower ? u_tolower(wc) : tolower(wc);
}
__strong_alias(towlower_l, towlower);

wint_t towupper(wint_t wc) {
  if (wc < 0x80) return toupper(wc);

  typedef UChar32 (*FnT)(UChar32);
  static auto u_toupper = reinterpret_cast<FnT>(__find_icu_symbol("u_toupper"));
  return u_toupper ? u_toupper(wc) : toupper(wc);
}
__strong_alias(towupper_l, towupper);

wctype_t wctype(const char* property) {
  static const char* const  properties[WC_TYPE_MAX - 1] = {
    "alnum", "alpha", "blank", "cntrl", "digit", "graph",
    "lower", "print", "punct", "space", "upper", "xdigit"
  };
  for (size_t i = 0; i < arraysize(properties); ++i) {
    if (!strcmp(properties[i], property)) {
      return static_cast<wctype_t>(WC_TYPE_ALNUM + i);
    }
  }
  return static_cast<wctype_t>(0);
}
__strong_alias(wctype_l, wctype);

static wctrans_t wctrans_tolower = wctrans_t(1);
static wctrans_t wctrans_toupper = wctrans_t(2);

wctrans_t wctrans(const char* name) {
  if (strcmp(name, "tolower") == 0) return wctrans_tolower;
  if (strcmp(name, "toupper") == 0) return wctrans_toupper;
  errno = EINVAL;
  return nullptr;
}
__strong_alias(wctrans_l, wctrans);

wint_t towctrans(wint_t c, wctrans_t t) {
  if (t == wctrans_tolower) return towlower(c);
  if (t == wctrans_toupper) return towupper(c);
  errno = EINVAL;
  return c;
}
__strong_alias(towctrans_l, towctrans);
