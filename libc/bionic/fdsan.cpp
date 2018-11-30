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

#include <android/fdsan.h>

#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <async_safe/log.h>
#include <sys/system_properties.h>

#include "private/bionic_globals.h"
#include "private/bionic_inline_raise.h"
#include "pthread_internal.h"

extern "C" int ___close(int fd);
pid_t __get_cached_pid();

static constexpr const char* kFdsanPropertyName = "debug.fdsan";

template<size_t inline_fds>
FdEntry* FdTableImpl<inline_fds>::at(size_t idx) {
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

void __libc_init_fdsan() {
  constexpr auto default_level = ANDROID_FDSAN_ERROR_LEVEL_WARN_ONCE;
  const prop_info* pi = __system_property_find(kFdsanPropertyName);
  if (!pi) {
    android_fdsan_set_error_level(default_level);
    return;
  }
  __system_property_read_callback(
      pi,
      [](void*, const char*, const char* value, uint32_t) {
        if (strcasecmp(value, "1") == 0 || strcasecmp(value, "fatal") == 0) {
          android_fdsan_set_error_level(ANDROID_FDSAN_ERROR_LEVEL_FATAL);
        } else if (strcasecmp(value, "warn") == 0) {
          android_fdsan_set_error_level(ANDROID_FDSAN_ERROR_LEVEL_WARN_ALWAYS);
        } else if (strcasecmp(value, "warn_once") == 0) {
          android_fdsan_set_error_level(ANDROID_FDSAN_ERROR_LEVEL_WARN_ONCE);
        } else {
          if (strlen(value) != 0 && strcasecmp(value, "0") != 0) {
            async_safe_format_log(ANDROID_LOG_ERROR, "libc",
                                  "debug.fdsan set to unknown value '%s', disabling", value);
          }
          android_fdsan_set_error_level(default_level);
        }
      },
      nullptr);
}

static FdTable& GetFdTable() {
  return __libc_shared_globals()->fd_table;
}

// Exposed to the platform to allow crash_dump to print out the fd table.
extern "C" void* android_fdsan_get_fd_table() {
  return &GetFdTable();
}

static FdEntry* GetFdEntry(int fd) {
  if (fd < 0) {
    return nullptr;
  }

  return GetFdTable().at(fd);
}

__printflike(1, 0) static void fdsan_error(const char* fmt, ...) {
  auto& fd_table = GetFdTable();

  auto error_level = atomic_load(&fd_table.error_level);
  if (error_level == ANDROID_FDSAN_ERROR_LEVEL_DISABLED) {
    return;
  }

  // Lots of code will (sensibly) fork, blindly call close on all of their fds,
  // and then exec. Compare our cached pid value against the real one to detect
  // this scenario and permit it.
  pid_t cached_pid = __get_cached_pid();
  if (cached_pid == 0 || cached_pid != syscall(__NR_getpid)) {
    return;
  }

  struct {
    size_t size;
    char buf[512];
  } abort_message;

  va_list va;
  va_start(va, fmt);
  if (error_level == ANDROID_FDSAN_ERROR_LEVEL_FATAL) {
    async_safe_fatal_va_list("fdsan", fmt, va);
  } else {
    async_safe_format_log_va_list(ANDROID_LOG_ERROR, "fdsan", fmt, va);
    va_end(va);
    va_start(va, fmt);
    size_t len =
        async_safe_format_buffer_va_list(abort_message.buf, sizeof(abort_message.buf), fmt, va);
    abort_message.size = len + sizeof(size_t);
  }
  va_end(va);

  switch (error_level) {
    case ANDROID_FDSAN_ERROR_LEVEL_WARN_ONCE:
      atomic_compare_exchange_strong(&fd_table.error_level, &error_level,
                                     ANDROID_FDSAN_ERROR_LEVEL_DISABLED);
      __BIONIC_FALLTHROUGH;
    case ANDROID_FDSAN_ERROR_LEVEL_WARN_ALWAYS:
      // DEBUGGER_SIGNAL
      inline_raise(__SIGRTMIN + 3, &abort_message);
      break;

    case ANDROID_FDSAN_ERROR_LEVEL_FATAL:
      inline_raise(SIGABRT);
      abort();

    case ANDROID_FDSAN_ERROR_LEVEL_DISABLED:
      break;
  }
}

uint64_t android_fdsan_create_owner_tag(android_fdsan_owner_type type, uint64_t tag) {
  if (tag == 0) {
    return 0;
  }

  if (__predict_false((type & 0xff) != type)) {
    async_safe_fatal("invalid android_fdsan_owner_type value: %x", type);
  }

  uint64_t result = static_cast<uint64_t>(type) << 56;
  uint64_t mask = (static_cast<uint64_t>(1) << 56) - 1;
  result |= tag & mask;
  return result;
}

const char* android_fdsan_get_tag_type(uint64_t tag) {
  uint64_t type = tag >> 56;
  switch (type) {
    case ANDROID_FDSAN_OWNER_TYPE_FILE:
      return "FILE*";
    case ANDROID_FDSAN_OWNER_TYPE_DIR:
      return "DIR*";
    case ANDROID_FDSAN_OWNER_TYPE_UNIQUE_FD:
      return "unique_fd";
    case ANDROID_FDSAN_OWNER_TYPE_FILEINPUTSTREAM:
      return "FileInputStream";
    case ANDROID_FDSAN_OWNER_TYPE_FILEOUTPUTSTREAM:
      return "FileOutputStream";
    case ANDROID_FDSAN_OWNER_TYPE_RANDOMACCESSFILE:
      return "RandomAccessFile";
    case ANDROID_FDSAN_OWNER_TYPE_PARCELFILEDESCRIPTOR:
      return "ParcelFileDescriptor";
    case ANDROID_FDSAN_OWNER_TYPE_SQLITE:
      return "sqlite";
    case ANDROID_FDSAN_OWNER_TYPE_ART_FDFILE:
      return "ART FdFile";
    case ANDROID_FDSAN_OWNER_TYPE_DATAGRAMSOCKETIMPL:
      return "DatagramSocketImpl";
    case ANDROID_FDSAN_OWNER_TYPE_SOCKETIMPL:
      return "SocketImpl";
    case ANDROID_FDSAN_OWNER_TYPE_ZIPARCHIVE:
      return "ZipArchive";

    case ANDROID_FDSAN_OWNER_TYPE_GENERIC_00:
    default:
      return "native object of unknown type";

    case ANDROID_FDSAN_OWNER_TYPE_GENERIC_FF:
      // If bits 48 to 56 are set, this is a sign-extended generic native pointer
      uint64_t high_bits = tag >> 48;
      if (high_bits == (1 << 16) - 1) {
        return "native object of unknown type";
      }

      return "Java object of unknown type";
  }
}

uint64_t android_fdsan_get_tag_value(uint64_t tag) {
  // Lop off the most significant byte and sign extend.
  return static_cast<uint64_t>(static_cast<int64_t>(tag << 8) >> 8);
}

int android_fdsan_close_with_tag(int fd, uint64_t expected_tag) {
  FdEntry* fde = GetFdEntry(fd);
  if (!fde) {
    return ___close(fd);
  }

  uint64_t tag = expected_tag;
  if (!atomic_compare_exchange_strong(&fde->close_tag, &tag, 0)) {
    const char* expected_type = android_fdsan_get_tag_type(expected_tag);
    uint64_t expected_owner = android_fdsan_get_tag_value(expected_tag);
    const char* actual_type = android_fdsan_get_tag_type(tag);
    uint64_t actual_owner = android_fdsan_get_tag_value(tag);
    if (expected_tag && tag) {
      fdsan_error(
          "attempted to close file descriptor %d, "
          "expected to be owned by %s 0x%" PRIx64 ", actually owned by %s 0x%" PRIx64,
          fd, expected_type, expected_owner, actual_type, actual_owner);
    } else if (expected_tag && !tag) {
      fdsan_error(
          "attempted to close file descriptor %d, "
          "expected to be owned by %s 0x%" PRIx64 ", actually unowned",
          fd, expected_type, expected_owner);
    } else if (!expected_tag && tag) {
      fdsan_error(
          "attempted to close file descriptor %d, "
          "expected to be unowned, actually owned by %s 0x%" PRIx64,
          fd, actual_type, actual_owner);
    } else if (!expected_tag && !tag) {
      // This should never happen: our CAS failed, but expected == actual?
      async_safe_fatal("fdsan atomic_compare_exchange_strong failed unexpectedly while closing");
    }
  }

  int rc = ___close(fd);
  // If we were expecting to close with a tag, abort on EBADF.
  if (expected_tag && rc == -1 && errno == EBADF) {
    fdsan_error("double-close of file descriptor %d detected", fd);
  }
  return rc;
}

uint64_t android_fdsan_get_owner_tag(int fd) {
  FdEntry* fde = GetFdEntry(fd);
  if (!fde) {
    return 0;
  }
  return fde->close_tag;
}

void android_fdsan_exchange_owner_tag(int fd, uint64_t expected_tag, uint64_t new_tag) {
  FdEntry* fde = GetFdEntry(fd);
  if (!fde) {
    return;
  }

  uint64_t tag = expected_tag;
  if (!atomic_compare_exchange_strong(&fde->close_tag, &tag, new_tag)) {
    if (expected_tag && tag) {
      fdsan_error(
          "failed to exchange ownership of file descriptor: fd %d is "
          "owned by %s 0x%" PRIx64 ", was expected to be owned by %s 0x%" PRIx64,
          fd, android_fdsan_get_tag_type(tag), android_fdsan_get_tag_value(tag),
          android_fdsan_get_tag_type(expected_tag), android_fdsan_get_tag_value(expected_tag));
    } else if (expected_tag && !tag) {
      fdsan_error(
          "failed to exchange ownership of file descriptor: fd %d is "
          "unowned, was expected to be owned by %s 0x%" PRIx64,
          fd, android_fdsan_get_tag_type(expected_tag), android_fdsan_get_tag_value(expected_tag));
    } else if (!expected_tag && tag) {
      fdsan_error(
          "failed to exchange ownership of file descriptor: fd %d is "
          "owned by %s 0x%" PRIx64 ", was expected to be unowned",
          fd, android_fdsan_get_tag_type(tag), android_fdsan_get_tag_value(tag));
    } else if (!expected_tag && !tag) {
      // This should never happen: our CAS failed, but expected == actual?
      async_safe_fatal(
          "fdsan atomic_compare_exchange_strong failed unexpectedly while exchanging owner tag");
    }
  }
}

android_fdsan_error_level android_fdsan_get_error_level() {
  return GetFdTable().error_level;
}

android_fdsan_error_level android_fdsan_set_error_level(android_fdsan_error_level new_level) {
  return atomic_exchange(&GetFdTable().error_level, new_level);
}

int close(int fd) {
  int rc = android_fdsan_close_with_tag(fd, 0);
  if (rc == -1 && errno == EINTR) {
    return 0;
  }
  return rc;
}
