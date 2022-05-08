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

#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <android-base/stringprintf.h>
#include <android-base/thread_annotations.h>
#include <platform/bionic/macros.h>

#include "Config.h"
#include "DebugData.h"
#include "PointerData.h"
#include "backtrace.h"
#include "debug_log.h"
#include "malloc_debug.h"
#include "UnwindBacktrace.h"

extern "C" char* __cxa_demangle(const char*, char*, size_t*, int*);

std::atomic_uint8_t PointerData::backtrace_enabled_;
std::atomic_bool PointerData::backtrace_dump_;

std::mutex PointerData::pointer_mutex_;
std::unordered_map<uintptr_t, PointerInfoType> PointerData::pointers_ GUARDED_BY(
    PointerData::pointer_mutex_);

std::mutex PointerData::frame_mutex_;
std::unordered_map<FrameKeyType, size_t> PointerData::key_to_index_ GUARDED_BY(
    PointerData::frame_mutex_);
std::unordered_map<size_t, FrameInfoType> PointerData::frames_ GUARDED_BY(PointerData::frame_mutex_);
std::unordered_map<size_t, std::vector<unwindstack::FrameData>> PointerData::backtraces_info_
    GUARDED_BY(PointerData::frame_mutex_);
constexpr size_t kBacktraceEmptyIndex = 1;
size_t PointerData::cur_hash_index_ GUARDED_BY(PointerData::frame_mutex_);

std::mutex PointerData::free_pointer_mutex_;
std::deque<FreePointerInfoType> PointerData::free_pointers_ GUARDED_BY(
    PointerData::free_pointer_mutex_);

// Buffer to use for comparison.
static constexpr size_t kCompareBufferSize = 512 * 1024;
static std::vector<uint8_t> g_cmp_mem(0);

static void ToggleBacktraceEnable(int, siginfo_t*, void*) {
  g_debug->pointer->ToggleBacktraceEnabled();
}

static void EnableDump(int, siginfo_t*, void*) {
  g_debug->pointer->EnableDumping();
}

PointerData::PointerData(DebugData* debug_data) : OptionData(debug_data) {}

bool PointerData::Initialize(const Config& config) NO_THREAD_SAFETY_ANALYSIS {
  pointers_.clear();
  key_to_index_.clear();
  frames_.clear();
  free_pointers_.clear();
  // A hash index of kBacktraceEmptyIndex indicates that we tried to get
  // a backtrace, but there was nothing recorded.
  cur_hash_index_ = kBacktraceEmptyIndex + 1;

  backtrace_enabled_ = config.backtrace_enabled();
  if (config.backtrace_enable_on_signal()) {
    struct sigaction64 enable_act = {};
    enable_act.sa_sigaction = ToggleBacktraceEnable;
    enable_act.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
    if (sigaction64(config.backtrace_signal(), &enable_act, nullptr) != 0) {
      error_log("Unable to set up backtrace signal enable function: %s", strerror(errno));
      return false;
    }
    if (config.options() & VERBOSE) {
      info_log("%s: Run: 'kill -%d %d' to enable backtracing.", getprogname(),
               config.backtrace_signal(), getpid());
    }
  }

  if (config.options() & BACKTRACE) {
    struct sigaction64 act = {};
    act.sa_sigaction = EnableDump;
    act.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
    if (sigaction64(config.backtrace_dump_signal(), &act, nullptr) != 0) {
      error_log("Unable to set up backtrace dump signal function: %s", strerror(errno));
      return false;
    }
    if (config.options() & VERBOSE) {
      info_log("%s: Run: 'kill -%d %d' to dump the backtrace.", getprogname(),
               config.backtrace_dump_signal(), getpid());
    }
  }

  backtrace_dump_ = false;

  if (config.options() & FREE_TRACK) {
    g_cmp_mem.resize(kCompareBufferSize, config.fill_free_value());
  }
  return true;
}

