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

#include <syslog.h>

#include "header_checks.h"

static void syslog_h() {
  MACRO(LOG_PID);
  MACRO(LOG_CONS);
  MACRO(LOG_NDELAY);
  MACRO(LOG_ODELAY);
  MACRO(LOG_NOWAIT);

  MACRO(LOG_KERN);
  MACRO(LOG_USER);
  MACRO(LOG_MAIL);
  MACRO(LOG_NEWS);
  MACRO(LOG_UUCP);
  MACRO(LOG_DAEMON);
  MACRO(LOG_AUTH);
  MACRO(LOG_CRON);
  MACRO(LOG_LPR);
  MACRO(LOG_LOCAL0);
  MACRO(LOG_LOCAL1);
  MACRO(LOG_LOCAL2);
  MACRO(LOG_LOCAL3);
  MACRO(LOG_LOCAL4);
  MACRO(LOG_LOCAL5);
  MACRO(LOG_LOCAL6);
  MACRO(LOG_LOCAL7);

#if !defined(LOG_MASK)
#error LOG_MASK
#endif

  MACRO(LOG_EMERG);
  MACRO(LOG_ALERT);
  MACRO(LOG_CRIT);
  MACRO(LOG_ERR);
  MACRO(LOG_WARNING);
  MACRO(LOG_NOTICE);
  MACRO(LOG_INFO);
  MACRO(LOG_DEBUG);

  FUNCTION(closelog, void (*f)(void));
  FUNCTION(openlog, void (*f)(const char*, int, int));
  FUNCTION(setlogmask, int (*f)(int));
  FUNCTION(syslog, void (*f)(int, const char*, ...));
}
