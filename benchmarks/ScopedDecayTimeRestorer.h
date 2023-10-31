/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include <malloc.h>

#if defined(__BIONIC__)

#include "platform/bionic/malloc.h"

class ScopedDecayTimeRestorer {
 public:
  ScopedDecayTimeRestorer() {
    bool value;
    if (android_mallopt(M_GET_DECAY_TIME_ENABLED, &value, sizeof(value))) {
      saved_value_ = value ? 1 : 0;
    }
  }

  virtual ~ScopedDecayTimeRestorer() { mallopt(M_DECAY_TIME, saved_value_); }

 private:
  int saved_value_ = 0;
};

#endif
