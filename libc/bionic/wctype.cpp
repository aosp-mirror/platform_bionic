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

int iswalnum(wint_t wc) { return __icu_hasBinaryProperty(wc, UCHAR_POSIX_ALNUM, isalnum); }
int iswalpha(wint_t wc) { return __icu_hasBinaryProperty(wc, UCHAR_ALPHABETIC, isalpha); }
int iswblank(wint_t wc) { return __icu_hasBinaryProperty(wc, UCHAR_POSIX_BLANK, isblank); }
int iswgraph(wint_t wc) { return __icu_hasBinaryProperty(wc, UCHAR_POSIX_GRAPH, isgraph); }
int iswlower(wint_t wc) { return __icu_hasBinaryProperty(wc, UCHAR_LOWERCASE, islower); }
int iswprint(wint_t wc) { return __icu_hasBinaryProperty(wc, UCHAR_POSIX_PRINT, isprint); }
int iswspace(wint_t wc) { return __icu_hasBinaryProperty(wc, UCHAR_WHITE_SPACE, isspace); }
int iswupper(wint_t wc) { return __icu_hasBinaryProperty(wc, UCHAR_UPPERCASE, isupper); }
int iswxdigit(wint_t wc) { return __icu_hasBinaryProperty(wc, UCHAR_POSIX_XDIGIT, isxdigit); }

int iswcntrl(wint_t wc) {
  typedef int8_t (*FnT)(UChar32);
  static auto u_charType = reinterpret_cast<FnT>(__find_icu_symbol("u_charType"));
  return u_charType ? (u_charType(wc) == U_CONTROL_CHAR) : iscntrl(wc);
}

int iswdigit(wint_t wc) {
  typedef UBool (*FnT)(UChar32);
  static auto u_isdigit = reinterpret_cast<FnT>(__find_icu_symbol("u_isdigit"));
  return u_isdigit ? u_isdigit(wc) : isdigit(wc);
}

int iswpunct(wint_t wc) {
  typedef UBool (*FnT)(UChar32);
  static auto u_ispunct = reinterpret_cast<FnT>(__find_icu_symbol("u_ispunct"));
  return u_ispunct ? u_ispunct(wc) : ispunct(wc);
}

int iswalnum_l(wint_t c, locale_t) { return iswalnum(c); }
int iswalpha_l(wint_t c, locale_t) { return iswalpha(c); }
int iswblank_l(wint_t c, locale_t) { return iswblank(c); }
int iswcntrl_l(wint_t c, locale_t) { return iswcntrl(c); }
int iswdigit_l(wint_t c, locale_t) { return iswdigit(c); }
int iswgraph_l(wint_t c, locale_t) { return iswgraph(c); }
int iswlower_l(wint_t c, locale_t) { return iswlower(c); }
int iswprint_l(wint_t c, locale_t) { return iswprint(c); }
int iswpunct_l(wint_t c, locale_t) { return iswpunct(c); }
int iswspace_l(wint_t c, locale_t) { return iswspace(c); }
int iswupper_l(wint_t c, locale_t) { return iswupper(c); }
int iswxdigit_l(wint_t c, locale_t) { return iswxdigit(c); }

int iswctype(wint_t wc, wctype_t char_class) {
  switch (char_class) {
    case WC_TYPE_ALNUM: return iswalnum(wc);
    case WC_TYPE_ALPHA: return iswalpha(wc);
    case WC_TYPE_BLANK: return iswblank(wc);
    case WC_TYPE_CNTRL: return iswcntrl(wc);
    case WC_TYPE_DIGIT: return iswdigit(wc);
    case WC_TYPE_GRAPH: return iswgraph(wc);
    case WC_TYPE_LOWER: return iswlower(wc);
    case WC_TYPE_PRINT: return iswprint(wc);
    case WC_TYPE_PUNCT: return iswpunct(wc);
    case WC_TYPE_SPACE: return iswspace(wc);
    case WC_TYPE_UPPER: return iswupper(wc);
    case WC_TYPE_XDIGIT: return iswxdigit(wc);
    default: return 0;
  }
}

int iswctype_l(wint_t wc, wctype_t char_class, locale_t) {
  return iswctype(wc, char_class);
}

wint_t towlower(wint_t wc) {
  if (wc < 0x80) {
    if (wc >= 'A' && wc <= 'Z') return wc | 0x20;
    return wc;
  }

  typedef UChar32 (*FnT)(UChar32);
  static auto u_tolower = reinterpret_cast<FnT>(__find_icu_symbol("u_tolower"));
  return u_tolower ? u_tolower(wc) : tolower(wc);
}

wint_t towupper(wint_t wc) {
  if (wc < 0x80) {
    // Using EOR rather than AND makes no difference on arm, but saves an
    // instruction on arm64.
    if (wc >= 'a' && wc <= 'z') return wc ^ 0x20;
    return wc;
  }

  typedef UChar32 (*FnT)(UChar32);
  static auto u_toupper = reinterpret_cast<FnT>(__find_icu_symbol("u_toupper"));
  return u_toupper ? u_toupper(wc) : toupper(wc);
}

wint_t towupper_l(wint_t c, locale_t) { return towupper(c); }
wint_t towlower_l(wint_t c, locale_t) { return towlower(c); }

wctype_t wctype(const char* property) {
  static const char* const  properties[WC_TYPE_MAX] = {
    "<invalid>",
    "alnum", "alpha", "blank", "cntrl", "digit", "graph",
    "lower", "print", "punct", "space", "upper", "xdigit"
  };
  for (size_t i = 0; i < WC_TYPE_MAX; ++i) {
    if (!strcmp(properties[i], property)) {
      return static_cast<wctype_t>(i);
    }
  }
  return static_cast<wctype_t>(0);
}

wctype_t wctype_l(const char* property, locale_t) {
  return wctype(property);
}

static wctrans_t wctrans_tolower = wctrans_t(1);
static wctrans_t wctrans_toupper = wctrans_t(2);

wctrans_t wctrans(const char* name) {
  if (strcmp(name, "tolower") == 0) return wctrans_tolower;
  if (strcmp(name, "toupper") == 0) return wctrans_toupper;
  return nullptr;
}

wctrans_t wctrans_l(const char* name, locale_t) {
  return wctrans(name);
}

wint_t towctrans(wint_t c, wctrans_t t) {
  if (t == wctrans_tolower) return towlower(c);
  if (t == wctrans_toupper) return towupper(c);
  errno = EINVAL;
  return 0;
}

wint_t towctrans_l(wint_t c, wctrans_t t, locale_t) {
  return towctrans(c, t);
}
