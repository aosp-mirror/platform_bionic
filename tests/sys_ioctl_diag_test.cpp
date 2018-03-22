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

// This file makes uses of clang's built-in diagnostic checker.
// While not officially supported by clang, it's used by clang for all of its
// own diagnostic tests. Please see
// https://clang.llvm.org/doxygen/classclang_1_1VerifyDiagnosticConsumer.html#details
// for details.

// expected-no-diagnostics

#include <sys/ioctl.h>

#pragma clang diagnostic warning "-Wsign-conversion"

void check_no_signedness_warnings(int i, unsigned x) {
  ioctl(i, i);
  ioctl(i, x);

  ioctl(i, i, nullptr);
  ioctl(i, x, nullptr);
}
