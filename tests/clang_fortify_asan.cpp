/*
 * Copyright (C) 2019 The Android Open Source Project
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

/*
 * This test ensures that ensures that FORTIFY's run-time bits aren't enabled with ASAN on. Most
 * ways of getting FORTIFY to break turn into UB unless you get creative. Rather than remaking the
 * entire FORTIFY test-suite with this added constraint, we pick a function with well-defined
 * behavior when a FORTIFY check would fail (umask), and hope that the success of that is indicative
 * of the rest working.
 */

#ifndef __clang__
#error "Non-clang isn't supported"
#endif

#ifndef _FORTIFY_SOURCE
#error "_FORTIFY_SOURCE must be defined"
#endif

#include <sys/cdefs.h>

#if defined(__BIONIC__) && __has_feature(address_sanitizer)
#include <sys/stat.h>
#include <gtest/gtest.h>

TEST(ClangFortifyASAN, NoRuntimeChecksAreEnabled) {
  volatile mode_t unknown = 01000;
  mode_t previous = umask(unknown);

  // Not necessary, but polite.
  umask(previous);
}
#endif
