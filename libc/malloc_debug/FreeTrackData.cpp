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

#include <stdint.h>

#include "backtrace.h"
#include "Config.h"
#include "DebugData.h"
#include "debug_disable.h"
#include "debug_log.h"
#include "FreeTrackData.h"
#include "malloc_debug.h"

FreeTrackData::FreeTrackData(const Config& config)
    : backtrace_num_frames_(config.free_track_backtrace_num_frames) {
  cmp_mem_.resize(4096);
  memset(cmp_mem_.data(), config.fill_free_value, cmp_mem_.size());
}

void FreeTrackData::LogFreeError(DebugData& debug, const Header* header,
                                 const uint8_t* pointer) {
  ScopedDisableDebugCalls disable;

  error_log(LOG_DIVIDER);
  error_log("+++ ALLOCATION %p USED AFTER FREE", pointer);
  uint8_t fill_free_value = debug.config().fill_free_value;
  for (size_t i = 0; i < header->usable_size; i++) {
    if (pointer[i] != fill_free_value) {
      error_log("  allocation[%zu] = 0x%02x (expected 0x%02x)", i, pointer[i], fill_free_value);
    }
  }
  auto back_iter = backtraces_.find(header);
  if (back_iter != backtraces_.end()) {
    const BacktraceHeader* back_header = back_iter->second;
    error_log("Backtrace at time of free:");
    backtrace_log(&back_header->frames[0], back_header->num_frames);
  }
  error_log(LOG_DIVIDER);
}

void FreeTrackData::VerifyAndFree(DebugData& debug, const Header* header,
                                  const void* pointer) {
  ScopedDisableDebugCalls disable;

  if (header->tag != DEBUG_FREE_TAG) {
    error_log(LOG_DIVIDER);
    error_log("+++ ALLOCATION %p HAS CORRUPTED HEADER TAG 0x%x AFTER FREE", pointer, header->tag);
    error_log(LOG_DIVIDER);
  } else {
    const uint8_t* memory = reinterpret_cast<const uint8_t*>(pointer);
    size_t bytes = header->usable_size;
    bytes = (bytes < debug.config().fill_on_free_bytes) ? bytes : debug.config().fill_on_free_bytes;
    while (bytes > 0) {
      size_t bytes_to_cmp = (bytes < cmp_mem_.size()) ? bytes : cmp_mem_.size();
      if (memcmp(memory, cmp_mem_.data(), bytes_to_cmp) != 0) {
        LogFreeError(debug, header, reinterpret_cast<const uint8_t*>(pointer));
        break;
      }
      bytes -= bytes_to_cmp;
      memory = &memory[bytes_to_cmp];
    }
  }

  auto back_iter = backtraces_.find(header);
  if (back_iter != backtraces_.end()) {
    g_dispatch->free(reinterpret_cast<void*>(back_iter->second));
    backtraces_.erase(header);
  }
  g_dispatch->free(header->orig_pointer);
}

void FreeTrackData::Add(DebugData& debug, const Header* header) {
  // Make sure the stl calls below don't call the debug_XXX functions.
  ScopedDisableDebugCalls disable;

  pthread_mutex_lock(&mutex_);
  if (list_.size() == debug.config().free_track_allocations) {
    const Header* old_header = list_.back();
    VerifyAndFree(debug, old_header, debug.GetPointer(old_header));
    list_.pop_back();
  }

  if (backtrace_num_frames_ > 0) {
    BacktraceHeader* back_header = reinterpret_cast<BacktraceHeader*>(
      g_dispatch->malloc(sizeof(BacktraceHeader) + backtrace_num_frames_ * sizeof(uintptr_t)));
    if (back_header) {
      back_header->num_frames = backtrace_get(&back_header->frames[0], backtrace_num_frames_);
      backtraces_[header] = back_header;
    }
  }
  list_.push_front(header);

  pthread_mutex_unlock(&mutex_);
}

void FreeTrackData::VerifyAll(DebugData& debug) {
  // Make sure the stl calls below don't call the debug_XXX functions.
  ScopedDisableDebugCalls disable;

  for (const auto& header : list_) {
    VerifyAndFree(debug, header, debug.GetPointer(header));
  }
  list_.clear();
}

void FreeTrackData::LogBacktrace(const Header* header) {
  ScopedDisableDebugCalls disable;

  auto back_iter = backtraces_.find(header);
  if (back_iter == backtraces_.end()) {
    return;
  }

  error_log("Backtrace of original free:");
  backtrace_log(&back_iter->second->frames[0], back_iter->second->num_frames);
}
