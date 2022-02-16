/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef _GNU_SOURCE
  #define _GNU_SOURCE 1
#endif

#include <string.h>

#if defined(basename)
  #error basename should not be defined at this point
#endif

static const char* gnu_basename(const char* in) {
  return basename(in);
}

#include <libgen.h>

#if !defined(basename)
  #error basename should be defined at this point
#endif

static char* posix_basename(char* in) {
  return basename(in);
}

#include <errno.h>
#include <gtest/gtest.h>

static void __TestGnuBasename(const char* in, const char* expected_out, int line) {
  errno = 0;
  const char* out = gnu_basename(in);
  ASSERT_STREQ(expected_out, out) << "(" << line << "): " << in << std::endl;
  ASSERT_EQ(0, errno) << "(" << line << "): " << in << std::endl;
}

static void __TestPosixBasename(const char* in, const char* expected_out, int line) {
  char* writable_in = (in != nullptr) ? strdup(in) : nullptr;
  errno = 0;
  const char* out = posix_basename(&writable_in[0]);
  ASSERT_STREQ(expected_out, out) << "(" << line << "): " << in << std::endl;
  ASSERT_EQ(0, errno) << "(" << line << "): " << in << std::endl;
  free(writable_in);
}

#define TestGnuBasename(in, expected) __TestGnuBasename(in, expected, __LINE__)
#define TestPosixBasename(in, expected) __TestPosixBasename(in, expected, __LINE__)

TEST(libgen_basename, gnu_basename) {
  // GNU's basename doesn't accept NULL
  // TestGnuBasename(NULL, ".");
  TestGnuBasename("", "");
  TestGnuBasename("/usr/lib", "lib");
  TestGnuBasename("/system/bin/sh/", "");
  TestGnuBasename("/usr/", "");
  TestGnuBasename("usr", "usr");
  TestGnuBasename("/", "");
  TestGnuBasename(".", ".");
  TestGnuBasename("..", "..");
  TestGnuBasename("///", "");
  TestGnuBasename("//usr//lib//", "");
}

TEST(libgen_basename, posix_basename) {
  TestPosixBasename(nullptr, ".");
  TestPosixBasename("", ".");
  TestPosixBasename("/usr/lib", "lib");
  TestPosixBasename("/system/bin/sh/", "sh");
  TestPosixBasename("/usr/", "usr");
  TestPosixBasename("usr", "usr");
  TestPosixBasename("/", "/");
  TestPosixBasename(".", ".");
  TestPosixBasename("..", "..");
  TestPosixBasename("///", "/");
  TestPosixBasename("//usr//lib//", "lib");
}
