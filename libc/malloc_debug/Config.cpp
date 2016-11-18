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

static constexpr size_t DEFAULT_EXPAND_BYTES = 16;
static constexpr size_t MAX_EXPAND_BYTES = 16384;

static constexpr size_t DEFAULT_FREE_TRACK_ALLOCATIONS = 100;
static constexpr size_t MAX_FREE_TRACK_ALLOCATIONS = 16384;

static constexpr size_t DEFAULT_RECORD_ALLOCS = 8000000;
static constexpr size_t MAX_RECORD_ALLOCS = 50000000;
static constexpr const char DEFAULT_RECORD_ALLOCS_FILE[] = "/data/local/tmp/record_allocs.txt";

struct Option {
  Option(std::string name, uint64_t option, bool combo_option = false, bool* config = nullptr)
      : name(name), option(option), combo_option(combo_option), config(config) {}
  virtual ~Option() = default;

  std::string name;

  uint64_t option;
  // If set to true, then all of the options following are set on until
  // the combo_option value is set to false.
  bool combo_option = false;
  bool* config;

  virtual bool ParseValue(const std::string& option_name, const std::string& value) const;

  virtual void SetDefault() const { }
};

bool Option::ParseValue(const std::string& option_name, const std::string& raw_value) const {
  if (!raw_value.empty()) {
    error_log("%s: value set for option '%s' which does not take a value",
              getprogname(), option_name.c_str());
    return false;
  }
  return true;
}

struct OptionString : public Option {
  OptionString(std::string name, uint64_t option, std::string default_value,
                std::string* value, bool combo_option = false,
                bool* config = nullptr)
      : Option(name, option, combo_option, config), default_value(default_value), value(value) {}
  virtual ~OptionString() = default;

  std::string default_value;
  std::string* value;

  bool ParseValue(const std::string& option_name, const std::string& value) const override;

  void SetDefault() const override { if (value) *value = default_value; }
};

bool OptionString::ParseValue(const std::string&, const std::string& raw_value) const {
  if (!raw_value.empty()) {
    *value = raw_value;
  }
  return true;
}

struct OptionSizeT : public Option {
  OptionSizeT(std::string name, size_t default_value, size_t min_value, size_t max_value,
          uint64_t option, size_t* value, bool combo_option = false, bool* config = nullptr)
      : Option(name, option, combo_option, config), default_value(default_value),
        min_value(min_value), max_value(max_value), value(value) {}
  virtual ~OptionSizeT() = default;

  size_t default_value;
  size_t min_value;
  size_t max_value;

  size_t* value;

  bool ParseValue(const std::string& option_name, const std::string& value) const override;

  void SetDefault() const override { if (value) *value = default_value; }
};

bool OptionSizeT::ParseValue(const std::string& option_name, const std::string& raw_value) const {
  if (raw_value.empty()) {
    // Value should have been set by the SetDefault() pass.
    return true;
  }

  // Parse the value into a size_t value.
  errno = 0;
  char* end;
  long parsed_value = strtol(raw_value.c_str(), &end, 10);
  if (errno != 0) {
    error_log("%s: bad value for option '%s': %s", getprogname(), option_name.c_str(),
              strerror(errno));
    return false;
  }
  if (end == raw_value.c_str()) {
    error_log("%s: bad value for option '%s'", getprogname(), option_name.c_str());
    return false;
  }
  if (static_cast<size_t>(end - raw_value.c_str()) != raw_value.size()) {
    error_log("%s: bad value for option '%s', non space found after option: %s",
              getprogname(), option_name.c_str(), end);
    return false;
  }
  if (parsed_value < 0) {
    error_log("%s: bad value for option '%s', value cannot be negative: %ld",
              getprogname(), option_name.c_str(), parsed_value);
    return false;
  }

  if (static_cast<size_t>(parsed_value) < min_value) {
    error_log("%s: bad value for option '%s', value must be >= %zu: %ld",
              getprogname(), option_name.c_str(), min_value, parsed_value);
    return false;
  }
  if (static_cast<size_t>(parsed_value) > max_value) {
    error_log("%s: bad value for option '%s', value must be <= %zu: %ld",
              getprogname(), option_name.c_str(), max_value, parsed_value);
    return false;
  }
  *value = static_cast<size_t>(parsed_value);
  return true;
}

class PropertyParser {
 public:
  explicit PropertyParser(const char* property) : cur_(property) {}

  bool Get(std::string* property, std::string* value);

  bool Done() { return done_; }

  void LogUsage();

