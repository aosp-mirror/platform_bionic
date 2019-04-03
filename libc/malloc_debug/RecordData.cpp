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
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <mutex>

#include <android-base/stringprintf.h>

#include "Config.h"
#include "DebugData.h"
#include "RecordData.h"
#include "debug_disable.h"
#include "debug_log.h"

RecordEntry::RecordEntry() : tid_(gettid()) {
}

std::string ThreadCompleteEntry::GetString() const {
  return android::base::StringPrintf("%d: thread_done 0x0\n", tid_);
}

AllocEntry::AllocEntry(void* pointer) : pointer_(pointer) {}

MallocEntry::MallocEntry(void* pointer, size_t size) : AllocEntry(pointer), size_(size) {}

std::string MallocEntry::GetString() const {
  return android::base::StringPrintf("%d: malloc %p %zu\n", tid_, pointer_, size_);
}

FreeEntry::FreeEntry(void* pointer) : AllocEntry(pointer) {}

std::string FreeEntry::GetString() const {
  return android::base::StringPrintf("%d: free %p\n", tid_, pointer_);
}

CallocEntry::CallocEntry(void* pointer, size_t nmemb, size_t size)
    : MallocEntry(pointer, size), nmemb_(nmemb) {}

std::string CallocEntry::GetString() const {
  return android::base::StringPrintf("%d: calloc %p %zu %zu\n", tid_, pointer_, nmemb_, size_);
}

ReallocEntry::ReallocEntry(void* pointer, size_t size, void* old_pointer)
    : MallocEntry(pointer, size), old_pointer_(old_pointer) {}

std::string ReallocEntry::GetString() const {
  return android::base::StringPrintf("%d: realloc %p %p %zu\n", tid_, pointer_, old_pointer_, size_);
}

// aligned_alloc, posix_memalign, memalign, pvalloc, valloc all recorded with this class.
MemalignEntry::MemalignEntry(void* pointer, size_t size, size_t alignment)
    : MallocEntry(pointer, size), alignment_(alignment) {}

std::string MemalignEntry::GetString() const {
  return android::base::StringPrintf("%d: memalign %p %zu %zu\n", tid_, pointer_, alignment_, size_);
}

struct ThreadData {
  ThreadData(RecordData* record_data, ThreadCompleteEntry* entry)
      : record_data(record_data), entry(entry) {}
  RecordData* record_data;
  ThreadCompleteEntry* entry;
  size_t count = 0;
};

static void ThreadKeyDelete(void* data) {
  ThreadData* thread_data = reinterpret_cast<ThreadData*>(data);

  thread_data->count++;

  // This should be the last time we are called.
  if (thread_data->count == 4) {
    ScopedDisableDebugCalls disable;

    thread_data->record_data->AddEntryOnly(thread_data->entry);
    delete thread_data;
  } else {
    pthread_setspecific(thread_data->record_data->key(), data);
  }
}

static void RecordDump(int, siginfo_t*, void*) {
  // It's not necessarily safe to do the dump here, instead wait for the
  // next allocation call to do the dump.
  g_debug->record->SetToDump();
}

void RecordData::Dump() {
  std::lock_guard<std::mutex> lock(dump_lock_);

  // Make it so that no more entries can be added while dumping.
  unsigned int last_entry_index = cur_index_.exchange(static_cast<unsigned int>(num_entries_));
  if (dump_ == false) {
    // Multiple Dump() calls from different threads, and we lost. Do nothing.
    return;
  }

  // cur_index_ keeps getting incremented even if we hit the num_entries_.
  // If that happens, cap the entries to dump by num_entries_.
  if (last_entry_index > num_entries_) {
    last_entry_index = num_entries_;
  }

  int dump_fd =
      open(dump_file_.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC | O_NOFOLLOW, 0755);
  if (dump_fd != -1) {
    for (size_t i = 0; i < last_entry_index; i++) {
      std::string line = entries_[i]->GetString();
      ssize_t bytes = write(dump_fd, line.c_str(), line.length());
      if (bytes == -1 || static_cast<size_t>(bytes) != line.length()) {
        error_log("Failed to write record alloc information: %s", strerror(errno));
        // Free all of the rest of the errors, we don't have any way
        // to dump a partial list of the entries.
        for (i++; i < last_entry_index; i++) {
          delete entries_[i];
          entries_[i] = nullptr;
        }
        break;
      }
      delete entries_[i];
      entries_[i] = nullptr;
    }
    close(dump_fd);

    // Mark the entries dumped.
    cur_index_ = 0U;
  } else {
    error_log("Cannot create record alloc file %s: %s", dump_file_.c_str(), strerror(errno));
    // Since we couldn't create the file, reset the entries dumped back
    // to the original value.
    cur_index_ = last_entry_index;
  }

  dump_ = false;
}

RecordData::RecordData() {
  pthread_key_create(&key_, ThreadKeyDelete);
}

bool RecordData::Initialize(const Config& config) {
  struct sigaction64 dump_act = {};
  dump_act.sa_sigaction = RecordDump;
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

  num_entries_ = config.record_allocs_num_entries();
  entries_ = new const RecordEntry*[num_entries_];
  cur_index_ = 0;
  dump_ = false;
  dump_file_ = config.record_allocs_file();

  return true;
}

RecordData::~RecordData() {
  delete[] entries_;
  pthread_key_delete(key_);
}

void RecordData::AddEntryOnly(const RecordEntry* entry) {
  unsigned int entry_index = cur_index_.fetch_add(1);
  if (entry_index < num_entries_) {
    entries_[entry_index] = entry;
  }
}

void RecordData::AddEntry(const RecordEntry* entry) {
  void* data = pthread_getspecific(key_);
  if (data == nullptr) {
    ThreadData* thread_data = new ThreadData(this, new ThreadCompleteEntry());
    pthread_setspecific(key_, thread_data);
  }

  AddEntryOnly(entry);

  // Check to see if it's time to dump the entries.
  if (dump_) {
    Dump();
  }
}
