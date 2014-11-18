/*
 * Copyright (C) 2014 The Android Open Source Project
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

extern "C" int check_order_reloc_get_answer_impl();

extern "C" int check_order_reloc_nephew_get_answer() {
  return check_order_reloc_get_answer_impl();
}

namespace {
// The d-tor for this class is called on dlclose() -> __on_dlclose() -> __cxa_finalize()
// We use it to detect calls to prematurely unmapped libraries during dlclose.
// See also b/18338888
class CallNephewInDtor {
 public:
  ~CallNephewInDtor() {
    check_order_reloc_get_answer_impl();
  }
} instance;
};

extern "C" void* get_instance() {
  return &instance;
}
