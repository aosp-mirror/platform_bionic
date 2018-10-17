/*
 * Copyright (C) 2018 The Android Open Source Project
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

extern "C" int getRandomNumber();

class B {
public:
  virtual int getBiggerRandomNumber() {
    // Call to the other library.
    return getRandomNumber() * 2;
  }

  virtual ~B() {}
};

B b;

// nested macros to make it easy to define a large amount of read-only data
// which will require relocation.
#define B_16 &b, &b, &b, &b, &b, &b, &b, &b, &b, &b, &b, &b, &b, &b, &b, &b,
#define B_128 B_16 B_16 B_16 B_16 B_16 B_16 B_16 B_16
#define B_1024 B_128 B_128 B_128 B_128 B_128 B_128 B_128 B_128

extern "C" B* const lots_more_relro[] = {
  B_1024 B_1024 B_1024 B_1024 B_1024 B_1024 B_1024 B_1024
};

extern "C" int getBiggerRandomNumber() {
  // access the relro section (twice, in fact, once for the pointer, and once
  // for the vtable of B) to check it's actually there.
  return lots_more_relro[0]->getBiggerRandomNumber();
}
