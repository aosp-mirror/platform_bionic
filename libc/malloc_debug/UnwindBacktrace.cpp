/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <cxxabi.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <android-base/stringprintf.h>
#include <unwindstack/AndroidUnwinder.h>
#include <unwindstack/Unwinder.h>

#include "UnwindBacktrace.h"
#include "debug_log.h"

#if defined(__LP64__)
#define PAD_PTR "016" PRIx64
#else
#define PAD_PTR "08" PRIx64
#endif

bool Unwind(std::vector<uintptr_t>* frames, std::vector<unwindstack::FrameData>* frame_info,
            size_t max_frames) {
  [[clang::no_destroy]] static unwindstack::AndroidLocalUnwinder unwinder(
      std::vector<std::string>{"libc_malloc_debug.so"});
  unwindstack::AndroidUnwinderData data(max_frames);
  if (!unwinder.Unwind(data)) {
    frames->clear();
    frame_info->clear();
    return false;
  }

  frames->resize(data.frames.size());
  for (const auto& frame : data.frames) {
    frames->at(frame.num) = frame.pc;
  }
  *frame_info = std::move(data.frames);
  return true;
}

void UnwindLog(const std::vector<unwindstack::FrameData>& frame_info) {
  for (size_t i = 0; i < frame_info.size(); i++) {
    const unwindstack::FrameData* info = &frame_info[i];
    auto map_info = info->map_info;

    std::string line = android::base::StringPrintf("          #%0zd  pc %" PAD_PTR "  ", i, info->rel_pc);
    if (map_info != nullptr && map_info->offset() != 0) {
      line += android::base::StringPrintf("(offset 0x%" PRIx64 ") ", map_info->offset());
    }

    if (map_info == nullptr) {
      line += "<unknown>";
    } else if (map_info->name().empty()) {
      line += android::base::StringPrintf("<anonymous:%" PRIx64 ">", map_info->start());
    } else {
      line += map_info->name();
    }

    if (!info->function_name.empty()) {
      line += " (";
      char* demangled_name =
          abi::__cxa_demangle(info->function_name.c_str(), nullptr, nullptr, nullptr);
      if (demangled_name != nullptr) {
        line += demangled_name;
        free(demangled_name);
      } else {
        line += info->function_name;
      }
      if (info->function_offset != 0) {
        line += "+" + std::to_string(info->function_offset);
      }
      line += ")";
    }
    error_log_string(line.c_str());
  }
}
