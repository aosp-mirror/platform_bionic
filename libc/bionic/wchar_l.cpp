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
#include <xlocale.h>

int wcscasecmp_l(const wchar_t* ws1, const wchar_t* ws2, locale_t) {
  return wcscasecmp(ws1, ws2);
}

int wcsncasecmp_l(const wchar_t* ws1, const wchar_t* ws2, size_t n, locale_t) {
  return wcsncasecmp(ws1, ws2, n);
}

int wcscoll_l(const wchar_t* ws1, const wchar_t* ws2, locale_t) {
  return wcscoll(ws1, ws2);
}

double wcstod_l(const wchar_t* s, wchar_t** end_ptr, locale_t) {
  return wcstod(s, end_ptr);
}

float wcstof_l(const wchar_t* s, wchar_t** end_ptr, locale_t) {
  return wcstof(s, end_ptr);
}

long wcstol_l(const wchar_t* s, wchar_t** end_ptr, int base, locale_t) {
  return wcstol(s, end_ptr, base);
}

long long wcstoll_l(const wchar_t* s, wchar_t** end_ptr, int base, locale_t) {
  return wcstoll(s, end_ptr, base);
}

unsigned long wcstoul_l(const wchar_t* s, wchar_t** end_ptr, int base, locale_t) {
  return wcstoul(s, end_ptr, base);
}

unsigned long long wcstoull_l(const wchar_t* s, wchar_t** end_ptr, int base, locale_t) {
  return wcstoull(s, end_ptr, base);
}

long double wcstold_l(const wchar_t* s, wchar_t** end_ptr, locale_t) {
  return wcstold(s, end_ptr);
}

size_t wcsxfrm_l(wchar_t* dst, const wchar_t* src, size_t n, locale_t) {
  return wcsxfrm(dst, src, n);
}
