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

#include <inttypes.h>

#include "header_checks.h"

static void inttypes_h() {
  TYPE(imaxdiv_t);
#if defined(__GLIBC__)
  // Despite POSIX, glibc goes out of its way to avoid defining wchar_t. Fix that.
  typedef __WCHAR_TYPE__ wchar_t;
#endif
  TYPE(wchar_t);

  // TODO: PRI macros
  // TODO: SCN macros

  FUNCTION(imaxabs, intmax_t (*f)(intmax_t));
  FUNCTION(imaxdiv, imaxdiv_t (*f)(intmax_t, intmax_t));
  FUNCTION(strtoimax, intmax_t (*f)(const char*, char**, int));
  FUNCTION(strtoumax, uintmax_t (*f)(const char*, char**, int));
  FUNCTION(wcstoimax, intmax_t (*f)(const wchar_t*, wchar_t**, int));
  FUNCTION(wcstoumax, uintmax_t (*f)(const wchar_t*, wchar_t**, int));
}
