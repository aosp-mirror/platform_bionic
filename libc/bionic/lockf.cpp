/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <unistd.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>

int lockf64(int fd, int cmd, off64_t length) {
  // Translate POSIX lockf into fcntl.
  struct flock64 fl;
  memset(&fl, 0, sizeof(fl));
  fl.l_whence = SEEK_CUR;
  fl.l_start = 0;
  fl.l_len = length;

  if (cmd == F_ULOCK) {
    fl.l_type = F_UNLCK;
    cmd = F_SETLK64;
    return fcntl(fd, F_SETLK64, &fl);
  }

  if (cmd == F_LOCK) {
    fl.l_type = F_WRLCK;
    return fcntl(fd, F_SETLKW64, &fl);
  }

  if (cmd == F_TLOCK) {
    fl.l_type = F_WRLCK;
    return fcntl(fd, F_SETLK64, &fl);
  }

  if (cmd == F_TEST) {
    fl.l_type = F_RDLCK;
    if (fcntl(fd, F_GETLK64, &fl) == -1) return -1;
    if (fl.l_type == F_UNLCK || fl.l_pid == getpid()) return 0;
    errno = EACCES;
    return -1;
  }

  errno = EINVAL;
  return -1;
}

int lockf(int fd, int cmd, off_t length) {
  return lockf64(fd, cmd, length);
}