size_t PointerData::AddBacktrace(size_t num_frames) {
  std::vector<uintptr_t> frames;
  std::vector<unwindstack::FrameData> frames_info;
  if (g_debug->config().options() & BACKTRACE_FULL) {
    if (!Unwind(&frames, &frames_info, num_frames)) {
      return kBacktraceEmptyIndex;
    }
  } else {
    frames.resize(num_frames);
    num_frames = backtrace_get(frames.data(), frames.size());
    if (num_frames == 0) {
      return kBacktraceEmptyIndex;
    }
  }

  FrameKeyType key{.num_frames = num_frames, .frames = frames.data()};
  size_t hash_index;
  std::lock_guard<std::mutex> frame_guard(frame_mutex_);
  auto entry = key_to_index_.find(key);
  if (entry == key_to_index_.end()) {
    frames.resize(num_frames);
    hash_index = cur_hash_index_++;
    key.frames = frames.data();
    key_to_index_.emplace(key, hash_index);

    frames_.emplace(hash_index, FrameInfoType{.references = 1, .frames = std::move(frames)});
    if (g_debug->config().options() & BACKTRACE_FULL) {
      backtraces_info_.emplace(hash_index, std::move(frames_info));
    }
  } else {
    hash_index = entry->second;
    FrameInfoType* frame_info = &frames_[hash_index];
    frame_info->references++;
  }
  return hash_index;
}

void PointerData::RemoveBacktrace(size_t hash_index) {
  if (hash_index <= kBacktraceEmptyIndex) {
    return;
  }

  std::lock_guard<std::mutex> frame_guard(frame_mutex_);
  auto frame_entry = frames_.find(hash_index);
  if (frame_entry == frames_.end()) {
    error_log("hash_index %zu does not have matching frame data.", hash_index);
    return;
  }
  FrameInfoType* frame_info = &frame_entry->second;
  if (--frame_info->references == 0) {
    FrameKeyType key{.num_frames = frame_info->frames.size(), .frames = frame_info->frames.data()};
    key_to_index_.erase(key);
    frames_.erase(hash_index);
    if (g_debug->config().options() & BACKTRACE_FULL) {
      backtraces_info_.erase(hash_index);
    }
  }
}

void PointerData::Add(const void* ptr, size_t pointer_size) {
  size_t hash_index = 0;
  if (backtrace_enabled_) {
    hash_index = AddBacktrace(g_debug->config().backtrace_frames());
  }

  std::lock_guard<std::mutex> pointer_guard(pointer_mutex_);
  uintptr_t mangled_ptr = ManglePointer(reinterpret_cast<uintptr_t>(ptr));
  pointers_[mangled_ptr] =
      PointerInfoType{PointerInfoType::GetEncodedSize(pointer_size), hash_index};
}

void PointerData::Remove(const void* ptr) {
  size_t hash_index;
  {
    std::lock_guard<std::mutex> pointer_guard(pointer_mutex_);
    uintptr_t mangled_ptr = ManglePointer(reinterpret_cast<uintptr_t>(ptr));
    auto entry = pointers_.find(mangled_ptr);
    if (entry == pointers_.end()) {
      // Attempt to remove unknown pointer.
      error_log("No tracked pointer found for 0x%" PRIxPTR, DemanglePointer(mangled_ptr));
      return;
    }
    hash_index = entry->second.hash_index;
    pointers_.erase(mangled_ptr);
  }

  RemoveBacktrace(hash_index);
}

size_t PointerData::GetFrames(const void* ptr, uintptr_t* frames, size_t max_frames) {
  size_t hash_index;
  {
    std::lock_guard<std::mutex> pointer_guard(pointer_mutex_);
    uintptr_t mangled_ptr = ManglePointer(reinterpret_cast<uintptr_t>(ptr));
    auto entry = pointers_.find(mangled_ptr);
    if (entry == pointers_.end()) {
      return 0;
    }
    hash_index = entry->second.hash_index;
  }

  if (hash_index <= kBacktraceEmptyIndex) {
    return 0;
  }

  std::lock_guard<std::mutex> frame_guard(frame_mutex_);
  auto frame_entry = frames_.find(hash_index);
  if (frame_entry == frames_.end()) {
    return 0;
  }
  FrameInfoType* frame_info = &frame_entry->second;
  if (max_frames > frame_info->frames.size()) {
    max_frames = frame_info->frames.size();
  }
  memcpy(frames, &frame_info->frames[0], max_frames * sizeof(uintptr_t));

  return max_frames;
}

