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

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <unistd.h>

#if __LP64__
#error This code is only needed on 32-bit systems!
#endif

// System calls we need.
extern "C" int __fcntl64(int, int, void*);
extern "C" int __fstatfs64(int, size_t, struct statfs*);
extern "C" int __llseek(int, unsigned long, unsigned long, off64_t*, int);
extern "C" int __statfs64(const char*, size_t, struct statfs*);

// For fcntl we use the fcntl64 system call to signal that we're using struct flock64.
int fcntl(int fd, int cmd, ...) {
  va_list ap;

  va_start(ap, cmd);
  void* arg = va_arg(ap, void*);
  va_end(ap);

  return __fcntl64(fd, cmd, arg);
}

// For fstatfs we need to add the extra argument giving the kernel the size of the buffer.
int fstatfs(int fd, struct statfs* stat) {
  return __fstatfs64(fd, sizeof(*stat), stat);
}
__strong_alias(fstatfs64, fstatfs);

// For statfs we need to add the extra argument giving the kernel the size of the buffer.
int statfs(const char* path, struct statfs* stat) {
  return __statfs64(path, sizeof(*stat), stat);
}
__strong_alias(statfs64, statfs);

// For lseek64 we need to use the llseek system call which splits the off64_t in two and
// returns the off64_t result via a pointer because 32-bit kernels can't return 64-bit results.
off64_t lseek64(int fd, off64_t off, int whence) {
  off64_t result;
  unsigned long off_hi = static_cast<unsigned long>(off >> 32);
  unsigned long off_lo = static_cast<unsigned long>(off);
  if (__llseek(fd, off_hi, off_lo, &result, whence) < 0) {
    return -1;
  }
  return result;
}

// There is no pread for 32-bit off_t, so we need to widen and call pread64.
ssize_t pread(int fd, void* buf, size_t byte_count, off_t offset) {
  return pread64(fd, buf, byte_count, static_cast<off64_t>(offset));
}

// There is no pwrite for 32-bit off_t, so we need to widen and call pwrite64.
ssize_t pwrite(int fd, const void* buf, size_t byte_count, off_t offset) {
  return pwrite64(fd, buf, byte_count, static_cast<off64_t>(offset));
}

// There is no fallocate for 32-bit off_t, so we need to widen and call fallocate64.
int fallocate(int fd, int mode, off_t offset, off_t length) {
  return fallocate64(fd, mode, static_cast<off64_t>(offset), static_cast<off64_t>(length));
}

// There is no getrlimit64 system call, so we need to use prlimit64.
int getrlimit64(int resource, rlimit64* limits64) {
  return prlimit64(0, resource, NULL, limits64);
}

// There is no setrlimit64 system call, so we need to use prlimit64.
int setrlimit64(int resource, const rlimit64* limits64) {
  return prlimit64(0, resource, limits64, NULL);
}
