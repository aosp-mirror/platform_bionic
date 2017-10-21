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
