// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/time.h>

#include "header_checks.h"

static void sys_time_h() {
  TYPE(struct timeval);
  STRUCT_MEMBER(struct timeval, time_t, tv_sec);
  STRUCT_MEMBER(struct timeval, suseconds_t, tv_usec);

  TYPE(struct itimerval);
  STRUCT_MEMBER(struct itimerval, struct timeval, it_interval);
  STRUCT_MEMBER(struct itimerval, struct timeval, it_value);

  TYPE(time_t);
  TYPE(suseconds_t);

  TYPE(fd_set);

  MACRO(ITIMER_REAL);
  MACRO(ITIMER_VIRTUAL);
  MACRO(ITIMER_PROF);

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
  MACRO(FD_SETSIZE);

  FUNCTION(getitimer, int (*f)(int, struct itimerval*));
#if defined(__BIONIC__)
  FUNCTION(gettimeofday, int (*f)(struct timeval*, struct timezone*));
#else
  FUNCTION(gettimeofday, int (*f)(struct timeval*, void*));
#endif
  FUNCTION(setitimer, int (*f)(int, const struct itimerval*, struct itimerval*));
  FUNCTION(select, int (*f)(int, fd_set*, fd_set*, fd_set*, struct timeval*));
  FUNCTION(utimes, int (*f)(const char*, const struct timeval[2]));
}
