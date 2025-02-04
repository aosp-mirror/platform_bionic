// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <fnmatch.h>

#include "header_checks.h"

static void fnmatch_h() {
  MACRO(FNM_NOMATCH);
  MACRO(FNM_PATHNAME);
  MACRO(FNM_PERIOD);
  MACRO(FNM_NOESCAPE);

  FUNCTION(fnmatch, int (*f)(const char*, const char*, int));
}