void PointerData::LogBacktrace(size_t hash_index) {
  std::lock_guard<std::mutex> frame_guard(frame_mutex_);
  if (g_debug->config().options() & BACKTRACE_FULL) {
    auto backtrace_info_entry = backtraces_info_.find(hash_index);
    if (backtrace_info_entry != backtraces_info_.end()) {
      UnwindLog(backtrace_info_entry->second);
      return;
    }
  } else {
    auto frame_entry = frames_.find(hash_index);
    if (frame_entry != frames_.end()) {
      FrameInfoType* frame_info = &frame_entry->second;
      backtrace_log(frame_info->frames.data(), frame_info->frames.size());
      return;
    }
  }
  error_log("  hash_index %zu does not have matching frame data.", hash_index);
}

void PointerData::LogFreeError(const FreePointerInfoType& info, size_t max_cmp_bytes) {
  error_log(LOG_DIVIDER);
  uintptr_t pointer = DemanglePointer(info.mangled_ptr);
  uint8_t* memory = reinterpret_cast<uint8_t*>(pointer);
  error_log("+++ ALLOCATION %p USED AFTER FREE", memory);
  uint8_t fill_free_value = g_debug->config().fill_free_value();
  for (size_t i = 0; i < max_cmp_bytes; i++) {
    if (memory[i] != fill_free_value) {
      error_log("  allocation[%zu] = 0x%02x (expected 0x%02x)", i, memory[i], fill_free_value);
    }
  }

  if (info.hash_index > kBacktraceEmptyIndex) {
    error_log("Backtrace at time of free:");
    LogBacktrace(info.hash_index);
  }

  error_log(LOG_DIVIDER);
  if (g_debug->config().options() & ABORT_ON_ERROR) {
    abort();
  }
}

void PointerData::VerifyFreedPointer(const FreePointerInfoType& info) {
  size_t usable_size;
  uintptr_t pointer = DemanglePointer(info.mangled_ptr);
  if (g_debug->HeaderEnabled()) {
    // Check to see if the tag data has been damaged.
    Header* header = g_debug->GetHeader(reinterpret_cast<const void*>(pointer));
    if (header->tag != DEBUG_FREE_TAG) {
      error_log(LOG_DIVIDER);
      error_log("+++ ALLOCATION 0x%" PRIxPTR " HAS CORRUPTED HEADER TAG 0x%x AFTER FREE", pointer,
                header->tag);
      error_log(LOG_DIVIDER);
      if (g_debug->config().options() & ABORT_ON_ERROR) {
        abort();
      }

      // Stop processing here, it is impossible to tell how the header
      // may have been damaged.
      return;
    }
    usable_size = header->usable_size;
  } else {
    usable_size = g_dispatch->malloc_usable_size(reinterpret_cast<const void*>(pointer));
  }

  size_t bytes = (usable_size < g_debug->config().fill_on_free_bytes())
                     ? usable_size
                     : g_debug->config().fill_on_free_bytes();
  size_t max_cmp_bytes = bytes;
  const uint8_t* memory = reinterpret_cast<const uint8_t*>(pointer);
  while (bytes > 0) {
    size_t bytes_to_cmp = (bytes < g_cmp_mem.size()) ? bytes : g_cmp_mem.size();
    if (memcmp(memory, g_cmp_mem.data(), bytes_to_cmp) != 0) {
      LogFreeError(info, max_cmp_bytes);
    }
    bytes -= bytes_to_cmp;
    memory = &memory[bytes_to_cmp];
  }
}

