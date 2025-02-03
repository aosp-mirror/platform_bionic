// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <dirent.h>

#include "header_checks.h"

static void dirent_h() {
  INCOMPLETE_TYPE(DIR);

  TYPE(struct dirent);
#if defined(__BIONIC__) && !defined(__LP64__) // Historical ABI accident.
  STRUCT_MEMBER(struct dirent, uint64_t, d_ino);
#else
  STRUCT_MEMBER(struct dirent, ino_t, d_ino);
#endif
  STRUCT_MEMBER_ARRAY(struct dirent, char/*[]*/, d_name);

  TYPE(ino_t);

  FUNCTION(alphasort, int (*f)(const struct dirent**, const struct dirent**));
  FUNCTION(closedir, int (*f)(DIR*));
  FUNCTION(dirfd, int (*f)(DIR*));
  FUNCTION(fdopendir, DIR* (*f)(int));
  FUNCTION(opendir, DIR* (*f)(const char*));
  FUNCTION(readdir, struct dirent* (*f)(DIR*));
  FUNCTION(readdir_r, int (*f)(DIR*, struct dirent*, struct dirent**));
  FUNCTION(rewinddir, void (*f)(DIR*));
  FUNCTION(scandir, int (*f)(const char*, struct dirent***,
                             int (*)(const struct dirent*),
                             int (*)(const struct dirent**, const struct dirent**)));
  FUNCTION(seekdir, void (*f)(DIR*, long));
  FUNCTION(telldir, long (*f)(DIR*));
}
