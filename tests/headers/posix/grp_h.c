// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <grp.h>

#include "header_checks.h"

static void grp_h() {
  TYPE(struct group);
  STRUCT_MEMBER(struct group, char*, gr_name);
  STRUCT_MEMBER(struct group, gid_t, gr_gid);
  STRUCT_MEMBER(struct group, char**, gr_mem);

  TYPE(gid_t);
  TYPE(size_t);

  FUNCTION(endgrent, void (*f)(void));
  FUNCTION(getgrent, struct group* (*f)(void));
  FUNCTION(getgrgid, struct group* (*f)(gid_t));
  FUNCTION(getgrgid_r, int (*f)(gid_t, struct group*, char*, size_t, struct group**));
  FUNCTION(getgrnam, struct group* (*f)(const char*));
  FUNCTION(getgrnam_r, int (*f)(const char*, struct group*, char*, size_t, struct group**));
  FUNCTION(setgrent, void (*f)(void));
}
