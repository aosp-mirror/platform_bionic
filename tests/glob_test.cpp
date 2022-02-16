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

#include <glob.h>

#include <dirent.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

#include <android-base/file.h>

#if defined(__BIONIC__)
#define ASSERT_MATCH_COUNT(n_,g_) ASSERT_EQ(n_, g_.gl_matchc)
#else
#define ASSERT_MATCH_COUNT(n_,g_)
#endif

//
// Helper for use with GLOB_ALTDIRFUNC to iterate over the elements of `fake_dir`.
//

static std::vector<std::string> fake_dir;
static size_t fake_dir_offset;
static void fake_closedir(void*) {
}
static dirent* fake_readdir(void*) {
  static dirent d;
  if (fake_dir_offset >= fake_dir.size()) return nullptr;
  strcpy(d.d_name, fake_dir[fake_dir_offset++].c_str());
  return &d;
}
static void* fake_opendir(const char* path) {
  fake_dir_offset = 0;
  if (strcmp(path, "/opendir-fail/") == 0) {
    errno = EINVAL;
    return nullptr;
  }
  return &fake_dir;
}
static int fake_lstat(const char*, struct stat*) {
  return 0;
}
static int fake_stat(const char*, struct stat*) {
  return 0;
}
static void InstallFake(glob_t* g) {
  g->gl_closedir = fake_closedir;
  g->gl_readdir = fake_readdir;
  g->gl_opendir = fake_opendir;
  g->gl_lstat = fake_lstat;
  g->gl_stat = fake_stat;
}

TEST(glob, glob_result_GLOB_NOMATCH) {
  glob_t g = {};
  ASSERT_EQ(GLOB_NOMATCH, glob("/will/match/nothing", 0, nullptr, &g));
  ASSERT_EQ(0U, g.gl_pathc);
  ASSERT_MATCH_COUNT(0U, g);
}

TEST(glob, glob_GLOB_APPEND) {
  glob_t g = {};
  ASSERT_EQ(0, glob("/proc/version", 0, nullptr, &g));
  ASSERT_EQ(1U, g.gl_pathc);
  ASSERT_MATCH_COUNT(1U, g);
  ASSERT_STREQ("/proc/version", g.gl_pathv[0]);
  ASSERT_EQ(nullptr, g.gl_pathv[1]);
  ASSERT_EQ(0, glob("/proc/version", GLOB_APPEND, nullptr, &g));
  ASSERT_EQ(2U, g.gl_pathc);
  ASSERT_MATCH_COUNT(1U, g);
  ASSERT_STREQ("/proc/version", g.gl_pathv[0]);
  ASSERT_STREQ("/proc/version", g.gl_pathv[1]);
  ASSERT_EQ(nullptr, g.gl_pathv[2]);
  globfree(&g);
}

TEST(glob, glob_GLOB_DOOFFS) {
  glob_t g = {};
  g.gl_offs = 2;
  ASSERT_EQ(0, glob("/proc/version", GLOB_DOOFFS, nullptr, &g));
  ASSERT_EQ(1U, g.gl_pathc);
  ASSERT_MATCH_COUNT(1U, g);
  ASSERT_EQ(nullptr, g.gl_pathv[0]);
  ASSERT_EQ(nullptr, g.gl_pathv[1]);
  ASSERT_STREQ("/proc/version", g.gl_pathv[2]);
  ASSERT_EQ(nullptr, g.gl_pathv[3]);
  globfree(&g);
}

static std::string g_failure_path;
static int g_failure_errno;
static int test_error_callback_result;
static int test_error_callback(const char* failure_path, int failure_errno) {
  g_failure_path = failure_path;
  g_failure_errno = failure_errno;
  return test_error_callback_result;
}

TEST(glob, glob_gl_errfunc) {
  glob_t g = {};
  InstallFake(&g);

  test_error_callback_result = 0;
  g_failure_errno = 0;
  ASSERT_EQ(GLOB_NOMATCH, glob("/opendir-fail/x*", GLOB_ALTDIRFUNC, test_error_callback, &g));
  ASSERT_EQ("/opendir-fail/", g_failure_path);
  ASSERT_EQ(EINVAL, g_failure_errno);

  test_error_callback_result = 1;
  g_failure_errno = 0;
  ASSERT_EQ(GLOB_ABORTED, glob("/opendir-fail/x*", GLOB_ALTDIRFUNC, test_error_callback, &g));
  ASSERT_EQ("/opendir-fail/", g_failure_path);
  ASSERT_EQ(EINVAL, g_failure_errno);
}

TEST(glob, glob_GLOB_ERR) {
  glob_t g = {};
  InstallFake(&g);

  ASSERT_EQ(GLOB_NOMATCH, glob("/opendir-fail/x*", GLOB_ALTDIRFUNC, nullptr, &g));

  ASSERT_EQ(GLOB_ABORTED, glob("/opendir-fail/x*", GLOB_ALTDIRFUNC | GLOB_ERR, nullptr, &g));
}

