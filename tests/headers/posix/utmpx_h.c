// Copyright (C) 2023 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <utmpx.h>

#include "header_checks.h"

static void utmpx_h() {
  TYPE(struct utmpx);
  STRUCT_MEMBER_ARRAY(struct utmpx, char/*[]*/, ut_user);
  STRUCT_MEMBER_ARRAY(struct utmpx, char/*[]*/, ut_id);
  STRUCT_MEMBER_ARRAY(struct utmpx, char/*[]*/, ut_line);
  STRUCT_MEMBER(struct utmpx, pid_t, ut_pid);
  STRUCT_MEMBER(struct utmpx, short, ut_type);
#if !defined(__GLIBC__)
  // POSIX says struct timeval, but glibc has an anonymous struct.
  STRUCT_MEMBER(struct utmpx, struct timeval, ut_tv);
#endif

  TYPE(pid_t);
  TYPE(struct timeval);

  MACRO(EMPTY);
  MACRO(BOOT_TIME);
  MACRO(OLD_TIME);
  MACRO(NEW_TIME);
  MACRO(USER_PROCESS);
  MACRO(INIT_PROCESS);
  MACRO(LOGIN_PROCESS);
  MACRO(DEAD_PROCESS);

  FUNCTION(endutxent, void (*f)(void));
  FUNCTION(getutxent, struct utmpx* (*f)(void));
  FUNCTION(getutxid, struct utmpx* (*f)(const struct utmpx*));
  FUNCTION(getutxline, struct utmpx* (*f)(const struct utmpx*));
  FUNCTION(pututxline, struct utmpx* (*f)(const struct utmpx*));
  FUNCTION(setutxent, void (*f)(void));
}
