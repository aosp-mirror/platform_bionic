// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

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
  // Musl only defines the file mode bits (S_IFUSR, etc.) and not the file
  // type bits (S_IFMT, etc.).
#if !defined(ANDROID_HOST_MUSL)
#include "sys_stat_h_mode_constants.h"
#endif

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
