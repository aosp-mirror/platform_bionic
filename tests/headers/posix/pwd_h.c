// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

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
