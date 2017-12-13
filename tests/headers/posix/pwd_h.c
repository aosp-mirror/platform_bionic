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

#include <pwd.h>

#include "header_checks.h"

static void pwd_h() {
  TYPE(struct passwd);
  STRUCT_MEMBER(struct passwd, char*, pw_name);
  STRUCT_MEMBER(struct passwd, uid_t, pw_uid);
  STRUCT_MEMBER(struct passwd, gid_t, pw_gid);
  STRUCT_MEMBER(struct passwd, char*, pw_dir);
  STRUCT_MEMBER(struct passwd, char*, pw_shell);

  TYPE(gid_t);
  TYPE(uid_t);
  TYPE(size_t);

  FUNCTION(endpwent, void (*f)(void));
  FUNCTION(getpwent, struct passwd* (*f)(void));
  FUNCTION(getpwnam, struct passwd* (*f)(const char*));
  FUNCTION(getpwnam_r, int (*f)(const char*, struct passwd*, char*, size_t, struct passwd**));
  FUNCTION(getpwuid, struct passwd* (*f)(uid_t));
  FUNCTION(getpwuid_r, int (*f)(uid_t, struct passwd*, char*, size_t, struct passwd**));
  FUNCTION(setpwent, void (*f)(void));
}
