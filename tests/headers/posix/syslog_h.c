// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

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
