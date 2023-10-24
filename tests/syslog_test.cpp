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

#include <stddef.h>  // glibc's <syslog.h> breaks without this; musl seems fine.

#define SYSLOG_NAMES
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

static int by_name(const CODE* array, const char* name) {
  for (auto c = array; c->c_name != nullptr; c++) {
    if (!strcmp(c->c_name, name)) return c->c_val;
  }
  return -1;
}

static const char* by_value(const CODE* array, int value) {
  for (auto c = array; c->c_name != nullptr; c++) {
    if (c->c_val == value) return c->c_name;
  }
  return nullptr;
}

TEST(syslog, facilitynames) {
  ASSERT_STREQ("auth", by_value(facilitynames, LOG_AUTH));
  ASSERT_STREQ("local7", by_value(facilitynames, LOG_LOCAL7));
  ASSERT_EQ(LOG_AUTH, by_name(facilitynames, "auth"));
  ASSERT_EQ(LOG_LOCAL7, by_name(facilitynames, "local7"));
}

TEST(syslog, prioritynames) {
  ASSERT_STREQ("alert", by_value(prioritynames, LOG_ALERT));
  ASSERT_STREQ("err", by_value(prioritynames, LOG_ERR));
  ASSERT_STREQ("warn", by_value(prioritynames, LOG_WARNING));
  ASSERT_EQ(LOG_ALERT, by_name(prioritynames, "alert"));
  ASSERT_EQ(LOG_ERR, by_name(prioritynames, "err"));
  ASSERT_EQ(LOG_WARNING, by_name(prioritynames, "warn"));
  ASSERT_EQ(LOG_WARNING, by_name(prioritynames, "warning"));
}
