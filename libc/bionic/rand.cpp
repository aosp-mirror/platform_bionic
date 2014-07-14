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

#include <stdlib.h>

// The BSD rand/srand is very weak. glibc just uses random/srandom instead.
// Since we're likely to run code intended for glibc, and POSIX doesn't seem
// to disallow this, we go that route too.

int rand() {
  return random();
}

void srand(unsigned int seed) {
  return srandom(seed);
}
