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
