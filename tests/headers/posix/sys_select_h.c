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
