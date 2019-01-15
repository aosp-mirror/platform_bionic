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

#include <memory>
#include <vector>

#include <private/bionic_macros.h>

#include "Config.h"
#include "GuardData.h"
#include "PointerData.h"
#include "RecordData.h"
#include "malloc_debug.h"

class DebugData {
 public:
  DebugData() = default;
  ~DebugData() = default;

  bool Initialize(const char* options);

  static bool Disabled();

  inline void* GetPointer(const Header* header) {
    uintptr_t value = reinterpret_cast<uintptr_t>(header);
    return reinterpret_cast<void*>(value + pointer_offset_);
  }

  Header* GetHeader(const void* pointer) {
    uintptr_t value = reinterpret_cast<uintptr_t>(pointer);
    return reinterpret_cast<Header*>(value - pointer_offset_);
  }

  uint8_t* GetFrontGuard(const Header* header) {
    uintptr_t value = reinterpret_cast<uintptr_t>(header);
    return reinterpret_cast<uint8_t*>(value + front_guard->offset());
  }

  uint8_t* GetRearGuard(const Header* header) {
    uintptr_t value = reinterpret_cast<uintptr_t>(GetPointer(header));
    return reinterpret_cast<uint8_t*>(value + header->size);
  }

  const Config& config() { return config_; }
  size_t pointer_offset() { return pointer_offset_; }
  size_t extra_bytes() { return extra_bytes_; }

  bool TrackPointers() { return config_.options() & TRACK_ALLOCS; }

  bool HeaderEnabled() { return config_.options() & HEADER_OPTIONS; }

  void PrepareFork();
  void PostForkParent();
  void PostForkChild();

  std::unique_ptr<FrontGuardData> front_guard;
  std::unique_ptr<PointerData> pointer;
  std::unique_ptr<RearGuardData> rear_guard;
  std::unique_ptr<RecordData> record;

 private:
  size_t extra_bytes_ = 0;

  size_t pointer_offset_ = 0;

  Config config_;

  BIONIC_DISALLOW_COPY_AND_ASSIGN(DebugData);
};

extern DebugData* g_debug;
