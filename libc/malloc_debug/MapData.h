/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <sys/cdefs.h>

#include <mutex>
#include <set>
#include <string>

#include <platform/bionic/macros.h>

class MapEntry {
 public:
  MapEntry() = default;
  MapEntry(uintptr_t start, uintptr_t end, uintptr_t offset, const char* name, size_t name_len,
           int flags)
      : start_(start), end_(end), offset_(offset), name_(name, name_len), flags_(flags) {}

  explicit MapEntry(uintptr_t pc) : start_(pc), end_(pc) {}

  void Init();

  uintptr_t GetLoadBias();

  void SetInvalid() {
    valid_ = false;
    init_ = true;
    load_bias_read_ = true;
  }

  bool valid() { return valid_; }
  uintptr_t start() const { return start_; }
  uintptr_t end() const { return end_; }
  uintptr_t offset() const { return offset_; }
  uintptr_t elf_start_offset() const { return elf_start_offset_; }
  void set_elf_start_offset(uintptr_t elf_start_offset) { elf_start_offset_ = elf_start_offset; }
  const std::string& name() const { return name_; }
  int flags() const { return flags_; }

 private:
  uintptr_t start_;
  uintptr_t end_;
  uintptr_t offset_;
  uintptr_t load_bias_ = 0;
  uintptr_t elf_start_offset_ = 0;
  std::string name_;
  int flags_;
  bool init_ = false;
  bool valid_ = false;
  bool load_bias_read_ = false;
};

// Ordering comparator that returns equivalence for overlapping entries
struct compare_entries {
  bool operator()(const MapEntry* a, const MapEntry* b) const { return a->end() <= b->start(); }
};

class MapData {
 public:
  MapData() = default;
  ~MapData();

  const MapEntry* find(uintptr_t pc, uintptr_t* rel_pc = nullptr);

  size_t NumMaps() { return entries_.size(); }

  void ReadMaps();

 private:
  std::mutex m_;
  std::set<MapEntry*, compare_entries> entries_;

  void ClearEntries();

  BIONIC_DISALLOW_COPY_AND_ASSIGN(MapData);
};
