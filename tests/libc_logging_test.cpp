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

#if defined(__BIONIC__)
#include "../libc/bionic/libc_logging.cpp"
extern int __libc_format_buffer(char* buffer, size_t buffer_size, const char* format, ...);
#endif // __BIONIC__

TEST(libc_logging, smoke) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];

  __libc_format_buffer(buf, sizeof(buf), "a");
  EXPECT_STREQ("a", buf);

  __libc_format_buffer(buf, sizeof(buf), "%%");
  EXPECT_STREQ("%", buf);

  __libc_format_buffer(buf, sizeof(buf), "01234");
  EXPECT_STREQ("01234", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%sb", "01234");
  EXPECT_STREQ("a01234b", buf);

  char* s = NULL;
  __libc_format_buffer(buf, sizeof(buf), "a%sb", s);
  EXPECT_STREQ("a(null)b", buf);

  __libc_format_buffer(buf, sizeof(buf), "aa%scc", "bb");
  EXPECT_STREQ("aabbcc", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%cc", 'b');
  EXPECT_STREQ("abc", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%db", 1234);
  EXPECT_STREQ("a1234b", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%db", -8123);
  EXPECT_STREQ("a-8123b", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%hdb", static_cast<short>(0x7fff0010));
  EXPECT_STREQ("a16b", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%hhdb", static_cast<char>(0x7fffff10));
  EXPECT_STREQ("a16b", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%lldb", 0x1000000000LL);
  EXPECT_STREQ("a68719476736b", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%ldb", 70000L);
  EXPECT_STREQ("a70000b", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%pb", reinterpret_cast<void*>(0xb0001234));
  EXPECT_STREQ("a0xb0001234b", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%xz", 0x12ab);
  EXPECT_STREQ("a12abz", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%Xz", 0x12ab);
  EXPECT_STREQ("a12ABz", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%08xz", 0x123456);
  EXPECT_STREQ("a00123456z", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%5dz", 1234);
  EXPECT_STREQ("a 1234z", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%05dz", 1234);
  EXPECT_STREQ("a01234z", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%8dz", 1234);
  EXPECT_STREQ("a    1234z", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%-8dz", 1234);
  EXPECT_STREQ("a1234    z", buf);

  __libc_format_buffer(buf, sizeof(buf), "A%-11sZ", "abcdef");
  EXPECT_STREQ("Aabcdef     Z", buf);

  __libc_format_buffer(buf, sizeof(buf), "A%s:%dZ", "hello", 1234);
  EXPECT_STREQ("Ahello:1234Z", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%03d:%d:%02dz", 5, 5, 5);
  EXPECT_STREQ("a005:5:05z", buf);

  void* p = NULL;
  __libc_format_buffer(buf, sizeof(buf), "a%d,%pz", 5, p);
  EXPECT_STREQ("a5,0x0z", buf);

  __libc_format_buffer(buf, sizeof(buf), "a%lld,%d,%d,%dz", 0x1000000000LL, 6, 7, 8);
  EXPECT_STREQ("a68719476736,6,7,8z", buf);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(libc_logging, d_INT_MAX) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  __libc_format_buffer(buf, sizeof(buf), "%d", INT_MAX);
  EXPECT_STREQ("2147483647", buf);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(libc_logging, d_INT_MIN) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  __libc_format_buffer(buf, sizeof(buf), "%d", INT_MIN);
  EXPECT_STREQ("-2147483648", buf);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(libc_logging, ld_LONG_MAX) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  __libc_format_buffer(buf, sizeof(buf), "%ld", LONG_MAX);
#if defined(__LP64__)
  EXPECT_STREQ("9223372036854775807", buf);
#else
  EXPECT_STREQ("2147483647", buf);
#endif
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(libc_logging, ld_LONG_MIN) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  __libc_format_buffer(buf, sizeof(buf), "%ld", LONG_MIN);
#if defined(__LP64__)
  EXPECT_STREQ("-9223372036854775808", buf);
#else
  EXPECT_STREQ("-2147483648", buf);
#endif
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(libc_logging, lld_LLONG_MAX) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  __libc_format_buffer(buf, sizeof(buf), "%lld", LLONG_MAX);
  EXPECT_STREQ("9223372036854775807", buf);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(libc_logging, lld_LLONG_MIN) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  __libc_format_buffer(buf, sizeof(buf), "%lld", LLONG_MIN);
  EXPECT_STREQ("-9223372036854775808", buf);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(libc_logging, buffer_overrun) {
#if defined(__BIONIC__)
  char buf[BUFSIZ];
  ASSERT_EQ(11, __libc_format_buffer(buf, sizeof(buf), "hello %s", "world"));
  EXPECT_STREQ("hello world", buf);
  ASSERT_EQ(11, __libc_format_buffer(buf, 8, "hello %s", "world"));
  EXPECT_STREQ("hello w", buf);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