 private:
  const char* cur_ = nullptr;

  bool done_ = false;

  DISALLOW_COPY_AND_ASSIGN(PropertyParser);
};

bool PropertyParser::Get(std::string* property, std::string* value) {
  // Process each property name we can find.
  while (isspace(*cur_))
    ++cur_;

  if (*cur_ == '\0') {
    done_ = true;
    return false;
  }

  const char* start = cur_;
  while (!isspace(*cur_) && *cur_ != '=' && *cur_ != '\0')
    ++cur_;

  *property = std::string(start, cur_ - start);

  // Skip any spaces after the name.
  while (isspace(*cur_))
    ++cur_;

  value->clear();
  if (*cur_ == '=') {
    ++cur_;
    // Skip the space after the equal.
    while (isspace(*cur_))
      ++cur_;

    start = cur_;
    while (!isspace(*cur_) && *cur_ != '\0')
      ++cur_;

    if (cur_ != start) {
      *value = std::string(start, cur_ - start);
    }
  }
  return true;
}

void PropertyParser::LogUsage() {
  error_log("malloc debug options usage:");
  error_log("");
  error_log("  front_guard[=XX]");
  error_log("    Enables a front guard on all allocations. If XX is set");
  error_log("    it sets the number of bytes in the guard. The default is");
  error_log("    %zu bytes, the max bytes is %zu.", DEFAULT_GUARD_BYTES, MAX_GUARD_BYTES);
  error_log("");
  error_log("  rear_guard[=XX]");
  error_log("    Enables a rear guard on all allocations. If XX is set");
  error_log("    it sets the number of bytes in the guard. The default is");
  error_log("    %zu bytes, the max bytes is %zu.", DEFAULT_GUARD_BYTES, MAX_GUARD_BYTES);
  error_log("");
  error_log("  guard[=XX]");
  error_log("    Enables both a front guard and a rear guard on all allocations.");
  error_log("    If XX is set it sets the number of bytes in both guards.");
  error_log("    The default is %zu bytes, the max bytes is %zu.",
            DEFAULT_GUARD_BYTES, MAX_GUARD_BYTES);
  error_log("");
  error_log("  backtrace[=XX]");
  error_log("    Enable capturing the backtrace at the point of allocation.");
  error_log("    If XX is set it sets the number of backtrace frames.");
  error_log("    The default is %zu frames, the max number of frames is %zu.",
            DEFAULT_BACKTRACE_FRAMES, MAX_BACKTRACE_FRAMES);
  error_log("");
  error_log("  backtrace_enable_on_signal[=XX]");
  error_log("    Enable capturing the backtrace at the point of allocation.");
  error_log("    The backtrace capture is not enabled until the process");
  error_log("    receives a signal. If XX is set it sets the number of backtrace");
  error_log("    frames. The default is %zu frames, the max number of frames is %zu.",
            DEFAULT_BACKTRACE_FRAMES, MAX_BACKTRACE_FRAMES);
  error_log("");
  error_log("  fill_on_alloc[=XX]");
  error_log("    On first allocation, fill with the value 0x%02x.", DEFAULT_FILL_ALLOC_VALUE);
  error_log("    If XX is set it will only fill up to XX bytes of the");
  error_log("    allocation. The default is to fill the entire allocation.");
  error_log("");
  error_log("  fill_on_free[=XX]");
  error_log("    On free, fill with the value 0x%02x. If XX is set it will",
            DEFAULT_FILL_FREE_VALUE);
  error_log("    only fill up to XX bytes of the allocation. The default is to");
  error_log("    fill the entire allocation.");
  error_log("");
  error_log("  fill[=XX]");
  error_log("    On both first allocation free, fill with the value 0x%02x on",
            DEFAULT_FILL_ALLOC_VALUE);
  error_log("    first allocation and the value 0x%02x. If XX is set, only fill",
            DEFAULT_FILL_FREE_VALUE);
  error_log("    up to XX bytes. The default is to fill the entire allocation.");
  error_log("");
  error_log("  expand_alloc[=XX]");
  error_log("    Allocate an extra number of bytes for every allocation call.");
  error_log("    If XX is set, that is the number of bytes to expand the");
  error_log("    allocation by. The default is %zu bytes, the max bytes is %zu.",
            DEFAULT_EXPAND_BYTES, MAX_EXPAND_BYTES);
  error_log("");
  error_log("  free_track[=XX]");
  error_log("    When a pointer is freed, do not free the memory right away.");
  error_log("    Instead, keep XX of these allocations around and then verify");
  error_log("    that they have not been modified when the total number of freed");
  error_log("    allocations exceeds the XX amount. When the program terminates,");
  error_log("    the rest of these allocations are verified. When this option is");
  error_log("    enabled, it automatically records the backtrace at the time of the free.");
  error_log("    The default is to record %zu allocations, the max allocations",
            DEFAULT_FREE_TRACK_ALLOCATIONS);
  error_log("    to record is %zu.", MAX_FREE_TRACK_ALLOCATIONS);
  error_log("");
  error_log("  free_track_backtrace_num_frames[=XX]");
  error_log("    This option only has meaning if free_track is set. This indicates");
  error_log("    how many backtrace frames to capture when an allocation is freed.");
  error_log("    If XX is set, that is the number of frames to capture. If XX");
  error_log("    is set to zero, then no backtrace will be captured.");
  error_log("    The default is to record %zu frames, the max number of frames is %zu.",
            DEFAULT_BACKTRACE_FRAMES, MAX_BACKTRACE_FRAMES);
  error_log("");
  error_log("  leak_track");
  error_log("    Enable the leak tracking of memory allocations.");
  error_log("");
  error_log("  record_allocs[=XX]");
  error_log("    Record every single allocation/free call. When a specific signal");
  error_log("    is sent to the process, the contents of recording are written to");
  error_log("    a file (%s) and the recording is cleared.", DEFAULT_RECORD_ALLOCS_FILE);
  error_log("    If XX is set, that is the total number of allocations/frees that can");
  error_log("    recorded. of frames to capture. The default value is %zu.", DEFAULT_RECORD_ALLOCS);
  error_log("    If the allocation list fills up, all further allocations are not recorded.");
  error_log("");
  error_log("  record_allocs_file[=FILE]");
  error_log("    This option only has meaning if the record_allocs options has been specified.");
  error_log("    This is the name of the file to which recording information will be dumped.");
  error_log("    The default is %s.", DEFAULT_RECORD_ALLOCS_FILE);
}

