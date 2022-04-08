/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <error.h>

#include "BionicDeathTest.h"

static size_t g_test_error_print_progname_invocation_count;

static void test_error_print_progname() {
  ++g_test_error_print_progname_invocation_count;
}

TEST(error, smoke) {
  error_message_count = 0;
  error(0, 0, "oops");
  ASSERT_EQ(1U, error_message_count);
  error(0, ENOENT, "couldn't open file '%s'", "blah");
  ASSERT_EQ(2U, error_message_count);

  error_print_progname = test_error_print_progname;
  g_test_error_print_progname_invocation_count = 0;
  error(0, 0, "oops");
  ASSERT_EQ(1U, g_test_error_print_progname_invocation_count);

  error_at_line(0, 0, "blah.c", 123, "hello %s", "world");

  error_print_progname = nullptr;
}

TEST(error_DeathTest, error_exit) {
  ASSERT_EXIT(error(22, 0, "x%c", 'y'), ::testing::ExitedWithCode(22), "xy");
}

TEST(error_DeathTest, error_exit_with_errno) {
  ASSERT_EXIT(error(22, EBADF, "x%c", 'y'), ::testing::ExitedWithCode(22), ": xy: Bad file descriptor");
}

TEST(error_DeathTest, error_at_line_exit) {
  ASSERT_EXIT(error_at_line(22, 0, "a.c", 123, "x%c", 'y'), ::testing::ExitedWithCode(22), ":a.c:123: xy");
}

TEST(error_DeathTest, error_at_line_exit_with_errno) {
  ASSERT_EXIT(error_at_line(22, EBADF, "a.c", 123, "x%c", 'y'), ::testing::ExitedWithCode(22), ":a.c:123: xy: Bad file descriptor");
}
