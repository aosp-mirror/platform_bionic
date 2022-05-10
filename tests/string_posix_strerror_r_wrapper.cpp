/*
 * Copyright (C) 2021 The Android Open Source Project
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

#undef _GNU_SOURCE
#include <string.h>

// At the time of writing, libcxx -- which is dragged in by gtest -- assumes
// declarations from glibc of things that aren't available without _GNU_SOURCE.
// This means we can't even build a test that directly calls the posix
// strerror_r.  Add a wrapper in a separate file that doesn't use any gtest.
// For glibc 2.15, the symbols in question are:
//   at_quick_exit, quick_exit, vasprintf, strtoll_l, strtoull_l, and strtold_l.

int posix_strerror_r(int errnum, char* buf, size_t buflen) {
  return strerror_r(errnum, buf, buflen);
}
