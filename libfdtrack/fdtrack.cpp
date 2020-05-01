/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <inttypes.h>
#include <stdint.h>

#include <array>
#include <mutex>
#include <vector>

#include <android/fdsan.h>
#include <bionic/fdtrack.h>

#include <android-base/no_destructor.h>
#include <android-base/thread_annotations.h>
#include <async_safe/log.h>
#include <bionic/reserved_signals.h>
#include <unwindstack/LocalUnwinder.h>

struct FdEntry {
  std::mutex mutex;
  std::vector<unwindstack::LocalFrameData> backtrace GUARDED_BY(mutex);
};

extern "C" void fdtrack_dump();

using fdtrack_callback_t = bool (*)(int fd, const char* const* function_names,
                                    const uint64_t* function_offsets, size_t count, void* arg);
extern "C" void fdtrack_iterate(fdtrack_callback_t callback, void* arg);

static void fd_hook(android_fdtrack_event* event);

// Backtraces for the first 4k file descriptors ought to be enough to diagnose an fd leak.
static constexpr size_t kFdTableSize = 4096;
static constexpr size_t kStackDepth = 10;

static bool installed = false;
static std::array<FdEntry, kFdTableSize> stack_traces [[clang::no_destroy]];
static unwindstack::LocalUnwinder& Unwinder() {
  static android::base::NoDestructor<unwindstack::LocalUnwinder> unwinder;
  return *unwinder.get();
}

__attribute__((constructor)) static void ctor() {
  for (auto& entry : stack_traces) {
    entry.backtrace.reserve(kStackDepth);
  }

  signal(BIONIC_SIGNAL_FDTRACK, [](int) { fdtrack_dump(); });
  if (Unwinder().Init()) {
    android_fdtrack_hook_t expected = nullptr;
    installed = android_fdtrack_compare_exchange_hook(&expected, &fd_hook);
  }
}

__attribute__((destructor)) static void dtor() {
  if (installed) {
    android_fdtrack_hook_t expected = &fd_hook;
    android_fdtrack_compare_exchange_hook(&expected, nullptr);
  }
}

FdEntry* GetFdEntry(int fd) {
  if (fd >= 0 && fd < static_cast<int>(kFdTableSize)) {
    return &stack_traces[fd];
  }
  return nullptr;
}

static void fd_hook(android_fdtrack_event* event) {
  if (event->type == ANDROID_FDTRACK_EVENT_TYPE_CREATE) {
    if (FdEntry* entry = GetFdEntry(event->fd); entry) {
      std::lock_guard<std::mutex> lock(entry->mutex);
      entry->backtrace.clear();
      Unwinder().Unwind(&entry->backtrace, kStackDepth);
    }
  } else if (event->type == ANDROID_FDTRACK_EVENT_TYPE_CLOSE) {
    if (FdEntry* entry = GetFdEntry(event->fd); entry) {
      std::lock_guard<std::mutex> lock(entry->mutex);
      entry->backtrace.clear();
    }
  }
}

void fdtrack_iterate(fdtrack_callback_t callback, void* arg) {
  bool prev = android_fdtrack_set_enabled(false);

  for (int fd = 0; fd < static_cast<int>(stack_traces.size()); ++fd) {
    const char* function_names[kStackDepth];
    uint64_t function_offsets[kStackDepth];
    FdEntry* entry = GetFdEntry(fd);
    if (!entry) {
      continue;
    }

    if (!entry->mutex.try_lock()) {
      async_safe_format_log(ANDROID_LOG_WARN, "fdtrack", "fd %d locked, skipping", fd);
      continue;
    }

    if (entry->backtrace.empty()) {
      entry->mutex.unlock();
      continue;
    } else if (entry->backtrace.size() < 2) {
      async_safe_format_log(ANDROID_LOG_WARN, "fdtrack", "fd %d missing frames: size = %zu", fd,
                            entry->backtrace.size());

      entry->mutex.unlock();
      continue;
    }

    constexpr size_t frame_skip = 2;
    for (size_t i = frame_skip; i < entry->backtrace.size(); ++i) {
      size_t j = i - frame_skip;
      function_names[j] = entry->backtrace[i].function_name.c_str();
      function_offsets[j] = entry->backtrace[i].function_offset;
    }

    bool should_continue =
        callback(fd, function_names, function_offsets, entry->backtrace.size() - frame_skip, arg);

    entry->mutex.unlock();

    if (!should_continue) {
      break;
    }
  }

  android_fdtrack_set_enabled(prev);
}

void fdtrack_dump() {
  if (!installed) {
    async_safe_format_log(ANDROID_LOG_INFO, "fdtrack", "fdtrack not installed");
  } else {
    async_safe_format_log(ANDROID_LOG_INFO, "fdtrack", "fdtrack dumping...");
  }

  fdtrack_iterate(
      [](int fd, const char* const* function_names, const uint64_t* function_offsets, size_t count,
         void*) {
        uint64_t fdsan_owner = android_fdsan_get_owner_tag(fd);
        if (fdsan_owner != 0) {
          async_safe_format_log(ANDROID_LOG_INFO, "fdtrack", "fd %d: (owner = 0x%" PRIx64 ")", fd,
                                fdsan_owner);
        } else {
          async_safe_format_log(ANDROID_LOG_INFO, "fdtrack", "fd %d: (unowned)", fd);
        }

        for (size_t i = 0; i < count; ++i) {
          async_safe_format_log(ANDROID_LOG_INFO, "fdtrack", "  %zu: %s+%" PRIu64, i,
                                function_names[i], function_offsets[i]);
        }

        return true;
      },
      nullptr);
}
