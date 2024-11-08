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

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <mutex>

#include <memory_trace/MemoryTrace.h>

#include "Config.h"
#include "DebugData.h"
#include "Nanotime.h"
#include "RecordData.h"
#include "debug_disable.h"
#include "debug_log.h"

struct ThreadData {
  ThreadData(RecordData* record_data) : record_data(record_data) {}

  RecordData* record_data = nullptr;
  size_t count = 0;
};

void RecordData::ThreadKeyDelete(void* data) {
  ThreadData* thread_data = reinterpret_cast<ThreadData*>(data);

  thread_data->count++;

  // This should be the last time we are called.
  if (thread_data->count == 4) {
    ScopedDisableDebugCalls disable;

    memory_trace::Entry* entry = thread_data->record_data->InternalReserveEntry();
    if (entry != nullptr) {
      *entry = memory_trace::Entry{
          .tid = gettid(), .type = memory_trace::THREAD_DONE, .end_ns = Nanotime()};
    }
    delete thread_data;
  } else {
    pthread_setspecific(thread_data->record_data->key(), data);
  }
}

RecordData* RecordData::record_obj_ = nullptr;

void RecordData::WriteData(int, siginfo_t*, void*) {
  // Dump from here, the function must not allocate so this is safe.
  record_obj_->WriteEntries();
}

void RecordData::WriteEntriesOnExit() {
  if (record_obj_ == nullptr) return;

  // Append the current pid to the file name to avoid multiple processes
  // writing to the same file.
  std::string file(record_obj_->file());
  file += "." + std::to_string(getpid());
  record_obj_->WriteEntries(file);
}

void RecordData::WriteEntries() {
  WriteEntries(file_);
}

void RecordData::WriteEntries(const std::string& file) {
  std::lock_guard<std::mutex> entries_lock(entries_lock_);
  if (cur_index_ == 0) {
    info_log("No alloc entries to write.");
    return;
  }

  int dump_fd = open(file.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC | O_NOFOLLOW, 0755);
  if (dump_fd == -1) {
    error_log("Cannot create record alloc file %s: %s", file.c_str(), strerror(errno));
    return;
  }

  for (size_t i = 0; i < cur_index_; i++) {
    if (entries_[i].type == memory_trace::UNKNOWN) {
      // This can happen if an entry was reserved but not filled in due to some
      // type of error during the operation.
      continue;
    }
    if (!memory_trace::WriteEntryToFd(dump_fd, entries_[i])) {
      error_log("Failed to write record alloc information: %s", strerror(errno));
      break;
    }
  }
  close(dump_fd);

  // Mark the entries dumped.
  cur_index_ = 0U;
}

RecordData::RecordData() {
  pthread_key_create(&key_, ThreadKeyDelete);
}

bool RecordData::Initialize(const Config& config) {
  record_obj_ = this;
  struct sigaction64 dump_act = {};
  dump_act.sa_sigaction = RecordData::WriteData;
  dump_act.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
  if (sigaction64(config.record_allocs_signal(), &dump_act, nullptr) != 0) {
    error_log("Unable to set up record dump signal function: %s", strerror(errno));
    return false;
  }
  pthread_setspecific(key_, nullptr);

  if (config.options() & VERBOSE) {
    info_log("%s: Run: 'kill -%d %d' to dump the allocation records.", getprogname(),
             config.record_allocs_signal(), getpid());
  }

  entries_.resize(config.record_allocs_num_entries());
  cur_index_ = 0U;
  file_ = config.record_allocs_file();

  pagemap_fd_ = TEMP_FAILURE_RETRY(open("/proc/self/pagemap", O_RDONLY | O_CLOEXEC));
  if (pagemap_fd_ == -1) {
    error_log("Unable to open /proc/self/pagemap: %s", strerror(errno));
    return false;
  }

  return true;
}

RecordData::~RecordData() {
  if (pagemap_fd_ != -1) {
    close(pagemap_fd_);
  }

  pthread_key_delete(key_);
}

memory_trace::Entry* RecordData::InternalReserveEntry() {
  std::lock_guard<std::mutex> entries_lock(entries_lock_);
  if (cur_index_ == entries_.size()) {
    return nullptr;
  }

  memory_trace::Entry* entry = &entries_[cur_index_];
  entry->type = memory_trace::UNKNOWN;
  if (++cur_index_ == entries_.size()) {
    info_log("Maximum number of records added, all new operations will be dropped.");
  }
  return entry;
}

memory_trace::Entry* RecordData::ReserveEntry() {
  void* data = pthread_getspecific(key_);
  if (data == nullptr) {
    ThreadData* thread_data = new ThreadData(this);
    pthread_setspecific(key_, thread_data);
  }

  return InternalReserveEntry();
}

static inline bool IsPagePresent(uint64_t page_data) {
  // Page Present is bit 63
  return (page_data & (1ULL << 63)) != 0;
}

int64_t RecordData::GetPresentBytes(void* ptr, size_t alloc_size) {
  uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
  if (addr == 0 || alloc_size == 0) {
    return -1;
  }

  uintptr_t page_size = getpagesize();
  uintptr_t page_size_mask = page_size - 1;

  size_t start_page = (addr & ~page_size_mask) / page_size;
  size_t last_page = ((addr + alloc_size - 1) & ~page_size_mask) / page_size;

  constexpr size_t kMaxReadPages = 1024;
  uint64_t page_data[kMaxReadPages];

  int64_t present_bytes = 0;
  size_t cur_page = start_page;
  while (cur_page <= last_page) {
    size_t num_pages = last_page - cur_page + 1;
    size_t last_page_index;
    if (num_pages > kMaxReadPages) {
      num_pages = kMaxReadPages;
      last_page_index = num_pages;
    } else {
      // Handle the last page differently, so do not handle it in the loop.
      last_page_index = num_pages - 1;
    }
    ssize_t bytes_read =
        pread64(pagemap_fd_, page_data, num_pages * sizeof(uint64_t), cur_page * sizeof(uint64_t));
    if (bytes_read <= 0) {
      error_log("Failed to read page data: %s", strerror(errno));
      return -1;
    }

    size_t page_index = 0;
    // Handling the first page is special, handle it separately.
    if (cur_page == start_page) {
      if (IsPagePresent(page_data[0])) {
        present_bytes = page_size - (addr & page_size_mask);
        if (present_bytes >= alloc_size) {
          // The allocation fits on a single page and that page is present.
          return alloc_size;
        }
      } else if (start_page == last_page) {
        // Only one page that isn't present.
        return 0;
      }
      page_index = 1;
    }

    for (; page_index < last_page_index; page_index++) {
      if (IsPagePresent(page_data[page_index])) {
        present_bytes += page_size;
      }
    }

    cur_page += last_page_index;

    // Check the last page in the allocation.
    if (cur_page == last_page) {
      if (IsPagePresent(page_data[num_pages - 1])) {
        present_bytes += ((addr + alloc_size - 1) & page_size_mask) + 1;
      }
      return present_bytes;
    }
  }

  return present_bytes;
}
