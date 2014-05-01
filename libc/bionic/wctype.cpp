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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// TODO: these only work for the ASCII range; rewrite to dlsym icu4c?

int iswalnum(wint_t wc) { return isalnum(wc); }
int iswalpha(wint_t wc) { return isalpha(wc); }
int iswblank(wint_t wc) { return isblank(wc); }
int iswcntrl(wint_t wc) { return iscntrl(wc); }
int iswdigit(wint_t wc) { return isdigit(wc); }
int iswgraph(wint_t wc) { return isgraph(wc); }
int iswlower(wint_t wc) { return islower(wc); }
int iswprint(wint_t wc) { return isprint(wc); }
int iswpunct(wint_t wc) { return ispunct(wc); }
int iswspace(wint_t wc) { return isspace(wc); }
int iswupper(wint_t wc) { return isupper(wc); }
int iswxdigit(wint_t wc) { return isxdigit(wc); }

int iswctype(wint_t wc, wctype_t char_class) {
  switch (char_class) {
    case WC_TYPE_ALNUM: return isalnum(wc);
    case WC_TYPE_ALPHA: return isalpha(wc);
    case WC_TYPE_BLANK: return isblank(wc);
    case WC_TYPE_CNTRL: return iscntrl(wc);
    case WC_TYPE_DIGIT: return isdigit(wc);
    case WC_TYPE_GRAPH: return isgraph(wc);
    case WC_TYPE_LOWER: return islower(wc);
    case WC_TYPE_PRINT: return isprint(wc);
    case WC_TYPE_PUNCT: return ispunct(wc);
    case WC_TYPE_SPACE: return isspace(wc);
    case WC_TYPE_UPPER: return isupper(wc);
    case WC_TYPE_XDIGIT: return isxdigit(wc);
    default: return 0;
  }
}

wint_t towlower(wint_t wc) { return tolower(wc); }
wint_t towupper(wint_t wc) { return toupper(wc); }

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

int wcwidth(wchar_t wc) {
  return (wc > 0);
}

// TODO: implement wcsftime.
size_t wcsftime(wchar_t* wcs, size_t maxsize, const wchar_t* format,  const struct tm* timptr) {
  abort();
}
