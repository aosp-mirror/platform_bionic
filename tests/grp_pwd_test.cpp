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

// Below are the header files we want to test.
#include <grp.h>
#include <pwd.h>

#include <errno.h>
#include <limits.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <vector>

#include <android-base/file.h>
#include <android-base/strings.h>
#include <private/android_filesystem_config.h>

#if defined(__BIONIC__)
#include <android/api-level.h>
#include <android-base/properties.h>
#endif

// Generated android_ids array
#include "generated_android_ids.h"

#include "utils.h"

using android::base::Join;
using android::base::ReadFileToString;
using android::base::Split;
using android::base::StartsWith;

using namespace std::literals;

enum uid_type_t {
  TYPE_APP,
  TYPE_SYSTEM,
  TYPE_VENDOR,
};

#if defined(__BIONIC__)

static void check_passwd(const passwd* pwd, const char* username, uid_t uid, uid_type_t uid_type,
                         bool check_username) {
  ASSERT_TRUE(pwd != nullptr);
  if (check_username) {
    EXPECT_STREQ(username, pwd->pw_name);
  }
  EXPECT_EQ(uid, pwd->pw_uid);
  EXPECT_EQ(uid, pwd->pw_gid);
  EXPECT_EQ(nullptr, pwd->pw_passwd);
#ifdef __LP64__
  EXPECT_EQ(nullptr, pwd->pw_gecos);
#endif

  if (uid_type == TYPE_APP) {
    EXPECT_STREQ("/data", pwd->pw_dir);
  } else {
    EXPECT_STREQ("/", pwd->pw_dir);
  }

  // This has changed over time and that causes new GSI + old vendor images testing to fail.
  // This parameter doesn't matter on Android, so simply ignore its value for older vendor images.
  if (android::base::GetIntProperty("ro.product.first_api_level", 0) >= 30) {
    EXPECT_STREQ("/bin/sh", pwd->pw_shell);
  }
}

static void check_getpwuid(const char* username, uid_t uid, uid_type_t uid_type,
                           bool check_username) {
  errno = 0;
  passwd* pwd = getpwuid(uid);
  ASSERT_ERRNO(0);
  SCOPED_TRACE("getpwuid");
  check_passwd(pwd, username, uid, uid_type, check_username);
}

static void check_getpwnam(const char* username, uid_t uid, uid_type_t uid_type,
                           bool check_username) {
  errno = 0;
  passwd* pwd = getpwnam(username);
  ASSERT_ERRNO(0);
  SCOPED_TRACE("getpwnam");
  check_passwd(pwd, username, uid, uid_type, check_username);
}

static void check_getpwuid_r(const char* username, uid_t uid, uid_type_t uid_type,
                             bool check_username) {
  passwd pwd_storage;
  char buf[512];
  int result;

  errno = 0;
  passwd* pwd = nullptr;
  result = getpwuid_r(uid, &pwd_storage, buf, sizeof(buf), &pwd);
  ASSERT_EQ(0, result);
  ASSERT_ERRNO(0);
  SCOPED_TRACE("getpwuid_r");
  check_passwd(pwd, username, uid, uid_type, check_username);
}

static void check_getpwnam_r(const char* username, uid_t uid, uid_type_t uid_type,
                             bool check_username) {
  passwd pwd_storage;
  char buf[512];
  int result;

  errno = 0;
  passwd* pwd = nullptr;
  result = getpwnam_r(username, &pwd_storage, buf, sizeof(buf), &pwd);
  ASSERT_EQ(0, result);
  ASSERT_ERRNO(0);
  SCOPED_TRACE("getpwnam_r");
  check_passwd(pwd, username, uid, uid_type, check_username);
}

static void check_get_passwd(const char* username, uid_t uid, uid_type_t uid_type,
                             bool check_username = true) {
  SCOPED_TRACE("username '"s + username + "'");
  check_getpwuid(username, uid, uid_type, check_username);
  check_getpwnam(username, uid, uid_type, check_username);
  check_getpwuid_r(username, uid, uid_type, check_username);
  check_getpwnam_r(username, uid, uid_type, check_username);
}

