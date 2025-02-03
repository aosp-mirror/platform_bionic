// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/utsname.h>

#include "header_checks.h"

static void sys_utsname_h() {
  TYPE(struct utsname);
  STRUCT_MEMBER_ARRAY(struct utsname, char/*[]*/, sysname);
  STRUCT_MEMBER_ARRAY(struct utsname, char/*[]*/, nodename);
  STRUCT_MEMBER_ARRAY(struct utsname, char/*[]*/, release);
  STRUCT_MEMBER_ARRAY(struct utsname, char/*[]*/, version);
  STRUCT_MEMBER_ARRAY(struct utsname, char/*[]*/, machine);

  FUNCTION(uname, int (*f)(struct utsname*));
}
