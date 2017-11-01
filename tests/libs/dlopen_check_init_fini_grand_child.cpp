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
#include <string>

// These two function are called by local group's constructors and destructors
extern "C" __attribute__((weak)) void record_init(int digit);
extern "C" __attribute__((weak)) void record_fini(const char* s);

static void __attribute__((constructor)) init() {
  record_init(3);
}

static void __attribute__((destructor)) fini() {
  record_fini("(grandchild)");
}