void* PointerData::AddFreed(const void* ptr) {
  size_t hash_index = 0;
  size_t num_frames = g_debug->config().free_track_backtrace_num_frames();
  if (num_frames) {
    hash_index = AddBacktrace(num_frames);
  }

  void* last = nullptr;
  std::lock_guard<std::mutex> freed_guard(free_pointer_mutex_);
  if (free_pointers_.size() == g_debug->config().free_track_allocations()) {
    FreePointerInfoType info(free_pointers_.front());
    free_pointers_.pop_front();
    VerifyFreedPointer(info);
    RemoveBacktrace(info.hash_index);
    last = reinterpret_cast<void*>(DemanglePointer(info.mangled_ptr));
  }

  uintptr_t mangled_ptr = ManglePointer(reinterpret_cast<uintptr_t>(ptr));
  free_pointers_.emplace_back(FreePointerInfoType{mangled_ptr, hash_index});
  return last;
}

void PointerData::LogFreeBacktrace(const void* ptr) {
  size_t hash_index = 0;
  {
    uintptr_t pointer = reinterpret_cast<uintptr_t>(ptr);
    std::lock_guard<std::mutex> freed_guard(free_pointer_mutex_);
    for (const auto& info : free_pointers_) {
      if (DemanglePointer(info.mangled_ptr) == pointer) {
        hash_index = info.hash_index;
        break;
      }
    }
  }

  if (hash_index <= kBacktraceEmptyIndex) {
    return;
  }

  error_log("Backtrace of original free:");
  LogBacktrace(hash_index);
}

void PointerData::VerifyAllFreed() {
  std::lock_guard<std::mutex> freed_guard(free_pointer_mutex_);
  for (auto& free_info : free_pointers_) {
    VerifyFreedPointer(free_info);
  }
}

void PointerData::GetList(std::vector<ListInfoType>* list, bool only_with_backtrace)
    REQUIRES(pointer_mutex_, frame_mutex_) {
  for (const auto& entry : pointers_) {
    FrameInfoType* frame_info = nullptr;
    std::vector<unwindstack::FrameData>* backtrace_info = nullptr;
    uintptr_t pointer = DemanglePointer(entry.first);
    size_t hash_index = entry.second.hash_index;
    if (hash_index > kBacktraceEmptyIndex) {
      auto frame_entry = frames_.find(hash_index);
      if (frame_entry == frames_.end()) {
        // Somehow wound up with a pointer with a valid hash_index, but
        // no frame data. This should not be possible since adding a pointer
        // occurs after the hash_index and frame data have been added.
        // When removing a pointer, the pointer is deleted before the frame
        // data.
        error_log("Pointer 0x%" PRIxPTR " hash_index %zu does not exist.", pointer, hash_index);
      } else {
        frame_info = &frame_entry->second;
      }

      if (g_debug->config().options() & BACKTRACE_FULL) {
        auto backtrace_entry = backtraces_info_.find(hash_index);
        if (backtrace_entry == backtraces_info_.end()) {
          error_log("Pointer 0x%" PRIxPTR " hash_index %zu does not exist.", pointer, hash_index);
        } else {
          backtrace_info = &backtrace_entry->second;
        }
      }
    }
    if (hash_index == 0 && only_with_backtrace) {
      continue;
    }

    list->emplace_back(ListInfoType{pointer, 1, entry.second.RealSize(),
                                    entry.second.ZygoteChildAlloc(), frame_info, backtrace_info});
  }

  // Sort by the size of the allocation.
  std::sort(list->begin(), list->end(), [](const ListInfoType& a, const ListInfoType& b) {
    // Put zygote child allocations first.
    bool a_zygote_child_alloc = a.zygote_child_alloc;
    bool b_zygote_child_alloc = b.zygote_child_alloc;
    if (a_zygote_child_alloc && !b_zygote_child_alloc) {
      return false;
    }
    if (!a_zygote_child_alloc && b_zygote_child_alloc) {
      return true;
    }

    // Sort by size, descending order.
    if (a.size != b.size) return a.size > b.size;

    // Put pointers with no backtrace last.
    FrameInfoType* a_frame = a.frame_info;
    FrameInfoType* b_frame = b.frame_info;
    if (a_frame == nullptr && b_frame != nullptr) {
      return false;
    } else if (a_frame != nullptr && b_frame == nullptr) {
      return true;
    } else if (a_frame == nullptr && b_frame == nullptr) {
      return a.pointer < b.pointer;
    }

    // Put the pointers with longest backtrace first.
    if (a_frame->frames.size() != b_frame->frames.size()) {
      return a_frame->frames.size() > b_frame->frames.size();
    }

    // Last sort by pointer.
    return a.pointer < b.pointer;
  });
}

