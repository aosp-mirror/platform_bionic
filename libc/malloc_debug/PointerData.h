/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <stdint.h>
#include <stdio.h>

#include <atomic>
#include <deque>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <platform/bionic/macros.h>
#include <unwindstack/Unwinder.h>

#include "OptionData.h"
#include "UnwindBacktrace.h"

extern bool* g_zygote_child;

// Forward declarations.
class Config;

struct FrameKeyType {
  size_t num_frames;
  uintptr_t* frames;

  bool operator==(const FrameKeyType& comp) const {
    if (num_frames != comp.num_frames) return false;
    for (size_t i = 0; i < num_frames; i++) {
      if (frames[i] != comp.frames[i]) {
        return false;
      }
    }
    return true;
  }
};

namespace std {
template <>
struct hash<FrameKeyType> {
  std::size_t operator()(const FrameKeyType& key) const {
    std::size_t cur_hash = key.frames[0];
    // Limit the number of frames to speed up hashing.
    size_t max_frames = (key.num_frames > 5) ? 5 : key.num_frames;
    for (size_t i = 1; i < max_frames; i++) {
      cur_hash ^= key.frames[i];
    }
    return cur_hash;
  }
};
};  // namespace std

struct FrameInfoType {
  size_t references = 0;
  std::vector<uintptr_t> frames;
};

struct PointerInfoType {
  size_t size;
  size_t hash_index;
  size_t RealSize() const { return size & ~(1U << 31); }
  bool ZygoteChildAlloc() const { return size & (1U << 31); }
  static size_t GetEncodedSize(size_t size) {
    return GetEncodedSize(*g_zygote_child, size);
  }
  static size_t GetEncodedSize(bool child_alloc, size_t size) {
    return size | ((child_alloc) ? (1U << 31) : 0);
  }
  static size_t MaxSize() { return (1U << 31) - 1; }
};

struct FreePointerInfoType {
  uintptr_t mangled_ptr;
  size_t hash_index;
};

struct ListInfoType {
  uintptr_t pointer;
  size_t num_allocations;
  size_t size;
  bool zygote_child_alloc;
  FrameInfoType* frame_info;
  std::vector<unwindstack::FrameData>* backtrace_info;
};

class PointerData : public OptionData {
 public:
  explicit PointerData(DebugData* debug_data);
  virtual ~PointerData() = default;

  bool Initialize(const Config& config);

  inline size_t alloc_offset() { return alloc_offset_; }

  bool ShouldBacktrace() { return backtrace_enabled_ == 1; }
  void ToggleBacktraceEnabled() { backtrace_enabled_.fetch_xor(1); }

  void EnableDumping() { backtrace_dump_ = true; }
  bool ShouldDumpAndReset() {
    bool expected = true;
    return backtrace_dump_.compare_exchange_strong(expected, false);
  }

  void PrepareFork();
  void PostForkParent();
  void PostForkChild();

  static void IteratePointers(std::function<void(uintptr_t pointer)> fn);

  static size_t AddBacktrace(size_t num_frames);
  static void RemoveBacktrace(size_t hash_index);

  static void Add(const void* pointer, size_t size);
  static void Remove(const void* pointer);

  static void* AddFreed(const void* pointer);
  static void LogFreeError(const FreePointerInfoType& info, size_t usable_size);
  static void LogFreeBacktrace(const void* ptr);
  static void VerifyFreedPointer(const FreePointerInfoType& info);
  static void VerifyAllFreed();

  static void GetAllocList(std::vector<ListInfoType>* list);
  static void LogLeaks();
  static void DumpLiveToFile(int fd);

  static void GetInfo(uint8_t** info, size_t* overall_size, size_t* info_size, size_t* total_memory,
                      size_t* backtrace_size);

  static size_t GetFrames(const void* pointer, uintptr_t* frames, size_t max_frames);

  static bool Exists(const void* pointer);

 private:
  // Only keep mangled pointers in internal data structures. This avoids
  // problems where libmemunreachable finds these pointers and thinks they
  // are not unreachable.
  static inline uintptr_t ManglePointer(uintptr_t pointer) { return pointer ^ UINTPTR_MAX; }
  static inline uintptr_t DemanglePointer(uintptr_t pointer) { return pointer ^ UINTPTR_MAX; }

  static std::string GetHashString(uintptr_t* frames, size_t num_frames);
  static void LogBacktrace(size_t hash_index);

  static void GetList(std::vector<ListInfoType>* list, bool only_with_backtrace);
  static void GetUniqueList(std::vector<ListInfoType>* list, bool only_with_backtrace);

  size_t alloc_offset_ = 0;
  std::vector<uint8_t> cmp_mem_;

  static std::atomic_uint8_t backtrace_enabled_;

  static std::atomic_bool backtrace_dump_;

  static std::mutex pointer_mutex_;
  static std::unordered_map<uintptr_t, PointerInfoType> pointers_;

  static std::mutex frame_mutex_;
  static std::unordered_map<FrameKeyType, size_t> key_to_index_;
  static std::unordered_map<size_t, FrameInfoType> frames_;
  static std::unordered_map<size_t, std::vector<unwindstack::FrameData>> backtraces_info_;
  static size_t cur_hash_index_;

  static std::mutex free_pointer_mutex_;
  static std::deque<FreePointerInfoType> free_pointers_;

  BIONIC_DISALLOW_COPY_AND_ASSIGN(PointerData);
};
