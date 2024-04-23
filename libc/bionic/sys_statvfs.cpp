/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/statfs.h>
#include <sys/statvfs.h>

static inline void __bionic_statfs_to_statvfs(const struct statfs* src, struct statvfs* dst) {
  dst->f_bsize = src->f_bsize;
  dst->f_frsize = src->f_frsize;
  dst->f_blocks = src->f_blocks;
  dst->f_bfree = src->f_bfree;
  dst->f_bavail = src->f_bavail;
  dst->f_files = src->f_files;
  dst->f_ffree = src->f_ffree;
  dst->f_favail = src->f_ffree;
  dst->f_fsid = src->f_fsid.__val[0] | static_cast<uint64_t>(src->f_fsid.__val[1]) << 32;
  dst->f_flag = src->f_flags;
  dst->f_namemax = src->f_namelen;
}

int statvfs(const char* path, struct statvfs* result) {
  struct statfs tmp;
  if (statfs(path, &tmp) == -1) return -1;
  __bionic_statfs_to_statvfs(&tmp, result);
  return 0;
}

int fstatvfs(int fd, struct statvfs* result) {
  struct statfs tmp;
  if (fstatfs(fd, &tmp) == -1) return -1;
  __bionic_statfs_to_statvfs(&tmp, result);
  return 0;
}

// Historically we provided actual symbols for statvfs64 and fstatvfs64.
// They're not particularly useful, but we can't take them away.
__strong_alias(statvfs64, statvfs);
__strong_alias(fstatvfs64, fstatvfs);
