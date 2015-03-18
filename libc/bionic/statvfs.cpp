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

#include <sys/statvfs.h>

#include <sys/statfs.h>

// Paper over the fact that 32-bit kernels use fstatfs64/statfs64 with an extra argument,
// but 64-bit kernels don't have the "64" bit suffix or the extra size_t argument.
#if __LP64__
extern "C" int __fstatfs(int, struct statfs*);
extern "C" int __statfs(const char*, struct statfs*);
#  define __fstatfs64(fd,size,buf) __fstatfs(fd,buf)
#  define __statfs64(path,size,buf) __statfs(path,buf)
#else
extern "C" int __fstatfs64(int, size_t, struct statfs*);
extern "C" int __statfs64(const char*, size_t, struct statfs*);
#endif

// The kernel sets a private ST_VALID flag to signal to the C library whether the
// f_flags field is valid. This flag should not be exposed to users of the C library.
#define ST_VALID 0x0020

static void __statfs_to_statvfs(const struct statfs& in, struct statvfs* out) {
  out->f_bsize = in.f_bsize;
  out->f_frsize = in.f_frsize;
  out->f_blocks = in.f_blocks;
  out->f_bfree = in.f_bfree;
  out->f_bavail = in.f_bavail;
  out->f_files = in.f_files;
  out->f_ffree = in.f_ffree;
  out->f_favail = in.f_ffree;
  out->f_fsid = in.f_fsid.__val[0] | (static_cast<uint64_t>(in.f_fsid.__val[1]) << 32);
  out->f_flag = in.f_flags;
  out->f_namemax = in.f_namelen;
}

int fstatfs(int fd, struct statfs* result) {
  int rc = __fstatfs64(fd, sizeof(*result), result);
  if (rc != 0) {
    return rc;
  }
  result->f_flags &= ~ST_VALID;
  return 0;
}
__strong_alias(fstatfs64, fstatfs);

int statfs(const char* path, struct statfs* result) {
  int rc = __statfs64(path, sizeof(*result), result);
  if (rc != 0) {
    return rc;
  }
  result->f_flags &= ~ST_VALID;
  return 0;
}
__strong_alias(statfs64, statfs);

int statvfs(const char* path, struct statvfs* result) {
  struct statfs tmp;
  int rc = statfs(path, &tmp);
  if (rc != 0) {
    return rc;
  }
  __statfs_to_statvfs(tmp, result);
  return 0;
}
__strong_alias(statvfs64, statvfs);

int fstatvfs(int fd, struct statvfs* result) {
  struct statfs tmp;
  int rc = fstatfs(fd, &tmp);
  if (rc != 0) {
    return rc;
  }
  __statfs_to_statvfs(tmp, result);
  return 0;
}
__strong_alias(fstatvfs64, fstatvfs);
