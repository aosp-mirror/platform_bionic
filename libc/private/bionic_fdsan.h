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

#pragma once

#include <android/fdsan.h>

#include <errno.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/user.h>

#include <async_safe/log.h>

struct FdEntry {
  _Atomic(uint64_t) close_tag;
};

struct FdTableOverflow {
  size_t len;
  FdEntry entries[0];
};

template <size_t inline_fds>
struct FdTable {
  _Atomic(android_fdsan_error_level) error_level;

  FdEntry entries[inline_fds];
  _Atomic(FdTableOverflow*) overflow;

  FdEntry* at(size_t idx) {
    if (idx < inline_fds) {
      return &entries[idx];
    }

    // Try to create the overflow table ourselves.
    FdTableOverflow* local_overflow = atomic_load(&overflow);
    if (__predict_false(!local_overflow)) {
      struct rlimit rlim = { .rlim_max = 32768 };
      getrlimit(RLIMIT_NOFILE, &rlim);
      rlim_t max = rlim.rlim_max;

      if (max == RLIM_INFINITY) {
        // This isn't actually possible (the kernel has a hard limit), but just
        // in case...
        max = 32768;
      }

      if (idx > max) {
        // This can happen if an fd is created and then the rlimit is lowered.
        // In this case, just return nullptr and ignore the fd.
        return nullptr;
      }

      size_t required_count = max - inline_fds;
      size_t required_size = sizeof(FdTableOverflow) + required_count * sizeof(FdEntry);
      size_t aligned_size = __BIONIC_ALIGN(required_size, PAGE_SIZE);
      size_t aligned_count = (aligned_size - sizeof(FdTableOverflow)) / sizeof(FdEntry);

      void* allocation =
          mmap(nullptr, aligned_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
      if (allocation == MAP_FAILED) {
        async_safe_fatal("fdsan: mmap failed: %s", strerror(errno));
      }

      FdTableOverflow* new_overflow = reinterpret_cast<FdTableOverflow*>(allocation);
      new_overflow->len = aligned_count;

      if (atomic_compare_exchange_strong(&overflow, &local_overflow, new_overflow)) {
        local_overflow = new_overflow;
      } else {
        // Someone beat us to it. Deallocate and use theirs.
        munmap(allocation, aligned_size);
      }
    }

    size_t offset = idx - inline_fds;
    if (local_overflow->len < offset) {
      return nullptr;
    }
    return &local_overflow->entries[offset];
  }
};
