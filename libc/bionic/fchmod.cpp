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

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

#include "private/FdPath.h"

extern "C" int ___fchmod(int, mode_t);

int fchmod(int fd, mode_t mode) {
  int saved_errno = errno;
  int result = ___fchmod(fd, mode);
  if (result == 0 || errno != EBADF) {
    return result;
  }

  // fd could be an O_PATH file descriptor, and the kernel
  // may not directly support fchmod() on such a file descriptor.
  // Use /proc/self/fd instead to emulate this support.
  // https://sourceware.org/bugzilla/show_bug.cgi?id=14578
  //
  // As of February 2015, there are no kernels which support fchmod
  // on an O_PATH file descriptor, and "man open" documents fchmod
  // on O_PATH file descriptors as returning EBADF.
  int fd_flag = fcntl(fd, F_GETFL);
  if (fd_flag == -1 || (fd_flag & O_PATH) == 0) {
    errno = EBADF;
    return -1;
  }

  errno = saved_errno;
  result = chmod(FdPath(fd).c_str(), mode);
  if (result == -1 && errno == ELOOP) {
    // Linux does not support changing the mode of a symlink.
    // For fchmodat(AT_SYMLINK_NOFOLLOW), POSIX requires a return
    // value of ENOTSUP. Assume that's true here too.
    errno = ENOTSUP;
  }

  return result;
}