static void expect_no_passwd_id(uid_t uid) {
  SCOPED_TRACE("uid '" + std::to_string(uid) + "'");
  errno = 0;
  passwd* passwd = nullptr;
  passwd = getpwuid(uid);
  EXPECT_EQ(nullptr, passwd) << "name = '" << passwd->pw_name << "'";
  EXPECT_ERRNO(ENOENT);

  struct passwd passwd_storage;
  char buf[512];
  EXPECT_EQ(ENOENT, getpwuid_r(uid, &passwd_storage, buf, sizeof(buf), &passwd));
  EXPECT_EQ(nullptr, passwd) << "name = '" << passwd->pw_name << "'";
}

static void expect_no_passwd_name(const char* username) {
  SCOPED_TRACE("username '"s + username + "'");
  errno = 0;
  passwd* passwd = nullptr;
  passwd = getpwnam(username);
  EXPECT_EQ(nullptr, passwd) << "name = '" << passwd->pw_name << "'";
  EXPECT_ERRNO(ENOENT);

  struct passwd passwd_storage;
  char buf[512];
  EXPECT_EQ(ENOENT, getpwnam_r(username, &passwd_storage, buf, sizeof(buf), &passwd));
  EXPECT_EQ(nullptr, passwd) << "name = '" << passwd->pw_name << "'";
}

#else // !defined(__BIONIC__)

static void check_get_passwd(const char* /* username */, uid_t /* uid */, uid_type_t /* uid_type */,
                             bool /* check_username */) {
  GTEST_SKIP() << "bionic-only test";
}

static void check_get_passwd(const char* /* username */, uid_t /* uid */, uid_type_t /* uid_type */) {
  GTEST_SKIP() << "bionic-only test";
}

static void expect_no_passwd_id(uid_t /* uid */) {
  GTEST_SKIP() << "bionic-only test";
}

static void expect_no_passwd_name(const char* /* username */) {
  GTEST_SKIP() << "bionic-only test";
}

#endif

TEST(pwd, getpwnam_platform_ids) {
  check_get_passwd("root", 0, TYPE_SYSTEM);
  check_get_passwd("daemon", 1, TYPE_SYSTEM);
  check_get_passwd("bin", 2, TYPE_SYSTEM);

  check_get_passwd("system", 1000, TYPE_SYSTEM);
  check_get_passwd("radio", 1001, TYPE_SYSTEM);

  check_get_passwd("shell", 2000, TYPE_SYSTEM);

  check_get_passwd("nobody", 9999, TYPE_SYSTEM);
}

TEST(pwd, getpwnam_oem_ids) {
  check_get_passwd("oem_2900", 2900, TYPE_VENDOR, false);
  check_get_passwd("oem_2945", 2945, TYPE_VENDOR, false);
  check_get_passwd("oem_2999", 2999, TYPE_VENDOR, false);
  check_get_passwd("oem_5000", 5000, TYPE_VENDOR, false);
  check_get_passwd("oem_5454", 5454, TYPE_VENDOR, false);
  check_get_passwd("oem_5999", 5999, TYPE_VENDOR, false);
}

TEST(pwd, getpwnam_non_exist) {
  expect_no_passwd_id(999);   // End of the system reserved range, unallocated.
  expect_no_passwd_id(1999);  // End of the system reserved range, unallocated.
  expect_no_passwd_id(2899);  // End of the system reserved range, unallocated.

  // These ranges are for GIDs only.
  expect_no_passwd_id(20000);
  expect_no_passwd_id(30000);
  expect_no_passwd_id(40000);
  expect_no_passwd_id(50000);

  // These should not be parsed as users, only as groups.
  expect_no_passwd_name("u0_a9999_cache");
  expect_no_passwd_name("u0_a9999_ext");
  expect_no_passwd_name("u0_a9999_ext_cache");
  expect_no_passwd_name("all_a9999");
}

TEST(pwd, getpwnam_u0_app_ids) {
  check_get_passwd("u0_a0", 10000, TYPE_APP);
  check_get_passwd("u0_a1234", 11234, TYPE_APP);
  check_get_passwd("u0_a9999", 19999, TYPE_APP);

  check_get_passwd("u0_i1", 90001, TYPE_APP);
  check_get_passwd("u0_i4545", 94545, TYPE_APP);
  check_get_passwd("u0_i9999", 99999, TYPE_APP);
}

