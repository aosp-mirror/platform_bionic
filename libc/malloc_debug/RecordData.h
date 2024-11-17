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

#include <memory_trace/MemoryTrace.h>
#include <platform/bionic/macros.h>

class Config;

class RecordData {
 public:
  RecordData();
  virtual ~RecordData();

  bool Initialize(const Config& config);

  void AddEntry(const memory_trace::Entry& entry);
  void AddEntryOnly(const memory_trace::Entry& entry);

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
  std::vector<memory_trace::Entry> entries_;
  size_t cur_index_;
  std::string file_;

  BIONIC_DISALLOW_COPY_AND_ASSIGN(RecordData);
};
