/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <stdlib.h>

char* nl_langinfo(nl_item item) {
  const char* result = "";
  switch (item) {
    case CODESET: result = (MB_CUR_MAX == 1) ? "ASCII" : "UTF-8"; break;

    case D_T_FMT: result = "%F %T %z"; break;
    case D_FMT: result = "%F"; break;
    case T_FMT: result = "%T"; break;
    case T_FMT_AMPM: result = "%I:%M:%S %p"; break;
    case AM_STR: result = "AM"; break;
    case PM_STR: result = "PM"; break;
    case DAY_1: result = "Sunday"; break;
    case DAY_2: result = "Monday"; break;
    case DAY_3: result = "Tuesday"; break;
    case DAY_4: result = "Wednesday"; break;
    case DAY_5: result = "Thursday"; break;
    case DAY_6: result = "Friday"; break;
    case DAY_7: result = "Saturday"; break;
    case ABDAY_1: result = "Sun"; break;
    case ABDAY_2: result = "Mon"; break;
    case ABDAY_3: result = "Tue"; break;
    case ABDAY_4: result = "Wed"; break;
    case ABDAY_5: result = "Thu"; break;
    case ABDAY_6: result = "Fri"; break;
    case ABDAY_7: result = "Sat"; break;
    case MON_1: result = "January"; break;
    case MON_2: result = "February"; break;
    case MON_3: result = "March"; break;
    case MON_4: result = "April"; break;
    case MON_5: result = "May"; break;
    case MON_6: result = "June"; break;
    case MON_7: result = "July"; break;
    case MON_8: result = "August"; break;
    case MON_9: result = "September"; break;
    case MON_10: result = "October"; break;
    case MON_11: result = "November"; break;
    case MON_12: result = "December"; break;
    case ABMON_1: result = "Jan"; break;
    case ABMON_2: result = "Feb"; break;
    case ABMON_3: result = "Mar"; break;
    case ABMON_4: result = "Apr"; break;
    case ABMON_5: result = "May"; break;
    case ABMON_6: result = "Jun"; break;
    case ABMON_7: result = "Jul"; break;
    case ABMON_8: result = "Aug"; break;
    case ABMON_9: result = "Sep"; break;
    case ABMON_10: result = "Oct"; break;
    case ABMON_11: result = "Nov"; break;
    case ABMON_12: result = "Dec"; break;
    case ERA: result = ""; break;
    case ERA_D_FMT: result = ""; break;
    case ERA_D_T_FMT: result = ""; break;
    case ERA_T_FMT: result = ""; break;
    case ALT_DIGITS: result = ""; break;

    case RADIXCHAR: result = "."; break;
    case THOUSEP: result = ""; break;

    case YESEXPR: result = "^[yY]"; break;
    case NOEXPR: result = "^[nN]"; break;

    case CRNCYSTR: result = ""; break;

    default: break;
  }
  return const_cast<char*>(result);
}

char* nl_langinfo_l(nl_item item, locale_t) {
  return nl_langinfo(item);
}
