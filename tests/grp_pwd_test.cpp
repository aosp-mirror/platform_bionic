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

#include <bitset>

#include <private/android_filesystem_config.h>

// Generated android_ids array
#include "generated_android_ids.h"

enum uid_type_t {
  TYPE_SYSTEM,
  TYPE_APP
};

#if defined(__BIONIC__)

static void check_passwd(const passwd* pwd, const char* username, uid_t uid, uid_type_t uid_type) {
  ASSERT_TRUE(pwd != NULL);
  ASSERT_STREQ(username, pwd->pw_name);
  ASSERT_EQ(uid, pwd->pw_uid);
  ASSERT_EQ(uid, pwd->pw_gid);
  ASSERT_EQ(NULL, pwd->pw_passwd);
#ifdef __LP64__
  ASSERT_EQ(NULL, pwd->pw_gecos);
#endif

  if (uid_type == TYPE_SYSTEM) {
    ASSERT_STREQ("/", pwd->pw_dir);
  } else {
    ASSERT_STREQ("/data", pwd->pw_dir);
  }
  ASSERT_STREQ("/system/bin/sh", pwd->pw_shell);
}

static void check_getpwuid(const char* username, uid_t uid, uid_type_t uid_type) {
  errno = 0;
  passwd* pwd = getpwuid(uid);
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getpwuid");
  check_passwd(pwd, username, uid, uid_type);
}

static void check_getpwnam(const char* username, uid_t uid, uid_type_t uid_type) {
  errno = 0;
  passwd* pwd = getpwnam(username);
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getpwnam");
  check_passwd(pwd, username, uid, uid_type);
}

static void check_getpwuid_r(const char* username, uid_t uid, uid_type_t uid_type) {
  passwd pwd_storage;
  char buf[512];
  int result;

  errno = 0;
  passwd* pwd = NULL;
  result = getpwuid_r(uid, &pwd_storage, buf, sizeof(buf), &pwd);
  ASSERT_EQ(0, result);
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getpwuid_r");
  check_passwd(pwd, username, uid, uid_type);
}

static void check_getpwnam_r(const char* username, uid_t uid, uid_type_t uid_type) {
  passwd pwd_storage;
  char buf[512];
  int result;

  errno = 0;
  passwd* pwd = NULL;
  result = getpwnam_r(username, &pwd_storage, buf, sizeof(buf), &pwd);
  ASSERT_EQ(0, result);
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getpwnam_r");
  check_passwd(pwd, username, uid, uid_type);
}

static void check_get_passwd(const char* username, uid_t uid, uid_type_t uid_type) {
  check_getpwuid(username, uid, uid_type);
  check_getpwnam(username, uid, uid_type);
  check_getpwuid_r(username, uid, uid_type);
  check_getpwnam_r(username, uid, uid_type);
}

#else // !defined(__BIONIC__)

static void check_get_passwd(const char* /* username */, uid_t /* uid */, uid_type_t /* uid_type */) {
  GTEST_LOG_(INFO) << "This test is about uid/username translation for Android, which does nothing on libc other than bionic.\n";
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
  check_get_passwd("oem_5000", 5000, TYPE_SYSTEM);
}

TEST(pwd, getpwnam_oem_id_5999) {
  check_get_passwd("oem_5999", 5999, TYPE_SYSTEM);
}

TEST(pwd, getpwnam_oem_id_2900) {
  check_get_passwd("oem_2900", 2900, TYPE_SYSTEM);
}

TEST(pwd, getpwnam_oem_id_2999) {
  check_get_passwd("oem_2999", 2999, TYPE_SYSTEM);
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
  check_get_passwd("u0_i1", 99001, TYPE_APP);
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
  check_get_passwd("u1_i0", 199000, TYPE_APP);
}

