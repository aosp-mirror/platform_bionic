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

bool ThreadCompleteEntry::Write(int fd) const {
  return dprintf(fd, "%d: thread_done 0x0\n", tid_) > 0;
}

AllocEntry::AllocEntry(void* pointer) : pointer_(pointer) {}

MallocEntry::MallocEntry(void* pointer, size_t size) : AllocEntry(pointer), size_(size) {}

bool MallocEntry::Write(int fd) const {
  return dprintf(fd, "%d: malloc %p %zu\n", tid_, pointer_, size_) > 0;
}

FreeEntry::FreeEntry(void* pointer) : AllocEntry(pointer) {}

bool FreeEntry::Write(int fd) const {
  return dprintf(fd, "%d: free %p\n", tid_, pointer_) > 0;
}

CallocEntry::CallocEntry(void* pointer, size_t nmemb, size_t size)
    : MallocEntry(pointer, size), nmemb_(nmemb) {}

bool CallocEntry::Write(int fd) const {
  return dprintf(fd, "%d: calloc %p %zu %zu\n", tid_, pointer_, nmemb_, size_) > 0;
}

ReallocEntry::ReallocEntry(void* pointer, size_t size, void* old_pointer)
    : MallocEntry(pointer, size), old_pointer_(old_pointer) {}

bool ReallocEntry::Write(int fd) const {
  return dprintf(fd, "%d: realloc %p %p %zu\n", tid_, pointer_, old_pointer_, size_) > 0;
}

// aligned_alloc, posix_memalign, memalign, pvalloc, valloc all recorded with this class.
MemalignEntry::MemalignEntry(void* pointer, size_t size, size_t alignment)
    : MallocEntry(pointer, size), alignment_(alignment) {}

bool MemalignEntry::Write(int fd) const {
  return dprintf(fd, "%d: memalign %p %zu %zu\n", tid_, pointer_, alignment_, size_) > 0;
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

RecordData* RecordData::record_obj_ = nullptr;

void RecordData::WriteData(int, siginfo_t*, void*) {
  // Dump from here, the function must not allocate so this is safe.
  record_obj_->WriteEntries();
}

void RecordData::WriteEntries() {
  std::lock_guard<std::mutex> entries_lock(entries_lock_);
  if (cur_index_ == 0) {
    info_log("No alloc entries to write.");
    return;
  }

  int dump_fd =
      open(dump_file_.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC | O_NOFOLLOW, 0755);
  if (dump_fd == -1) {
    error_log("Cannot create record alloc file %s: %s", dump_file_.c_str(), strerror(errno));
    return;
  }

  for (size_t i = 0; i < cur_index_; i++) {
    if (!entries_[i]->Write(dump_fd)) {
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
  dump_file_ = config.record_allocs_file();

  return true;
}

RecordData::~RecordData() {
  pthread_key_delete(key_);
}

void RecordData::AddEntryOnly(const RecordEntry* entry) {
  std::lock_guard<std::mutex> entries_lock(entries_lock_);
  if (cur_index_ == entries_.size()) {
    // Maxed out, throw the entry away.
    return;
  }

  entries_[cur_index_++].reset(entry);
  if (cur_index_ == entries_.size()) {
    info_log("Maximum number of records added, all new operations will be dropped.");
  }
}

void RecordData::AddEntry(const RecordEntry* entry) {
  void* data = pthread_getspecific(key_);
  if (data == nullptr) {
    ThreadData* thread_data = new ThreadData(this, new ThreadCompleteEntry());
    pthread_setspecific(key_, thread_data);
  }

  AddEntryOnly(entry);
}
