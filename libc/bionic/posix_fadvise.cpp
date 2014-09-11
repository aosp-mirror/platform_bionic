/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include "private/ErrnoRestorer.h"

extern "C" int __arm_fadvise64_64(int, int, off64_t, off64_t);
extern "C" int __fadvise64(int, off64_t, off64_t, int);

// No architecture actually has the 32-bit off_t system call.
int posix_fadvise(int fd, off_t offset, off_t length, int advice) {
  return posix_fadvise64(fd, offset, length, advice);
}

#if defined(__arm__)
int posix_fadvise64(int fd, off64_t offset, off64_t length, int advice) {
  ErrnoRestorer errno_restorer;
  return (__arm_fadvise64_64(fd, advice, offset, length) == 0) ? 0 : errno;
}
#else
int posix_fadvise64(int fd, off64_t offset, off64_t length, int advice) {
  ErrnoRestorer errno_restorer;
  return (__fadvise64(fd, offset, length, advice) == 0) ? 0 : errno;
}
#endif
