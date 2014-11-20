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

#include <stdlib.h>
#include <sys/stat.h>

#include <gtest/gtest.h>

void sanity_check_ftw(const char* fpath, const struct stat* sb, int tflag) {
  ASSERT_TRUE(fpath != NULL);
  ASSERT_TRUE(sb != NULL);
  if (S_ISDIR(sb->st_mode)) {
    ASSERT_TRUE(tflag == FTW_D || tflag == FTW_DNR || tflag == FTW_DP) << fpath;
  } else if (S_ISLNK(sb->st_mode)) {
    ASSERT_EQ(FTW_SL, tflag) << fpath;
  } else {
    ASSERT_EQ(FTW_F, tflag) << fpath;
  }
}

void sanity_check_nftw(const char* fpath, const struct stat* sb, int tflag, struct FTW* ftwbuf) {
  sanity_check_ftw(fpath, sb, tflag);

  size_t slash_count = 0;
  const char* p = fpath;
  while ((p = strchr(p + 1, '/')) != NULL) {
    ++slash_count;
  }

  ASSERT_EQ('/', fpath[ftwbuf->base - 1]) << fpath;
  ASSERT_EQ(slash_count, static_cast<size_t>(ftwbuf->level)) << fpath;
}

int check_ftw(const char* fpath, const struct stat* sb, int tflag) {
  sanity_check_ftw(fpath, sb, tflag);
  return 0;
}

int check_ftw64(const char* fpath, const struct stat64* sb, int tflag) {
  sanity_check_ftw(fpath, reinterpret_cast<const struct stat*>(sb), tflag);
  return 0;
}

int check_nftw(const char* fpath, const struct stat* sb, int tflag, struct FTW* ftwbuf) {
  sanity_check_nftw(fpath, sb, tflag, ftwbuf);
  return 0;
}

int check_nftw64(const char* fpath, const struct stat64* sb, int tflag, struct FTW* ftwbuf) {
  sanity_check_nftw(fpath, reinterpret_cast<const struct stat*>(sb), tflag, ftwbuf);
  return 0;
}

TEST(ftw, ftw) {
  ASSERT_EQ(0, ftw("/sys", check_ftw, 128));
}

TEST(ftw, ftw64) {
  ASSERT_EQ(0, ftw64("/sys", check_ftw64, 128));
}

TEST(ftw, nftw) {
  ASSERT_EQ(0, nftw("/sys", check_nftw, 128, 0));
}

TEST(ftw, nftw64) {
  ASSERT_EQ(0, nftw64("/sys", check_nftw64, 128, 0));
}