void PointerData::GetUniqueList(std::vector<ListInfoType>* list, bool only_with_backtrace)
    REQUIRES(pointer_mutex_, frame_mutex_) {
  GetList(list, only_with_backtrace);

  // Remove duplicates of size/backtraces.
  for (auto iter = list->begin(); iter != list->end();) {
    auto dup_iter = iter + 1;
    bool zygote_child_alloc = iter->zygote_child_alloc;
    size_t size = iter->size;
    FrameInfoType* frame_info = iter->frame_info;
    for (; dup_iter != list->end(); ++dup_iter) {
      if (zygote_child_alloc != dup_iter->zygote_child_alloc || size != dup_iter->size ||
          frame_info != dup_iter->frame_info) {
        break;
      }
      iter->num_allocations++;
    }
    iter = list->erase(iter + 1, dup_iter);
  }
}

void PointerData::LogLeaks() {
  std::vector<ListInfoType> list;

  std::lock_guard<std::mutex> pointer_guard(pointer_mutex_);
  std::lock_guard<std::mutex> frame_guard(frame_mutex_);
  GetList(&list, false);

  size_t track_count = 0;
  for (const auto& list_info : list) {
    error_log("+++ %s leaked block of size %zu at 0x%" PRIxPTR " (leak %zu of %zu)", getprogname(),
              list_info.size, list_info.pointer, ++track_count, list.size());
    if (list_info.backtrace_info != nullptr) {
      error_log("Backtrace at time of allocation:");
      UnwindLog(*list_info.backtrace_info);
    } else if (list_info.frame_info != nullptr) {
      error_log("Backtrace at time of allocation:");
      backtrace_log(list_info.frame_info->frames.data(), list_info.frame_info->frames.size());
    }
    // Do not bother to free the pointers, we are about to exit any way.
  }
}

void PointerData::GetAllocList(std::vector<ListInfoType>* list) {
  std::lock_guard<std::mutex> pointer_guard(pointer_mutex_);
  std::lock_guard<std::mutex> frame_guard(frame_mutex_);

  if (pointers_.empty()) {
    return;
  }

  GetList(list, false);
}

void PointerData::GetInfo(uint8_t** info, size_t* overall_size, size_t* info_size,
                          size_t* total_memory, size_t* backtrace_size) {
  std::lock_guard<std::mutex> pointer_guard(pointer_mutex_);
  std::lock_guard<std::mutex> frame_guard(frame_mutex_);

  if (pointers_.empty()) {
    return;
  }

  std::vector<ListInfoType> list;
  GetUniqueList(&list, true);
  if (list.empty()) {
    return;
  }

  *backtrace_size = g_debug->config().backtrace_frames();
  *info_size = sizeof(size_t) * 2 + sizeof(uintptr_t) * *backtrace_size;
  *overall_size = *info_size * list.size();
  *info = reinterpret_cast<uint8_t*>(g_dispatch->calloc(*info_size, list.size()));
  if (*info == nullptr) {
    return;
  }

  uint8_t* data = *info;
  *total_memory = 0;
  for (const auto& list_info : list) {
    FrameInfoType* frame_info = list_info.frame_info;
    *total_memory += list_info.size * list_info.num_allocations;
    size_t allocation_size =
        PointerInfoType::GetEncodedSize(list_info.zygote_child_alloc, list_info.size);
    memcpy(data, &allocation_size, sizeof(size_t));
    memcpy(&data[sizeof(size_t)], &list_info.num_allocations, sizeof(size_t));
    if (frame_info != nullptr) {
      memcpy(&data[2 * sizeof(size_t)], frame_info->frames.data(),
             frame_info->frames.size() * sizeof(uintptr_t));
    }
    data += *info_size;
  }
}

