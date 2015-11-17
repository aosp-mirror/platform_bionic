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

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

#include <algorithm>
#include <vector>

#include <private/ScopedPthreadMutexLocker.h>

#include "backtrace.h"
#include "BacktraceData.h"
#include "Config.h"
#include "DebugData.h"
#include "debug_disable.h"
#include "debug_log.h"
#include "malloc_debug.h"
#include "TrackData.h"

void TrackData::GetList(std::vector<Header*>* list) {
  ScopedDisableDebugCalls disable;

  for (const auto& header : headers_) {
    list->push_back(header);
  }

  // Sort by the size of the allocation.
  std::sort(list->begin(), list->end(), [](Header* a, Header* b) {
    if (a->size == b->size) return a < b;
    return a->size > b->size;
  });
}

void TrackData::Add(Header* header, bool backtrace_found) {
  ScopedDisableDebugCalls disable;

  pthread_mutex_lock(&mutex_);
  if (backtrace_found) {
    total_backtrace_allocs_++;
  }
  headers_.insert(header);
  pthread_mutex_unlock(&mutex_);
}

void TrackData::Remove(Header* header, bool backtrace_found) {
  ScopedDisableDebugCalls disable;

  pthread_mutex_lock(&mutex_);
  headers_.erase(header);
  if (backtrace_found) {
    total_backtrace_allocs_--;
  }
  pthread_mutex_unlock(&mutex_);
}

void TrackData::DisplayLeaks(DebugData& debug) {
  ScopedDisableDebugCalls disable;

  std::vector<Header*> list;
  GetList(&list);

  size_t track_count = 0;
  for (const auto& header : list) {
    error_log("+++ %s leaked block of size %zu at %p (leak %zu of %zu)", getprogname(),
              header->real_size(), debug.GetPointer(header), ++track_count, list.size());
    if (debug.config().options & BACKTRACE) {
      BacktraceHeader* back_header = debug.GetAllocBacktrace(header);
      if (back_header->num_frames > 0) {
        error_log("Backtrace at time of allocation:");
        backtrace_log(&back_header->frames[0], back_header->num_frames);
      }
    }
    g_dispatch->free(header->orig_pointer);
  }
}

void TrackData::GetInfo(DebugData& debug, uint8_t** info, size_t* overall_size,
                        size_t* info_size, size_t* total_memory, size_t* backtrace_size) {
  ScopedPthreadMutexLocker scoped(&mutex_);

  if (headers_.size() == 0 || total_backtrace_allocs_ == 0) {
    return;
  }

  *backtrace_size = debug.config().backtrace_frames;
  *info_size = sizeof(size_t) * 2 + sizeof(uintptr_t) * *backtrace_size;
  *info = reinterpret_cast<uint8_t*>(g_dispatch->calloc(*info_size, total_backtrace_allocs_));
  if (*info == nullptr) {
    return;
  }
  *overall_size = *info_size * total_backtrace_allocs_;

  std::vector<Header*> list;
  GetList(&list);

  uint8_t* data = *info;
  for (const auto& header : list) {
    BacktraceHeader* back_header = debug.GetAllocBacktrace(header);
    if (back_header->num_frames > 0) {
      memcpy(data, &header->size, sizeof(size_t));
      memcpy(&data[sizeof(size_t)], &back_header->num_frames, sizeof(size_t));
      memcpy(&data[2 * sizeof(size_t)], &back_header->frames[0],
            back_header->num_frames * sizeof(uintptr_t));

      *total_memory += header->real_size();

      data += *info_size;
    }
  }
}