TEST(pwd, getpwnam_app_id_u1_ids) {
  check_get_passwd("u1_system", 101000, TYPE_SYSTEM);
  check_get_passwd("u1_radio", 101001, TYPE_SYSTEM);

  check_get_passwd("u1_a0", 110000, TYPE_APP);
  check_get_passwd("u1_a1234", 111234, TYPE_APP);
  check_get_passwd("u1_a9999", 119999, TYPE_APP);

  check_get_passwd("u1_i1", 190001, TYPE_APP);
  check_get_passwd("u1_i4545", 194545, TYPE_APP);
  check_get_passwd("u1_i9999", 199999, TYPE_APP);
}

TEST(pwd, getpwnam_app_id_u31_ids) {
  check_get_passwd("u31_system", 3101000, TYPE_SYSTEM);
  check_get_passwd("u31_radio", 3101001, TYPE_SYSTEM);

  check_get_passwd("u31_a0", 3110000, TYPE_APP);
  check_get_passwd("u31_a1234", 3111234, TYPE_APP);
  check_get_passwd("u31_a9999", 3119999, TYPE_APP);

  check_get_passwd("u31_i1", 3190001, TYPE_APP);
  check_get_passwd("u31_i4545", 3194545, TYPE_APP);
  check_get_passwd("u31_i9999", 3199999, TYPE_APP);
}

TEST(pwd, getpwnam_app_id_not_allowed_platform) {
  expect_no_passwd_name("u1_root");
  expect_no_passwd_name("u1_debuggerd");

  expect_no_passwd_name("u31_root");
  expect_no_passwd_name("u31_debuggerd");
}

TEST(pwd, getpwuid_app_id_u1_non_exist) {
  expect_no_passwd_id(100000);  // There is no 'root' for secondary users.
  expect_no_passwd_id(101999);  // End of the system reserved range, unallocated.
  expect_no_passwd_id(102900);  // The OEM ranges were never allocated to secondary users.
  expect_no_passwd_id(105000);  // The OEM ranges were never allocated to secondary users.

  // These ranges are for GIDs only.
  expect_no_passwd_id(120000);
  expect_no_passwd_id(130000);
  expect_no_passwd_id(140000);
  expect_no_passwd_id(150000);
}

TEST(pwd, getpwuid_app_id_u31_non_exist) {
  expect_no_passwd_id(3100000);  // There is no 'root' for secondary users.
  expect_no_passwd_id(3101999);  // End of the system reserved range, unallocated.
  expect_no_passwd_id(3102900);  // The OEM ranges were never allocated to secondary users.
  expect_no_passwd_id(3105000);  // The OEM ranges were never allocated to secondary users.

  // These ranges are for GIDs only.
  expect_no_passwd_id(3120000);
  expect_no_passwd_id(3130000);
  expect_no_passwd_id(3140000);
  expect_no_passwd_id(3150000);
}

