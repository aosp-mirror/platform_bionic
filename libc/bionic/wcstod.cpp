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

#include <wchar.h>

#include <stdlib.h>
#include <string.h>

#include "local.h"

/// Performs wide-character string to floating point conversion.
template <typename float_type>
float_type wcstod(const wchar_t* str, wchar_t** end, float_type strtod_fn(const char*, char**)) {
  const wchar_t* original_str = str;
  while (iswspace(*str)) {
    str++;
  }

  // What's the longest span of the input that might be part of the float?
  size_t max_len = wcsspn(str, L"-+0123456789.xXeEpP()nNaAiIfFtTyY");

  // We know the only valid characters are ASCII, so convert them by brute force.
  char* ascii_str = new char[max_len + 1];
  if (!ascii_str) return float_type();
  for (size_t i = 0; i < max_len; ++i) {
    ascii_str[i] = str[i] & 0xff;
  }
  ascii_str[max_len] = 0;

  // Set up a fake FILE that points to those ASCII characters, for `parsefloat`.
  FILE f;
  __sfileext fext;
  _FILEEXT_SETUP(&f, &fext);
  f._flags = __SRD;
  f._bf._base = f._p = reinterpret_cast<unsigned char*>(ascii_str);
  f._bf._size = f._r = max_len;
  f._read = [](void*, char*, int) { return 0; }; // aka `eofread`, aka "no more data".
  f._lb._base = nullptr;

  // Ask `parsefloat` to look at the same data more carefully.

  // We can't just do this straight away because we can't construct a suitable FILE*
  // in the absence of any `fwmemopen` analogous to `fmemopen`. And we don't want to
  // duplicate the `parsefloat` logic. We also don't want to actually have to have wchar_t
  // implementations of the ASCII `strtod` logic (though if you were designing a libc
  // from scratch, you'd probably want to just make that more generic and lose all the
  // cruft on top).
  size_t actual_len = parsefloat(&f, ascii_str, ascii_str + max_len);

  // Finally let the ASCII conversion function do the work.
  char* ascii_end;
  float_type result = strtod_fn(ascii_str, &ascii_end);
  if (ascii_end != ascii_str + actual_len) abort();

  if (end) {
    if (actual_len == 0) {
      // There was an error. We need to set the end pointer back to the original string, not the
      // one we advanced past the leading whitespace.
      *end = const_cast<wchar_t*>(original_str);
    } else {
      *end = const_cast<wchar_t*>(str) + actual_len;
    }
  }

  delete[] ascii_str;
  return result;
}

float wcstof(const wchar_t* s, wchar_t** end) {
  return wcstod<float>(s, end, strtof);
}

double wcstod(const wchar_t* s, wchar_t** end) {
  return wcstod<double>(s, end, strtod);
}

long double wcstold(const wchar_t* s, wchar_t** end) {
  return wcstod<long double>(s, end, strtold);
}
