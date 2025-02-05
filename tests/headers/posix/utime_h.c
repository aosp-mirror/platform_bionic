// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <utime.h>

#include "header_checks.h"

static void utime_h() {
  TYPE(struct utimbuf);
  STRUCT_MEMBER(struct utimbuf, time_t, actime);
  STRUCT_MEMBER(struct utimbuf, time_t, modtime);

  TYPE(time_t);

  FUNCTION(utime, int (*f)(const char*, const struct utimbuf*));
}
