/*
 * Copyright (C) 2017 The Android Open Source Project
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

// <sys/random.h> was only added as of glibc version 2.25.
// Don't try to compile this code on older glibc versions.

#include <sys/cdefs.h>
#if defined(__BIONIC__)
  #define HAVE_SYS_RANDOM 1
#elif defined(__GLIBC_PREREQ)
  #if __GLIBC_PREREQ(2, 25)
    #define HAVE_SYS_RANDOM 1
  #endif
#endif


#if defined(HAVE_SYS_RANDOM)
#include <sys/random.h>
#endif

#include <errno.h>
#include <gtest/gtest.h>

#include "utils.h"

TEST(sys_random, getentropy) {
#if defined(HAVE_SYS_RANDOM)
  char buf1[64];
  char buf2[64];

  ASSERT_EQ(0, getentropy(buf1, sizeof(buf1)));
  ASSERT_EQ(0, getentropy(buf2, sizeof(buf2)));
  ASSERT_TRUE(memcmp(buf1, buf2, sizeof(buf1)) != 0);
#else
  GTEST_SKIP() << "<sys/random.h> not available";
#endif
}

TEST(sys_random, getentropy_EFAULT) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
#if defined(HAVE_SYS_RANDOM)
  errno = 0;
  ASSERT_EQ(-1, getentropy(nullptr, 1));
  ASSERT_ERRNO(EFAULT);
#else
  GTEST_SKIP() << "<sys/random.h> not available";
#endif
#pragma clang diagnostic pop
}

TEST(sys_random, getentropy_EIO) {
#if defined(HAVE_SYS_RANDOM)
  char buf[BUFSIZ];
  static_assert(BUFSIZ > 256, "BUFSIZ <= 256!");

  errno = 0;
  ASSERT_EQ(-1, getentropy(buf, sizeof(buf)));
  ASSERT_ERRNO(EIO);
#else
  GTEST_SKIP() << "<sys/random.h> not available";
#endif
}

TEST(sys_random, getrandom) {
#if defined(HAVE_SYS_RANDOM)
  char buf1[64];
  char buf2[64];

  ASSERT_EQ(64, getrandom(buf1, sizeof(buf1), 0));
  ASSERT_EQ(64, getrandom(buf2, sizeof(buf2), 0));
  ASSERT_TRUE(memcmp(buf1, buf2, sizeof(buf1)) != 0);
#else
  GTEST_SKIP() << "<sys/random.h> not available";
#endif
}

TEST(sys_random, getrandom_EFAULT) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
#if defined(HAVE_SYS_RANDOM)
  errno = 0;
  ASSERT_EQ(-1, getrandom(nullptr, 256, 0));
  ASSERT_ERRNO(EFAULT);
#else
  GTEST_SKIP() << "<sys/random.h> not available";
#endif
#pragma clang diagnostic pop
}

TEST(sys_random, getrandom_EINVAL) {
#if defined(HAVE_SYS_RANDOM)
  errno = 0;
  char buf[64];
  ASSERT_EQ(-1, getrandom(buf, sizeof(buf), ~0));
  ASSERT_ERRNO(EINVAL);
#else
  GTEST_SKIP() << "<sys/random.h> not available";
#endif
}
