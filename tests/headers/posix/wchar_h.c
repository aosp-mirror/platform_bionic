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

#include "header_checks.h"

static void wchar_h() {
  TYPE(FILE*);
  TYPE(locale_t);
  TYPE(mbstate_t);
  TYPE(size_t);
  TYPE(va_list);
  TYPE(wchar_t);
  TYPE(wctype_t);
  TYPE(wint_t);

  MACRO(WCHAR_MAX);
  MACRO(WCHAR_MIN);
  MACRO(WEOF);
  MACRO(NULL);

  FUNCTION(btowc, wint_t (*f)(int));
  FUNCTION(fgetwc, wint_t (*f)(FILE*));
  FUNCTION(fgetws, wchar_t* (*f)(wchar_t*, int, FILE*));
  FUNCTION(fputwc, wint_t (*f)(wchar_t, FILE*));
  FUNCTION(fputws, int (*f)(const wchar_t*, FILE*));
  FUNCTION(fwide, int (*f)(FILE*, int));
  FUNCTION(fwprintf, int (*f)(FILE*, const wchar_t*, ...));
  FUNCTION(fwscanf, int (*f)(FILE*, const wchar_t*, ...));
  FUNCTION(getwc, wint_t (*f)(FILE*));
  FUNCTION(getwchar, wint_t (*f)(void));
  FUNCTION(mbrlen, size_t (*f)(const char*, size_t, mbstate_t*));
  FUNCTION(mbrtowc, size_t (*f)(wchar_t*, const char*, size_t, mbstate_t*));
  FUNCTION(mbsinit, int (*f)(const mbstate_t*));
  FUNCTION(mbsnrtowcs, size_t (*f)(wchar_t*, const char**, size_t, size_t, mbstate_t*));
  FUNCTION(mbsrtowcs, size_t (*f)(wchar_t*, const char**, size_t, mbstate_t*));
  FUNCTION(open_wmemstream, FILE* (*f)(wchar_t**, size_t*));
  FUNCTION(putwc, wint_t (*f)(wchar_t, FILE*));
  FUNCTION(putwchar, wint_t (*f)(wchar_t));
  FUNCTION(swprintf, int (*f)(wchar_t*, size_t, const wchar_t*, ...));
  FUNCTION(swscanf, int (*f)(const wchar_t*, const wchar_t*, ...));
  FUNCTION(ungetwc, wint_t (*f)(wint_t, FILE*));
  FUNCTION(vfwprintf, int (*f)(FILE*, const wchar_t*, va_list));
  FUNCTION(vfwscanf, int (*f)(FILE*, const wchar_t*, va_list));
  FUNCTION(vswprintf, int (*f)(wchar_t*, size_t, const wchar_t*, va_list));
  FUNCTION(vswscanf, int (*f)(const wchar_t*, const wchar_t*, va_list));
  FUNCTION(vwprintf, int (*f)(const wchar_t*, va_list));
  FUNCTION(vwscanf, int (*f)(const wchar_t*, va_list));
  FUNCTION(wcpcpy, wchar_t* (*f)(wchar_t*, const wchar_t*));
  FUNCTION(wcpncpy, wchar_t* (*f)(wchar_t*, const wchar_t*, size_t));
  FUNCTION(wcrtomb, size_t (*f)(char*, wchar_t, mbstate_t*));
  FUNCTION(wcscasecmp, int (*f)(const wchar_t*, const wchar_t*));
  FUNCTION(wcscasecmp_l, int (*f)(const wchar_t*, const wchar_t*, locale_t));
  FUNCTION(wcscat, wchar_t* (*f)(wchar_t*, const wchar_t*));
  FUNCTION(wcschr, wchar_t* (*f)(const wchar_t*, wchar_t));
  FUNCTION(wcscmp, int (*f)(const wchar_t*, const wchar_t*));
  FUNCTION(wcscoll, int (*f)(const wchar_t*, const wchar_t*));
  FUNCTION(wcscoll_l, int (*f)(const wchar_t*, const wchar_t*, locale_t));
  FUNCTION(wcscpy, wchar_t* (*f)(wchar_t*, const wchar_t*));
  FUNCTION(wcscspn, size_t (*f)(const wchar_t*, const wchar_t*));
  FUNCTION(wcsdup, wchar_t* (*f)(const wchar_t*));
  FUNCTION(wcsftime, size_t (*f)(wchar_t*, size_t, const wchar_t*, const struct tm*));
  FUNCTION(wcslen, size_t (*f)(const wchar_t*));
  FUNCTION(wcsncasecmp, int (*f)(const wchar_t*, const wchar_t*, size_t));
  FUNCTION(wcsncasecmp_l, int (*f)(const wchar_t*, const wchar_t*, size_t, locale_t));
  FUNCTION(wcsncat, wchar_t* (*f)(wchar_t*, const wchar_t*, size_t));
  FUNCTION(wcsncmp, int (*f)(const wchar_t*, const wchar_t*, size_t));
  FUNCTION(wcsncpy, wchar_t* (*f)(wchar_t*, const wchar_t*, size_t));
  FUNCTION(wcsnlen, size_t (*f)(const wchar_t*, size_t));
  FUNCTION(wcsnrtombs, size_t (*f)(char*, const wchar_t**, size_t, size_t, mbstate_t*));
  FUNCTION(wcspbrk, wchar_t* (*f)(const wchar_t*, const wchar_t*));
  FUNCTION(wcsrchr, wchar_t* (*f)(const wchar_t*, wchar_t));
  FUNCTION(wcsrtombs, size_t (*f)(char*, const wchar_t**, size_t, mbstate_t*));
  FUNCTION(wcsspn, size_t (*f)(const wchar_t*, const wchar_t*));
  FUNCTION(wcsstr, wchar_t* (*f)(const wchar_t*, const wchar_t*));
  FUNCTION(wcstod, double (*f)(const wchar_t*, wchar_t**));
  FUNCTION(wcstof, float (*f)(const wchar_t*, wchar_t**));
  FUNCTION(wcstok, wchar_t* (*f)(wchar_t*, const wchar_t*, wchar_t**));
  FUNCTION(wcstol, long (*f)(const wchar_t*, wchar_t**, int));
  FUNCTION(wcstold, long double (*f)(const wchar_t*, wchar_t**));
  FUNCTION(wcstoll, long long (*f)(const wchar_t*, wchar_t**, int));
  FUNCTION(wcstoul, unsigned long (*f)(const wchar_t*, wchar_t**, int));
  FUNCTION(wcstoull, unsigned long long (*f)(const wchar_t*, wchar_t**, int));
  FUNCTION(wcswidth, int (*f)(const wchar_t*, size_t));
  FUNCTION(wcsxfrm, size_t (*f)(wchar_t*, const wchar_t*, size_t));
  FUNCTION(wcsxfrm_l, size_t (*f)(wchar_t*, const wchar_t*, size_t, locale_t));
  FUNCTION(wctob, int (*f)(wint_t));
  FUNCTION(wcwidth, int (*f)(wchar_t));
  FUNCTION(wmemchr, wchar_t* (*f)(const wchar_t*, wchar_t, size_t));
  FUNCTION(wmemcmp, int (*f)(const wchar_t*, const wchar_t*, size_t));
  FUNCTION(wmemcpy, wchar_t* (*f)(wchar_t*, const wchar_t*, size_t));
  FUNCTION(wmemmove, wchar_t* (*f)(wchar_t*, const wchar_t*, size_t));
  FUNCTION(wmemset, wchar_t* (*f)(wchar_t*, wchar_t, size_t));
  FUNCTION(wprintf, int (*f)(const wchar_t*, ...));
  FUNCTION(wscanf, int (*f)(const wchar_t*, ...));
}
