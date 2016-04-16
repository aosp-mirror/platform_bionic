/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <ftw.h>

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "TemporaryFile.h"

#include <android-base/stringprintf.h>
#include <gtest/gtest.h>

static void MakeTree(const char* root) {
  char path[PATH_MAX];

  snprintf(path, sizeof(path), "%s/dir", root);
  ASSERT_EQ(0, mkdir(path, 0755)) << path;
  snprintf(path, sizeof(path), "%s/dir/sub", root);
  ASSERT_EQ(0, mkdir(path, 0555)) << path;
  snprintf(path, sizeof(path), "%s/unreadable-dir", root);
  ASSERT_EQ(0, mkdir(path, 0000)) << path;

  snprintf(path, sizeof(path), "%s/dangler", root);
  ASSERT_EQ(0, symlink("/does-not-exist", path));
  snprintf(path, sizeof(path), "%s/symlink", root);
  ASSERT_EQ(0, symlink("dir/sub", path));

  int fd;
  snprintf(path, sizeof(path), "%s/regular", root);
  ASSERT_NE(-1, fd = open(path, O_CREAT|O_TRUNC, 0666));
  ASSERT_EQ(0, close(fd));
}

void sanity_check_ftw(const char* fpath, const struct stat* sb, int tflag) {
  ASSERT_TRUE(fpath != NULL);
  ASSERT_TRUE(sb != NULL);

  // Was it a case where the struct stat we're given is meaningless?
  if (tflag == FTW_NS || tflag == FTW_SLN) {
    // If so, double-check that we really can't stat.
    struct stat sb;
    EXPECT_EQ(-1, stat(fpath, &sb));
    return;
  }

  // Otherwise check that the struct stat matches the type flag.
  if (S_ISDIR(sb->st_mode)) {
    if (access(fpath, R_OK) == 0) {
      EXPECT_TRUE(tflag == FTW_D || tflag == FTW_DP) << fpath << ' ' << tflag;
    } else {
      EXPECT_EQ(FTW_DNR, tflag) << fpath;
    }
  } else if (S_ISLNK(sb->st_mode)) {
    EXPECT_EQ(FTW_SL, tflag) << fpath;
  } else {
    EXPECT_EQ(FTW_F, tflag) << fpath;
  }
}

void sanity_check_nftw(const char* fpath, const struct stat* sb, int tflag, FTW* ftwbuf) {
  sanity_check_ftw(fpath, sb, tflag);
  ASSERT_EQ('/', fpath[ftwbuf->base - 1]) << fpath;
}

int check_ftw(const char* fpath, const struct stat* sb, int tflag) {
  sanity_check_ftw(fpath, sb, tflag);
  return 0;
}

int check_ftw64(const char* fpath, const struct stat64* sb, int tflag) {
  sanity_check_ftw(fpath, reinterpret_cast<const struct stat*>(sb), tflag);
  return 0;
}

int check_nftw(const char* fpath, const struct stat* sb, int tflag, FTW* ftwbuf) {
  sanity_check_nftw(fpath, sb, tflag, ftwbuf);
  return 0;
}

int check_nftw64(const char* fpath, const struct stat64* sb, int tflag, FTW* ftwbuf) {
  sanity_check_nftw(fpath, reinterpret_cast<const struct stat*>(sb), tflag, ftwbuf);
  return 0;
}

TEST(ftw, ftw) {
  TemporaryDir root;
  MakeTree(root.dirname);
  ASSERT_EQ(0, ftw(root.dirname, check_ftw, 128));
}

TEST(ftw, ftw64) {
  TemporaryDir root;
  MakeTree(root.dirname);
  ASSERT_EQ(0, ftw64(root.dirname, check_ftw64, 128));
}

TEST(ftw, nftw) {
  TemporaryDir root;
  MakeTree(root.dirname);
  ASSERT_EQ(0, nftw(root.dirname, check_nftw, 128, 0));
}

TEST(ftw, nftw64) {
  TemporaryDir root;
  MakeTree(root.dirname);
  ASSERT_EQ(0, nftw64(root.dirname, check_nftw64, 128, 0));
}

template <typename StatT>
static int bug_28197840_ftw(const char* path, const StatT*, int flag) {
  EXPECT_EQ(strstr(path, "unreadable") != nullptr ? FTW_DNR : FTW_D, flag) << path;
  return 0;
}

template <typename StatT>
static int bug_28197840_nftw(const char* path, const StatT* sb, int flag, FTW*) {
  return bug_28197840_ftw(path, sb, flag);
}

TEST(ftw, bug_28197840) {
  // Drop root for this test, because root can still read directories even if
  // permissions would imply otherwise.
  if (getuid() == 0) {
    passwd* pwd = getpwnam("shell");
    ASSERT_EQ(0, setuid(pwd->pw_uid));
  }

  TemporaryDir root;

  std::string path = android::base::StringPrintf("%s/unreadable-directory", root.dirname);
  ASSERT_EQ(0, mkdir(path.c_str(), 0000)) << path;

  ASSERT_EQ(0, ftw(root.dirname, bug_28197840_ftw<struct stat>, 128));
  ASSERT_EQ(0, ftw64(root.dirname, bug_28197840_ftw<struct stat64>, 128));
  ASSERT_EQ(0, nftw(root.dirname, bug_28197840_nftw<struct stat>, 128, FTW_PHYS));
  ASSERT_EQ(0, nftw64(root.dirname, bug_28197840_nftw<struct stat64>, 128, FTW_PHYS));
}
