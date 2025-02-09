// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <iconv.h>

#include "header_checks.h"

static void iconv_h() {
  TYPE(iconv_t);
  TYPE(size_t);

  FUNCTION(iconv, size_t (*f)(iconv_t, char**, size_t*, char**, size_t*));
  FUNCTION(iconv_close, int (*f)(iconv_t));
  FUNCTION(iconv_open, iconv_t (*f)(const char*, const char*));
}
