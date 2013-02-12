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

#include <sys/types.h>
#include <sys/cdefs.h>
#include <pwd.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#if __BIONIC__

#define CHECK_GETPWNAM_FOR(username, uid, uid_type) \
    SCOPED_TRACE(username); \
    ASSERT_NO_FATAL_FAILURE(check_getpwnam(username, uid, uid_type));

typedef enum {
  TYPE_SYSTEM,
  TYPE_APP
} uid_type_t;

static void check_getpwnam(const char* username, uid_t uid, uid_type_t uid_type) {
  errno = 0;
  passwd* pwd = getpwuid(uid);
  ASSERT_TRUE(pwd != NULL);
  ASSERT_EQ(0, errno);
  EXPECT_STREQ(username, pwd->pw_name);
  EXPECT_EQ(uid, pwd->pw_uid);
  EXPECT_EQ(uid, pwd->pw_gid);

  if (uid_type == TYPE_SYSTEM) {
    EXPECT_STREQ("/", pwd->pw_dir);
  } else if (uid_type == TYPE_APP) {
    EXPECT_STREQ("/data", pwd->pw_dir);
  }

  EXPECT_STREQ("/system/bin/sh", pwd->pw_shell);
}

TEST(getpwnam, system_id_root) {
  CHECK_GETPWNAM_FOR("root", 0, TYPE_SYSTEM);
}

TEST(getpwnam, system_id_system) {
  CHECK_GETPWNAM_FOR("system", 1000, TYPE_SYSTEM);
}

TEST(getpwnam, app_id_radio) {
  CHECK_GETPWNAM_FOR("radio", 1001, TYPE_SYSTEM);
}

TEST(getpwnam, app_id_nobody) {
  CHECK_GETPWNAM_FOR("nobody", 9999, TYPE_SYSTEM);
}

TEST(getpwnam, app_id_all_a0) {
  CHECK_GETPWNAM_FOR("all_a0", 50000, TYPE_APP);
}

TEST(getpwnam, app_id_u1_a40000) {
  CHECK_GETPWNAM_FOR("u1_a40000", 150000, TYPE_APP);
}

TEST(getpwnam, app_id_u0_a0) {
  CHECK_GETPWNAM_FOR("u0_a0", 10000, TYPE_APP);
}

TEST(getpwnam, app_id_u0_a1234) {
  CHECK_GETPWNAM_FOR("u0_a1234", 11234, TYPE_APP);
}

TEST(getpwnam, app_id_u0_a9999) {
  CHECK_GETPWNAM_FOR("u0_a9999", 19999, TYPE_APP);
}

// nonsensical, but expected
TEST(getpwnam, app_id_u1_root) {
  CHECK_GETPWNAM_FOR("u1_root", 100000, TYPE_SYSTEM);
}

TEST(getpwnam, app_id_u1_radio) {
  CHECK_GETPWNAM_FOR("u1_radio", 101001, TYPE_SYSTEM);
}

TEST(getpwnam, app_id_u1_a0) {
  CHECK_GETPWNAM_FOR("u1_a0", 110000, TYPE_APP);
}

TEST(getpwnam, app_id_u1_i0) {
  CHECK_GETPWNAM_FOR("u1_i0", 199000, TYPE_APP);
}

#endif /* __BIONIC__ */
