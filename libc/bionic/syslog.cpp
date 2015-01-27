/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "private/libc_logging.h"

static const char* syslog_log_tag = NULL;
static int syslog_priority_mask = 0xff;

void closelog() {
  syslog_log_tag = NULL;
}

void openlog(const char* log_tag, int /*options*/, int /*facility*/) {
  syslog_log_tag = log_tag;
}

int setlogmask(int new_mask) {
  int old_mask = syslog_priority_mask;
  // 0 is used to query the current mask.
  if (new_mask != 0) {
    syslog_priority_mask = new_mask;
  }
  return old_mask;
}

void syslog(int priority, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsyslog(priority, fmt, args);
  va_end(args);
}

void vsyslog(int priority, const char* fmt, va_list args) {
  int caller_errno = errno;

  // Check whether we're supposed to be logging messages of this priority.
  if ((syslog_priority_mask & LOG_MASK(LOG_PRI(priority))) == 0) {
    return;
  }

  // What's our log tag?
  const char* log_tag = syslog_log_tag;
  if (log_tag == NULL) {
    log_tag = getprogname();
  }

  // What's our Android log priority?
  priority &= LOG_PRIMASK;
  int android_log_priority;
  if (priority <= LOG_ERR) {
    android_log_priority = ANDROID_LOG_ERROR;
  } else if (priority == LOG_WARNING) {
    android_log_priority = ANDROID_LOG_WARN;
  } else if (priority <= LOG_INFO) {
    android_log_priority = ANDROID_LOG_INFO;
  } else {
    android_log_priority = ANDROID_LOG_DEBUG;
  }

  // glibc's printf family support %m directly, but our BSD-based one doesn't.
  // If the format string seems to contain "%m", rewrite it.
  const char* log_fmt = fmt;
  if (strstr(fmt, "%m") != NULL) {
    size_t dst_len = 1024;
    char* dst = reinterpret_cast<char*>(malloc(dst_len));
    log_fmt = dst;

    const char* src = fmt;
    for (; dst_len > 0 && *src != '\0'; ++src) {
      if (*src == '%' && *(src + 1) == 'm') {
        // Expand %m.
        size_t n = strlcpy(dst, strerror(caller_errno), dst_len);
        if (n >= dst_len) {
          n = dst_len;
        }
        dst += n;
        dst_len -= n;
        ++src;
      } else if (*src == '%' && *(src + 1) == '%') {
        // We need to copy pairs of '%'s so the %m test works.
        if (dst_len <= 2) {
          break;
        }
        *dst++ = '%'; --dst_len;
        *dst++ = '%'; --dst_len;
        ++src;
      } else {
        *dst++ = *src; --dst_len;
      }
    }
    *dst = '\0';
  }

  // We can't let __libc_format_log do the formatting because it doesn't support
  // all the printf functionality.
  char log_line[1024];
  vsnprintf(log_line, sizeof(log_line), log_fmt, args);

  if (log_fmt != fmt) {
    free(const_cast<char*>(log_fmt));
  }

  __libc_format_log(android_log_priority, log_tag, "%s", log_line);
}
