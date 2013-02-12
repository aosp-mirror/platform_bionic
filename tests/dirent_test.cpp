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

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <set>
#include <string>

static void CheckProcSelf(std::set<std::string>& names) {
  // We have a good idea of what should be in /proc/self.
  ASSERT_TRUE(names.find(".") != names.end());
  ASSERT_TRUE(names.find("..") != names.end());
  ASSERT_TRUE(names.find("cmdline") != names.end());
  ASSERT_TRUE(names.find("fd") != names.end());
  ASSERT_TRUE(names.find("stat") != names.end());
}

TEST(dirent, scandir) {
  // Get everything from /proc/self...
  dirent** entries;
  int entry_count = scandir("/proc/self", &entries, NULL, alphasort);
  ASSERT_GE(entry_count, 0);

  // Turn the directory entries into a set and vector of the names.
  std::set<std::string> name_set;
  std::vector<std::string> unsorted_name_list;
  for (size_t i = 0; i < static_cast<size_t>(entry_count); ++i) {
    name_set.insert(entries[i]->d_name);
    unsorted_name_list.push_back(entries[i]->d_name);
    free(entries[i]);
  }
  free(entries);

  // No duplicates.
  ASSERT_EQ(name_set.size(), unsorted_name_list.size());

  // All entries sorted.
  std::vector<std::string> sorted_name_list(unsorted_name_list);
  std::sort(sorted_name_list.begin(), sorted_name_list.end());
  ASSERT_EQ(sorted_name_list, unsorted_name_list);

  CheckProcSelf(name_set);
}

TEST(dirent, fdopendir_invalid) {
  ASSERT_TRUE(fdopendir(-1) == NULL);
  ASSERT_EQ(EBADF, errno);

  int fd = open("/dev/null", O_RDONLY);
  ASSERT_NE(fd, -1);
  ASSERT_TRUE(fdopendir(fd) == NULL);
  ASSERT_EQ(ENOTDIR, errno);
  close(fd);
}

TEST(dirent, fdopendir) {
  int fd = open("/proc/self", O_RDONLY);
  DIR* d = fdopendir(fd);
  ASSERT_TRUE(d != NULL);
  dirent* e = readdir(d);
  ASSERT_STREQ(e->d_name, ".");
  ASSERT_EQ(closedir(d), 0);

  // fdopendir(3) took ownership, so closedir(3) closed our fd.
  ASSERT_EQ(close(fd), -1);
  ASSERT_EQ(EBADF, errno);
}

TEST(dirent, opendir_invalid) {
  ASSERT_TRUE(opendir("/does/not/exist") == NULL);
  ASSERT_EQ(ENOENT, errno);

  ASSERT_TRUE(opendir("/dev/null") == NULL);
  ASSERT_EQ(ENOTDIR, errno);
}

TEST(dirent, opendir) {
  DIR* d = opendir("/proc/self");
  ASSERT_TRUE(d != NULL);
  dirent* e = readdir(d);
  ASSERT_STREQ(e->d_name, ".");
  ASSERT_EQ(closedir(d), 0);
}

TEST(dirent, closedir_invalid) {
  DIR* d = NULL;
  ASSERT_EQ(closedir(d), -1);
  ASSERT_EQ(EINVAL, errno);
}

TEST(dirent, closedir) {
  DIR* d = opendir("/proc/self");
  ASSERT_TRUE(d != NULL);
  ASSERT_EQ(closedir(d), 0);
}

TEST(dirent, readdir) {
  DIR* d = opendir("/proc/self");
  ASSERT_TRUE(d != NULL);
  std::set<std::string> name_set;
  errno = 0;
  dirent* e;
  while ((e = readdir(d)) != NULL) {
    name_set.insert(e->d_name);
  }
  // Reading to the end of the directory is not an error.
  // readdir(3) returns NULL, but leaves errno as 0.
  ASSERT_EQ(0, errno);
  ASSERT_EQ(closedir(d), 0);

  CheckProcSelf(name_set);
}

TEST(dirent, readdir_r) {
  DIR* d = opendir("/proc/self");
  ASSERT_TRUE(d != NULL);
  std::set<std::string> name_set;
  errno = 0;
  dirent storage;
  dirent* e = NULL;
  while (readdir_r(d, &storage, &e) == 0 && e != NULL) {
    name_set.insert(e->d_name);
  }
  // Reading to the end of the directory is not an error.
  // readdir_r(3) returns NULL, but leaves errno as 0.
  ASSERT_EQ(0, errno);
  ASSERT_EQ(closedir(d), 0);

  CheckProcSelf(name_set);
}

TEST(dirent, rewinddir) {
  DIR* d = opendir("/proc/self");
  ASSERT_TRUE(d != NULL);

  // Get all the names once...
  std::vector<std::string> pass1;
  dirent* e;
  while ((e = readdir(d)) != NULL) {
    pass1.push_back(e->d_name);
  }

  // ...rewind...
  rewinddir(d);

  // ...and get all the names again.
  std::vector<std::string> pass2;
  while ((e = readdir(d)) != NULL) {
    pass2.push_back(e->d_name);
  }

  ASSERT_EQ(closedir(d), 0);

  // We should have seen the same names in the same order both times.
  ASSERT_EQ(pass1.size(), pass2.size());
  for (size_t i = 0; i < pass1.size(); ++i) {
    ASSERT_EQ(pass1[i], pass2[i]);
  }
}
