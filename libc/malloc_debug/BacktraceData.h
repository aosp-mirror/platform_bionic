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

#include <atomic>

#include <private/bionic_macros.h>

#include "OptionData.h"

// Forward declarations.
class Config;

class BacktraceData : public OptionData {
 public:
  BacktraceData(DebugData* debug_data, const Config& config, size_t* offset);
  virtual ~BacktraceData() = default;

  bool Initialize(const Config& config);

  inline size_t alloc_offset() { return alloc_offset_; }

  bool ShouldBacktrace() { return enabled_ == 1; }
  void ToggleBacktraceEnabled() { enabled_.fetch_xor(1); }

  void EnableDumping() { dump_ = true; }
  bool ShouldDumpAndReset() {
    bool expected = true;
    return dump_.compare_exchange_strong(expected, false);
  }

 private:
  size_t alloc_offset_ = 0;

  std::atomic_uint8_t enabled_;

  std::atomic_bool dump_;

  DISALLOW_COPY_AND_ASSIGN(BacktraceData);
};
