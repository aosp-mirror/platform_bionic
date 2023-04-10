/*
 * Copyright (C) 2023 The Android Open Source Project
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
