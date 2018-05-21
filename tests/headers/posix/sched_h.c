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

#if !defined(DO_NOT_INCLUDE_SCHED_H)
#include <sched.h>
#endif

#include "header_checks.h"

static void sched_h() {
  TYPE(pid_t);
  TYPE(time_t);
  TYPE(struct timespec);

  TYPE(struct sched_param);
  STRUCT_MEMBER(struct sched_param, int, sched_priority);
#if !defined(__linux__)
  STRUCT_MEMBER(struct sched_param, int, sched_ss_low_priority);
  STRUCT_MEMBER(struct sched_param, struct timespec, sched_ss_repl_period);
  STRUCT_MEMBER(struct sched_param, struct timespec, sched_ss_init_budget);
  STRUCT_MEMBER(struct sched_param, int, sched_ss_max_repl);
#endif

  MACRO(SCHED_FIFO);
  MACRO(SCHED_RR);
#if !defined(__linux__)
  MACRO(SCHED_SPORADIC);
#endif
  MACRO(SCHED_OTHER);

  FUNCTION(sched_get_priority_max, int (*f)(int));
  FUNCTION(sched_get_priority_min, int (*f)(int));
  FUNCTION(sched_getparam, int (*f)(pid_t, struct sched_param*));
  FUNCTION(sched_getscheduler, int (*f)(pid_t));
  FUNCTION(sched_rr_get_interval, int (*f)(pid_t, struct timespec*));
  FUNCTION(sched_setparam, int (*f)(pid_t, const struct sched_param*));
  FUNCTION(sched_setscheduler, int (*f)(pid_t, int, const struct sched_param*));
  FUNCTION(sched_yield, int (*f)(void));
}
