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

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

#include <string>
#include <vector>

#include <private/bionic_macros.h>

#include "Config.h"
#include "debug_log.h"

// Config constants
static constexpr uint8_t DEFAULT_FILL_ALLOC_VALUE = 0xeb;
static constexpr uint8_t DEFAULT_FILL_FREE_VALUE = 0xef;

static constexpr uint8_t DEFAULT_FRONT_GUARD_VALUE = 0xaa;
static constexpr uint8_t DEFAULT_REAR_GUARD_VALUE = 0xbb;

// Used as the default for all guard values.
static constexpr size_t DEFAULT_GUARD_BYTES = 32;
static constexpr size_t MAX_GUARD_BYTES = 16384;

static constexpr size_t DEFAULT_BACKTRACE_FRAMES = 16;
static constexpr size_t MAX_BACKTRACE_FRAMES = 256;
static constexpr const char DEFAULT_BACKTRACE_DUMP_PREFIX[] = "/data/local/tmp/backtrace_heap";

static constexpr size_t DEFAULT_EXPAND_BYTES = 16;
static constexpr size_t MAX_EXPAND_BYTES = 16384;

static constexpr size_t DEFAULT_FREE_TRACK_ALLOCATIONS = 100;
static constexpr size_t MAX_FREE_TRACK_ALLOCATIONS = 16384;

static constexpr size_t DEFAULT_RECORD_ALLOCS = 8000000;
static constexpr size_t MAX_RECORD_ALLOCS = 50000000;
static constexpr const char DEFAULT_RECORD_ALLOCS_FILE[] = "/data/local/tmp/record_allocs.txt";

const std::unordered_map<std::string, Config::OptionInfo> Config::kOptions = {
    {
        "guard", {FRONT_GUARD | REAR_GUARD | TRACK_ALLOCS, &Config::SetGuard},
    },
    {
        "front_guard", {FRONT_GUARD | TRACK_ALLOCS, &Config::SetFrontGuard},
    },
    {
        "rear_guard", {REAR_GUARD | TRACK_ALLOCS, &Config::SetRearGuard},
    },

    {
        "backtrace", {BACKTRACE | TRACK_ALLOCS, &Config::SetBacktrace},
    },
    {
        "backtrace_enable_on_signal",
        {BACKTRACE | TRACK_ALLOCS, &Config::SetBacktraceEnableOnSignal},
    },

    {
        "backtrace_dump_on_exit", {0, &Config::SetBacktraceDumpOnExit},
    },
    {
        "backtrace_dump_prefix", {0, &Config::SetBacktraceDumpPrefix},
    },
    {
        "backtrace_full", {BACKTRACE_FULL, &Config::VerifyValueEmpty},
    },

    {
        "fill", {FILL_ON_ALLOC | FILL_ON_FREE, &Config::SetFill},
    },
    {
        "fill_on_alloc", {FILL_ON_ALLOC, &Config::SetFillOnAlloc},
    },
    {
        "fill_on_free", {FILL_ON_FREE, &Config::SetFillOnFree},
    },

    {
        "expand_alloc", {EXPAND_ALLOC, &Config::SetExpandAlloc},
    },

    {
        "free_track", {FREE_TRACK | FILL_ON_FREE | TRACK_ALLOCS, &Config::SetFreeTrack},
    },
    {
        "free_track_backtrace_num_frames", {0, &Config::SetFreeTrackBacktraceNumFrames},
    },

    {
        "leak_track", {LEAK_TRACK | TRACK_ALLOCS, &Config::VerifyValueEmpty},
    },

    {
        "record_allocs", {RECORD_ALLOCS, &Config::SetRecordAllocs},
    },
    {
        "record_allocs_file", {0, &Config::SetRecordAllocsFile},
    },

    {
        "verify_pointers", {TRACK_ALLOCS, &Config::VerifyValueEmpty},
    },
    {
        "abort_on_error", {ABORT_ON_ERROR, &Config::VerifyValueEmpty},
    },
};

