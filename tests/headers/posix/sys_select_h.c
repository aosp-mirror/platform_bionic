// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/select.h>

#include "header_checks.h"

static void sys_select_h() {
  TYPE(struct timeval);
  STRUCT_MEMBER(struct timeval, time_t, tv_sec);
  STRUCT_MEMBER(struct timeval, suseconds_t, tv_usec);

  TYPE(time_t);
  TYPE(suseconds_t);

  TYPE(sigset_t);
  TYPE(struct timespec);
  TYPE(fd_set);

  MACRO(FD_SETSIZE);

#if !defined(FD_CLR)
#error FD_CLR
#endif
#if !defined(FD_ISSET)
#error FD_ISSET
#endif
#if !defined(FD_SET)
#error FD_SET
#endif
#if !defined(FD_ZERO)
#error FD_ZERO
#endif

  FUNCTION(pselect, int (*f)(int, fd_set*, fd_set*, fd_set*, const struct timespec*, const sigset_t*));
  FUNCTION(select, int (*f)(int, fd_set*, fd_set*, fd_set*, struct timeval*));
}
