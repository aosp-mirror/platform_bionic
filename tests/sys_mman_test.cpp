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

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <sys/types.h>
#include <unistd.h>

#include <android-base/file.h>
#include <gtest/gtest.h>

TEST(sys_mman, mmap_std) {
  void* map = mmap(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  ASSERT_NE(MAP_FAILED, map);
  ASSERT_EQ(0, munmap(map, 4096));
}

TEST(sys_mman, mmap64_std) {
  void* map = mmap64(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  ASSERT_NE(MAP_FAILED, map);
  ASSERT_EQ(0, munmap(map, 4096));
}

TEST(sys_mman, mmap_file_bad_offset) {
  TemporaryFile tf;

  void* map = mmap(nullptr, 100, PROT_READ, MAP_SHARED, tf.fd, 1);
  ASSERT_EQ(MAP_FAILED, map);
}

TEST(sys_mman, mmap64_file_bad_offset) {
  TemporaryFile tf;

  void* map = mmap64(nullptr, 100, PROT_READ, MAP_SHARED, tf.fd, 1);
  ASSERT_EQ(MAP_FAILED, map);
}

#define STR_SSIZE(str) static_cast<ssize_t>(sizeof(str))

#define STRING_MSG  "012345678\nabcdefgh\n"
#define INITIAL_MSG "000000000\n00000000\n"

TEST(sys_mman, mmap_file_read) {
  TemporaryFile tf;

  ASSERT_EQ(STR_SSIZE(STRING_MSG), write(tf.fd, STRING_MSG, sizeof(STRING_MSG)));

  void* map = mmap(nullptr, sizeof(STRING_MSG), PROT_READ, MAP_SHARED, tf.fd, 0);
  ASSERT_NE(MAP_FAILED, map);

  char* data = reinterpret_cast<char*>(map);
  ASSERT_STREQ(STRING_MSG, data);

  ASSERT_EQ(0, munmap(map, sizeof(STRING_MSG)));
}

TEST(sys_mman, mmap_file_write) {
  TemporaryFile tf;

  ASSERT_EQ(STR_SSIZE(INITIAL_MSG), write(tf.fd, INITIAL_MSG, sizeof(INITIAL_MSG)));
  lseek(tf.fd, 0, SEEK_SET);

  void* map = mmap(nullptr, sizeof(STRING_MSG), PROT_WRITE, MAP_SHARED, tf.fd, 0);
  ASSERT_NE(MAP_FAILED, map);
  close(tf.fd);

  memcpy(map, STRING_MSG, sizeof(STRING_MSG));

  ASSERT_EQ(0, munmap(map, sizeof(STRING_MSG)));

  tf.fd = open(tf.path, O_RDWR);
  char buf[sizeof(STRING_MSG)];
  memset(buf, 0, sizeof(STRING_MSG));
  ASSERT_EQ(STR_SSIZE(STRING_MSG), read(tf.fd, buf, sizeof(STRING_MSG)));

  ASSERT_STREQ(STRING_MSG, buf);
}

#define PAGE0_MSG "00PAGE00"
#define PAGE1_MSG "111PAGE111"
#define PAGE2_MSG "2222PAGE2222"
#define END_MSG "E"

TEST(sys_mman, mmap_file_read_at_offset) {
  TemporaryFile tf;
  size_t pagesize = sysconf(_SC_PAGESIZE);

  // Create the file with three pages worth of data.
  ASSERT_EQ(STR_SSIZE(PAGE0_MSG), write(tf.fd, PAGE0_MSG, sizeof(PAGE0_MSG)));
  ASSERT_NE(-1, lseek(tf.fd, pagesize, SEEK_SET));
  ASSERT_EQ(STR_SSIZE(PAGE1_MSG), write(tf.fd, PAGE1_MSG, sizeof(PAGE1_MSG)));
  ASSERT_NE(-1, lseek(tf.fd, 2 * pagesize, SEEK_SET));
  ASSERT_EQ(STR_SSIZE(PAGE2_MSG), write(tf.fd, PAGE2_MSG, sizeof(PAGE2_MSG)));
  ASSERT_NE(-1, lseek(tf.fd, 3 * pagesize - sizeof(END_MSG), SEEK_SET));
  ASSERT_EQ(STR_SSIZE(END_MSG), write(tf.fd, END_MSG, sizeof(END_MSG)));

  ASSERT_NE(-1, lseek(tf.fd, 0, SEEK_SET));

  void* map = mmap(nullptr, pagesize, PROT_READ, MAP_SHARED, tf.fd, pagesize);
  ASSERT_NE(MAP_FAILED, map);

  char* data = reinterpret_cast<char*>(map);
  ASSERT_STREQ(PAGE1_MSG, data);

  ASSERT_EQ(0, munmap(map, pagesize));

  map = mmap(nullptr, pagesize, PROT_READ, MAP_SHARED, tf.fd, 2 * pagesize);
  ASSERT_NE(MAP_FAILED, map);

  data = reinterpret_cast<char*>(map);
  ASSERT_STREQ(PAGE2_MSG, data);
  ASSERT_STREQ(END_MSG, data+pagesize-sizeof(END_MSG));

  ASSERT_EQ(0, munmap(map, pagesize));
}

#define NEWPAGE1_MSG "1NEW1PAGE1"
#define NEWPAGE2_MSG "22NEW22PAGE22"

TEST(sys_mman, mmap_file_write_at_offset) {
  TemporaryFile tf;
  size_t pagesize = sysconf(_SC_PAGESIZE);

  // Create the file with three pages worth of data.
  ASSERT_EQ(STR_SSIZE(PAGE0_MSG), write(tf.fd, PAGE0_MSG, sizeof(PAGE0_MSG)));
  ASSERT_NE(-1, lseek(tf.fd, pagesize, SEEK_SET));
  ASSERT_EQ(STR_SSIZE(PAGE1_MSG), write(tf.fd, PAGE1_MSG, sizeof(PAGE1_MSG)));
  ASSERT_NE(-1, lseek(tf.fd, 2 * pagesize, SEEK_SET));
  ASSERT_EQ(STR_SSIZE(PAGE2_MSG), write(tf.fd, PAGE2_MSG, sizeof(PAGE2_MSG)));
  ASSERT_NE(-1, lseek(tf.fd, 3 * pagesize - sizeof(END_MSG), SEEK_SET));
  ASSERT_EQ(STR_SSIZE(END_MSG), write(tf.fd, END_MSG, sizeof(END_MSG)));

  ASSERT_NE(-1, lseek(tf.fd, 0, SEEK_SET));

  void* map = mmap(nullptr, pagesize, PROT_WRITE, MAP_SHARED, tf.fd, pagesize);
  ASSERT_NE(MAP_FAILED, map);
  close(tf.fd);

  memcpy(map, NEWPAGE1_MSG, sizeof(NEWPAGE1_MSG));
  ASSERT_EQ(0, munmap(map, pagesize));

  tf.fd = open(tf.path, O_RDWR);
  map = mmap(nullptr, pagesize, PROT_WRITE, MAP_SHARED, tf.fd, 2 * pagesize);
  ASSERT_NE(MAP_FAILED, map);
  close(tf.fd);

  memcpy(map, NEWPAGE2_MSG, sizeof(NEWPAGE2_MSG));
  ASSERT_EQ(0, munmap(map, pagesize));

  tf.fd = open(tf.path, O_RDWR);
  char buf[pagesize];
  ASSERT_EQ(static_cast<ssize_t>(pagesize), read(tf.fd, buf, pagesize));
  ASSERT_STREQ(PAGE0_MSG, buf);
  ASSERT_NE(-1, lseek(tf.fd, pagesize, SEEK_SET));
  ASSERT_EQ(static_cast<ssize_t>(pagesize), read(tf.fd, buf, pagesize));
  ASSERT_STREQ(NEWPAGE1_MSG, buf);
  ASSERT_NE(-1, lseek(tf.fd, 2 * pagesize, SEEK_SET));
  ASSERT_EQ(static_cast<ssize_t>(pagesize), read(tf.fd, buf, pagesize));
  ASSERT_STREQ(NEWPAGE2_MSG, buf);
  ASSERT_STREQ(END_MSG, buf+pagesize-sizeof(END_MSG));
}

TEST(sys_mman, posix_madvise) {
  TemporaryFile tempfile;
  size_t pagesize = sysconf(_SC_PAGESIZE);
  char buf[pagesize];

  // Prepare environment.
  ASSERT_EQ(static_cast<ssize_t>(pagesize), write(tempfile.fd, buf, pagesize));
  void* map = mmap(nullptr, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, tempfile.fd, 0);
  ASSERT_NE(MAP_FAILED, map);

  // Verify different options of posix_madvise.
  ASSERT_EQ(0, posix_madvise(map, pagesize, POSIX_MADV_NORMAL));
  ASSERT_EQ(0, posix_madvise(map, pagesize, POSIX_MADV_SEQUENTIAL));
  ASSERT_EQ(0, posix_madvise(map, pagesize, POSIX_MADV_RANDOM));
  ASSERT_EQ(0, posix_madvise(map, pagesize, POSIX_MADV_WILLNEED));

  ASSERT_EQ(0, munmap(map, pagesize));
}

// Verify that memory can still access after posix_madvise(POSIX_MADV_DONTNEED).
// We should test on MAP_ANONYMOUS memory to verify whether the memory is discarded,
// because the content of non MAP_ANONYMOUS memory can be reread from file.
TEST(sys_mman, posix_madvise_POSIX_MADV_DONTNEED) {
  size_t pagesize = sysconf(_SC_PAGESIZE);

  void* map = mmap(nullptr, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_NE(MAP_FAILED, map);

  int* int_ptr = reinterpret_cast<int*>(map);
  for (int i = 0; i < static_cast<int>(pagesize / sizeof(int)); ++i) {
    *int_ptr++ = i;
  }

  ASSERT_EQ(0, posix_madvise(map, pagesize, POSIX_MADV_DONTNEED));

  int_ptr = reinterpret_cast<int*>(map);
  for (int i = 0; i < static_cast<int>(pagesize / sizeof(int)); ++i) {
    ASSERT_EQ(i, *int_ptr++);
  }

  ASSERT_EQ(0, munmap(map, pagesize));
}

TEST(sys_mman, mremap) {
  ASSERT_EQ(MAP_FAILED, mremap(nullptr, 0, 0, 0));
}

constexpr size_t kHuge = size_t(PTRDIFF_MAX) + 1;

TEST(sys_mman, mmap_PTRDIFF_MAX) {
  ASSERT_EQ(MAP_FAILED, mmap(nullptr, kHuge, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

TEST(sys_mman, mremap_PTRDIFF_MAX) {
  void* map = mmap(nullptr, PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_NE(MAP_FAILED, map);
  ASSERT_EQ(MAP_FAILED, mremap(map, PAGE_SIZE, kHuge, MREMAP_MAYMOVE));
}

TEST(sys_mman, mmap_bug_27265969) {
  char* base = reinterpret_cast<char*>(mmap(nullptr, PAGE_SIZE * 2, PROT_EXEC | PROT_READ,
                                            MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
  // Some kernels had bugs that would cause segfaults here...
  __builtin___clear_cache(base, base + (PAGE_SIZE * 2));
}