bool Config::ParseValue(const std::string& option, const std::string& value, size_t min_value,
                        size_t max_value, size_t* parsed_value) const {
  assert(!value.empty());

  // Parse the value into a size_t value.
  errno = 0;
  char* end;
  long long_value = strtol(value.c_str(), &end, 10);
  if (errno != 0) {
    error_log("%s: bad value for option '%s': %s", getprogname(), option.c_str(), strerror(errno));
    return false;
  }
  if (end == value.c_str()) {
    error_log("%s: bad value for option '%s'", getprogname(), option.c_str());
    return false;
  }
  if (static_cast<size_t>(end - value.c_str()) != value.size()) {
    error_log("%s: bad value for option '%s', non space found after option: %s", getprogname(),
              option.c_str(), end);
    return false;
  }
  if (long_value < 0) {
    error_log("%s: bad value for option '%s', value cannot be negative: %ld", getprogname(),
              option.c_str(), long_value);
    return false;
  }

  if (static_cast<size_t>(long_value) < min_value) {
    error_log("%s: bad value for option '%s', value must be >= %zu: %ld", getprogname(),
              option.c_str(), min_value, long_value);
    return false;
  }
  if (static_cast<size_t>(long_value) > max_value) {
    error_log("%s: bad value for option '%s', value must be <= %zu: %ld", getprogname(),
              option.c_str(), max_value, long_value);
    return false;
  }
  *parsed_value = static_cast<size_t>(long_value);
  return true;
}

bool Config::ParseValue(const std::string& option, const std::string& value, size_t default_value,
                        size_t min_value, size_t max_value, size_t* new_value) const {
  if (value.empty()) {
    *new_value = default_value;
    return true;
  }
  return ParseValue(option, value, min_value, max_value, new_value);
}

bool Config::SetGuard(const std::string& option, const std::string& value) {
  if (value.empty()) {
    // Set the defaults.
    front_guard_bytes_ = DEFAULT_GUARD_BYTES;
    rear_guard_bytes_ = DEFAULT_GUARD_BYTES;
    return true;
  }

  if (!ParseValue(option, value, 1, MAX_GUARD_BYTES, &rear_guard_bytes_)) {
    return false;
  }

  // It's necessary to align the front guard to MINIMUM_ALIGNMENT_BYTES to
  // make sure that the header is aligned properly.
  front_guard_bytes_ = __BIONIC_ALIGN(rear_guard_bytes_, MINIMUM_ALIGNMENT_BYTES);
  return true;
}

bool Config::SetFrontGuard(const std::string& option, const std::string& value) {
  if (!ParseValue(option, value, DEFAULT_GUARD_BYTES, 1, MAX_GUARD_BYTES, &front_guard_bytes_)) {
    return false;
  }
  // It's necessary to align the front guard to MINIMUM_ALIGNMENT_BYTES to
  // make sure that the header is aligned properly.
  front_guard_bytes_ = __BIONIC_ALIGN(front_guard_bytes_, MINIMUM_ALIGNMENT_BYTES);
  return true;
}

bool Config::SetRearGuard(const std::string& option, const std::string& value) {
  return ParseValue(option, value, DEFAULT_GUARD_BYTES, 1, MAX_GUARD_BYTES, &rear_guard_bytes_);
}

bool Config::SetFill(const std::string& option, const std::string& value) {
  if (value.empty()) {
    // Set the defaults.
    fill_on_alloc_bytes_ = SIZE_MAX;
    fill_on_free_bytes_ = SIZE_MAX;
    return true;
  }

  if (!ParseValue(option, value, 1, SIZE_MAX, &fill_on_alloc_bytes_)) {
    return false;
  }
  fill_on_free_bytes_ = fill_on_alloc_bytes_;
  return true;
}

bool Config::SetFillOnAlloc(const std::string& option, const std::string& value) {
  return ParseValue(option, value, SIZE_MAX, 1, SIZE_MAX, &fill_on_alloc_bytes_);
}

bool Config::SetFillOnFree(const std::string& option, const std::string& value) {
  return ParseValue(option, value, SIZE_MAX, 1, SIZE_MAX, &fill_on_free_bytes_);
}

bool Config::SetBacktrace(const std::string& option, const std::string& value) {
  backtrace_enabled_ = true;
  return ParseValue(option, value, DEFAULT_BACKTRACE_FRAMES, 1, MAX_BACKTRACE_FRAMES,
                    &backtrace_frames_);
}

bool Config::SetBacktraceEnableOnSignal(const std::string& option, const std::string& value) {
  backtrace_enable_on_signal_ = true;
  return ParseValue(option, value, DEFAULT_BACKTRACE_FRAMES, 1, MAX_BACKTRACE_FRAMES,
                    &backtrace_frames_);
}

bool Config::SetBacktraceDumpOnExit(const std::string& option, const std::string& value) {
  if (Config::VerifyValueEmpty(option, value)) {
    backtrace_dump_on_exit_ = true;
    return true;
  }
  return false;
}

bool Config::SetBacktraceDumpPrefix(const std::string&, const std::string& value) {
  if (value.empty()) {
    backtrace_dump_prefix_ = DEFAULT_BACKTRACE_DUMP_PREFIX;
  } else {
    backtrace_dump_prefix_ = value;
  }
  return true;
}

