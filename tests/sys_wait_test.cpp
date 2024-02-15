/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <sys/wait.h>

TEST(sys_wait, waitid) {
  pid_t pid = fork();
  ASSERT_NE(pid, -1);

  if (pid == 0) _exit(66);

  siginfo_t si = {};
  ASSERT_EQ(0, waitid(P_PID, pid, &si, WEXITED));
  ASSERT_EQ(pid, si.si_pid);
  ASSERT_EQ(66, si.si_status);
  ASSERT_EQ(CLD_EXITED, si.si_code);
}

// https://github.com/android/ndk/issues/1878
TEST(sys_wait, macros) {
#if defined(__GLIBC__)
  // glibc before 2016 requires an lvalue.
#else
  ASSERT_FALSE(WIFEXITED(0x7f));
  ASSERT_TRUE(WIFSTOPPED(0x7f));
  ASSERT_FALSE(WIFCONTINUED(0x7f));

  ASSERT_TRUE(WIFEXITED(0x80));
  ASSERT_FALSE(WIFSTOPPED(0x80));
  ASSERT_FALSE(WIFCONTINUED(0x80));

  ASSERT_FALSE(WIFEXITED(0xffff));
  ASSERT_FALSE(WIFSTOPPED(0xffff));
  ASSERT_TRUE(WIFCONTINUED(0xffff));
#endif
}
