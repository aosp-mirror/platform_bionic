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

#include <errno.h>
#include <gtest/gtest.h>

// Defined in string_posix_strerror_r_wrapper.cpp as a wrapper around the posix
// strerror_r to work around an incompatibility between libc++ (required by
// gtest) and !_GNU_SOURCE.
int posix_strerror_r(int errnum, char* buf, size_t buflen);

TEST(string, posix_strerror_r) {
  char buf[256];

  // Valid.
  ASSERT_EQ(0, posix_strerror_r(0, buf, sizeof(buf)));
#if defined(ANDROID_HOST_MUSL)
  ASSERT_STREQ("No error information", buf);
#else
  ASSERT_STREQ("Success", buf);
#endif
  ASSERT_EQ(0, posix_strerror_r(1, buf, sizeof(buf)));
  ASSERT_STREQ("Operation not permitted", buf);

#if defined(__BIONIC__) || defined(ANDROID_HOST_MUSL)
  // Invalid.
  ASSERT_EQ(0, posix_strerror_r(-1, buf, sizeof(buf)));
# if defined(__BIONIC__)
  ASSERT_STREQ("Unknown error -1", buf);
# else
  ASSERT_STREQ("No error information", buf);
# endif
  ASSERT_EQ(0, posix_strerror_r(1234, buf, sizeof(buf)));
# if defined(__BIONIC__)
  ASSERT_STREQ("Unknown error 1234", buf);
# else
  ASSERT_STREQ("No error information", buf);
# endif
#else
  // glibc returns EINVAL for unknown errors
  ASSERT_EQ(EINVAL, posix_strerror_r(-1, buf, sizeof(buf)));
  ASSERT_EQ(EINVAL, posix_strerror_r(1234, buf, sizeof(buf)));
#endif

  // Buffer too small.
  errno = 0;
  memset(buf, 0, sizeof(buf));
  ASSERT_EQ(ERANGE, posix_strerror_r(EPERM, buf, 2));
  ASSERT_STREQ("O", buf);
  // POSIX strerror_r returns an error without updating errno.
  ASSERT_EQ(0, errno);
}
