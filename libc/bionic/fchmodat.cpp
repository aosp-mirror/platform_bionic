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

#include "private/ScopedFd.h"

extern "C" int __fchmodat(int, const char*, mode_t);

int fchmodat(int dirfd, const char* pathname, mode_t mode, int flags) {
  if ((flags & ~AT_SYMLINK_NOFOLLOW) != 0) {
    errno = EINVAL;
    return -1;
  }

  if (flags & AT_SYMLINK_NOFOLLOW) {
    // Emulate AT_SYMLINK_NOFOLLOW using the mechanism described
    // at https://sourceware.org/bugzilla/show_bug.cgi?id=14578
    // comment #10

    ScopedFd fd(openat(dirfd, pathname, O_PATH | O_NOFOLLOW | O_CLOEXEC));
    if (fd.get() == -1) return -1;

    // POSIX requires that ENOTSUP be returned when the system
    // doesn't support setting the mode of a symbolic link.
    // This is true for all Linux kernels.
    // We rely on the O_PATH compatibility layer added in the
    // fchmod() function to get errno correct.
    return fchmod(fd.get(), mode);
  }

  return __fchmodat(dirfd, pathname, mode);
}
