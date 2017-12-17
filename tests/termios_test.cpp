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

#include <termios.h>

#include <errno.h>

#include <gtest/gtest.h>

// TODO:
// tcdrain
// tcflow
// tcflush
// tcgetattr
// tcgetsid
// tcsendbreak
// tcsetattr

TEST(termios, cfgetispeed_cfsetispeed) {
  termios t = {};
  ASSERT_EQ(0, cfsetispeed(&t, B1200));
  ASSERT_EQ(static_cast<speed_t>(B1200), cfgetispeed(&t));
}

TEST(termios, cfsetispeed_EINVAL) {
  termios t = {};
  errno = 0;
  ASSERT_EQ(-1, cfsetispeed(&t, 1200));
  ASSERT_EQ(EINVAL, errno);
}

TEST(termios, cfgetospeed_cfsetospeed) {
  termios t = {};
  ASSERT_EQ(0, cfsetospeed(&t, B1200));
  ASSERT_EQ(static_cast<speed_t>(B1200), cfgetospeed(&t));
}

TEST(termios, cfsetospeed_EINVAL) {
  termios t = {};
  errno = 0;
  ASSERT_EQ(-1, cfsetospeed(&t, 1200));
  ASSERT_EQ(EINVAL, errno);
}

TEST(termios, cfsetspeed) {
  termios t = {};
  ASSERT_EQ(0, cfsetspeed(&t, B1200));
  ASSERT_EQ(static_cast<speed_t>(B1200), cfgetispeed(&t));
  ASSERT_EQ(static_cast<speed_t>(B1200), cfgetospeed(&t));
}

TEST(termios, cfsetspeed_EINVAL) {
  termios t = {};
  errno = 0;
  // glibc seems to allow 1200 as well as B1200 here, presumably for
  // BSD compatibility (where Bxxx == xxx, unlike Linux).
  ASSERT_EQ(-1, cfsetspeed(&t, 123));
  ASSERT_EQ(EINVAL, errno);
}

TEST(termios, cfmakeraw) {
  termios t;
  memset(&t, 0xff, sizeof(t));
  cfmakeraw(&t);

  EXPECT_EQ(0U, (t.c_iflag & (IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON)));
  EXPECT_EQ(0U, (t.c_oflag & OPOST));
  EXPECT_EQ(0U, (t.c_lflag & (ECHO|ECHONL|ICANON|ISIG|IEXTEN)));
  EXPECT_EQ(0U, (t.c_cflag & PARENB));
  EXPECT_EQ(CS8, static_cast<int>(t.c_cflag & CSIZE));
  EXPECT_EQ(1, t.c_cc[VMIN]);
  EXPECT_EQ(0, t.c_cc[VTIME]);
}
