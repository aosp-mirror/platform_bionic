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

static int volatile g_initialization_order_code;

void (*g_fini_callback)(const char*) = nullptr;

// These two function are called by local group's constructors and destructors
extern "C" void record_init(int digit) {
  g_initialization_order_code = g_initialization_order_code*10 + digit;
}

extern "C" void record_fini(const char* s) {
  g_fini_callback(s);
}

// these 2 functions are used by the test
extern "C" int get_init_order_number() {
  return g_initialization_order_code;
}

extern "C" void set_fini_callback(void (*f)(const char*)) {
  g_fini_callback = f;
}

static void __attribute__((constructor)) init() {
  record_init(1);
}

static void __attribute__((destructor)) fini() {
  record_fini("(root)");
}
