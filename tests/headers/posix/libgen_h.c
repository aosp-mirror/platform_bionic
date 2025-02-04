// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <libgen.h>

#include "header_checks.h"

static void libgen_h() {
#if defined(__BIONIC__) // bionic doesn't modify its argument, and admits as much.
  FUNCTION(basename, char* (*f)(const char*));
  FUNCTION(dirname, char* (*f)(const char*));
#else
  FUNCTION(basename, char* (*f)(char*));
  FUNCTION(dirname, char* (*f)(char*));
#endif
}
