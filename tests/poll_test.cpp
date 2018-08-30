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

#include <gtest/gtest.h>

#include <errno.h>
#include <poll.h>

TEST(poll, poll_null_fds) {
  // Because nanosleep(2) is relatively new to POSIX, code sometimes abuses poll.
  errno = 0;
  ASSERT_EQ(0, poll(nullptr, 0, 1));
  ASSERT_EQ(0, errno);
}

TEST(poll, ppoll_null_fds) {
  // Because nanosleep(2) is relatively new to POSIX, code sometimes abuses poll.
  errno = 0;
  timespec ts = { .tv_nsec = 100 };
  ASSERT_EQ(0, ppoll(nullptr, 0, &ts, nullptr));
  ASSERT_EQ(0, errno);
}

TEST(poll, ppoll64_null_fds) {
#if __BIONIC__
  // Because nanosleep(2) is relatively new to POSIX, code sometimes abuses poll.
  errno = 0;
  timespec ts = { .tv_nsec = 100 };
  ASSERT_EQ(0, ppoll64(nullptr, 0, &ts, nullptr));
  ASSERT_EQ(0, errno);
#endif
}
