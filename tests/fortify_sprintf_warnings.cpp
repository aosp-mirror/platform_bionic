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

#undef _FORTIFY_SOURCE
#define _FORTIFY_SOURCE 2
#include <stdio.h>

void test_sprintf() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___sprintf_chk(char*, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  sprintf(buf, "foobar"); // NOLINT(runtime/printf)

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___sprintf_chk(char*, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  sprintf(buf, "%s", "foobar"); // NOLINT(runtime/printf)
}

void test_snprintf() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___snprintf_chk(char*, {{(long )?}}unsigned int, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, "foobar"); // NOLINT(runtime/printf)

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___snprintf_chk(char*, {{(long )?}}unsigned int, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, "%s", "foobar"); // NOLINT(runtime/printf)

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___snprintf_chk(char*, {{(long )?}}unsigned int, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, " %s ", "foobar"); // NOLINT(runtime/printf)

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___snprintf_chk(char*, {{(long )?}}unsigned int, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, "%d", 100000); // NOLINT(runtime/printf)
}