bool PointerData::Exists(const void* ptr) {
  std::lock_guard<std::mutex> pointer_guard(pointer_mutex_);
  uintptr_t mangled_ptr = ManglePointer(reinterpret_cast<uintptr_t>(ptr));
  return pointers_.count(mangled_ptr) != 0;
}

void PointerData::DumpLiveToFile(int fd) {
  std::vector<ListInfoType> list;

  std::lock_guard<std::mutex> pointer_guard(pointer_mutex_);
  std::lock_guard<std::mutex> frame_guard(frame_mutex_);
  GetUniqueList(&list, false);

  size_t total_memory = 0;
  for (const auto& info : list) {
    total_memory += info.size * info.num_allocations;
  }

  dprintf(fd, "Total memory: %zu\n", total_memory);
  dprintf(fd, "Allocation records: %zd\n", list.size());
  dprintf(fd, "Backtrace size: %zu\n", g_debug->config().backtrace_frames());
  dprintf(fd, "\n");

  for (const auto& info : list) {
    dprintf(fd, "z %d  sz %8zu  num    %zu  bt", (info.zygote_child_alloc) ? 1 : 0, info.size,
            info.num_allocations);
    FrameInfoType* frame_info = info.frame_info;
    if (frame_info != nullptr) {
      for (size_t i = 0; i < frame_info->frames.size(); i++) {
        if (frame_info->frames[i] == 0) {
          break;
        }
        dprintf(fd, " %" PRIxPTR, frame_info->frames[i]);
      }
    }
    dprintf(fd, "\n");
    if (info.backtrace_info != nullptr) {
      dprintf(fd, "  bt_info");
      for (const auto& frame : *info.backtrace_info) {
        dprintf(fd, " {");
        if (frame.map_info != nullptr && !frame.map_info->name().empty()) {
          dprintf(fd, "\"%s\"", frame.map_info->name().c_str());
        } else {
          dprintf(fd, "\"\"");
        }
        dprintf(fd, " %" PRIx64, frame.rel_pc);
        if (frame.function_name.empty()) {
          dprintf(fd, " \"\" 0}");
        } else {
          char* demangled_name = __cxa_demangle(frame.function_name.c_str(), nullptr, nullptr,
                                                nullptr);
          const char* name;
          if (demangled_name != nullptr) {
            name = demangled_name;
          } else {
            name = frame.function_name.c_str();
          }
          dprintf(fd, " \"%s\" %" PRIx64 "}", name, frame.function_offset);
          free(demangled_name);
        }
      }
      dprintf(fd, "\n");
    }
  }
}

void PointerData::PrepareFork() NO_THREAD_SAFETY_ANALYSIS {
  free_pointer_mutex_.lock();
  pointer_mutex_.lock();
  frame_mutex_.lock();
}

void PointerData::PostForkParent() NO_THREAD_SAFETY_ANALYSIS {
  frame_mutex_.unlock();
  pointer_mutex_.unlock();
  free_pointer_mutex_.unlock();
}

void PointerData::PostForkChild() __attribute__((no_thread_safety_analysis)) {
  // Make sure that any potential mutexes have been released and are back
  // to an initial state.
  frame_mutex_.try_lock();
  frame_mutex_.unlock();
  pointer_mutex_.try_lock();
  pointer_mutex_.unlock();
  free_pointer_mutex_.try_lock();
  free_pointer_mutex_.unlock();
}

void PointerData::IteratePointers(std::function<void(uintptr_t pointer)> fn) {
  std::lock_guard<std::mutex> pointer_guard(pointer_mutex_);
  for (const auto entry : pointers_) {
    fn(DemanglePointer(entry.first));
  }
}
