// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <nl_types.h>

#include "header_checks.h"

static void nl_types_h() {
  TYPE(nl_catd);
  TYPE(nl_item);

  MACRO(NL_SETD);
  MACRO(NL_CAT_LOCALE);

  FUNCTION(catclose, int (*f)(nl_catd));
  FUNCTION(catgets, char* (*f)(nl_catd, int, int, const char*));
  FUNCTION(catopen, nl_catd (*f)(const char*, int));
}
