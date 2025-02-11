// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

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
