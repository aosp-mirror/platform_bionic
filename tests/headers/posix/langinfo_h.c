// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <langinfo.h>

#include "header_checks.h"

static void langinfo_h() {
  TYPE(locale_t);
  TYPE(nl_item);

  MACRO_TYPE(nl_item, CODESET);
  MACRO_TYPE(nl_item, D_T_FMT);
  MACRO_TYPE(nl_item, D_FMT);
  MACRO_TYPE(nl_item, T_FMT);
  MACRO_TYPE(nl_item, T_FMT_AMPM);
  MACRO_TYPE(nl_item, AM_STR);
  MACRO_TYPE(nl_item, PM_STR);
  MACRO_TYPE(nl_item, DAY_1);
  MACRO_TYPE(nl_item, DAY_2);
  MACRO_TYPE(nl_item, DAY_3);
  MACRO_TYPE(nl_item, DAY_4);
  MACRO_TYPE(nl_item, DAY_5);
  MACRO_TYPE(nl_item, DAY_6);
  MACRO_TYPE(nl_item, DAY_7);
  MACRO_TYPE(nl_item, ABDAY_1);
  MACRO_TYPE(nl_item, ABDAY_2);
  MACRO_TYPE(nl_item, ABDAY_3);
  MACRO_TYPE(nl_item, ABDAY_4);
  MACRO_TYPE(nl_item, ABDAY_5);
  MACRO_TYPE(nl_item, ABDAY_6);
  MACRO_TYPE(nl_item, ABDAY_7);
  MACRO_TYPE(nl_item, MON_1);
  MACRO_TYPE(nl_item, MON_2);
  MACRO_TYPE(nl_item, MON_3);
  MACRO_TYPE(nl_item, MON_4);
  MACRO_TYPE(nl_item, MON_5);
  MACRO_TYPE(nl_item, MON_6);
  MACRO_TYPE(nl_item, MON_7);
  MACRO_TYPE(nl_item, MON_8);
  MACRO_TYPE(nl_item, MON_9);
  MACRO_TYPE(nl_item, MON_10);
  MACRO_TYPE(nl_item, MON_11);
  MACRO_TYPE(nl_item, MON_12);
  MACRO_TYPE(nl_item, ABMON_1);
  MACRO_TYPE(nl_item, ABMON_2);
  MACRO_TYPE(nl_item, ABMON_3);
  MACRO_TYPE(nl_item, ABMON_4);
  MACRO_TYPE(nl_item, ABMON_5);
  MACRO_TYPE(nl_item, ABMON_6);
  MACRO_TYPE(nl_item, ABMON_7);
  MACRO_TYPE(nl_item, ABMON_8);
  MACRO_TYPE(nl_item, ABMON_9);
  MACRO_TYPE(nl_item, ABMON_10);
  MACRO_TYPE(nl_item, ABMON_11);
  MACRO_TYPE(nl_item, ABMON_12);
  MACRO_TYPE(nl_item, ERA);
  MACRO_TYPE(nl_item, ERA_D_FMT);
  MACRO_TYPE(nl_item, ERA_D_T_FMT);
  MACRO_TYPE(nl_item, ERA_T_FMT);
  MACRO_TYPE(nl_item, ALT_DIGITS);
  MACRO_TYPE(nl_item, RADIXCHAR);
  MACRO_TYPE(nl_item, THOUSEP);
  MACRO_TYPE(nl_item, YESEXPR);
  MACRO_TYPE(nl_item, NOEXPR);
  MACRO_TYPE(nl_item, CRNCYSTR);

  FUNCTION(nl_langinfo, char* (*f)(nl_item));
  FUNCTION(nl_langinfo_l, char* (*f)(nl_item, locale_t));
}