TEST(glob, glob_GLOB_MARK) {
  TemporaryDir td;
  // The pattern we're about to pass doesn't have a trailing '/'...
  ASSERT_NE('/', std::string(td.path).back());

  glob_t g = {};
  // Using GLOB_MARK gets you a trailing '/' on a directory...
  ASSERT_EQ(0, glob(td.path, GLOB_MARK, nullptr, &g));
  ASSERT_EQ(1U, g.gl_pathc);
  ASSERT_MATCH_COUNT(1U, g);
  ASSERT_EQ(std::string(td.path) + "/", g.gl_pathv[0]);
  ASSERT_EQ(nullptr, g.gl_pathv[1]);

  TemporaryFile tf;
  // But not on a file...
  ASSERT_EQ(0, glob(tf.path, GLOB_MARK, nullptr, &g));
  ASSERT_EQ(1U, g.gl_pathc);
  ASSERT_MATCH_COUNT(1U, g);
  ASSERT_STREQ(tf.path, g.gl_pathv[0]);
  ASSERT_EQ(nullptr, g.gl_pathv[1]);

  globfree(&g);
}

TEST(glob, glob_GLOB_NOCHECK) {
  glob_t g = {};
  ASSERT_EQ(0, glob("/will/match/nothing", GLOB_NOCHECK, nullptr, &g));
  ASSERT_EQ(1U, g.gl_pathc);
  ASSERT_MATCH_COUNT(0U, g);
  ASSERT_STREQ("/will/match/nothing", g.gl_pathv[0]);
  ASSERT_EQ(nullptr, g.gl_pathv[1]);
  globfree(&g);
}

TEST(glob, glob_GLOB_NOSORT) {
  fake_dir = { "c", "a", "d", "b" };

  glob_t g = {};
  InstallFake(&g);

  ASSERT_EQ(0, glob("*", GLOB_ALTDIRFUNC, nullptr, &g));
  ASSERT_EQ(4U, g.gl_pathc);
  ASSERT_MATCH_COUNT(4U, g);
  ASSERT_STREQ("a", g.gl_pathv[0]);
  ASSERT_STREQ("b", g.gl_pathv[1]);
  ASSERT_STREQ("c", g.gl_pathv[2]);
  ASSERT_STREQ("d", g.gl_pathv[3]);
  ASSERT_EQ(nullptr, g.gl_pathv[4]);

  ASSERT_EQ(0, glob("*", GLOB_ALTDIRFUNC | GLOB_NOSORT, nullptr, &g));
  ASSERT_EQ(4U, g.gl_pathc);
  ASSERT_MATCH_COUNT(4U, g);
  ASSERT_STREQ("c", g.gl_pathv[0]);
  ASSERT_STREQ("a", g.gl_pathv[1]);
  ASSERT_STREQ("d", g.gl_pathv[2]);
  ASSERT_STREQ("b", g.gl_pathv[3]);
  ASSERT_EQ(nullptr, g.gl_pathv[4]);
}

TEST(glob, glob_GLOB_MAGCHAR) {
  glob_t g = {};
  ASSERT_EQ(GLOB_NOMATCH, glob("/does-not-exist", 0, nullptr, &g));
  ASSERT_TRUE((g.gl_flags & GLOB_MAGCHAR) == 0);
  ASSERT_EQ(GLOB_NOMATCH, glob("/does-not-exist*", 0, nullptr, &g));
  ASSERT_TRUE((g.gl_flags & GLOB_MAGCHAR) != 0);

  // We can lie, but glob(3) will turn that into truth...
  ASSERT_EQ(GLOB_NOMATCH, glob("/does-not-exist", GLOB_MAGCHAR, nullptr, &g));
  ASSERT_TRUE((g.gl_flags & GLOB_MAGCHAR) == 0);
}

static void CheckGlob(const char* pattern, const std::vector<std::string>& expected_matches) {
  glob_t g = {};
  InstallFake(&g);

  int expected_result = expected_matches.empty() ? GLOB_NOMATCH : 0;
  ASSERT_EQ(expected_result, glob(pattern, GLOB_ALTDIRFUNC, nullptr, &g)) << pattern;
  ASSERT_EQ(expected_matches.size(), g.gl_pathc);
  ASSERT_MATCH_COUNT(expected_matches.size(), g);
  for (size_t i = 0; i < expected_matches.size(); ++i) {
    ASSERT_EQ(expected_matches[i], g.gl_pathv[i]);
  }
  if (!expected_matches.empty()) {
    ASSERT_EQ(nullptr, g.gl_pathv[expected_matches.size()]);
  }
  globfree(&g);
}

TEST(glob, glob_globbing) {
  fake_dir = { "f1", "f2", "f30", "f40" };

  CheckGlob("f?", { "f1", "f2" });
  CheckGlob("f??", { "f30", "f40" });
  CheckGlob("f*", { "f1", "f2", "f30", "f40" });
}

TEST(glob, glob_globbing_rsc) {
  // https://research.swtch.com/glob
  fake_dir = { "axbxcxdxe" };
  CheckGlob("a*b*c*d*e*", { "axbxcxdxe" });
  fake_dir = { "axbxcxdxexxx" };
  CheckGlob("a*b*c*d*e*", { "axbxcxdxexxx" });
  fake_dir = { "abxbbxdbxebxczzx" };
  CheckGlob("a*b?c*x", { "abxbbxdbxebxczzx" });
  fake_dir = { "abxbbxdbxebxczzy" };
  CheckGlob("a*b?c*x", {});

  fake_dir = { std::string(100, 'a') };
  CheckGlob("a*a*a*a*b", {});
}
