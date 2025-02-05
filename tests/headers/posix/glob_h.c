// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <glob.h>

#include "header_checks.h"

static void glob_h() {
  TYPE(glob_t);
  STRUCT_MEMBER(glob_t, size_t, gl_pathc);
  STRUCT_MEMBER(glob_t, char**, gl_pathv);
  STRUCT_MEMBER(glob_t, size_t, gl_offs);
  TYPE(size_t);

  MACRO(GLOB_APPEND);
  MACRO(GLOB_DOOFFS);
  MACRO(GLOB_ERR);
  MACRO(GLOB_MARK);
  MACRO(GLOB_NOCHECK);
  MACRO(GLOB_NOESCAPE);
  MACRO(GLOB_NOSORT);

  MACRO(GLOB_ABORTED);
  MACRO(GLOB_NOMATCH);
  MACRO(GLOB_NOSPACE);

  FUNCTION(glob, int (*f)(const char*, int, int (*)(const char*, int), glob_t*));
  FUNCTION(globfree, void (*f)(glob_t*));
}
