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

#pragma once

#include "platform/bionic/macros.h"

#include "prop_info.h"

// Tracing class for sysprop. To begin a trace at a specified point:
//   SyspropTrace trace ("prop_name", "prop_value");
// The trace will end when the constructor goes out of scope.
// For read-only properties (ro.*), also need to pass prop_info struct.

enum class PropertyAction {
  kPropertyFind = 0,
  kPropertySet,
  kPropertyGetReadOnly,
  kPropertyGetReadWrite,
};

class __LIBC_HIDDEN__ SyspropTrace {
 public:
  explicit SyspropTrace(const char* prop_name, const char* prop_value, const prop_info* pi,
                        PropertyAction action);
  ~SyspropTrace();

 private:
  const char* prop_name_;
  const char* prop_value_;
  const prop_info* prop_info_;
  PropertyAction prop_action_;
  bool output_trace_;

  BIONIC_DISALLOW_COPY_AND_ASSIGN(SyspropTrace);
};
