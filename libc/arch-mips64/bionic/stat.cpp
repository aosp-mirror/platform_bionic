/*
 * Copyright (C) 2015 The Android Open Source Project
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
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

struct kernel_stat {
  unsigned int st_dev;
  unsigned int st_pad0[3];
  unsigned long st_ino;
  mode_t st_mode;
  __u32 st_nlink;
  uid_t st_uid;
  gid_t st_gid;
  unsigned int st_rdev;
  unsigned int st_pad1[3];
  __kernel_off_t st_size;
  unsigned int _st_atime;
  unsigned int _st_atime_nsec;
  unsigned int _st_mtime;
  unsigned int _st_mtime_nsec;
  unsigned int _st_ctime;
  unsigned int _st_ctime_nsec;
  unsigned int st_blksize;
  unsigned int st_pad2;
  unsigned long st_blocks;
};

static void copy_stat(struct stat* st, struct kernel_stat* s) {
  st->st_dev = static_cast<dev_t>(s->st_dev);
  st->st_ino = static_cast<ino_t>(s->st_ino);
  st->st_mode = static_cast<mode_t>(s->st_mode);
  st->st_nlink = static_cast<nlink_t>(s->st_nlink);
  st->st_uid = static_cast<uid_t>(s->st_uid);
  st->st_gid = static_cast<gid_t>(s->st_gid);
  st->st_rdev = static_cast<dev_t>(s->st_rdev);
  st->st_size = static_cast<off_t>(s->st_size);
  st->st_blksize = static_cast<int>(s->st_blksize);
  st->st_blocks = static_cast<long>(s->st_blocks);
  st->st_atim.tv_sec = static_cast<time_t>(s->_st_atime);
  st->st_atim.tv_nsec = static_cast<long>(s->_st_atime_nsec);
  st->st_mtim.tv_sec = static_cast<time_t>(s->_st_mtime);
  st->st_mtim.tv_nsec = static_cast<long>(s->_st_mtime_nsec);
  st->st_ctim.tv_sec = static_cast<time_t>(s->_st_ctime);
  st->st_ctim.tv_nsec = static_cast<long>(s->_st_ctime_nsec);
}

int fstat(int fp, struct stat* st) {
  kernel_stat s;
  int ret = syscall(__NR_fstat, fp, &s);
  copy_stat(st, &s);
  return ret;
}
__strong_alias(fstat64, fstat);

int fstatat(int dirfd, const char* pathname, struct stat* buf, int flags) {
   kernel_stat s;
   int ret = syscall(__NR_newfstatat, dirfd, pathname, &s, flags);
   copy_stat(buf, &s);
   return ret;
}
__strong_alias(fstatat64, fstatat);
