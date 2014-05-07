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
#include <stdio.h>
#include <stdlib.h>

#include <string>

// use external control number from main test
static std::string* atexit_sequence = NULL;
static bool* atexit_valid_this_in_static_dtor = NULL;

static class AtExitStaticClass {
 public:
  AtExitStaticClass() { expected_this = this; }
  ~AtExitStaticClass() {
    if (atexit_valid_this_in_static_dtor) {
      *atexit_valid_this_in_static_dtor = (expected_this == this);
    }
  }
 private:
  static const AtExitStaticClass* expected_this;

} static_obj;

const AtExitStaticClass* AtExitStaticClass::expected_this = NULL;

// 4
static void atexit_handler_from_atexit_from_atexit2() {
  *atexit_sequence += " on";
}

// 3
static void atexit_handler_from_atexit_from_atexit1() {
  *atexit_sequence += " sat";
}

// 2
static void atexit_handler_from_atexit() {
  *atexit_sequence += " Dumpty";
  // register 2 others
  atexit(atexit_handler_from_atexit_from_atexit2);
  atexit(atexit_handler_from_atexit_from_atexit1);
}

// 1
static void atexit_handler_with_atexit() {
  *atexit_sequence += "Humpty";
  atexit(atexit_handler_from_atexit);
}

// last
static void atexit_handler_regular() {
  *atexit_sequence += " a wall";
}

extern "C" void register_atexit(std::string* sequence, bool* valid_this_in_static_dtor) {
  atexit_sequence = sequence;
  atexit_valid_this_in_static_dtor = valid_this_in_static_dtor;
  atexit(atexit_handler_regular);
  atexit(atexit_handler_with_atexit);
  atexit(NULL);
}

