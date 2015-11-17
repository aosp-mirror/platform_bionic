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
#include <string.h>

#include <vector>

#include "backtrace.h"
#include "Config.h"
#include "debug_disable.h"
#include "debug_log.h"
#include "DebugData.h"
#include "malloc_debug.h"
#include "GuardData.h"

GuardData::GuardData(int init_value, size_t num_bytes) {
  // Create a buffer for fast comparisons of the front guard.
  cmp_mem_.resize(num_bytes);
  memset(cmp_mem_.data(), init_value, cmp_mem_.size());
}

void GuardData::LogFailure(const Header* header, const void* pointer, const void* data) {
  ScopedDisableDebugCalls disable;

  error_log(LOG_DIVIDER);
  error_log("+++ ALLOCATION %p SIZE %zu HAS A CORRUPTED %s GUARD", pointer,
            header->real_size(), GetTypeName());

  // Log all of the failing bytes.
  const uint8_t* expected = cmp_mem_.data();
  int pointer_idx = reinterpret_cast<uintptr_t>(data) - reinterpret_cast<uintptr_t>(pointer);
  const uint8_t* real = reinterpret_cast<const uint8_t*>(data);
  for (size_t i = 0; i < cmp_mem_.size(); i++, pointer_idx++) {
    if (real[i] != expected[i]) {
      error_log("  pointer[%d] = 0x%02x (expected 0x%02x)", pointer_idx, real[i], expected[i]);
    }
  }

  error_log("Backtrace at time of failure:");
  std::vector<uintptr_t> frames(64);
  size_t frame_num = backtrace_get(frames.data(), frames.size());
  frames.resize(frame_num);
  backtrace_log(frames.data(), frames.size());
  error_log(LOG_DIVIDER);
}

FrontGuardData::FrontGuardData(const Config& config, size_t* offset)
   : GuardData(config.front_guard_value, config.front_guard_bytes) {
  // Create a buffer for fast comparisons of the front guard.
  cmp_mem_.resize(config.front_guard_bytes);
  memset(cmp_mem_.data(), config.front_guard_value, cmp_mem_.size());
  // Assumes that front_bytes is a multiple of sizeof(uintptr_t).
  offset_ = *offset;
  *offset += config.front_guard_bytes;
}

bool FrontGuardData::Valid(DebugData& debug, const Header* header) {
  return GuardData::Valid(debug.GetFrontGuard(header));
}

void FrontGuardData::LogFailure(DebugData& debug, const Header* header) {
  GuardData::LogFailure(header, debug.GetPointer(header), debug.GetFrontGuard(header));
}

RearGuardData::RearGuardData(const Config& config)
    : GuardData(config.rear_guard_value, config.rear_guard_bytes) {
}

bool RearGuardData::Valid(DebugData& debug, const Header* header) {
  return GuardData::Valid(debug.GetRearGuard(header));
}

void RearGuardData::LogFailure(DebugData& debug, const Header* header) {
  GuardData::LogFailure(header, debug.GetPointer(header), debug.GetRearGuard(header));
}
