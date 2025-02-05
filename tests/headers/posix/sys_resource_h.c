// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/resource.h>

#include "header_checks.h"

static void sys_resource_h() {
  MACRO(PRIO_PROCESS);
  MACRO(PRIO_PGRP);
  MACRO(PRIO_USER);

  TYPE(rlim_t);

  MACRO(RLIM_INFINITY);
  MACRO(RLIM_SAVED_MAX);
  MACRO(RLIM_SAVED_CUR);

  MACRO(RUSAGE_SELF);
  MACRO(RUSAGE_CHILDREN);

  TYPE(struct rlimit);
  STRUCT_MEMBER(struct rlimit, rlim_t, rlim_cur);
  STRUCT_MEMBER(struct rlimit, rlim_t, rlim_max);

  TYPE(struct rusage);
  STRUCT_MEMBER(struct rusage, struct timeval, ru_utime);
  STRUCT_MEMBER(struct rusage, struct timeval, ru_stime);

  TYPE(struct timeval);

  MACRO(RLIMIT_CORE);
  MACRO(RLIMIT_CPU);
  MACRO(RLIMIT_DATA);
  MACRO(RLIMIT_FSIZE);
  MACRO(RLIMIT_NOFILE);
  MACRO(RLIMIT_STACK);
  MACRO(RLIMIT_AS);

  FUNCTION(getpriority, int (*f)(int, id_t));
  FUNCTION(getrlimit, int (*f)(int, struct rlimit*));
  FUNCTION(getrusage, int (*f)(int, struct rusage*));
  FUNCTION(setpriority, int (*f)(int, id_t, int));
  FUNCTION(setrlimit, int (*f)(int, const struct rlimit*));
}
