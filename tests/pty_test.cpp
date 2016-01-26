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

#include <gtest/gtest.h>

#include <pty.h>
#include <sys/ioctl.h>

#include "utils.h"

TEST(pty, openpty) {
  int master, slave;
  char name[32];
  struct winsize w = { 123, 456, 9999, 999 };
  ASSERT_EQ(0, openpty(&master, &slave, name, NULL, &w));
  ASSERT_NE(-1, master);
  ASSERT_NE(-1, slave);
  ASSERT_NE(master, slave);

  char tty_name[32];
  ASSERT_EQ(0, ttyname_r(slave, tty_name, sizeof(tty_name)));
  ASSERT_STREQ(tty_name, name);

  struct winsize w_actual;
  ASSERT_EQ(0, ioctl(slave, TIOCGWINSZ, &w_actual));
  ASSERT_EQ(w_actual.ws_row, w.ws_row);
  ASSERT_EQ(w_actual.ws_col, w.ws_col);
  ASSERT_EQ(w_actual.ws_xpixel, w.ws_xpixel);
  ASSERT_EQ(w_actual.ws_ypixel, w.ws_ypixel);

  close(master);
  close(slave);
}

TEST(pty, forkpty) {
  pid_t sid = getsid(0);

  int master;
  pid_t pid = forkpty(&master, NULL, NULL, NULL);
  ASSERT_NE(-1, pid);

  if (pid == 0) {
    // We're the child.
    ASSERT_NE(sid, getsid(0));
    _exit(0);
  }

  ASSERT_EQ(sid, getsid(0));

  AssertChildExited(pid, 0);

  close(master);
}
