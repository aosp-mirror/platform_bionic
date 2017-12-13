/*
 * Copyright (C) 2017 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

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
