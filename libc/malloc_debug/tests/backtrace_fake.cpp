/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>

#include <deque>
#include <vector>
#include <utility>

#include <unwindstack/LocalUnwinder.h>

#include "backtrace.h"
#include "backtrace_fake.h"
#include "debug_log.h"

static std::deque<std::vector<uintptr_t>> g_fake_backtrace;

void backtrace_fake_clear_all() {
  g_fake_backtrace.clear();
}

void backtrace_fake_add(const std::vector<uintptr_t>& ips) {
  g_fake_backtrace.push_back(ips);
}

void backtrace_startup() {
}

void backtrace_shutdown() {
}

size_t backtrace_get(uintptr_t* frames, size_t frame_num) {
  if (frame_num == 0 || g_fake_backtrace.size() == 0) {
    return 0;
  }

  size_t ips_size = g_fake_backtrace[0].size();
  size_t total_frames = (frame_num < ips_size) ? frame_num : ips_size;
  memcpy(frames, g_fake_backtrace[0].data(), sizeof(uintptr_t) * total_frames);
  g_fake_backtrace.pop_front();
  return total_frames;
}

void backtrace_log(const uintptr_t* frames, size_t frame_count) {
  for (size_t i = 0; i < frame_count; i++) {
    error_log("  #%02zd pc %p", i, reinterpret_cast<void*>(frames[i]));
  }
}

static std::deque<std::vector<unwindstack::LocalFrameData>> g_fake_local_frame_data;

void BacktraceUnwindFakeClearAll() {
  g_fake_local_frame_data.clear();
}

void BacktraceUnwindFake(const std::vector<unwindstack::LocalFrameData>& frames) {
  g_fake_local_frame_data.push_back(frames);
}

bool Unwind(std::vector<uintptr_t>* frames, std::vector<unwindstack::LocalFrameData>* info, size_t) {
  if (g_fake_local_frame_data.empty()) {
    return false;
  }

  *info = g_fake_local_frame_data.front();
  g_fake_local_frame_data.pop_front();
  frames->clear();
  for (const auto& frame : *info) {
    frames->push_back(frame.pc);
  }

  return true;
}

void UnwindLog(const std::vector<unwindstack::LocalFrameData>& /*frame_info*/) {
}
