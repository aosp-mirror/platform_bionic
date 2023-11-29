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

#include "Config.h"
#include "DebugData.h"
#include "GuardData.h"
#include "LogAllocatorStats.h"
#include "PointerData.h"
#include "debug_disable.h"
#include "malloc_debug.h"

bool DebugData::Initialize(const char* options) {
  if (!config_.Init(options)) {
    return false;
  }

  // Check to see if the options that require a header are enabled.
  if (config_.options() & HEADER_OPTIONS) {
    // Initialize all of the static header offsets.
    pointer_offset_ = __BIONIC_ALIGN(sizeof(Header), MINIMUM_ALIGNMENT_BYTES);

    if (config_.options() & FRONT_GUARD) {
      front_guard.reset(new FrontGuardData(this, config_, &pointer_offset_));
    }

    extra_bytes_ = pointer_offset_;

    // Initialize all of the non-header data.
    if (config_.options() & REAR_GUARD) {
      rear_guard.reset(new RearGuardData(this, config_));
      extra_bytes_ += config_.rear_guard_bytes();
    }
  }

  if (TrackPointers()) {
    pointer.reset(new PointerData(this));
    if (!pointer->Initialize(config_)) {
      return false;
    }
  }

  if (config_.options() & RECORD_ALLOCS) {
    record.reset(new RecordData());
    if (!record->Initialize(config_)) {
      return false;
    }
  }

  if (config_.options() & EXPAND_ALLOC) {
    extra_bytes_ += config_.expand_alloc_bytes();
  }

  if (config_.options() & LOG_ALLOCATOR_STATS_ON_SIGNAL) {
    if (!LogAllocatorStats::Initialize(config_)) {
      return false;
    }
  }

  return true;
}

void DebugData::PrepareFork() {
  if (pointer != nullptr) {
    pointer->PrepareFork();
  }
}

void DebugData::PostForkParent() {
  if (pointer != nullptr) {
    pointer->PostForkParent();
  }
}

void DebugData::PostForkChild() {
  if (pointer != nullptr) {
    pointer->PostForkChild();
  }
}
