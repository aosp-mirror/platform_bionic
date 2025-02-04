// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/statvfs.h>

#include "header_checks.h"

static void sys_statvfs_h() {
  TYPE(struct statvfs);
  STRUCT_MEMBER(struct statvfs, unsigned long, f_bsize);
  STRUCT_MEMBER(struct statvfs, unsigned long, f_frsize);
  STRUCT_MEMBER(struct statvfs, fsblkcnt_t, f_blocks);
  STRUCT_MEMBER(struct statvfs, fsblkcnt_t, f_bfree);
  STRUCT_MEMBER(struct statvfs, fsblkcnt_t, f_bavail);
  STRUCT_MEMBER(struct statvfs, fsfilcnt_t, f_files);
  STRUCT_MEMBER(struct statvfs, fsfilcnt_t, f_ffree);
  STRUCT_MEMBER(struct statvfs, fsfilcnt_t, f_favail);
  STRUCT_MEMBER(struct statvfs, unsigned long, f_fsid);
  STRUCT_MEMBER(struct statvfs, unsigned long, f_flag);
  STRUCT_MEMBER(struct statvfs, unsigned long, f_namemax);

  TYPE(fsblkcnt_t);
  TYPE(fsfilcnt_t);

  MACRO(ST_RDONLY);
  MACRO(ST_NOSUID);

  FUNCTION(fstatvfs, int (*f)(int, struct statvfs*));
  FUNCTION(statvfs, int (*f)(const char*, struct statvfs*));
}
