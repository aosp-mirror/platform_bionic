/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <unistd.h>

#include <errno.h>
#include <limits.h>
#include <sys/vfs.h>

static long __filesizebits(const struct statfs& s) {
  switch (s.f_type) {
    case JFFS2_SUPER_MAGIC:
    case MSDOS_SUPER_MAGIC:
    case NCP_SUPER_MAGIC:
      return 32;
  }
  // There won't be any new 32-bit file systems.
  return 64;
}

static long __link_max(const struct statfs& s) {
  // These constant values were taken from kernel headers.
  // They're not available in uapi headers.
  switch (s.f_type) {
    case EXT2_SUPER_MAGIC:
      return 32000;
    case MINIX_SUPER_MAGIC:
      return 250;
    case MINIX2_SUPER_MAGIC:
      return 65530;
    case REISERFS_SUPER_MAGIC:
      return 0xffff - 1000;
    case UFS_MAGIC:
      return 32000;
  }
  return LINK_MAX;
}

static long __2_symlinks(const struct statfs& s) {
  switch (s.f_type) {
    case ADFS_SUPER_MAGIC:
    case BFS_MAGIC:
    case CRAMFS_MAGIC:
    case EFS_SUPER_MAGIC:
    case MSDOS_SUPER_MAGIC:
    case QNX4_SUPER_MAGIC:
      return 0;
  }
  return 1;
}

static long __pathconf(const struct statfs& s, int name) {
  switch (name) {
    case _PC_FILESIZEBITS:
      return __filesizebits(s);

    case _PC_LINK_MAX:
      return __link_max(s);

    case _PC_MAX_CANON:
      return MAX_CANON;

    case _PC_MAX_INPUT:
      return MAX_INPUT;

    case _PC_NAME_MAX:
      return s.f_namelen;

    case _PC_PATH_MAX:
      return PATH_MAX;

    case _PC_PIPE_BUF:
      return PIPE_BUF;

    case _PC_2_SYMLINKS:
      return __2_symlinks(s);

    case _PC_ALLOC_SIZE_MIN:  /* fall through */
    case _PC_REC_XFER_ALIGN:
      return s.f_frsize;

    case _PC_REC_MIN_XFER_SIZE:
      return s.f_bsize;

#if 0
    case _PC_REC_INCR_XFER_SIZE:
    case _PC_REC_MAX_XFER_SIZE:
#endif

    case _PC_SYMLINK_MAX:
      return -1; /* no limit */

    case _PC_CHOWN_RESTRICTED:
      return _POSIX_CHOWN_RESTRICTED;

    case _PC_NO_TRUNC:
      return _POSIX_NO_TRUNC;

    case _PC_VDISABLE:
      return _POSIX_VDISABLE;

    case _PC_ASYNC_IO:
      return -1;

    case _PC_PRIO_IO:
      return -1;

    case _PC_SYNC_IO:
      return -1;

    default:
      errno = EINVAL;
      return -1;
  }
}

long pathconf(const char* path, int name) {
  struct statfs sb;
  if (statfs(path, &sb) == -1) {
    return -1;
  }
  return __pathconf(sb, name);
}

long fpathconf(int fd, int name) {
  struct statfs sb;
  if (fstatfs(fd, &sb) == -1) {
    return -1;
  }
  return __pathconf(sb, name);
}