TEST(pwd, getpwent_iterate) {
  passwd* pwd;
  std::bitset<10000> exist;
  bool application = false;

  exist.reset();

  setpwent();
  while ((pwd = getpwent()) != NULL) {
    ASSERT_TRUE(NULL != pwd->pw_name);
    ASSERT_EQ(pwd->pw_gid, pwd->pw_uid);
    ASSERT_EQ(NULL, pwd->pw_passwd);
#ifdef __LP64__
    ASSERT_TRUE(NULL == pwd->pw_gecos);
#endif
    ASSERT_TRUE(NULL != pwd->pw_shell);
    if (pwd->pw_uid >= exist.size()) {
      ASSERT_STREQ("/data", pwd->pw_dir);
      application = true;
    } else {
      ASSERT_STREQ("/", pwd->pw_dir);
      ASSERT_FALSE(exist[pwd->pw_uid]);
      exist[pwd->pw_uid] = true;
    }
  }
  endpwent();

  // Required content
  for (size_t n = 0; n < android_id_count; ++n) {
    ASSERT_TRUE(exist[android_ids[n].aid]);
  }
  for (size_t n = 2900; n < 2999; ++n) {
    ASSERT_TRUE(exist[n]);
  }
  for (size_t n = 5000; n < 5999; ++n) {
    ASSERT_TRUE(exist[n]);
  }
  ASSERT_TRUE(application);
}

static void check_group(const group* grp, const char* group_name, gid_t gid) {
  ASSERT_TRUE(grp != NULL);
  ASSERT_STREQ(group_name, grp->gr_name);
  ASSERT_EQ(gid, grp->gr_gid);
  ASSERT_TRUE(grp->gr_mem != NULL);
  ASSERT_STREQ(group_name, grp->gr_mem[0]);
  ASSERT_TRUE(grp->gr_mem[1] == NULL);
}

#if defined(__BIONIC__)

static void check_getgrgid(const char* group_name, gid_t gid) {
  errno = 0;
  group* grp = getgrgid(gid);
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getgrgid");
  check_group(grp, group_name, gid);
}

static void check_getgrnam(const char* group_name, gid_t gid) {
  errno = 0;
  group* grp = getgrnam(group_name);
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getgrnam");
  check_group(grp, group_name, gid);
}

static void check_getgrgid_r(const char* group_name, gid_t gid) {
  group grp_storage;
  char buf[512];
  group* grp;

  errno = 0;
  int result = getgrgid_r(gid, &grp_storage, buf, sizeof(buf), &grp);
  ASSERT_EQ(0, result);
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getgrgid_r");
  check_group(grp, group_name, gid);
}

static void check_getgrnam_r(const char* group_name, gid_t gid) {
  group grp_storage;
  char buf[512];
  group* grp;

  errno = 0;
  int result = getgrnam_r(group_name, &grp_storage, buf, sizeof(buf), &grp);
  ASSERT_EQ(0, result);
  ASSERT_EQ(0, errno);
  SCOPED_TRACE("getgrnam_r");
  check_group(grp, group_name, gid);
}

static void check_get_group(const char* group_name, gid_t gid) {
  check_getgrgid(group_name, gid);
  check_getgrnam(group_name, gid);
  check_getgrgid_r(group_name, gid);
  check_getgrnam_r(group_name, gid);
}

#else // !defined(__BIONIC__)

static void print_no_getgrnam_test_info() {
  GTEST_LOG_(INFO) << "This test is about gid/group_name translation for Android, which does nothing on libc other than bionic.\n";
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
  check_get_group("oem_5000", 5000);
}

TEST(grp, getgrnam_oem_id_5999) {
  check_get_group("oem_5999", 5999);
}

TEST(grp, getgrnam_oem_id_2900) {
  check_get_group("oem_2900", 2900);
}

TEST(grp, getgrnam_oem_id_2999) {
  check_get_group("oem_2999", 2999);
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
  check_get_group("u0_i1", 99001);
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
  check_get_group("u1_i0", 199000);
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
  group* grp;
  std::bitset<10000> exist;
  bool application = false;

  exist.reset();

  setgrent();
  while ((grp = getgrent()) != NULL) {
    ASSERT_TRUE(grp->gr_name != NULL);
    ASSERT_TRUE(grp->gr_mem != NULL);
    ASSERT_STREQ(grp->gr_name, grp->gr_mem[0]);
    ASSERT_TRUE(grp->gr_mem[1] == NULL);
    if (grp->gr_gid >= exist.size()) {
      application = true;
    } else {
      ASSERT_FALSE(exist[grp->gr_gid]);
      exist[grp->gr_gid] = true;
    }
  }
  endgrent();

  // Required content
  for (size_t n = 0; n < android_id_count; ++n) {
    ASSERT_TRUE(exist[android_ids[n].aid]);
  }
  for (size_t n = 2900; n < 2999; ++n) {
    ASSERT_TRUE(exist[n]);
  }
  for (size_t n = 5000; n < 5999; ++n) {
    ASSERT_TRUE(exist[n]);
  }
  ASSERT_TRUE(application);
}
