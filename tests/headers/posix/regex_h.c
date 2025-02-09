// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <regex.h>

#include "header_checks.h"

static void regex_h() {
  TYPE(regex_t);
  STRUCT_MEMBER(regex_t, size_t, re_nsub);

  TYPE(size_t);

  TYPE(regmatch_t);
  STRUCT_MEMBER(regmatch_t, regoff_t, rm_so);
  STRUCT_MEMBER(regmatch_t, regoff_t, rm_eo);

  MACRO(REG_EXTENDED);
  MACRO(REG_ICASE);
  MACRO(REG_NOSUB);
  MACRO(REG_NEWLINE);

  MACRO(REG_NOTBOL);
  MACRO(REG_NOTEOL);

  MACRO(REG_NOMATCH);
  MACRO(REG_BADPAT);
  MACRO(REG_ECOLLATE);
  MACRO(REG_ECTYPE);
  MACRO(REG_EESCAPE);
  MACRO(REG_ESUBREG);
  MACRO(REG_EBRACK);
  MACRO(REG_EPAREN);
  MACRO(REG_EBRACE);
  MACRO(REG_BADBR);
  MACRO(REG_ERANGE);
  MACRO(REG_ESPACE);
  MACRO(REG_BADRPT);

  FUNCTION(regcomp, int (*f)(regex_t*, const char*, int));
  FUNCTION(regerror, size_t (*f)(int, const regex_t*, char*, size_t));
  FUNCTION(regexec, int (*f)(const regex_t*, const char*, size_t, regmatch_t*, int));
}
