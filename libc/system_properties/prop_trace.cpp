/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include "system_properties/prop_trace.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "private/CachedProperty.h"
#include "private/bionic_lock.h"
#include "private/bionic_systrace.h"

#include <async_safe/log.h>
#include <cutils/trace.h>  // For ATRACE_TAG_SYSPROP.

#define PROP_TRACE_MSG_LENGTH 1024

static bool should_trace_prop(const char* prop_name) {
  // Should not trace kTraceTagsProp to avoid infinite recursion.
  // Because the following g_trace_enable_flags.Get() will get the property value
  // of kTraceTagsProp again, which in turn invokes should_trace_prop() here.
  if (prop_name == nullptr || !strcmp(prop_name, kTraceTagsProp)) {
    return false;
  }

  return should_trace(ATRACE_TAG_SYSPROP);
}

static void sysprop_trace_end() {
  int trace_marker_fd = get_trace_marker_fd();
  if (trace_marker_fd == -1) {
    return;
  }

  TEMP_FAILURE_RETRY(write(trace_marker_fd, "E|", 2));
}

static void get_sysprop_trace_end(const prop_info* pi, const char* prop_value,
                                  bool read_only = false) {
  const char* output_value;
  char message[PROP_TRACE_MSG_LENGTH];

  if (read_only) {
    if (pi->is_long()) {
      output_value = pi->long_value();
    } else {
      output_value = pi->value;
    }
  } else {
    output_value = prop_value;
  }

  snprintf(message, sizeof(message), "prop_get: %s, value: %s", pi->name,
           output_value ? output_value : "null_value");
  output_trace(message, 'E');  // 'E' for end.
}

SyspropTrace::SyspropTrace(const char* prop_name, const char* prop_value, const prop_info* pi,
                           PropertyAction action)
    : prop_name_(prop_name),
      prop_value_(prop_value),
      prop_info_(pi),
      prop_action_(action),
      output_trace_(false) {
  if (!should_trace_prop(prop_name)) {
    return;
  }

  char message[PROP_TRACE_MSG_LENGTH];
  if (prop_action_ == PropertyAction::kPropertyFind) {
    snprintf(message, sizeof(message), "prop_find: %s", prop_name_);
  } else if (prop_action_ == PropertyAction::kPropertySet) {
    snprintf(message, sizeof(message), "prop_set: %s, value: %s", prop_name_,
             prop_value_ ? prop_value_ : "null_value");
  } else {
    // For property get, the prop_value_ will be resolved then printed in the destructor.
    snprintf(message, sizeof(message), "prop_get: %s", prop_name_);
  }

  output_trace(message, 'B');  // 'B' for begin.
  output_trace_ = true;
}

SyspropTrace::~SyspropTrace() {
  if (!output_trace_) {
    return;
  }
  if (prop_action_ == PropertyAction::kPropertyFind ||
      prop_action_ == PropertyAction::kPropertySet) {
    sysprop_trace_end();
  } else if (prop_action_ == PropertyAction::kPropertyGetReadOnly) {
    get_sysprop_trace_end(prop_info_, prop_value_, true /* read_only */);
  } else if (prop_action_ == PropertyAction::kPropertyGetReadWrite) {
    get_sysprop_trace_end(prop_info_, prop_value_, false /* read_only */);
  }
  output_trace_ = false;
}
