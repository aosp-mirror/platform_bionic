// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <strings.h>

#include "header_checks.h"

static void strings_h() {
  FUNCTION(ffs, int (*f)(int));
#if !defined(__GLIBC__)
  FUNCTION(ffsl, int (*f)(long));
  FUNCTION(ffsll, int (*f)(long long));
#endif
  FUNCTION(strcasecmp, int (*f)(const char*, const char*));
  FUNCTION(strcasecmp_l, int (*f)(const char*, const char*, locale_t));
  FUNCTION(strncasecmp, int (*f)(const char*, const char*, size_t));
  FUNCTION(strncasecmp_l, int (*f)(const char*, const char*, size_t, locale_t));

  TYPE(locale_t);
  TYPE(size_t);
}
