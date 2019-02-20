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

using android::base::Join;
using android::base::ReadFileToString;
using android::base::Split;
using android::base::StartsWith;

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

  if (uid_type == TYPE_VENDOR) {
    EXPECT_STREQ("/vendor/bin/sh", pwd->pw_shell);
  } else {
    EXPECT_STREQ("/system/bin/sh", pwd->pw_shell);
  }
}

static void check_getpwuid(const char* username, uid_t uid, uid_type_t uid_type,
                           bool check_username) {
  errno = 0;
  passwd* pwd = getpwuid(uid);
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getpwuid");
  check_passwd(pwd, username, uid, uid_type, check_username);
}

static void check_getpwnam(const char* username, uid_t uid, uid_type_t uid_type,
                           bool check_username) {
  errno = 0;
  passwd* pwd = getpwnam(username);
  ASSERT_EQ(0, errno);
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
  ASSERT_EQ(0, errno);
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
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getpwnam_r");
  check_passwd(pwd, username, uid, uid_type, check_username);
}

static void check_get_passwd(const char* username, uid_t uid, uid_type_t uid_type,
                             bool check_username = true) {
  check_getpwuid(username, uid, uid_type, check_username);
  check_getpwnam(username, uid, uid_type, check_username);
  check_getpwuid_r(username, uid, uid_type, check_username);
  check_getpwnam_r(username, uid, uid_type, check_username);
}

#else // !defined(__BIONIC__)

static void print_no_getpwnam_test_info() {
  GTEST_LOG_(INFO) << "This test is about uid/username translation for Android, which does nothing on libc other than bionic.\n";
}

static void check_get_passwd(const char* /* username */, uid_t /* uid */, uid_type_t /* uid_type */,
                             bool /* check_username */) {
  print_no_getpwnam_test_info();
}

static void check_get_passwd(const char* /* username */, uid_t /* uid */, uid_type_t /* uid_type */) {
  print_no_getpwnam_test_info();
}

#endif

TEST(pwd, getpwnam_system_id_root) {
  check_get_passwd("root", 0, TYPE_SYSTEM);
}

TEST(pwd, getpwnam_system_id_system) {
  check_get_passwd("system", 1000, TYPE_SYSTEM);
}

TEST(pwd, getpwnam_app_id_radio) {
  check_get_passwd("radio", 1001, TYPE_SYSTEM);
}

TEST(pwd, getpwnam_oem_id_5000) {
  check_get_passwd("oem_5000", 5000, TYPE_VENDOR, false);
}

TEST(pwd, getpwnam_oem_id_5999) {
  check_get_passwd("oem_5999", 5999, TYPE_VENDOR, false);
}

TEST(pwd, getpwnam_oem_id_2900) {
  check_get_passwd("oem_2900", 2900, TYPE_VENDOR, false);
}

TEST(pwd, getpwnam_oem_id_2999) {
  check_get_passwd("oem_2999", 2999, TYPE_VENDOR, false);
}

TEST(pwd, getpwnam_app_id_nobody) {
  check_get_passwd("nobody", 9999, TYPE_SYSTEM);
}

TEST(pwd, getpwnam_app_id_u0_a0) {
  check_get_passwd("u0_a0", 10000, TYPE_APP);
}

TEST(pwd, getpwnam_app_id_u0_a1234) {
  check_get_passwd("u0_a1234", 11234, TYPE_APP);
}

// Test the difference between uid and shared gid.
TEST(pwd, getpwnam_app_id_u0_a49999) {
  check_get_passwd("u0_a49999", 59999, TYPE_APP);
}

TEST(pwd, getpwnam_app_id_u0_i1) {
  check_get_passwd("u0_i1", 90001, TYPE_APP);
}

TEST(pwd, getpwnam_app_id_u1_root) {
  check_get_passwd("u1_root", 100000, TYPE_SYSTEM);
}

TEST(pwd, getpwnam_app_id_u1_radio) {
  check_get_passwd("u1_radio", 101001, TYPE_SYSTEM);
}

TEST(pwd, getpwnam_app_id_u1_a0) {
  check_get_passwd("u1_a0", 110000, TYPE_APP);
}

TEST(pwd, getpwnam_app_id_u1_a40000) {
  check_get_passwd("u1_a40000", 150000, TYPE_APP);
}

TEST(pwd, getpwnam_app_id_u1_i0) {
  check_get_passwd("u1_i0", 190000, TYPE_APP);
}
#if defined(__BIONIC__)
template <typename T>
static void expect_ids(const T& ids) {
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
  expect_range(AID_CACHE_GID_START, AID_CACHE_GID_END);
  expect_range(AID_EXT_GID_START, AID_EXT_GID_END);
  expect_range(AID_EXT_CACHE_GID_START, AID_EXT_CACHE_GID_END);
  expect_range(AID_SHARED_GID_START, AID_SHARED_GID_END);
  expect_range(AID_ISOLATED_START, AID_ISOLATED_END);

  // TODO(73062966): We still don't have a good way to create vendor AIDs in the system or other
  // non-vendor partitions, therefore we keep this check disabled.
  if (android::base::GetIntProperty("ro.product.first_api_level", 0) <= __ANDROID_API_Q__) {
    return;
  }

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

    // TODO(b/27999086): fix this check with the OEM range
    // If OEMs add their own AIDs to private/android_filesystem_config.h, this check will fail.
    // Long term we want to create a better solution for OEMs adding AIDs, but we're not there
    // yet, so therefore we do not check for uid's in the OEM range.
    if (!(pwd->pw_uid >= 2900 && pwd->pw_uid <= 2999) &&
        !(pwd->pw_uid >= 5000 && pwd->pw_uid <= 5999)) {
      EXPECT_EQ(0U, uids.count(pwd->pw_uid)) << "pwd->pw_uid: " << pwd->pw_uid;
    }
    uids.emplace(pwd->pw_uid);
  }
  endpwent();

  expect_ids(uids);
