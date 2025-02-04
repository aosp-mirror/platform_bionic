// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/times.h>

#include "header_checks.h"

static void sys_times_h() {
  TYPE(struct tms);
  STRUCT_MEMBER(struct tms, clock_t, tms_utime);
  STRUCT_MEMBER(struct tms, clock_t, tms_stime);
  STRUCT_MEMBER(struct tms, clock_t, tms_cutime);
  STRUCT_MEMBER(struct tms, clock_t, tms_cstime);

  TYPE(clock_t);

  FUNCTION(times, clock_t (*f)(struct tms*));
}
