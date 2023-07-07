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

#include <gtest/gtest.h>

#include <errno.h>

#if defined(__BIONIC__)
#include <async_safe/log.h>
#endif // __BIONIC__

TEST(async_safe_log, smoke) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];

  async_safe_format_buffer(buf, sizeof(buf), "a");
  EXPECT_STREQ("a", buf);

  async_safe_format_buffer(buf, sizeof(buf), "%%");
  EXPECT_STREQ("%", buf);

  async_safe_format_buffer(buf, sizeof(buf), "01234");
  EXPECT_STREQ("01234", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%sb", "01234");
  EXPECT_STREQ("a01234b", buf);

  char* s = nullptr;
  async_safe_format_buffer(buf, sizeof(buf), "a%sb", s);
  EXPECT_STREQ("a(null)b", buf);

  async_safe_format_buffer(buf, sizeof(buf), "aa%scc", "bb");
  EXPECT_STREQ("aabbcc", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%cc", 'b');
  EXPECT_STREQ("abc", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%db", 1234);
  EXPECT_STREQ("a1234b", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%db", -8123);
  EXPECT_STREQ("a-8123b", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%hdb", static_cast<short>(0x7fff0010));
  EXPECT_STREQ("a16b", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%hhdb", static_cast<char>(0x7fffff10));
  EXPECT_STREQ("a16b", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%lldb", 0x1000000000LL);
  EXPECT_STREQ("a68719476736b", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%ldb", 70000L);
  EXPECT_STREQ("a70000b", buf);

  errno = EINVAL;
  async_safe_format_buffer(buf, sizeof(buf), "a%mZ");
  EXPECT_STREQ("aInvalid argumentZ", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%pb", reinterpret_cast<void*>(0xb0001234));
  EXPECT_STREQ("a0xb0001234b", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%xz", 0x12ab);
  EXPECT_STREQ("a12abz", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%Xz", 0x12ab);
  EXPECT_STREQ("a12ABz", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%08xz", 0x123456);
  EXPECT_STREQ("a00123456z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%5dz", 1234);
  EXPECT_STREQ("a 1234z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%05dz", 1234);
  EXPECT_STREQ("a01234z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%8dz", 1234);
  EXPECT_STREQ("a    1234z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%-8dz", 1234);
  EXPECT_STREQ("a1234    z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "A%-11sZ", "abcdef");
  EXPECT_STREQ("Aabcdef     Z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "A%s:%dZ", "hello", 1234);
  EXPECT_STREQ("Ahello:1234Z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%03d:%d:%02dz", 5, 5, 5);
  EXPECT_STREQ("a005:5:05z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%#xZ", 34);
  EXPECT_STREQ("a0x22Z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%#xZ", 0);
  EXPECT_STREQ("a0Z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%#5xZ", 20);
  EXPECT_STREQ("a 0x14Z", buf);

  snprintf(buf, sizeof(buf), "a%#08.8xZ", 1);
  EXPECT_STREQ("a0x00000001Z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%#oZ", 777);
  EXPECT_STREQ("a01411Z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%#oZ", 0);
  EXPECT_STREQ("a0Z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%#6oZ", 15);
  EXPECT_STREQ("a   017Z", buf);

  snprintf(buf, sizeof(buf), "a%#08.8oZ", 11);
  EXPECT_STREQ("a00000013Z", buf);

  void* p = nullptr;
  async_safe_format_buffer(buf, sizeof(buf), "a%d,%pz", 5, p);
  EXPECT_STREQ("a5,0x0z", buf);

  async_safe_format_buffer(buf, sizeof(buf), "a%lld,%d,%d,%dz", 0x1000000000LL, 6, 7, 8);
  EXPECT_STREQ("a68719476736,6,7,8z", buf);
#else // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(async_safe_log, d_INT_MAX) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  async_safe_format_buffer(buf, sizeof(buf), "%d", INT_MAX);
  EXPECT_STREQ("2147483647", buf);
#else // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(async_safe_log, d_INT_MIN) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  async_safe_format_buffer(buf, sizeof(buf), "%d", INT_MIN);
  EXPECT_STREQ("-2147483648", buf);
#else // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(async_safe_log, ld_LONG_MAX) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  async_safe_format_buffer(buf, sizeof(buf), "%ld", LONG_MAX);
#if defined(__LP64__)
  EXPECT_STREQ("9223372036854775807", buf);
#else
  EXPECT_STREQ("2147483647", buf);
#endif
#else // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(async_safe_log, ld_LONG_MIN) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  async_safe_format_buffer(buf, sizeof(buf), "%ld", LONG_MIN);
#if defined(__LP64__)
  EXPECT_STREQ("-9223372036854775808", buf);
#else
  EXPECT_STREQ("-2147483648", buf);
#endif
#else // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(async_safe_log, lld_LLONG_MAX) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  async_safe_format_buffer(buf, sizeof(buf), "%lld", LLONG_MAX);
  EXPECT_STREQ("9223372036854775807", buf);
#else // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(async_safe_log, lld_LLONG_MIN) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  async_safe_format_buffer(buf, sizeof(buf), "%lld", LLONG_MIN);
  EXPECT_STREQ("-9223372036854775808", buf);
#else // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

TEST(async_safe_log, buffer_overrun) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  ASSERT_EQ(11, async_safe_format_buffer(buf, sizeof(buf), "hello %s", "world"));
  EXPECT_STREQ("hello world", buf);

  ASSERT_EQ(11, async_safe_format_buffer(buf, 8, "hello %s", "world"));
  EXPECT_STREQ("hello w", buf);

  ASSERT_EQ(11, async_safe_format_buffer(buf, 6, "hello %s", "world"));
  EXPECT_STREQ("hello", buf);

  ASSERT_EQ(4, async_safe_format_buffer(nullptr, 0, "xxxx"));

  ASSERT_EQ(4, async_safe_format_buffer(buf, 1, "xxxx"));
  EXPECT_STREQ("", buf);

  ASSERT_EQ(4, async_safe_format_buffer(buf, 2, "xxxx"));
  EXPECT_STREQ("x", buf);
#else // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif // __BIONIC__
}

// Verify that using %m is never cut off.
TEST(async_safe_format_buffer, percent_m_fits_in_buffer) {
#if defined(__BIONIC__)
  for (int i = 0; i < 256; i++) {
    errno = i;
    char async_buf[256];
    async_safe_format_buffer(async_buf, sizeof(async_buf), "%m");
    char strerror_buf[1024];
    strerror_r(errno, strerror_buf, sizeof(strerror_buf));
    ASSERT_STREQ(strerror_buf, async_buf);
  }
#else   // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif  // __BIONIC__
}
