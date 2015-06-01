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

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

extern "C" int ___fsetxattr(int, const char*, const void*, size_t, int);

int fsetxattr(int fd, const char* name, const void* value, size_t size, int flags) {
  int saved_errno = errno;
  int result = ___fsetxattr(fd, name, value, size, flags);

  if ((result == 0) || (errno != EBADF)) {
    return result;
  }

  // fd could be an O_PATH file descriptor, and the kernel
  // may not directly support fsetxattr() on such a file descriptor.
  // Use /proc/self/fd instead to emulate this support.
  int fd_flag = fcntl(fd, F_GETFL);
  if ((fd_flag == -1) || ((fd_flag & O_PATH) == 0)) {
    errno = EBADF;
    return -1;
  }

  char buf[40];
  snprintf(buf, sizeof(buf), "/proc/self/fd/%d", fd);
  errno = saved_errno;
  return setxattr(buf, name, value, size, flags);
}