bool Config::SetExpandAlloc(const std::string& option, const std::string& value) {
  return ParseValue(option, value, DEFAULT_EXPAND_BYTES, 1, MAX_EXPAND_BYTES, &expand_alloc_bytes_);
}

bool Config::SetFreeTrack(const std::string& option, const std::string& value) {
  // This option enables fill on free, so set the bytes to the default value.
  if (fill_on_free_bytes_ == 0) {
    fill_on_free_bytes_ = SIZE_MAX;
  }
  if (free_track_backtrace_num_frames_ == 0) {
    free_track_backtrace_num_frames_ = DEFAULT_BACKTRACE_FRAMES;
  }

  return ParseValue(option, value, DEFAULT_FREE_TRACK_ALLOCATIONS, 1, MAX_FREE_TRACK_ALLOCATIONS,
                    &free_track_allocations_);
}

bool Config::SetFreeTrackBacktraceNumFrames(const std::string& option, const std::string& value) {
  return ParseValue(option, value, DEFAULT_BACKTRACE_FRAMES, 0, MAX_BACKTRACE_FRAMES,
                    &free_track_backtrace_num_frames_);
}

bool Config::SetRecordAllocs(const std::string& option, const std::string& value) {
  if (record_allocs_file_.empty()) {
    record_allocs_file_ = DEFAULT_RECORD_ALLOCS_FILE;
  }
  return ParseValue(option, value, DEFAULT_RECORD_ALLOCS, 1, MAX_RECORD_ALLOCS,
                    &record_allocs_num_entries_);
}

bool Config::SetRecordAllocsFile(const std::string&, const std::string& value) {
  if (value.empty()) {
    // Set the default.
    record_allocs_file_ = DEFAULT_RECORD_ALLOCS_FILE;
    return true;
  }
  record_allocs_file_ = value;
  return true;
}

bool Config::VerifyValueEmpty(const std::string& option, const std::string& value) {
  if (!value.empty()) {
    // This is not valid.
    error_log("%s: value set for option '%s' which does not take a value", getprogname(),
              option.c_str());
    return false;
  }
  return true;
}

void Config::LogUsage() const {
  error_log("For malloc debug option descriptions go to:");
  error_log("  https://android.googlesource.com/platform/bionic/+/master/libc/malloc_debug/README.md");
}

bool Config::GetOption(const char** options_str, std::string* option, std::string* value) {
  const char* cur = *options_str;
  // Process each property name we can find.
  while (isspace(*cur)) ++cur;

  if (*cur == '\0') {
    *options_str = cur;
    return false;
  }

  const char* start = cur;
  while (!isspace(*cur) && *cur != '=' && *cur != '\0') ++cur;

  *option = std::string(start, cur - start);

  // Skip any spaces after the name.
  while (isspace(*cur)) ++cur;

  value->clear();
  if (*cur == '=') {
    ++cur;
    // Skip the space after the equal.
    while (isspace(*cur)) ++cur;

    start = cur;
    while (!isspace(*cur) && *cur != '\0') ++cur;

    if (cur != start) {
      *value = std::string(start, cur - start);
    }
  }
  *options_str = cur;
  return true;
}

bool Config::Init(const char* options_str) {
  // Initialize a few default values.
  fill_alloc_value_ = DEFAULT_FILL_ALLOC_VALUE;
  fill_free_value_ = DEFAULT_FILL_FREE_VALUE;
  front_guard_value_ = DEFAULT_FRONT_GUARD_VALUE;
  rear_guard_value_ = DEFAULT_REAR_GUARD_VALUE;
  backtrace_signal_ = SIGRTMAX - 19;
  backtrace_dump_signal_ = SIGRTMAX - 17;
  record_allocs_signal_ = SIGRTMAX - 18;
  free_track_backtrace_num_frames_ = 0;
  record_allocs_file_.clear();
  fill_on_free_bytes_ = 0;
  backtrace_enable_on_signal_ = false;
  backtrace_enabled_ = false;
  backtrace_dump_on_exit_ = false;
  backtrace_dump_prefix_ = DEFAULT_BACKTRACE_DUMP_PREFIX;

  // Process each option name we can find.
  std::string option;
  std::string value;
  bool valid = true;
  while (GetOption(&options_str, &option, &value)) {
    auto entry = kOptions.find(option);
    if (entry == kOptions.end()) {
      error_log("%s: unknown option %s", getprogname(), option.c_str());
      valid = false;
      break;
    }

    const OptionInfo* info = &entry->second;
    auto process_func = info->process_func;
    if (process_func != nullptr && !(this->*process_func)(option, value)) {
      valid = false;
      break;
    }
    options_ |= info->option;
  }

  if (!valid || *options_str != '\0') {
    LogUsage();
    return false;
  }

  return true;
}
