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

#include <syslog.h>

#include <errno.h>
#include <gtest/gtest.h>

#include "utils.h"

TEST(syslog, syslog_percent_m) {
  ExecTestHelper eth;
  eth.Run(
      [&]() {
        openlog("foo", LOG_PERROR, LOG_AUTH);
        errno = EINVAL;
        syslog(LOG_ERR, "a b c: %m");
        closelog();
        exit(0);
      },
      0, "foo: a b c: Invalid argument\n");
}

TEST(syslog, syslog_empty) {
  ExecTestHelper eth;
  eth.Run(
      [&]() {
        openlog("foo", LOG_PERROR, LOG_AUTH);
        errno = EINVAL;
        syslog(LOG_ERR, "");
        closelog();
        exit(0);
      },
      0, "foo: \n");
}

TEST(syslog, syslog_truncation) {
  ExecTestHelper eth;
  eth.Run(
      [&]() {
        openlog("bar", LOG_PERROR, LOG_AUTH);
        char too_long[2048] = {};
        memset(too_long, 'x', sizeof(too_long) - 1);
        syslog(LOG_ERR, "%s", too_long);
        closelog();
        exit(0);
      },
      0, "bar: x{1023}\n");
}
