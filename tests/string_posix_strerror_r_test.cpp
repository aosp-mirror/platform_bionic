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

#undef _GNU_SOURCE

// Old versions of glibc (like our current host prebuilt sysroot one) have
// headers that don't work if you #undef _GNU_SOURCE, which makes it
// impossible to build this test.
#include <features.h>

#if !defined(__GLIBC__)
#include <string.h>

#include <errno.h>
#include <gtest/gtest.h>

TEST(string, posix_strerror_r) {
  char buf[256];

  // Valid.
  ASSERT_EQ(0, strerror_r(0, buf, sizeof(buf)));
  ASSERT_STREQ("Success", buf);
  ASSERT_EQ(0, strerror_r(1, buf, sizeof(buf)));
  ASSERT_STREQ("Operation not permitted", buf);

  // Invalid.
  ASSERT_EQ(0, strerror_r(-1, buf, sizeof(buf)));
  ASSERT_STREQ("Unknown error -1", buf);
  ASSERT_EQ(0, strerror_r(1234, buf, sizeof(buf)));
  ASSERT_STREQ("Unknown error 1234", buf);

  // Buffer too small.
  errno = 0;
  memset(buf, 0, sizeof(buf));
  ASSERT_EQ(-1, strerror_r(4567, buf, 2));
  ASSERT_STREQ("U", buf);
  // The POSIX strerror_r sets errno to ERANGE (the GNU one doesn't).
  ASSERT_EQ(ERANGE, errno);
}
#else
# if __GLIBC_PREREQ(2, 15)
#  error this test should work now
# endif
#endif