// This function is designed to be called once. A second call will not
// reset all variables.
bool Config::Set(const char* options_str) {
  // Initialize a few default values.
  fill_alloc_value = DEFAULT_FILL_ALLOC_VALUE;
  fill_free_value = DEFAULT_FILL_FREE_VALUE;
  front_guard_value = DEFAULT_FRONT_GUARD_VALUE;
  rear_guard_value = DEFAULT_REAR_GUARD_VALUE;
  backtrace_signal = SIGRTMAX - 19;
  record_allocs_signal = SIGRTMAX - 18;
  free_track_backtrace_num_frames = 0;
  record_allocs_file.clear();

  // Parse the options are of the format:
  //   option_name or option_name=XX

  // Supported options:
  const OptionSizeT option_guard(
      "guard", DEFAULT_GUARD_BYTES, 1, MAX_GUARD_BYTES, 0, nullptr, true);
  // Enable front guard. Value is the size of the guard.
  const OptionSizeT option_front_guard(
      "front_guard", DEFAULT_GUARD_BYTES, 1, MAX_GUARD_BYTES, FRONT_GUARD,
      &this->front_guard_bytes, true);
  // Enable end guard. Value is the size of the guard.
  const OptionSizeT option_rear_guard(
      "rear_guard", DEFAULT_GUARD_BYTES, 1, MAX_GUARD_BYTES, REAR_GUARD, &this->rear_guard_bytes,
      true);

  // Enable logging the backtrace on allocation. Value is the total
  // number of frames to log.
  const OptionSizeT option_backtrace(
      "backtrace", DEFAULT_BACKTRACE_FRAMES, 1, MAX_BACKTRACE_FRAMES, BACKTRACE | TRACK_ALLOCS,
      &this->backtrace_frames, false, &this->backtrace_enabled);
  // Enable gathering backtrace values on a signal.
  const OptionSizeT option_backtrace_enable_on_signal(
      "backtrace_enable_on_signal", DEFAULT_BACKTRACE_FRAMES, 1, MAX_BACKTRACE_FRAMES,
      BACKTRACE | TRACK_ALLOCS, &this->backtrace_frames, false, &this->backtrace_enable_on_signal);

  const OptionSizeT option_fill("fill", SIZE_MAX, 1, SIZE_MAX, 0, nullptr, true);
  // Fill the allocation with an arbitrary pattern on allocation.
  // Value is the number of bytes of the allocation to fill
  // (default entire allocation).
  const OptionSizeT option_fill_on_alloc(
      "fill_on_alloc", SIZE_MAX, 1, SIZE_MAX, FILL_ON_ALLOC, &this->fill_on_alloc_bytes, true);
  // Fill the allocation with an arbitrary pattern on free.
  // Value is the number of bytes of the allocation to fill
  // (default entire allocation).
  const OptionSizeT option_fill_on_free(
      "fill_on_free", SIZE_MAX, 1, SIZE_MAX, FILL_ON_FREE, &this->fill_on_free_bytes, true);

  // Expand the size of every alloc by this number bytes. Value is
  // the total number of bytes to expand every allocation by.
  const OptionSizeT option_expand_alloc(
      "expand_alloc", DEFAULT_EXPAND_BYTES, 1, MAX_EXPAND_BYTES, EXPAND_ALLOC,
      &this->expand_alloc_bytes);

  // Keep track of the freed allocations and verify at a later date
  // that they have not been used. Turning this on, also turns on
  // fill on free.
  const OptionSizeT option_free_track(
      "free_track", DEFAULT_FREE_TRACK_ALLOCATIONS, 1, MAX_FREE_TRACK_ALLOCATIONS,
      FREE_TRACK | FILL_ON_FREE, &this->free_track_allocations);
  // Number of backtrace frames to keep when free_track is enabled. If this
  // value is set to zero, no backtrace will be kept.
  const OptionSizeT option_free_track_backtrace_num_frames(
      "free_track_backtrace_num_frames", DEFAULT_BACKTRACE_FRAMES, 0, MAX_BACKTRACE_FRAMES, 0,
      &this->free_track_backtrace_num_frames);

  // Enable printing leaked allocations.
  const Option option_leak_track("leak_track", LEAK_TRACK | TRACK_ALLOCS);

  const OptionSizeT option_record_allocs(
      "record_allocs", DEFAULT_RECORD_ALLOCS, 1, MAX_RECORD_ALLOCS, RECORD_ALLOCS,
      &this->record_allocs_num_entries);
  const OptionString option_record_allocs_file(
      "record_allocs_file", 0, DEFAULT_RECORD_ALLOCS_FILE, &this->record_allocs_file);

  const Option* option_list[] = {
    &option_guard, &option_front_guard, &option_rear_guard,
    &option_backtrace, &option_backtrace_enable_on_signal,
    &option_fill, &option_fill_on_alloc, &option_fill_on_free,
    &option_expand_alloc,
    &option_free_track, &option_free_track_backtrace_num_frames,
    &option_leak_track,
    &option_record_allocs, &option_record_allocs_file,
  };

  // Set defaults for all of the options.
  for (size_t i = 0; i < sizeof(option_list)/sizeof(Option*); i++) {
    option_list[i]->SetDefault();
  }

  // Process each property name we can find.
  PropertyParser parser(options_str);
  bool valid = true;
  std::string property;
  std::string value;
  while (valid && parser.Get(&property, &value)) {
    bool found = false;
    for (size_t i = 0; i < sizeof(option_list)/sizeof(Option*); i++) {
      if (property == option_list[i]->name) {
        if (option_list[i]->option == 0 && option_list[i]->combo_option) {
          const std::string* option_name = &option_list[i]->name;
          i++;
          for (; i < sizeof(option_list)/sizeof(Option*) && option_list[i]->combo_option; i++) {
            if (!option_list[i]->ParseValue(*option_name, value)) {
              valid = false;
              break;
            }
            if (option_list[i]->config) {
              *option_list[i]->config = true;
            }
            options |= option_list[i]->option;
          }
          if (!valid) {
            break;
          }
        } else {
          if (!option_list[i]->ParseValue(option_list[i]->name, value)) {
            valid = false;
            break;
          }
          if (option_list[i]->config) {
            *option_list[i]->config = true;
          }
          options |= option_list[i]->option;
        }
        found = true;
        break;
      }
    }
    if (valid && !found) {
      error_log("%s: unknown option %s", getprogname(), property.c_str());
      valid = false;
      break;
    }
  }

  valid = valid && parser.Done();

  if (valid) {
    // It's necessary to align the front guard to MINIMUM_ALIGNMENT_BYTES to
    // make sure that the header is aligned properly.
    if (options & FRONT_GUARD) {
      front_guard_bytes = BIONIC_ALIGN(front_guard_bytes, MINIMUM_ALIGNMENT_BYTES);
    }
  } else {
    parser.LogUsage();
  }

  return valid;
}
