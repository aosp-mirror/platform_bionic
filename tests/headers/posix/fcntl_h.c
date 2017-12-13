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

#include <fcntl.h>

#include "header_checks.h"

static void fcntl_h() {
  MACRO(F_DUPFD);
  MACRO(F_DUPFD_CLOEXEC);
  MACRO(F_GETFD);
  MACRO(F_SETFD);
  MACRO(F_GETFL);
  MACRO(F_SETFL);
  MACRO(F_GETLK);
  MACRO(F_SETLK);
  MACRO(F_SETLKW);
  MACRO(F_GETOWN);
  MACRO(F_SETOWN);

  MACRO(FD_CLOEXEC);

  MACRO(F_RDLCK);
  MACRO(F_UNLCK);
  MACRO(F_WRLCK);

  MACRO(SEEK_SET);
  MACRO(SEEK_CUR);
  MACRO(SEEK_END);

  MACRO(O_CLOEXEC);
  MACRO(O_CREAT);
  MACRO(O_DIRECTORY);
  MACRO(O_EXCL);
  MACRO(O_NOCTTY);
  MACRO(O_NOFOLLOW);
  MACRO(O_TRUNC);
#if !defined(__linux__)
  MACRO(O_TTY_INIT);
#endif

  MACRO(O_APPEND);
  MACRO(O_DSYNC);
  MACRO(O_NONBLOCK);
  MACRO(O_RSYNC);
  MACRO(O_SYNC);

  MACRO(O_ACCMODE);

#if !defined(__linux__)
  MACRO(O_EXEC);
#endif
  MACRO(O_RDONLY);
  MACRO(O_RDWR);
#if !defined(__linux__)
  MACRO(O_SEARCH);
#endif
  MACRO(O_WRONLY);

  // POSIX: "The <fcntl.h> header shall define the symbolic constants for
  // file modes for use as values of mode_t as described in <sys/stat.h>."
#include "sys_stat_h_mode_constants.h"

  MACRO(AT_FDCWD);
#if !defined(__BIONIC__) // See comment in "faccessat.cpp".
  MACRO(AT_EACCESS);
#endif
  MACRO(AT_SYMLINK_NOFOLLOW);
  MACRO(AT_REMOVEDIR);

  MACRO(POSIX_FADV_DONTNEED);
  MACRO(POSIX_FADV_NOREUSE);
  MACRO(POSIX_FADV_NORMAL);
  MACRO(POSIX_FADV_RANDOM);
  MACRO(POSIX_FADV_SEQUENTIAL);
  MACRO(POSIX_FADV_WILLNEED);

  TYPE(struct flock);
  STRUCT_MEMBER(struct flock, short, l_type);
  STRUCT_MEMBER(struct flock, short, l_whence);
  STRUCT_MEMBER(struct flock, off_t, l_start);
  STRUCT_MEMBER(struct flock, off_t, l_len);
  STRUCT_MEMBER(struct flock, pid_t, l_pid);

  TYPE(mode_t);
  TYPE(off_t);
  TYPE(pid_t);

  FUNCTION(creat, int (*f)(const char*, mode_t));
  FUNCTION(fcntl, int (*f)(int, int, ...));
  FUNCTION(open, int (*f)(const char*, int, ...));
  FUNCTION(openat, int (*f)(int, const char*, int, ...));
  FUNCTION(posix_fadvise, int (*f)(int, off_t, off_t, int));
  FUNCTION(posix_fallocate, int (*f)(int, off_t, off_t));
}