#else
  print_no_getpwnam_test_info();
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
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getgrgid");
  check_group(grp, group_name, gid, check_groupname);
}

static void check_getgrnam(const char* group_name, gid_t gid, bool check_groupname) {
  errno = 0;
  group* grp = getgrnam(group_name);
  ASSERT_EQ(0, errno);
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
  ASSERT_EQ(0, errno);
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
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getgrnam_r");
  check_group(grp, group_name, gid, check_groupname);
}

static void check_get_group(const char* group_name, gid_t gid, bool check_groupname = true) {
  check_getgrgid(group_name, gid, check_groupname);
  check_getgrnam(group_name, gid, check_groupname);
  check_getgrgid_r(group_name, gid, check_groupname);
  check_getgrnam_r(group_name, gid, check_groupname);
}

#else // !defined(__BIONIC__)

static void print_no_getgrnam_test_info() {
  GTEST_LOG_(INFO) << "This test is about gid/group_name translation for Android, which does nothing on libc other than bionic.\n";
}

static void check_get_group(const char*, gid_t, bool) {
  print_no_getgrnam_test_info();
}

static void check_get_group(const char*, gid_t) {
  print_no_getgrnam_test_info();
}

#endif

TEST(grp, getgrnam_system_id_root) {
  check_get_group("root", 0);
}

TEST(grp, getgrnam_system_id_system) {
  check_get_group("system", 1000);
}

TEST(grp, getgrnam_app_id_radio) {
  check_get_group("radio", 1001);
}

TEST(grp, getgrnam_oem_id_5000) {
  check_get_group("oem_5000", 5000, false);
}

TEST(grp, getgrnam_oem_id_5999) {
  check_get_group("oem_5999", 5999, false);
}

TEST(grp, getgrnam_oem_id_2900) {
  check_get_group("oem_2900", 2900, false);
}

TEST(grp, getgrnam_oem_id_2999) {
  check_get_group("oem_2999", 2999, false);
}

TEST(grp, getgrnam_app_id_nobody) {
  check_get_group("nobody", 9999);
}

TEST(grp, getgrnam_app_id_u0_a0) {
  check_get_group("u0_a0", 10000);
}

TEST(grp, getgrnam_app_id_u0_a1234) {
  check_get_group("u0_a1234", 11234);
}

TEST(grp, getgrnam_app_id_u0_a9999) {
  check_get_group("u0_a9999", 19999);
}

TEST(getgrnam, app_id_u0_a0_cache) {
  check_get_group("u0_a0_cache", 20000);
}

TEST(getgrnam, app_id_u0_a1234_cache) {
  check_get_group("u0_a1234_cache", 21234);
}

TEST(getgrnam, app_id_u0_a9999_cache) {
  check_get_group("u0_a9999_cache", 29999);
}

TEST(getgrnam, app_id_u10_a1234_cache) {
  check_get_group("u10_a1234_cache", 1021234);
}

// Test the difference between uid and shared gid.
TEST(grp, getgrnam_app_id_all_a9999) {
  check_get_group("all_a9999", 59999);
}

TEST(grp, getgrnam_app_id_u0_i1) {
  check_get_group("u0_i1", 90001);
}

TEST(grp, getgrnam_app_id_u1_root) {
  check_get_group("u1_root", 100000);
}

TEST(grp, getgrnam_app_id_u1_radio) {
  check_get_group("u1_radio", 101001);
}

TEST(grp, getgrnam_app_id_u1_a0) {
  check_get_group("u1_a0", 110000);
}

TEST(grp, getgrnam_app_id_u1_a40000) {
  check_get_group("u1_a40000", 150000);
}

TEST(grp, getgrnam_app_id_u1_i0) {
  check_get_group("u1_i0", 190000);
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
  print_no_getgrnam_test_info();
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
  print_no_getgrnam_test_info();
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

    // TODO(b/27999086): fix this check with the OEM range
    // If OEMs add their own AIDs to private/android_filesystem_config.h, this check will fail.
    // Long term we want to create a better solution for OEMs adding AIDs, but we're not there
    // yet, so therefore we do not check for gid's in the OEM range.
    if (!(grp->gr_gid >= 2900 && grp->gr_gid <= 2999) &&
        !(grp->gr_gid >= 5000 && grp->gr_gid <= 5999)) {
      EXPECT_EQ(0U, gids.count(grp->gr_gid)) << "grp->gr_gid: " << grp->gr_gid;
    }
    gids.emplace(grp->gr_gid);
  }
  endgrent();

  expect_ids(gids);
#else
  print_no_getgrnam_test_info();
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
  print_no_getpwnam_test_info();
#endif
}

TEST(pwd, vendor_prefix_groups) {
#if defined(__BIONIC__)
  if (android::base::GetIntProperty("ro.product.first_api_level", 0) <= 28) {
    return;
  }

  TestAidNamePrefix("/vendor/etc/group");
#else
  print_no_getgrnam_test_info();
#endif
}
