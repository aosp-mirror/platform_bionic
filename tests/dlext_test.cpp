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

#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <android/dlext.h>
#include <sys/mman.h>
#include <sys/wait.h>


#define ASSERT_DL_NOTNULL(ptr) \
    ASSERT_TRUE(ptr != NULL) << "dlerror: " << dlerror()

#define ASSERT_DL_ZERO(i) \
    ASSERT_EQ(0, i) << "dlerror: " << dlerror()

#define ASSERT_NOERROR(i) \
    ASSERT_NE(-1, i) << "errno: " << strerror(errno)


typedef int (*fn)(void);
#define LIBNAME "libdlext_test.so"
#define LIBSIZE 1024*1024 // how much address space to reserve for it


class DlExtTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    handle_ = NULL;
    // verify that we don't have the library loaded already
    ASSERT_EQ(NULL, dlsym(RTLD_DEFAULT, "getRandomNumber"));
    // call dlerror() to swallow the error, and check it was the one we wanted
    ASSERT_STREQ("undefined symbol: getRandomNumber", dlerror());
  }

  virtual void TearDown() {
    if (handle_ != NULL) {
      ASSERT_DL_ZERO(dlclose(handle_));
    }
  }

  void* handle_;
};

TEST_F(DlExtTest, ExtInfoNull) {
  handle_ = android_dlopen_ext(LIBNAME, RTLD_NOW, NULL);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, ExtInfoNoFlags) {
  android_dlextinfo extinfo;
  extinfo.flags = 0;
  handle_ = android_dlopen_ext(LIBNAME, RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, Reserved) {
  void* start = mmap(NULL, LIBSIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = LIBSIZE;
  handle_ = android_dlopen_ext(LIBNAME, RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_GE(f, start);
  EXPECT_LT(reinterpret_cast<void*>(f),
            reinterpret_cast<char*>(start) + LIBSIZE);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, ReservedTooSmall) {
  void* start = mmap(NULL, PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = PAGE_SIZE;
  handle_ = android_dlopen_ext(LIBNAME, RTLD_NOW, &extinfo);
  EXPECT_EQ(NULL, handle_);
}

TEST_F(DlExtTest, ReservedHint) {
  void* start = mmap(NULL, LIBSIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS_HINT;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = LIBSIZE;
  handle_ = android_dlopen_ext(LIBNAME, RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_GE(f, start);
  EXPECT_LT(reinterpret_cast<void*>(f),
            reinterpret_cast<char*>(start) + LIBSIZE);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, ReservedHintTooSmall) {
  void* start = mmap(NULL, PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS_HINT;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = PAGE_SIZE;
  handle_ = android_dlopen_ext(LIBNAME, RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_TRUE(f < start || (reinterpret_cast<void*>(f) >=
                            reinterpret_cast<char*>(start) + PAGE_SIZE));
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, RelroShareChildWrites) {
  void* start = mmap(NULL, LIBSIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = LIBSIZE;

  int relro_fd;
  char relro_file[PATH_MAX];
  const char* android_data = getenv("ANDROID_DATA");
  ASSERT_TRUE(android_data != NULL);
  snprintf(relro_file, sizeof(relro_file), "%s/local/tmp/libdlext_test.relro", android_data);
  relro_fd = open(relro_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS | ANDROID_DLEXT_WRITE_RELRO;
  ASSERT_NOERROR(relro_fd);
  extinfo.relro_fd = relro_fd;

  pid_t pid = fork();
  if (pid == 0) {
    // child process
    void* handle = android_dlopen_ext(LIBNAME, RTLD_NOW, &extinfo);
    if (handle == NULL) {
      fprintf(stderr, "in child: %s\n", dlerror());
      exit(1);
    }
    exit(0);
  }

  // continuing in parent
  ASSERT_NOERROR(close(relro_fd));
  ASSERT_NOERROR(pid);
  int status;
  ASSERT_EQ(pid, waitpid(pid, &status, 0));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(0, WEXITSTATUS(status));

  relro_fd = open(relro_file, O_RDONLY);
  ASSERT_NOERROR(relro_fd);
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS | ANDROID_DLEXT_USE_RELRO;
  extinfo.relro_fd = relro_fd;
  handle_ = android_dlopen_ext(LIBNAME, RTLD_NOW, &extinfo);
  ASSERT_NOERROR(close(relro_fd));

  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_EQ(4, f());
}
