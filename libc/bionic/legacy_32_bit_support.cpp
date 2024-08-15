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

#undef _FORTIFY_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>

#include "platform/bionic/macros.h"
#include "platform/bionic/page.h"
#include "private/ErrnoRestorer.h"
#include "private/bionic_fdtrack.h"

#if defined(__LP64__)
#error This code is only needed on 32-bit systems!
#endif

// To implement lseek64() on ILP32, we need to use the _llseek() system call
// which splits the off64_t into two 32-bit arguments and returns the off64_t
// result via a pointer because 32-bit kernels can't accept 64-bit arguments
// or return 64-bit results. (Our symbol is __llseek with two underscores for
// historical reasons, but it's exposed as ABI so we can't fix it.)
extern "C" int __llseek(int, unsigned long, unsigned long, off64_t*, int);

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
  return prlimit64(0, resource, nullptr, limits64);
}

// There is no setrlimit64 system call, so we need to use prlimit64.
int setrlimit64(int resource, const rlimit64* limits64) {
  return prlimit64(0, resource, limits64, nullptr);
}

// There is no prlimit system call, so we need to use prlimit64.
int prlimit(pid_t pid, int resource, const rlimit* n32, rlimit* o32) {
  rlimit64 n64;
  if (n32 != nullptr) {
    n64.rlim_cur = (n32->rlim_cur == RLIM_INFINITY) ? RLIM64_INFINITY : n32->rlim_cur;
    n64.rlim_max = (n32->rlim_max == RLIM_INFINITY) ? RLIM64_INFINITY : n32->rlim_max;
  }

  rlimit64 o64;
  int result = prlimit64(pid, resource,
                         (n32 != nullptr) ? &n64 : nullptr,
                         (o32 != nullptr) ? &o64 : nullptr);
  if (result != -1 && o32 != nullptr) {
    o32->rlim_cur = (o64.rlim_cur == RLIM64_INFINITY) ? RLIM_INFINITY : o64.rlim_cur;
    o32->rlim_max = (o64.rlim_max == RLIM64_INFINITY) ? RLIM_INFINITY : o64.rlim_max;
  }
  return result;
}

// mmap2(2) is like mmap(2), but the offset is in 4096-byte blocks (regardless
// of page size), not bytes, to enable mapping parts of large files past the
// 4GiB limit but without the inconvenience of dealing with 64-bit values, with
// no down side since mappings need to be page aligned anyway, and the 32-bit
// architectures that support this system call all have 4KiB pages.
extern "C" void* __mmap2(void*, size_t, int, int, int, size_t);

void* mmap64(void* addr, size_t size, int prot, int flags, int fd, off64_t offset) {
  static constexpr size_t MMAP2_SHIFT = 12;

  if (offset < 0 || (offset & ((1UL << MMAP2_SHIFT) - 1)) != 0) {
    errno = EINVAL;
    return MAP_FAILED;
  }

  // Prevent allocations large enough for `end - start` to overflow,
  // to avoid security bugs.
  size_t rounded = __BIONIC_ALIGN(size, page_size());
  if (rounded < size || rounded > PTRDIFF_MAX) {
    errno = ENOMEM;
    return MAP_FAILED;
  }

  return __mmap2(addr, size, prot, flags, fd, offset >> MMAP2_SHIFT);
}

void* mmap(void* addr, size_t size, int prot, int flags, int fd, off_t offset) {
  return mmap64(addr, size, prot, flags, fd, static_cast<off64_t>(offset));
}

// The only difference here is that the libc API uses varargs for the
// optional `new_address` argument that's only used by MREMAP_FIXED.
extern "C" void* __mremap(void*, size_t, size_t, int, void*);

void* mremap(void* old_address, size_t old_size, size_t new_size, int flags, ...) {
  // Prevent allocations large enough for `end - start` to overflow,
  // to avoid security bugs.
  size_t rounded = __BIONIC_ALIGN(new_size, page_size());
  if (rounded < new_size || rounded > PTRDIFF_MAX) {
    errno = ENOMEM;
    return MAP_FAILED;
  }

  // The optional argument is only valid if the MREMAP_FIXED flag is set,
  // so we assume it's not present otherwise.
  void* new_address = nullptr;
  if ((flags & MREMAP_FIXED) != 0) {
    va_list ap;
    va_start(ap, flags);
    new_address = va_arg(ap, void*);
    va_end(ap);
  }
  return __mremap(old_address, old_size, new_size, flags, new_address);
}

// mseal(2) is LP64-only.
int mseal(void*, size_t, unsigned long) {
  errno = ENOSYS;
  return -1;
}
