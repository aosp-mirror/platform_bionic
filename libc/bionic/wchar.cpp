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
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* stubs for wide-char functions */

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

int mbsinit(const mbstate_t* /*ps*/) {
  return 1;
}

size_t mbrtowc(wchar_t* pwc, const char* s, size_t n, mbstate_t* /*ps*/) {
  if (s == NULL) {
    return 0;
  }
  if (n == 0) {
    return 0;
  }
  if (pwc != NULL) {
    *pwc = *s;
  }
  return (*s != 0);
}

size_t mbsnrtowcs(wchar_t* dst, const char** src, size_t n, size_t dst_size, mbstate_t* /*ps*/) {
  size_t i = 0; // Number of input bytes read.
  size_t o = 0; // Number of output characters written.
  for (; i < n && (*src)[i] != 0; ++i) {
    // TODO: UTF-8 support.
    if (static_cast<uint8_t>((*src)[i]) > 0x7f) {
      errno = EILSEQ;
      if (dst != NULL) {
        *src = &(*src)[i];
      }
      return static_cast<size_t>(-1);
    }
    if (dst != NULL) {
      if (o + 1 > dst_size) {
        break;
      }
      dst[o++] = static_cast<wchar_t>((*src)[i]);
    } else {
      ++o;
    }
  }
  // If we consumed all the input, terminate the output.
  if (dst != NULL && o < dst_size) {
    dst[o] = 0;
  }
  // If we were actually consuming input, record how far we got.
  if (dst != NULL) {
    if ((*src)[i] != 0) {
      *src = &(*src)[i]; // This is where the next call should pick up.
    } else {
      *src = NULL; // We consumed everything.
    }
  }
  return o;
}

size_t mbsrtowcs(wchar_t* dst, const char** src, size_t dst_size, mbstate_t* ps) {
  return mbsnrtowcs(dst, src, SIZE_MAX, dst_size, ps);
}

wint_t towlower(wint_t wc) {
  return tolower(wc);
}

wint_t towupper(wint_t wc) {
  return toupper(wc);
}

int wctomb(char* s, wchar_t wc) {
  if (s == NULL) {
    return 0;
  }
  if (wc <= 0xff) {
    *s = static_cast<char>(wc);
  } else {
    *s = '?';
  }
  return 1;
}

size_t wcrtomb(char* s, wchar_t wc, mbstate_t* /*ps*/) {
  if (s == NULL) {
    char buf[MB_LEN_MAX];
    return wctomb(buf, L'\0');
  }
  return wctomb(s, wc);
}

size_t wcsftime(wchar_t* wcs, size_t maxsize, const wchar_t* format,  const struct tm* timptr) {
  return strftime(reinterpret_cast<char*>(wcs), maxsize, reinterpret_cast<const char*>(format), timptr);
}

size_t wcsnrtombs(char* dst, const wchar_t** src, size_t n, size_t dst_size, mbstate_t* /*ps*/) {
  size_t i = 0; // Number of input characters read.
  size_t o = 0; // Number of output bytes written.
  for (; i < n && (*src)[i] != 0; ++i) {
    // TODO: UTF-8 support.
    if ((*src)[i] > 0x7f) {
      errno = EILSEQ;
      if (dst != NULL) {
        *src = &(*src)[i];
      }
      return static_cast<size_t>(-1);
    }
    if (dst != NULL) {
      if (o + 1 > dst_size) {
        break;
      }
      dst[o++] = static_cast<char>((*src)[i]);
    } else {
      ++o;
    }
  }
  // If we consumed all the input, terminate the output.
  if (dst != NULL && o < dst_size) {
    dst[o] = 0;
  }
  // If we were actually consuming input, record how far we got.
  if (dst != NULL) {
    if ((*src)[i] != 0) {
      *src = &(*src)[i]; // This is where the next call should pick up.
    } else {
      *src = NULL; // We consumed everything.
    }
  }
  return o;
}

size_t wcsrtombs(char* dst, const wchar_t** src, size_t dst_size, mbstate_t* ps) {
  return wcsnrtombs(dst, src, SIZE_MAX, dst_size, ps);
}

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
