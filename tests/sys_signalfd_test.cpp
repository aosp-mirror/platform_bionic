/*
 * Copyright (C) 2018 The Android Open Source Project
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
#include <fcntl.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <unistd.h>

#include <thread>

#include "SignalUtils.h"

static void TestSignalFd(int fd, int signal) {
  ASSERT_NE(-1, fd) << strerror(errno);

  ASSERT_EQ(0, raise(signal));

  signalfd_siginfo sfd_si;
  ASSERT_EQ(static_cast<ssize_t>(sizeof(sfd_si)), read(fd, &sfd_si, sizeof(sfd_si)));

  ASSERT_EQ(signal, static_cast<int>(sfd_si.ssi_signo));

  close(fd);
}

TEST(sys_signalfd, signalfd) {
  SignalMaskRestorer smr;

  sigset_t mask = {};
  sigaddset(&mask, SIGALRM);
  ASSERT_EQ(0, sigprocmask(SIG_SETMASK, &mask, nullptr));

  TestSignalFd(signalfd(-1, &mask, SFD_CLOEXEC), SIGALRM);
}

TEST(sys_signalfd, signalfd64) {
#if defined(__BIONIC__)
  SignalMaskRestorer smr;

  sigset64_t mask = {};
  sigaddset64(&mask, SIGRTMIN);
  ASSERT_EQ(0, sigprocmask64(SIG_SETMASK, &mask, nullptr));

  TestSignalFd(signalfd64(-1, &mask, SFD_CLOEXEC), SIGRTMIN);
#endif
}
