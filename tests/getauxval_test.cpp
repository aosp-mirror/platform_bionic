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

#include <sys/auxv.h>

#include <errno.h>
#include <sys/cdefs.h>
#include <sys/utsname.h>
#include <gtest/gtest.h>

TEST(getauxval, expected_values) {
  ASSERT_EQ(0UL, getauxval(AT_SECURE));
  ASSERT_EQ(getuid(), getauxval(AT_UID));
  ASSERT_EQ(geteuid(), getauxval(AT_EUID));
  ASSERT_EQ(getgid(), getauxval(AT_GID));
  ASSERT_EQ(getegid(), getauxval(AT_EGID));
  ASSERT_EQ(static_cast<unsigned long>(getpagesize()), getauxval(AT_PAGESZ));

  ASSERT_NE(0UL, getauxval(AT_PHDR));
  ASSERT_NE(0UL, getauxval(AT_PHNUM));
  ASSERT_NE(0UL, getauxval(AT_ENTRY));
  ASSERT_NE(0UL, getauxval(AT_PAGESZ));
}

TEST(getauxval, unexpected_values) {
  errno = 0;
  ASSERT_EQ(0UL, getauxval(0xdeadbeef));
  ASSERT_EQ(ENOENT, errno);
}

TEST(getauxval, arm_has_AT_HWCAP2) {
#if defined(__arm__)
  // There are no known 32-bit processors that implement any of these instructions, so rather
  // than require that OEMs backport kernel patches, let's just ignore old hardware. Strictly
  // speaking this would be fooled by someone choosing to ship a 32-bit kernel on 64-bit hardware,
  // but that doesn't seem very likely in 2016.
  utsname u;
  ASSERT_EQ(0, uname(&u));
  if (strcmp(u.machine, "aarch64") == 0) {
    // If this test fails, apps that use getauxval to decide at runtime whether crypto hardware is
    // available will incorrectly assume that it isn't, and will have really bad performance.
    // If this test fails, ensure that you've enabled COMPAT_BINFMT_ELF in your kernel configuration.
    // Note that 0 ("I don't support any of these things") is a legitimate response --- we need
    // to check errno to see whether we got a "true" 0 or a "not found" 0.
    errno = 0;
    getauxval(AT_HWCAP2);
    ASSERT_EQ(0, errno) << "64-bit kernel not reporting AT_HWCAP2 to 32-bit ARM process";
    return;
  }
#endif
  GTEST_SKIP() << "This test is only meaningful for 32-bit ARM code on 64-bit devices";
}
