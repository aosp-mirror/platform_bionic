// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <ctype.h>

#include "header_checks.h"

static void ctype_h() {
  FUNCTION(isalnum, int (*f)(int));
  FUNCTION(isalnum_l, int (*f)(int, locale_t));
  FUNCTION(isalpha, int (*f)(int));
  FUNCTION(isalpha_l, int (*f)(int, locale_t));
  FUNCTION(isascii, int (*f)(int));
  FUNCTION(isblank, int (*f)(int));
  FUNCTION(isblank_l, int (*f)(int, locale_t));
  FUNCTION(iscntrl, int (*f)(int));
  FUNCTION(iscntrl_l, int (*f)(int, locale_t));
  FUNCTION(isdigit, int (*f)(int));
  FUNCTION(isdigit_l, int (*f)(int, locale_t));
  FUNCTION(isgraph, int (*f)(int));
  FUNCTION(isgraph_l, int (*f)(int, locale_t));
  FUNCTION(islower, int (*f)(int));
  FUNCTION(islower_l, int (*f)(int, locale_t));
  FUNCTION(isprint, int (*f)(int));
  FUNCTION(isprint_l, int (*f)(int, locale_t));
  FUNCTION(ispunct, int (*f)(int));
  FUNCTION(ispunct_l, int (*f)(int, locale_t));
  FUNCTION(isspace, int (*f)(int));
  FUNCTION(isspace_l, int (*f)(int, locale_t));
  FUNCTION(isupper, int (*f)(int));
  FUNCTION(isupper_l, int (*f)(int, locale_t));
  FUNCTION(isxdigit, int (*f)(int));
  FUNCTION(isxdigit_l, int (*f)(int, locale_t));

  FUNCTION(toascii, int (*f)(int));
  FUNCTION(tolower, int (*f)(int));
  FUNCTION(tolower_l, int (*f)(int, locale_t));
  FUNCTION(toupper, int (*f)(int));
  FUNCTION(toupper_l, int (*f)(int, locale_t));

#if !defined(__BIONIC__) // These are marked obsolescent.
  #if !defined(_toupper)
    #error _toupper
  #endif
  #if !defined(_tolower)
    #error _tolower
  #endif
#endif
}
