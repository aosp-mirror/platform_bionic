/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include "BionicDeathTest.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>

#if defined(__BIONIC__)
#include <android/fdsan.h>
#endif

#include <unordered_map>

#include <android-base/unique_fd.h>

#define FDSAN_TEST(test_name) TEST_F(FdsanTest, test_name)
#define EXPECT_FDSAN_DEATH(expression, regex)                                                \
  EXPECT_DEATH((android_fdsan_set_error_level(ANDROID_FDSAN_ERROR_LEVEL_FATAL), expression), \
               (regex))

struct FdsanTest : public ::testing::Test {
  void SetUp() override {
#if defined(__BIONIC__)
    // The bionic unit test running forks for each test by default, which turns
    // fdsan off as a side-effect, so we need to turn it back on.
    android_fdsan_set_error_level(ANDROID_FDSAN_ERROR_LEVEL_FATAL);
#endif
  }
};

TEST_F(FdsanTest, unowned_untagged_close) {
#if defined(__BIONIC__)
  int fd = open("/dev/null", O_RDONLY);
  ASSERT_EQ(0, close(fd));
#endif
}

TEST_F(FdsanTest, unowned_tagged_close) {
#if defined(__BIONIC__)
  int fd = open("/dev/null", O_RDONLY);
  ASSERT_EQ(0, android_fdsan_close_with_tag(fd, 0));
#endif
}

TEST_F(FdsanTest, unowned_improperly_tagged_close) {
#if defined(__BIONIC__)
  int fd = open("/dev/null", O_RDONLY);
  EXPECT_FDSAN_DEATH(android_fdsan_close_with_tag(fd, 0xdeadbeef), "actually unowned");
#endif
}

TEST_F(FdsanTest, unowned_incorrect_exchange) {
#if defined(__BIONIC__)
  int fd = open("/dev/null", O_RDONLY);
  EXPECT_FDSAN_DEATH(android_fdsan_exchange_owner_tag(fd, 0xbadc0de, 0xdeadbeef),
                     "failed to exchange ownership");
#endif
}

TEST_F(FdsanTest, owned_untagged_close) {
#if defined(__BIONIC__)
  int fd = open("/dev/null", O_RDONLY);
  android_fdsan_exchange_owner_tag(fd, 0, 0xdeadbeef);
  EXPECT_FDSAN_DEATH(close(fd), "expected to be unowned, actually owned");
#endif
}

TEST_F(FdsanTest, owned_tagged_close) {
#if defined(__BIONIC__)
  int fd = open("/dev/null", O_RDONLY);
  android_fdsan_exchange_owner_tag(fd, 0, 0xdeadbeef);
  ASSERT_EQ(0, android_fdsan_close_with_tag(fd, 0xdeadbeef));
#endif
}

TEST_F(FdsanTest, owned_improperly_tagged_close) {
#if defined(__BIONIC__)
  int fd = open("/dev/null", O_RDONLY);
  android_fdsan_exchange_owner_tag(fd, 0, 0xdeadbeef);
  EXPECT_FDSAN_DEATH(android_fdsan_close_with_tag(fd, 0xdeadc0de), "expected to be owned");
#endif
}

TEST_F(FdsanTest, owned_incorrect_exchange) {
#if defined(__BIONIC__)
  int fd = open("/dev/null", O_RDONLY);
  android_fdsan_exchange_owner_tag(fd, 0, 0xdeadbeef);
  EXPECT_FDSAN_DEATH(android_fdsan_exchange_owner_tag(fd, 0xbadc0de, 0xdeadbeef),
                     "failed to exchange");
#endif
}

TEST_F(FdsanTest, fopen) {
#if defined(__BIONIC__)
  FILE* f = fopen("/dev/null", "r");
  ASSERT_TRUE(f);
  EXPECT_FDSAN_DEATH(close(fileno(f)), "actually owned by FILE");
#endif
}

TEST_F(FdsanTest, closedir) {
#if defined(__BIONIC__)
  DIR* dir = opendir("/dev/");
  ASSERT_TRUE(dir);
  EXPECT_FDSAN_DEATH(close(dirfd(dir)), "actually owned by DIR");
#endif
}

TEST_F(FdsanTest, overflow) {
#if defined(__BIONIC__)
  std::unordered_map<int, uint64_t> fds;
  for (int i = 0; i < 4096; ++i) {
    int fd = open("/dev/null", O_RDONLY);
    auto tag = 0xdead00000000ULL | i;
    android_fdsan_exchange_owner_tag(fd, 0, tag);
    fds[fd] = tag;
  }

  for (auto [fd, tag] : fds) {
    android_fdsan_close_with_tag(fd, tag);
  }
#endif
}

TEST_F(FdsanTest, owner_value_high) {
#if defined(__BIONIC__)
  int fd = open("/dev/null", O_RDONLY);
  uint64_t tag = android_fdsan_create_owner_tag(ANDROID_FDSAN_OWNER_TYPE_UNIQUE_FD, ~0ULL);
  android_fdsan_exchange_owner_tag(fd, 0, tag);
  EXPECT_FDSAN_DEATH(android_fdsan_exchange_owner_tag(fd, 0xbadc0de, 0xdeadbeef),
                     "0xffffffffffffffff");
#endif
}

TEST_F(FdsanTest, owner_value_low) {
#if defined(__BIONIC__)
  int fd = open("/dev/null", O_RDONLY);
  uint64_t tag = android_fdsan_create_owner_tag(ANDROID_FDSAN_OWNER_TYPE_UNIQUE_FD, 1);
  android_fdsan_exchange_owner_tag(fd, 0, tag);
  EXPECT_FDSAN_DEATH(android_fdsan_exchange_owner_tag(fd, 0xbadc0de, 0xdeadbeef),
                     "0x1");
#endif
}

TEST_F(FdsanTest, unique_fd_unowned_close) {
#if defined(__BIONIC__)
  android::base::unique_fd fd(open("/dev/null", O_RDONLY));
  android_fdsan_set_error_level(ANDROID_FDSAN_ERROR_LEVEL_FATAL);
  EXPECT_FDSAN_DEATH(close(fd.get()), "expected to be unowned, actually owned by unique_fd");
#endif
}

TEST_F(FdsanTest, unique_fd_untag_on_release) {
  android::base::unique_fd fd(open("/dev/null", O_RDONLY));
  close(fd.release());
}

TEST_F(FdsanTest, unique_fd_move) {
  android::base::unique_fd fd(open("/dev/null", O_RDONLY));
  android::base::unique_fd fd_moved = std::move(fd);
  ASSERT_EQ(-1, fd.get());
  ASSERT_GT(fd_moved.get(), -1);
}

TEST_F(FdsanTest, unique_fd_unowned_close_after_move) {
#if defined(__BIONIC__)
  android::base::unique_fd fd(open("/dev/null", O_RDONLY));
  android::base::unique_fd fd_moved = std::move(fd);
  ASSERT_EQ(-1, fd.get());
  ASSERT_GT(fd_moved.get(), -1);

  android_fdsan_set_error_level(ANDROID_FDSAN_ERROR_LEVEL_FATAL);
  EXPECT_FDSAN_DEATH(close(fd_moved.get()), "expected to be unowned, actually owned by unique_fd");
#endif
}