TEST(pwd, getpwnam_r_alignment) {
#if defined(__BIONIC__)
  passwd pwd_storage;
  alignas(16) char buf[512];
  passwd* pwd;
  int result = getpwnam_r("root", &pwd_storage, buf + 1, sizeof(buf) - 1, &pwd);
  ASSERT_EQ(0, result);
  check_passwd(pwd, "root", 0, TYPE_SYSTEM, true);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(pwd, getpwuid_r_alignment) {
#if defined(__BIONIC__)
  passwd pwd_storage;
  alignas(16) char buf[512];
  passwd* pwd;
  int result = getpwuid_r(0, &pwd_storage, buf + 1, sizeof(buf) - 1, &pwd);
  ASSERT_EQ(0, result);
  check_passwd(pwd, "root", 0, TYPE_SYSTEM, true);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(pwd, getpwnam_r_reentrancy) {
#if defined(__BIONIC__)
  passwd pwd_storage[2];
  char buf[2][512];
  passwd* pwd[3];
  int result = getpwnam_r("root", &pwd_storage[0], buf[0], sizeof(buf[0]), &pwd[0]);
  ASSERT_EQ(0, result);
  check_passwd(pwd[0], "root", 0, TYPE_SYSTEM, true);
  pwd[1] = getpwnam("system");
  ASSERT_NE(nullptr, pwd[1]);
  check_passwd(pwd[1], "system", 1000, TYPE_SYSTEM, true);
  result = getpwnam_r("radio", &pwd_storage[1], buf[1], sizeof(buf[1]), &pwd[2]);
  ASSERT_EQ(0, result);
  check_passwd(pwd[2], "radio", 1001, TYPE_SYSTEM, true);
  check_passwd(pwd[0], "root", 0, TYPE_SYSTEM, true);
  check_passwd(pwd[1], "system", 1000, TYPE_SYSTEM, true);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(pwd, getpwuid_r_reentrancy) {
#if defined(__BIONIC__)
  passwd pwd_storage[2];
  char buf[2][512];
  passwd* pwd[3];
  int result = getpwuid_r(0, &pwd_storage[0], buf[0], sizeof(buf[0]), &pwd[0]);
  ASSERT_EQ(0, result);
  check_passwd(pwd[0], "root", 0, TYPE_SYSTEM, true);
  pwd[1] = getpwuid(1000);
  ASSERT_NE(nullptr, pwd[1]);
  check_passwd(pwd[1], "system", 1000, TYPE_SYSTEM, true);
  result = getpwuid_r(1001, &pwd_storage[1], buf[1], sizeof(buf[1]), &pwd[2]);
  ASSERT_EQ(0, result);
  check_passwd(pwd[2], "radio", 1001, TYPE_SYSTEM, true);
  check_passwd(pwd[0], "root", 0, TYPE_SYSTEM, true);
  check_passwd(pwd[1], "system", 1000, TYPE_SYSTEM, true);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(pwd, getpwnam_r_large_enough_suggested_buffer_size) {
#if defined(__BIONIC__)
  long size = sysconf(_SC_GETPW_R_SIZE_MAX);
  ASSERT_GT(size, 0);
  char buf[size];
  passwd pwd_storage;
  passwd* pwd;
  ASSERT_EQ(0, getpwnam_r("root", &pwd_storage, buf, size, &pwd));
  check_passwd(pwd, "root", 0, TYPE_SYSTEM, true);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

#if defined(__BIONIC__)
template <typename T>
static void expect_ids(T ids, bool is_group) {
  std::set<typename T::key_type> expected_ids;
  // Ensure that all android_ids are iterated through.
  for (size_t n = 0; n < android_id_count; ++n) {
    EXPECT_EQ(1U, ids.count(android_ids[n].aid)) << "android_ids[n].aid: " << android_ids[n].aid;
    expected_ids.emplace(android_ids[n].aid);
  }

  auto expect_range = [&ids, &expected_ids](uid_t start, uid_t end) {
    for (size_t n = start; n <= end; ++n) {
      EXPECT_EQ(1U, ids.count(n)) << "n: " << n;
      expected_ids.emplace(n);
    }
  };

  // Ensure that all reserved ranges are iterated through.
  expect_range(AID_OEM_RESERVED_START, AID_OEM_RESERVED_END);
  expect_range(AID_OEM_RESERVED_2_START, AID_OEM_RESERVED_2_END);
  expect_range(AID_APP_START, AID_APP_END);
  if (is_group) {
    expect_range(AID_CACHE_GID_START, AID_CACHE_GID_END);
    expect_range(AID_EXT_GID_START, AID_EXT_GID_END);
    expect_range(AID_EXT_CACHE_GID_START, AID_EXT_CACHE_GID_END);
    expect_range(AID_SHARED_GID_START, AID_SHARED_GID_END);
  }
  expect_range(AID_ISOLATED_START, AID_ISOLATED_END);

  // Prior to R, we didn't have a mechanism to create vendor AIDs in the system or other non-vendor
  // partitions, therefore we disabled the rest of these checks for older API levels.
  if (android::base::GetIntProperty("ro.product.first_api_level", 0) <= 29) {
    return;
  }

  auto allow_range = [&ids](uid_t start, uid_t end) {
    for (size_t n = start; n <= end; ++n) {
      ids.erase(n);
    }
  };

  allow_range(AID_SYSTEM_RESERVED_START, AID_SYSTEM_EXT_RESERVED_END);

  // Ensure that no other ids were returned.
  auto return_differences = [&ids, &expected_ids] {
    std::vector<typename T::key_type> missing_from_ids;
    std::set_difference(expected_ids.begin(), expected_ids.end(), ids.begin(), ids.end(),
                        std::inserter(missing_from_ids, missing_from_ids.begin()));
    std::vector<typename T::key_type> extra_in_ids;
    std::set_difference(ids.begin(), ids.end(), expected_ids.begin(), expected_ids.end(),
                        std::inserter(extra_in_ids, extra_in_ids.begin()));
    std::string result;
    if (!missing_from_ids.empty()) {
      result += "Missing ids from results: " + Join(missing_from_ids, " ");
    }
    if (!extra_in_ids.empty()) {
      if (!result.empty()) result += ", ";
      result += "Extra ids in results: " + Join(extra_in_ids, " ");
    }
    return result;
  };

  // AID_PRNG_SEEDER (1092) was added in TM-QPR2, but CTS is shared
  // across Android 13 versions so we may or may not find it in this
  // test (b/253185870).
  if (android::base::GetIntProperty("ro.build.version.sdk", 0) == __ANDROID_API_T__) {
#ifndef AID_PRNG_SEEDER
#define AID_PRNG_SEEDER 1092
#endif
    ids.erase(AID_PRNG_SEEDER);
    expected_ids.erase(AID_PRNG_SEEDER);
  }
  EXPECT_EQ(expected_ids, ids) << return_differences();
}
#endif

TEST(pwd, getpwent_iterate) {
#if defined(__BIONIC__)
  passwd* pwd;
  std::set<uid_t> uids;

  setpwent();
  while ((pwd = getpwent()) != nullptr) {
    ASSERT_TRUE(nullptr != pwd->pw_name);

    EXPECT_EQ(pwd->pw_gid, pwd->pw_uid) << "pwd->pw_uid: " << pwd->pw_uid;
    EXPECT_EQ(nullptr, pwd->pw_passwd) << "pwd->pw_uid: " << pwd->pw_uid;
#ifdef __LP64__
    EXPECT_TRUE(nullptr == pwd->pw_gecos) << "pwd->pw_uid: " << pwd->pw_uid;
#endif
    EXPECT_TRUE(nullptr != pwd->pw_shell);
    if (pwd->pw_uid < AID_APP_START || pwd->pw_uid == AID_OVERFLOWUID) {
      EXPECT_STREQ("/", pwd->pw_dir) << "pwd->pw_uid: " << pwd->pw_uid;
    } else {
      EXPECT_STREQ("/data", pwd->pw_dir) << "pwd->pw_uid: " << pwd->pw_uid;
    }

    EXPECT_EQ(0U, uids.count(pwd->pw_uid)) << "pwd->pw_uid: " << pwd->pw_uid;
    uids.emplace(pwd->pw_uid);
  }
  endpwent();

  expect_ids(uids, false);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

static void check_group(const group* grp, const char* group_name, gid_t gid,
                        bool check_groupname = true) {
  ASSERT_TRUE(grp != nullptr);
  if (check_groupname) {
    EXPECT_STREQ(group_name, grp->gr_name);
  }
  EXPECT_EQ(gid, grp->gr_gid);
  ASSERT_TRUE(grp->gr_mem != nullptr);
  if (check_groupname) {
    EXPECT_STREQ(group_name, grp->gr_mem[0]);
  }
  EXPECT_TRUE(grp->gr_mem[1] == nullptr);
}

#if defined(__BIONIC__)

static void check_getgrgid(const char* group_name, gid_t gid, bool check_groupname) {
  errno = 0;
  group* grp = getgrgid(gid);
  ASSERT_ERRNO(0);
  SCOPED_TRACE("getgrgid");
  check_group(grp, group_name, gid, check_groupname);
}

static void check_getgrnam(const char* group_name, gid_t gid, bool check_groupname) {
  errno = 0;
  group* grp = getgrnam(group_name);
  ASSERT_ERRNO(0);
  SCOPED_TRACE("getgrnam");
  check_group(grp, group_name, gid, check_groupname);
}

static void check_getgrgid_r(const char* group_name, gid_t gid, bool check_groupname) {
  group grp_storage;
  char buf[512];
  group* grp;

  errno = 0;
  int result = getgrgid_r(gid, &grp_storage, buf, sizeof(buf), &grp);
  ASSERT_EQ(0, result);
  ASSERT_ERRNO(0);
  SCOPED_TRACE("getgrgid_r");
  check_group(grp, group_name, gid, check_groupname);
}

static void check_getgrnam_r(const char* group_name, gid_t gid, bool check_groupname) {
  group grp_storage;
  char buf[512];
  group* grp;

  errno = 0;
  int result = getgrnam_r(group_name, &grp_storage, buf, sizeof(buf), &grp);
  ASSERT_EQ(0, result);
  ASSERT_ERRNO(0);
  SCOPED_TRACE("getgrnam_r");
  check_group(grp, group_name, gid, check_groupname);
}

static void check_get_group(const char* group_name, gid_t gid, bool check_groupname = true) {
  SCOPED_TRACE("groupname '"s + group_name + "'");
  check_getgrgid(group_name, gid, check_groupname);
  check_getgrnam(group_name, gid, check_groupname);
  check_getgrgid_r(group_name, gid, check_groupname);
  check_getgrnam_r(group_name, gid, check_groupname);
}

static void expect_no_group_id(gid_t gid) {
  SCOPED_TRACE("gid '" + std::to_string(gid) + "'");
  errno = 0;
  group* group = nullptr;
  group = getgrgid(gid);
  EXPECT_EQ(nullptr, group) << "name = '" << group->gr_name << "'";
  EXPECT_ERRNO(ENOENT);

  struct group group_storage;
  char buf[512];
  EXPECT_EQ(ENOENT, getgrgid_r(gid, &group_storage, buf, sizeof(buf), &group));
  EXPECT_EQ(nullptr, group) << "name = '" << group->gr_name << "'";
}

static void expect_no_group_name(const char* groupname) {
  SCOPED_TRACE("groupname '"s + groupname + "'");
  errno = 0;
  group* group = nullptr;
  group = getgrnam(groupname);
  EXPECT_EQ(nullptr, group) << "name = '" << group->gr_name << "'";
  EXPECT_ERRNO(ENOENT);

  struct group group_storage;
  char buf[512];
  EXPECT_EQ(ENOENT, getgrnam_r(groupname, &group_storage, buf, sizeof(buf), &group));
  EXPECT_EQ(nullptr, group) << "name = '" << group->gr_name << "'";
}

#else // !defined(__BIONIC__)

static void check_get_group(const char*, gid_t, bool) {
  GTEST_SKIP() << "bionic-only test";
}

static void check_get_group(const char*, gid_t) {
  GTEST_SKIP() << "bionic-only test";
}

static void expect_no_group_id(gid_t /* gid */) {
  GTEST_SKIP() << "bionic-only test";
}

static void expect_no_group_name(const char* /* groupname */) {
  GTEST_SKIP() << "bionic-only test";
}

#endif

TEST(grp, getgrnam_platform_ids) {
  check_get_group("root", 0);
  check_get_group("daemon", 1);
  check_get_group("bin", 2);

  check_get_group("system", 1000);
  check_get_group("radio", 1001);

  check_get_group("shell", 2000);

  check_get_group("nobody", 9999);
}

TEST(grp, getgrnam_oem_ids) {
  check_get_group("oem_2900", 2900, false);
  check_get_group("oem_2945", 2945, false);
  check_get_group("oem_2999", 2999, false);
  check_get_group("oem_5000", 5000, false);
  check_get_group("oem_5454", 5454, false);
  check_get_group("oem_5999", 5999, false);
}

TEST(grp, getgrnam_non_exist) {
  expect_no_passwd_id(999);   // End of the system reserved range, unallocated.
  expect_no_passwd_id(1999);  // End of the system reserved range, unallocated.
  expect_no_passwd_id(2899);  // End of the system reserved range, unallocated.
}

TEST(grp, getgrnam_u0_app_ids) {
  check_get_group("u0_a0", 10000);
  check_get_group("u0_a1234", 11234);
  check_get_group("u0_a9999", 19999);

  check_get_group("u0_a0_cache", 20000);
  check_get_group("u0_a1234_cache", 21234);
  check_get_group("u0_a9999_cache", 29999);

  check_get_group("u0_a0_ext", 30000);
  check_get_group("u0_a4545_ext", 34545);
  check_get_group("u0_a9999_ext", 39999);

  check_get_group("u0_a0_ext_cache", 40000);
  check_get_group("u0_a4545_ext_cache", 44545);
  check_get_group("u0_a9999_ext_cache", 49999);

  check_get_group("all_a0", 50000);
  check_get_group("all_a4545", 54545);
  check_get_group("all_a9999", 59999);

  check_get_group("u0_i1", 90001);
}

TEST(grp, getgrnam_u1_app_ids) {
  check_get_group("u1_system", 101000);
  check_get_group("u1_radio", 101001);

  check_get_group("u1_a0", 110000);
  check_get_group("u1_a1234", 111234);
  check_get_group("u1_a9999", 119999);

  check_get_group("u1_a0_cache", 120000);
  check_get_group("u1_a1234_cache", 121234);
  check_get_group("u1_a9999_cache", 129999);

  check_get_group("u1_a0_ext", 130000);
  check_get_group("u1_a4545_ext", 134545);
  check_get_group("u1_a9999_ext", 139999);

  check_get_group("u1_a0_ext_cache", 140000);
  check_get_group("u1_a4545_ext_cache", 144545);
  check_get_group("u1_a9999_ext_cache", 149999);

  check_get_group("u1_i1", 190001);
}

TEST(grp, getgrnam_u31_app_ids) {
  check_get_group("u31_system", 3101000);
  check_get_group("u31_radio", 3101001);

  check_get_group("u31_a0", 3110000);
  check_get_group("u31_a1234", 3111234);
  check_get_group("u31_a9999", 3119999);

  check_get_group("u31_a0_cache", 3120000);
  check_get_group("u31_a1234_cache", 3121234);
  check_get_group("u31_a9999_cache", 3129999);

  check_get_group("u31_a0_cache", 3120000);
  check_get_group("u31_a1234_cache", 3121234);
  check_get_group("u31_a9999_cache", 3129999);

  check_get_group("u31_a0_ext", 3130000);
  check_get_group("u31_a4545_ext", 3134545);
  check_get_group("u31_a9999_ext", 3139999);

  check_get_group("u31_a0_ext_cache", 3140000);
  check_get_group("u31_a4545_ext_cache", 3144545);
  check_get_group("u31_a9999_ext_cache", 3149999);

  check_get_group("u31_i1", 3190001);
}

TEST(grp, getpgram_app_id_not_allowed_platform) {
  expect_no_group_name("u1_root");
  expect_no_group_name("u1_debuggerd");

  expect_no_group_name("u31_root");
  expect_no_group_name("u31_debuggerd");
}

TEST(grp, getgrgid_app_id_u1_non_exist) {
  expect_no_group_id(100000);  // There is no 'root' for secondary users.
  expect_no_group_id(101999);  // End of the system reserved range, unallocated.
  expect_no_group_id(102900);  // The OEM ranges were never allocated to secondary users.
  expect_no_group_id(105000);  // The OEM ranges were never allocated to secondary users.

  // The shared range is shared among users, and therefore doesn't exist for secondary users.
  expect_no_group_id(150000);
}

TEST(grp, getgrgid_app_id_u31_non_exist) {
  expect_no_group_id(3100000);  // There is no 'root' for secondary users.
  expect_no_group_id(3101999);  // End of the system reserved range, unallocated.
  expect_no_group_id(3102900);  // The OEM ranges were never allocated to secondary users.
  expect_no_group_id(3105000);  // The OEM ranges were never allocated to secondary users.

  // The shared range is shared among users, and therefore doesn't exist for secondary users.
  expect_no_group_id(3150000);
}

TEST(grp, getgrnam_r_alignment) {
#if defined(__BIONIC__)
  group grp_storage;
  alignas(16) char buf[512];
  group* grp;
  int result = getgrnam_r("root", &grp_storage, buf + 1, sizeof(buf) - 1, &grp);
  ASSERT_EQ(0, result);
  check_group(grp, "root", 0);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(grp, getgrgid_r_alignment) {
#if defined(__BIONIC__)
  group grp_storage;
  alignas(16) char buf[512];
  group* grp;
  int result = getgrgid_r(0, &grp_storage, buf + 1, sizeof(buf) - 1, &grp);
  ASSERT_EQ(0, result);
  check_group(grp, "root", 0);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(grp, getgrnam_r_reentrancy) {
#if defined(__BIONIC__)
  group grp_storage[2];
  char buf[2][512];
  group* grp[3];
  int result = getgrnam_r("root", &grp_storage[0], buf[0], sizeof(buf[0]), &grp[0]);
  ASSERT_EQ(0, result);
  check_group(grp[0], "root", 0);
  grp[1] = getgrnam("system");
  check_group(grp[1], "system", 1000);
  result = getgrnam_r("radio", &grp_storage[1], buf[1], sizeof(buf[1]), &grp[2]);
  ASSERT_EQ(0, result);
  check_group(grp[2], "radio", 1001);
  check_group(grp[0], "root", 0);
  check_group(grp[1], "system", 1000);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(grp, getgrgid_r_reentrancy) {
#if defined(__BIONIC__)
  group grp_storage[2];
  char buf[2][512];
  group* grp[3];
  int result = getgrgid_r(0, &grp_storage[0], buf[0], sizeof(buf[0]), &grp[0]);
  ASSERT_EQ(0, result);
  check_group(grp[0], "root", 0);
  grp[1] = getgrgid(1000);
  check_group(grp[1], "system", 1000);
  result = getgrgid_r(1001, &grp_storage[1], buf[1], sizeof(buf[1]), &grp[2]);
  ASSERT_EQ(0, result);
  check_group(grp[2], "radio", 1001);
  check_group(grp[0], "root", 0);
  check_group(grp[1], "system", 1000);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(grp, getgrnam_r_large_enough_suggested_buffer_size) {
  long size = sysconf(_SC_GETGR_R_SIZE_MAX);
  ASSERT_GT(size, 0);
  char buf[size];
  group grp_storage;
  group* grp;
  ASSERT_EQ(0, getgrnam_r("root", &grp_storage, buf, size, &grp));
  check_group(grp, "root", 0);
}

TEST(grp, getgrent_iterate) {
#if defined(__BIONIC__)
  group* grp;
  std::set<gid_t> gids;

  setgrent();
  while ((grp = getgrent()) != nullptr) {
    ASSERT_TRUE(grp->gr_name != nullptr) << "grp->gr_gid: " << grp->gr_gid;
    ASSERT_TRUE(grp->gr_mem != nullptr) << "grp->gr_gid: " << grp->gr_gid;
    EXPECT_STREQ(grp->gr_name, grp->gr_mem[0]) << "grp->gr_gid: " << grp->gr_gid;
    EXPECT_TRUE(grp->gr_mem[1] == nullptr) << "grp->gr_gid: " << grp->gr_gid;

    EXPECT_EQ(0U, gids.count(grp->gr_gid)) << "grp->gr_gid: " << grp->gr_gid;
    gids.emplace(grp->gr_gid);
  }
  endgrent();

  expect_ids(gids, true);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(grp, getgrouplist) {
#if defined(__BIONIC__)
  // Query the number of groups.
  int ngroups = 0;
  ASSERT_EQ(-1, getgrouplist("root", 123, nullptr, &ngroups));
  ASSERT_EQ(1, ngroups);

  // Query the specific groups (just the one you pass in on Android).
  ngroups = 8;
  gid_t groups[ngroups];
  ASSERT_EQ(1, getgrouplist("root", 123, groups, &ngroups));
  ASSERT_EQ(1, ngroups);
  ASSERT_EQ(123u, groups[0]);
#else
  GTEST_SKIP() << "bionic-only test (groups too unpredictable)";
#endif
}

#if defined(__BIONIC__)
static void TestAidNamePrefix(const std::string& file_path) {
  std::string file_contents;
  if (!ReadFileToString(file_path, &file_contents)) {
    // If we cannot read this file, then there are no vendor defind AID names, in which case this
    // test passes by default.
    return;
  }
  auto lines = Split(file_contents, "\n");
  for (const auto& line : lines) {
    if (line.empty()) continue;
    auto name = Split(line, ":")[0];
    EXPECT_TRUE(StartsWith(name, "vendor_"));
  }
}
#endif

TEST(pwd, vendor_prefix_users) {
#if defined(__BIONIC__)
  if (android::base::GetIntProperty("ro.product.first_api_level", 0) <= 28) {
    return;
  }

  TestAidNamePrefix("/vendor/etc/passwd");
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(pwd, vendor_prefix_groups) {
#if defined(__BIONIC__)
  if (android::base::GetIntProperty("ro.product.first_api_level", 0) <= 28) {
    return;
  }

  TestAidNamePrefix("/vendor/etc/group");
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}
