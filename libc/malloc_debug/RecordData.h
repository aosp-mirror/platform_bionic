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

#pragma once

#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <platform/bionic/macros.h>

class RecordEntry {
 public:
  RecordEntry();
  virtual ~RecordEntry() = default;

  virtual bool Write(int fd) const = 0;

 protected:
  pid_t tid_;

 private:
  BIONIC_DISALLOW_COPY_AND_ASSIGN(RecordEntry);
};

class ThreadCompleteEntry : public RecordEntry {
 public:
  ThreadCompleteEntry() = default;
  virtual ~ThreadCompleteEntry() = default;

  bool Write(int fd) const override;

 private:
  BIONIC_DISALLOW_COPY_AND_ASSIGN(ThreadCompleteEntry);
};

class AllocEntry : public RecordEntry {
 public:
  explicit AllocEntry(void* pointer, uint64_t st, uint64_t et);
  virtual ~AllocEntry() = default;

 protected:
  void* pointer_;

  // The start/end time of this operation.
  uint64_t start_ns_;
  uint64_t end_ns_;

 private:
  BIONIC_DISALLOW_COPY_AND_ASSIGN(AllocEntry);
};

class MallocEntry : public AllocEntry {
 public:
  MallocEntry(void* pointer, size_t size, uint64_t st, uint64_t et);
  virtual ~MallocEntry() = default;

  bool Write(int fd) const override;

 protected:
  size_t size_;

 private:
  BIONIC_DISALLOW_COPY_AND_ASSIGN(MallocEntry);
};

class FreeEntry : public AllocEntry {
 public:
  explicit FreeEntry(void* pointer, uint64_t st, uint64_t et);
  virtual ~FreeEntry() = default;

  bool Write(int fd) const override;

 private:
  BIONIC_DISALLOW_COPY_AND_ASSIGN(FreeEntry);
};

class CallocEntry : public MallocEntry {
 public:
  CallocEntry(void* pointer, size_t nmemb, size_t size, uint64_t st, uint64_t et);
  virtual ~CallocEntry() = default;

  bool Write(int fd) const override;

 protected:
  size_t nmemb_;

 private:
  BIONIC_DISALLOW_COPY_AND_ASSIGN(CallocEntry);
};

class ReallocEntry : public MallocEntry {
 public:
  ReallocEntry(void* pointer, size_t size, void* old_pointer, uint64_t st, uint64_t et);
  virtual ~ReallocEntry() = default;

  bool Write(int fd) const override;

 protected:
  void* old_pointer_;

 private:
  BIONIC_DISALLOW_COPY_AND_ASSIGN(ReallocEntry);
};

// aligned_alloc, posix_memalign, memalign, pvalloc, valloc all recorded with this class.
class MemalignEntry : public MallocEntry {
 public:
  MemalignEntry(void* pointer, size_t size, size_t alignment, uint64_t st, uint64_t et);
  virtual ~MemalignEntry() = default;

  bool Write(int fd) const override;

 protected:
  size_t alignment_;

 private:
  BIONIC_DISALLOW_COPY_AND_ASSIGN(MemalignEntry);
};

class Config;

class RecordData {
 public:
  RecordData();
  virtual ~RecordData();

  bool Initialize(const Config& config);

  void AddEntry(const RecordEntry* entry);
  void AddEntryOnly(const RecordEntry* entry);

  const std::string& file() { return file_; }
  pthread_key_t key() { return key_; }

  static void WriteEntriesOnExit();

 private:
  static void WriteData(int, siginfo_t*, void*);
  static RecordData* record_obj_;

  void WriteEntries();
  void WriteEntries(const std::string& file);

  std::mutex entries_lock_;
  pthread_key_t key_;
  std::vector<std::unique_ptr<const RecordEntry>> entries_;
  size_t cur_index_;
  std::string file_;

  BIONIC_DISALLOW_COPY_AND_ASSIGN(RecordData);
};
