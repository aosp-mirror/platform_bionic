// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <ftw.h>

#include "header_checks.h"

static void ftw_h() {
  TYPE(struct FTW);
  STRUCT_MEMBER(struct FTW, int, base);
  STRUCT_MEMBER(struct FTW, int, level);

  MACRO(FTW_F);
  MACRO(FTW_D);
  MACRO(FTW_DNR);
  MACRO(FTW_DP);
  MACRO(FTW_NS);
  MACRO(FTW_SL);
  MACRO(FTW_SLN);

  MACRO(FTW_PHYS);
  MACRO(FTW_MOUNT);
  MACRO(FTW_DEPTH);
  MACRO(FTW_CHDIR);

  FUNCTION(ftw, int (*f)(const char*, int (*)(const char*, const struct stat*, int), int));

  TYPE(struct stat);

  // POSIX: "The <ftw.h> header shall define the ... the symbolic names for
  // st_mode and the file type test macros as described in <sys/stat.h>."
#include "sys_stat_h_mode_constants.h"
#include "sys_stat_h_file_type_test_macros.h"
}
