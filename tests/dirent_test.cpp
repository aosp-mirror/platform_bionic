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

template <typename DirEntT>
void ScanEntries(DirEntT** entries, int entry_count,
                 std::set<std::string>& name_set, std::vector<std::string>& name_list) {
  for (size_t i = 0; i < static_cast<size_t>(entry_count); ++i) {
    name_set.insert(entries[i]->d_name);
    name_list.push_back(entries[i]->d_name);
    free(entries[i]);
  }
  free(entries);
}

TEST(dirent, scandir_scandir64) {
  // Get everything from /proc/self...
  dirent** entries;
  int entry_count = scandir("/proc/self", &entries, NULL, alphasort);
  ASSERT_GE(entry_count, 0);

  dirent64** entries64;
  int entry_count64 = scandir64("/proc/self", &entries64, NULL, alphasort64);
  ASSERT_EQ(entry_count, entry_count64);

  // Turn the directory entries into a set and vector of the names.
  std::set<std::string> name_set;
  std::vector<std::string> unsorted_name_list;
  ScanEntries(entries, entry_count, name_set, unsorted_name_list);

  // No duplicates.
  ASSERT_EQ(name_set.size(), unsorted_name_list.size());

  // All entries sorted.
  std::vector<std::string> sorted_name_list(unsorted_name_list);
  std::sort(sorted_name_list.begin(), sorted_name_list.end());
  ASSERT_EQ(sorted_name_list, unsorted_name_list);

  // scandir64 returned the same results as scandir.
  std::set<std::string> name_set64;
  std::vector<std::string> unsorted_name_list64;
  ScanEntries(entries64, entry_count64, name_set64, unsorted_name_list64);
  ASSERT_EQ(name_set, name_set64);
  ASSERT_EQ(unsorted_name_list, unsorted_name_list64);

  CheckProcSelf(name_set);
}

TEST(dirent, scandirat_scandirat64) {
  // Get everything from /proc/self...
  dirent** entries;
  int entry_count = scandir("/proc/self", &entries, NULL, alphasort);
  ASSERT_GE(entry_count, 0);

  int proc_fd = open("/proc", O_DIRECTORY);
  ASSERT_NE(-1, proc_fd);

  dirent** entries_at;
  int entry_count_at = scandirat(proc_fd, "self", &entries_at, NULL, alphasort);
  ASSERT_EQ(entry_count, entry_count_at);

  dirent64** entries_at64;
  int entry_count_at64 = scandirat64(proc_fd, "self", &entries_at64, NULL, alphasort64);
  ASSERT_EQ(entry_count, entry_count_at64);

  close(proc_fd);

  // scandirat and scandirat64 should return the same results as scandir.
  std::set<std::string> name_set, name_set_at, name_set_at64;
  std::vector<std::string> unsorted_name_list, unsorted_name_list_at, unsorted_name_list_at64;
  ScanEntries(entries, entry_count, name_set, unsorted_name_list);
  ScanEntries(entries_at, entry_count_at, name_set_at, unsorted_name_list_at);
  ScanEntries(entries_at64, entry_count_at64, name_set_at64, unsorted_name_list_at64);

  ASSERT_EQ(name_set, name_set_at);
  ASSERT_EQ(name_set, name_set_at64);
  ASSERT_EQ(unsorted_name_list, unsorted_name_list_at);
  ASSERT_EQ(unsorted_name_list, unsorted_name_list_at64);
}

TEST(dirent, scandir_ENOENT) {
  dirent** entries;
  errno = 0;
  ASSERT_EQ(-1, scandir("/does-not-exist", &entries, nullptr, nullptr));
  ASSERT_EQ(ENOENT, errno);
}

TEST(dirent, scandir64_ENOENT) {
  dirent64** entries;
  errno = 0;
  ASSERT_EQ(-1, scandir64("/does-not-exist", &entries, nullptr, nullptr));
  ASSERT_EQ(ENOENT, errno);
}

TEST(dirent, scandirat_ENOENT) {
  int root_fd = open("/", O_DIRECTORY | O_RDONLY);
  ASSERT_NE(-1, root_fd);
  dirent** entries;
  errno = 0;
  ASSERT_EQ(-1, scandirat(root_fd, "does-not-exist", &entries, nullptr, nullptr));
  ASSERT_EQ(ENOENT, errno);
  close(root_fd);
}

TEST(dirent, scandirat64_ENOENT) {
  int root_fd = open("/", O_DIRECTORY | O_RDONLY);
  ASSERT_NE(-1, root_fd);
  dirent64** entries;
  errno = 0;
  ASSERT_EQ(-1, scandirat64(root_fd, "does-not-exist", &entries, nullptr, nullptr));
  ASSERT_EQ(ENOENT, errno);
  close(root_fd);
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

TEST(dirent, readdir64) {
  DIR* d = opendir("/proc/self");
  ASSERT_TRUE(d != NULL);
  std::set<std::string> name_set;
  errno = 0;
  dirent64* e;
  while ((e = readdir64(d)) != NULL) {
    name_set.insert(e->d_name);
  }
  // Reading to the end of the directory is not an error.
  // readdir64(3) returns NULL, but leaves errno as 0.
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

TEST(dirent, readdir64_r) {
  DIR* d = opendir("/proc/self");
  ASSERT_TRUE(d != NULL);
  std::set<std::string> name_set;
  errno = 0;
  dirent64 storage;
  dirent64* e = NULL;
  while (readdir64_r(d, &storage, &e) == 0 && e != NULL) {
    name_set.insert(e->d_name);
  }
  // Reading to the end of the directory is not an error.
  // readdir64_r(3) returns NULL, but leaves errno as 0.
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

TEST(dirent, seekdir_telldir) {
  DIR* d = opendir("/proc/self");
  ASSERT_TRUE(d != NULL);
  std::vector<long> offset_list;
  std::vector<std::string> name_list;
  dirent* e = NULL;

  offset_list.push_back(telldir(d));
  ASSERT_EQ(0L, offset_list.back());

  while ((e = readdir(d)) != NULL) {
    name_list.push_back(e->d_name);
    offset_list.push_back(telldir(d));
    // Make sure telldir() point to the next entry.
    ASSERT_EQ(e->d_off, offset_list.back());
  }

  long end_offset = telldir(d);
  // telldir() should not pass the end of the file.
  ASSERT_EQ(offset_list.back(), end_offset);
  offset_list.pop_back();

  for (size_t i = 0; i < offset_list.size(); ++i) {
    seekdir(d, offset_list[i]);
    ASSERT_EQ(offset_list[i], telldir(d));
    e = readdir(d);
    ASSERT_TRUE(e != NULL);
    ASSERT_STREQ(name_list[i].c_str(), e->d_name);
  }
  for (int i = static_cast<int>(offset_list.size()) - 1; i >= 0; --i) {
    seekdir(d, offset_list[i]);
    ASSERT_EQ(offset_list[i], telldir(d));
    e = readdir(d);
    ASSERT_TRUE(e != NULL);
    ASSERT_STREQ(name_list[i].c_str(), e->d_name);
  }

  // Seek to the end, read NULL.
  seekdir(d, end_offset);
  ASSERT_EQ(end_offset, telldir(d));
  errno = 0;
  ASSERT_EQ(NULL, readdir(d));
  ASSERT_EQ(0, errno);

  ASSERT_EQ(0, closedir(d));
}
