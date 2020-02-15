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

#include <sys/stat.h>

#include "header_checks.h"

static void sys_stat_h() {
  TYPE(struct stat);
#if defined(__BIONIC__) && (defined(__arm__) || defined(__i386__))
  STRUCT_MEMBER(struct stat, unsigned long long, st_dev);
#else
  STRUCT_MEMBER(struct stat, dev_t, st_dev);
#endif
#if defined(__BIONIC__) && !defined(__LP64__)
  STRUCT_MEMBER(struct stat, unsigned long long, st_ino);
#else
  STRUCT_MEMBER(struct stat, ino_t, st_ino);
#endif
#if defined(__BIONIC__) && (defined(__arm__) || defined(__i386__))
  STRUCT_MEMBER(struct stat, unsigned int, st_mode);
#else
  STRUCT_MEMBER(struct stat, mode_t, st_mode);
#endif
#if defined(__BIONIC__) && defined(__x86_64__)
  // We can't just fix the x86_64 nlink_t because it's ABI via <fts.h>.
  STRUCT_MEMBER(struct stat, unsigned long, st_nlink);
#else
  STRUCT_MEMBER(struct stat, nlink_t, st_nlink);
#endif
  STRUCT_MEMBER(struct stat, uid_t, st_uid);
  STRUCT_MEMBER(struct stat, gid_t, st_gid);
#if defined(__BIONIC__) && (defined(__arm__) || defined(__i386__))
  STRUCT_MEMBER(struct stat, unsigned long long, st_rdev);
#else
  STRUCT_MEMBER(struct stat, dev_t, st_rdev);
#endif
#if defined(__BIONIC__) && !defined(__LP64__)
  STRUCT_MEMBER(struct stat, long long, st_size);
#else
  STRUCT_MEMBER(struct stat, off_t, st_size);
#endif
  STRUCT_MEMBER(struct stat, struct timespec, st_atim);
  STRUCT_MEMBER(struct stat, struct timespec, st_mtim);
  STRUCT_MEMBER(struct stat, struct timespec, st_ctim);
#if defined(__BIONIC__)
#if defined(__aarch64__)
  STRUCT_MEMBER(struct stat, int, st_blksize);
#elif defined(__x86_64__)
  STRUCT_MEMBER(struct stat, long, st_blksize);
#else
  STRUCT_MEMBER(struct stat, unsigned long, st_blksize);
#endif
#else
  STRUCT_MEMBER(struct stat, blksize_t, st_blksize);
#endif
#if defined(__BIONIC__)
#if defined(__LP64__)
  STRUCT_MEMBER(struct stat, long, st_blocks);
#else
  STRUCT_MEMBER(struct stat, unsigned long long, st_blocks);
#endif
#else
  STRUCT_MEMBER(struct stat, blkcnt_t, st_blocks);
#endif

  TYPE(blkcnt_t);
  TYPE(blksize_t);
  TYPE(dev_t);
  TYPE(ino_t);
  TYPE(mode_t);
  TYPE(nlink_t);
  TYPE(uid_t);
  TYPE(gid_t);
  TYPE(off_t);
  TYPE(time_t);

  TYPE(struct timespec);

#if !defined(st_atime)
#error st_atime
#endif
#if !defined(st_ctime)
#error st_ctime
#endif
#if !defined(st_mtime)
#error st_mtime
#endif

#include "sys_stat_h_mode_constants.h"
#include "sys_stat_h_file_type_test_macros.h"

#if !defined(S_TYPEISMQ)
#error S_TYPEISMQ
#endif
#if !defined(S_TYPEISSEM)
#error S_TYPEISSEM
#endif
#if !defined(S_TYPEISSHM)
#error S_TYPEISSHM
#endif

#if !defined(__BIONIC__) && !defined(__GLIBC__)
#if !defined(S_TYPEISTMO)
#error S_TYPEISTMO
#endif
#endif

  MACRO(UTIME_NOW);
  MACRO(UTIME_OMIT);

  FUNCTION(chmod, int (*f)(const char*, mode_t));
  FUNCTION(fchmod, int (*f)(int, mode_t));
  FUNCTION(fchmodat, int (*f)(int, const char*, mode_t, int));
  FUNCTION(fstat, int (*f)(int, struct stat*));
  FUNCTION(fstatat, int (*f)(int, const char*, struct stat*, int));
  FUNCTION(futimens, int (*f)(int, const struct timespec[2]));
  FUNCTION(lstat, int (*f)(const char*, struct stat*));
  FUNCTION(lstat, int (*f)(const char*, struct stat*));
  FUNCTION(mkdir, int (*f)(const char*, mode_t));
  FUNCTION(mkdirat, int (*f)(int, const char*, mode_t));
  FUNCTION(mkfifo, int (*f)(const char*, mode_t));
  FUNCTION(mkfifoat, int (*f)(int, const char*, mode_t));
  FUNCTION(mknod, int (*f)(const char*, mode_t, dev_t));
  FUNCTION(mknodat, int (*f)(int, const char*, mode_t, dev_t));
  FUNCTION(stat, int (*f)(const char*, struct stat*));
  FUNCTION(umask, mode_t (*f)(mode_t));
  FUNCTION(utimensat, int (*f)(int, const char*, const struct timespec[2], int));
}
