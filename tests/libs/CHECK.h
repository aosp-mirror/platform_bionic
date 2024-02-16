/*
 * Copyright (C) 2017 The Android Open Source Project
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

// Tests proper can use libbase, but libraries for testing dlopen()
// should probably avoid dependencies other than ones we're specifically
// trying to test.

#include <assert.h>

#define CHECK(e) \
  ((e) ? static_cast<void>(0) : __assert2(__FILE__, __LINE__, __PRETTY_FUNCTION__, #e))
