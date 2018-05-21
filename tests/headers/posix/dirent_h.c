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
