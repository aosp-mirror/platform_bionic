/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <sys/uio.h>

// System calls we need.
extern "C" int __preadv64(int, const struct iovec*, int, long, long);
extern "C" int __preadv64v2(int, const struct iovec*, int, long, long, int);
extern "C" int __pwritev64(int, const struct iovec*, int, long, long);
extern "C" int __pwritev64v2(int, const struct iovec*, int, long, long, int);

// There is no 32-bit off_t preadv/pwritev (even on LP32).
// To avoid 32-bit ABI issues about which register pairs you're allowed
// to pass 64-bit values in, the kernel just takes two `long` arguments --
// which are int32_t for LP32, remember -- and stitches them together.
// It even does this for LP64, taking a second unused always-zero `long`.
// (The first long was int64_t, which is the same as off64_t.)
// The pair is split lo-hi (not hi-lo, as llseek is).

ssize_t preadv(int fd, const struct iovec* ios, int count, off_t offset) {
  return preadv64(fd, ios, count, offset);
}

ssize_t preadv64(int fd, const struct iovec* ios, int count, off64_t offset) {
#if defined(__LP64__)
  return __preadv64(fd, ios, count, offset, 0);
#else
  return __preadv64(fd, ios, count, offset, offset >> 32);
#endif
}

ssize_t pwritev(int fd, const struct iovec* ios, int count, off_t offset) {
  return pwritev64(fd, ios, count, offset);
}

ssize_t pwritev64(int fd, const struct iovec* ios, int count, off64_t offset) {
#if defined(__LP64__)
  return __pwritev64(fd, ios, count, offset, 0);
#else
  return __pwritev64(fd, ios, count, offset, offset >> 32);
#endif
}

ssize_t preadv2(int fd, const struct iovec* ios, int count, off_t offset, int flags) {
  return preadv64v2(fd, ios, count, offset, flags);
}

ssize_t preadv64v2(int fd, const struct iovec* ios, int count, off64_t offset, int flags) {
#if defined(__LP64__)
  return __preadv64v2(fd, ios, count, offset, 0, flags);
#else
  return __preadv64v2(fd, ios, count, offset, offset >> 32, flags);
#endif
}

ssize_t pwritev2(int fd, const struct iovec* ios, int count, off_t offset, int flags) {
  return pwritev64v2(fd, ios, count, offset, flags);
}

ssize_t pwritev64v2(int fd, const struct iovec* ios, int count, off64_t offset, int flags) {
#if defined(__LP64__)
  return __pwritev64v2(fd, ios, count, offset, 0, flags);
#else
  return __pwritev64v2(fd, ios, count, offset, offset >> 32, flags);
#endif
}
