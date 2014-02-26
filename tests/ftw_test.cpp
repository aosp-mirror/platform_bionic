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

#include <gtest/gtest.h>
#include "TemporaryFile.h"

#include <ftw.h>
#include <stdlib.h>
#include <sys/stat.h>

void sanity_check_ftw(const char* fpath, const struct stat* sb, int tflag) {
  ASSERT_TRUE(fpath != NULL);
  ASSERT_TRUE(sb != NULL);
  bool is_dir = S_ISDIR(sb->st_mode);
  ASSERT_TRUE((is_dir && tflag == FTW_D) || (!is_dir && tflag == FTW_F));
}

void sanity_check_nftw(
    const char* fpath, const struct stat* sb, int tflag, struct FTW* ftwbuf) {
  sanity_check_ftw(fpath, sb, tflag);
  // either the parent dir or the file
  bool is_dir = S_ISDIR(sb->st_mode);
  ASSERT_TRUE(
    (is_dir && ftwbuf->level == 0) || (!is_dir && ftwbuf->level == 1));
}

int check_ftw(const char* fpath, const struct stat* sb, int tflag) {
  sanity_check_ftw(fpath, sb, tflag);
  return 0;
}

int check_ftw64(const char* fpath, const struct stat64* sb, int tflag) {
  sanity_check_ftw(fpath, reinterpret_cast<const struct stat*>(sb), tflag);
  return 0;
}

int check_nftw(
  const char* fpath, const struct stat* sb, int tflag, struct FTW* ftwbuf) {
  sanity_check_nftw(fpath, sb, tflag, ftwbuf);
  return 0;
}

int check_nftw64(
  const char* fpath, const struct stat64* sb, int tflag, struct FTW* ftwbuf) {
  sanity_check_nftw(fpath, reinterpret_cast<const struct stat*>(sb),
    tflag, ftwbuf);
  return 0;
}

TEST(ftw, ftw) {
  TemporaryDir td;
  TemporaryFile tf(td.dirname);
  ftw(td.dirname, check_ftw, 1);
}

TEST(ftw, ftw64) {
  TemporaryDir td;
  GenericTemporaryFile<mkstemp64> tf(td.dirname);
  ftw64(td.dirname, check_ftw64, 1);
}

TEST(ftw, nftw) {
  TemporaryDir td;
  TemporaryFile tf(td.dirname);
  nftw(td.dirname, check_nftw, 1, 0);
}

TEST(ftw, nftw64) {
  TemporaryDir td;
  GenericTemporaryFile<mkstemp64> tf(td.dirname);
  nftw64(td.dirname, check_nftw64, 1, 0);
}
