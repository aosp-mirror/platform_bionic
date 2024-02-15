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

#pragma once

/**
 * @file sys/statvfs.h
 * @brief Filesystem statistics.
 */

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

#define __STATVFS64_BODY \
  /** Block size. */ \
  unsigned long f_bsize; \
  /** Fragment size. */ \
  unsigned long f_frsize; \
  /** Total size of filesystem in `f_frsize` blocks. */ \
  fsblkcnt_t f_blocks; \
  /** Number of free blocks. */ \
  fsblkcnt_t f_bfree; \
  /** Number of free blocks for non-root. */ \
  fsblkcnt_t f_bavail; \
  /** Number of inodes. */ \
  fsfilcnt_t f_files; \
  /** Number of free inodes. */ \
  fsfilcnt_t f_ffree; \
  /** Number of free inodes for non-root. */ \
  fsfilcnt_t f_favail; \
  /** Filesystem id. */ \
  unsigned long f_fsid; \
  /** Mount flags. (See `ST_` constants.) */ \
  unsigned long f_flag; \
  /** Maximum filename length. */ \
  unsigned long f_namemax; \

#if defined(__LP64__)
#define __STATVFS64_CODA uint32_t __f_reserved[6];
#else
#define __STATVFS64_CODA
#endif

struct statvfs { __STATVFS64_BODY __STATVFS64_CODA };

struct statvfs64 { __STATVFS64_BODY __STATVFS64_CODA };

/** Flag for `f_flag` in `struct statvfs`: mounted read-only. */
#define ST_RDONLY      0x0001

/** Flag for `f_flag` in `struct statvfs`: setuid/setgid ignored. */
#define ST_NOSUID      0x0002

/** Flag for `f_flag` in `struct statvfs`: access to device files disallowed. */
#define ST_NODEV       0x0004

/** Flag for `f_flag` in `struct statvfs`: execution disallowed. */
#define ST_NOEXEC      0x0008

/** Flag for `f_flag` in `struct statvfs`: writes synced immediately. */
#define ST_SYNCHRONOUS 0x0010

/** Flag for `f_flag` in `struct statvfs`: mandatory locking permitted. */
#define ST_MANDLOCK    0x0040

/** Flag for `f_flag` in `struct statvfs`: access times not updated. */
#define ST_NOATIME     0x0400

/** Flag for `f_flag` in `struct statvfs`: directory access times not updated. */
#define ST_NODIRATIME  0x0800

/** Flag for `f_flag` in `struct statvfs`: see `MS_RELATIME`. */
#define ST_RELATIME    0x1000

/** Flag for `f_flag` in `struct statvfs`: don't follow symlinks. */
#define ST_NOSYMFOLLOW 0x2000

/**
 * [statvfs(3)](http://man7.org/linux/man-pages/man3/statvfs.3.html)
 * queries filesystem statistics for the given path.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int statvfs(const char* _Nonnull __path, struct statvfs* _Nonnull __buf);

/**
 * [fstatvfs(3)](http://man7.org/linux/man-pages/man3/fstatvfs.3.html)
 * queries filesystem statistics for the given file descriptor.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int fstatvfs(int __fd, struct statvfs* _Nonnull __buf);

/** Equivalent to statvfs() . */
int statvfs64(const char* _Nonnull __path, struct statvfs64* _Nonnull __buf);

/** Equivalent to fstatvfs(). */
int fstatvfs64(int __fd, struct statvfs64* _Nonnull __buf);

__END_DECLS
