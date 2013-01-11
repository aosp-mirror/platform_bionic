/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <sys/cdefs.h>
#include <features.h>
#include <gtest/gtest.h>

// getauxval() was only added as of glibc version 2.16.
// See: http://lwn.net/Articles/519085/
// Don't try to compile this code on older glibc versions.

#if defined(__BIONIC__)
  #define GETAUXVAL_CAN_COMPILE 1
#elif defined(__GLIBC_PREREQ)
  #if __GLIBC_PREREQ(2, 16)
    #define GETAUXVAL_CAN_COMPILE 1
  #endif
#endif

#if defined(GETAUXVAL_CAN_COMPILE)

#include <sys/auxv.h>

TEST(getauxval, expected_values) {
  ASSERT_EQ((unsigned long int) 0, getauxval(AT_SECURE));
  ASSERT_EQ(getuid(), getauxval(AT_UID));
  ASSERT_EQ(geteuid(), getauxval(AT_EUID));
  ASSERT_EQ(getgid(), getauxval(AT_GID));
  ASSERT_EQ(getegid(), getauxval(AT_EGID));
  ASSERT_EQ((unsigned long int) getpagesize(), getauxval(AT_PAGESZ));

  ASSERT_NE((unsigned long int) 0, getauxval(AT_PHDR));
  ASSERT_NE((unsigned long int) 0, getauxval(AT_PHNUM));
  ASSERT_NE((unsigned long int) 0, getauxval(AT_ENTRY));
  ASSERT_NE((unsigned long int) 0, getauxval(AT_PAGESZ));
}

TEST(getauxval, unexpected_values) {
  ASSERT_EQ((unsigned long int) 0, getauxval(0xdeadbeef));
}

#endif /* GETAUXVAL_CAN_COMPILE */
