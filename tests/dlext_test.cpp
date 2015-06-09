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
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <android/dlext.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <pagemap/pagemap.h>

#include "TemporaryFile.h"

#define ASSERT_DL_NOTNULL(ptr) \
    ASSERT_TRUE(ptr != nullptr) << "dlerror: " << dlerror()

#define ASSERT_DL_ZERO(i) \
    ASSERT_EQ(0, i) << "dlerror: " << dlerror()

#define ASSERT_NOERROR(i) \
    ASSERT_NE(-1, i) << "errno: " << strerror(errno)

#define ASSERT_SUBSTR(needle, haystack) \
    ASSERT_PRED_FORMAT2(::testing::IsSubstring, needle, haystack)


typedef int (*fn)(void);
#define LIBNAME "libdlext_test.so"
#define LIBNAME_NORELRO "libdlext_test_norelro.so"
#define LIBSIZE 1024*1024 // how much address space to reserve for it

#if defined(__LP64__)
#define LIBPATH_PREFIX "/nativetest64/libdlext_test_fd/"
#else
#define LIBPATH_PREFIX "/nativetest/libdlext_test_fd/"
#endif

#define LIBPATH LIBPATH_PREFIX "libdlext_test_fd.so"
#define LIBZIPPATH LIBPATH_PREFIX "libdlext_test_fd_zipaligned.zip"

#define LIBZIP_OFFSET 2*PAGE_SIZE

class DlExtTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    handle_ = nullptr;
    // verify that we don't have the library loaded already
    void* h = dlopen(LIBNAME, RTLD_NOW | RTLD_NOLOAD);
    ASSERT_TRUE(h == nullptr);
    h = dlopen(LIBNAME_NORELRO, RTLD_NOW | RTLD_NOLOAD);
    ASSERT_TRUE(h == nullptr);
    // call dlerror() to swallow the error, and check it was the one we wanted
    ASSERT_STREQ("dlopen failed: library \"" LIBNAME_NORELRO "\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());
  }

  virtual void TearDown() {
    if (handle_ != nullptr) {
      ASSERT_DL_ZERO(dlclose(handle_));
    }
  }

  void* handle_;
};

TEST_F(DlExtTest, ExtInfoNull) {
  handle_ = android_dlopen_ext(LIBNAME, RTLD_NOW, nullptr);
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

TEST_F(DlExtTest, ExtInfoUseFd) {
  const std::string lib_path = std::string(getenv("ANDROID_DATA")) + LIBPATH;

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_LIBRARY_FD;
  extinfo.library_fd = TEMP_FAILURE_RETRY(open(lib_path.c_str(), O_RDONLY | O_CLOEXEC));
  ASSERT_TRUE(extinfo.library_fd != -1);
  handle_ = android_dlopen_ext(lib_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, ExtInfoUseFdWithOffset) {
  const std::string lib_path = std::string(getenv("ANDROID_DATA")) + LIBZIPPATH;

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_LIBRARY_FD | ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET;
  extinfo.library_fd = TEMP_FAILURE_RETRY(open(lib_path.c_str(), O_RDONLY | O_CLOEXEC));
  extinfo.library_fd_offset = LIBZIP_OFFSET;

  handle_ = android_dlopen_ext(lib_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);

  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, ExtInfoUseFdWithInvalidOffset) {
  const std::string lib_path = std::string(getenv("ANDROID_DATA")) + LIBZIPPATH;
  // lib_path is relative when $ANDROID_DATA is relative
  char lib_realpath_buf[PATH_MAX];
  ASSERT_TRUE(realpath(lib_path.c_str(), lib_realpath_buf) == lib_realpath_buf);
  const std::string lib_realpath = std::string(lib_realpath_buf);

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_LIBRARY_FD | ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET;
  extinfo.library_fd = TEMP_FAILURE_RETRY(open(lib_path.c_str(), O_RDONLY | O_CLOEXEC));
  extinfo.library_fd_offset = 17;

  handle_ = android_dlopen_ext("libname_placeholder", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle_ == nullptr);
  ASSERT_STREQ("dlopen failed: file offset for the library \"libname_placeholder\" is not page-aligned: 17", dlerror());

  // Test an address above 2^44, for http://b/18178121 .
  extinfo.library_fd_offset = (5LL<<48) + PAGE_SIZE;
  handle_ = android_dlopen_ext("libname_placeholder", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle_ == nullptr);
  ASSERT_SUBSTR("dlopen failed: file offset for the library \"libname_placeholder\" >= file size", dlerror());

  extinfo.library_fd_offset = 0LL - PAGE_SIZE;
  handle_ = android_dlopen_ext("libname_placeholder", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle_ == nullptr);
  ASSERT_SUBSTR("dlopen failed: file offset for the library \"libname_placeholder\" is negative", dlerror());

  extinfo.library_fd_offset = PAGE_SIZE;
  handle_ = android_dlopen_ext("libname_ignored", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle_ == nullptr);
  ASSERT_EQ("dlopen failed: \"" + lib_realpath + "\" has bad ELF magic", dlerror());

  close(extinfo.library_fd);
}

TEST_F(DlExtTest, ExtInfoUseOffsetWihtoutFd) {
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET;
  extinfo.library_fd_offset = LIBZIP_OFFSET;

  handle_ = android_dlopen_ext("/some/lib/that/does_not_exist", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle_ == nullptr);
  ASSERT_STREQ("dlopen failed: invalid extended flag combination (ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET without ANDROID_DLEXT_USE_LIBRARY_FD): 0x20", dlerror());
}

TEST(dlext, android_dlopen_ext_force_load_smoke) {
  // 1. Open actual file
  void* handle = dlopen("libdlext_test.so", RTLD_NOW);
  ASSERT_DL_NOTNULL(handle);
  // 2. Open link with force_load flag set
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_FORCE_LOAD;
  void* handle2 = android_dlopen_ext("libdlext_test_v2.so", RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle2);
  ASSERT_TRUE(handle != handle2);

  dlclose(handle2);
  dlclose(handle);
}

TEST(dlext, android_dlopen_ext_force_load_soname_exception) {
  // Check if soname lookup still returns already loaded library
  // when ANDROID_DLEXT_FORCE_LOAD flag is specified.
  void* handle = dlopen("libdlext_test_v2.so", RTLD_NOW);
  ASSERT_DL_NOTNULL(handle);

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_FORCE_LOAD;

  // Note that 'libdlext_test.so' is dt_soname for libdlext_test_v2.so
  void* handle2 = android_dlopen_ext("libdlext_test.so", RTLD_NOW, &extinfo);

  ASSERT_DL_NOTNULL(handle2);
  ASSERT_TRUE(handle == handle2);

  dlclose(handle2);
  dlclose(handle);
}

TEST(dlfcn, dlopen_from_zip_absolute_path) {
  const std::string lib_path = std::string(getenv("ANDROID_DATA")) + LIBZIPPATH;

  void* handle = dlopen((lib_path + "!/libdir/libdlext_test_fd.so").c_str(), RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  int (*fn)(void);
  fn = reinterpret_cast<int (*)(void)>(dlsym(handle, "getRandomNumber"));
  ASSERT_TRUE(fn != nullptr);
  EXPECT_EQ(4, fn());

  dlclose(handle);
}

TEST(dlfcn, dlopen_from_zip_ld_library_path) {
  const std::string lib_path = std::string(getenv("ANDROID_DATA")) + LIBZIPPATH + "!/libdir";

  typedef void (*fn_t)(const char*);
  fn_t android_update_LD_LIBRARY_PATH =
      reinterpret_cast<fn_t>(dlsym(RTLD_DEFAULT, "android_update_LD_LIBRARY_PATH"));

  ASSERT_TRUE(android_update_LD_LIBRARY_PATH != nullptr) << dlerror();

  void* handle = dlopen("libdlext_test_fd.so", RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);

  android_update_LD_LIBRARY_PATH(lib_path.c_str());

  handle = dlopen("libdlext_test_fd.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  int (*fn)(void);
  fn = reinterpret_cast<int (*)(void)>(dlsym(handle, "getRandomNumber"));
  ASSERT_TRUE(fn != nullptr);
  EXPECT_EQ(4, fn());

  dlclose(handle);
}


TEST_F(DlExtTest, Reserved) {
  void* start = mmap(nullptr, LIBSIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
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
  EXPECT_GE(reinterpret_cast<void*>(f), start);
  EXPECT_LT(reinterpret_cast<void*>(f),
            reinterpret_cast<char*>(start) + LIBSIZE);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, ReservedTooSmall) {
  void* start = mmap(nullptr, PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = PAGE_SIZE;
  handle_ = android_dlopen_ext(LIBNAME, RTLD_NOW, &extinfo);
  EXPECT_EQ(nullptr, handle_);
}

TEST_F(DlExtTest, ReservedHint) {
  void* start = mmap(nullptr, LIBSIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
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
  EXPECT_GE(reinterpret_cast<void*>(f), start);
  EXPECT_LT(reinterpret_cast<void*>(f),
            reinterpret_cast<char*>(start) + LIBSIZE);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, ReservedHintTooSmall) {
  void* start = mmap(nullptr, PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
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
  EXPECT_TRUE(reinterpret_cast<void*>(f) < start ||
              (reinterpret_cast<void*>(f) >=
               reinterpret_cast<char*>(start) + PAGE_SIZE));
  EXPECT_EQ(4, f());
}

class DlExtRelroSharingTest : public DlExtTest {
protected:
  virtual void SetUp() {
    DlExtTest::SetUp();
    void* start = mmap(nullptr, LIBSIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
                       -1, 0);
    ASSERT_TRUE(start != MAP_FAILED);
    extinfo_.flags = ANDROID_DLEXT_RESERVED_ADDRESS;
    extinfo_.reserved_addr = start;
    extinfo_.reserved_size = LIBSIZE;
    extinfo_.relro_fd = -1;
  }

  virtual void TearDown() {
    DlExtTest::TearDown();
  }

  void CreateRelroFile(const char* lib, const char* relro_file) {
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC);
    ASSERT_NOERROR(relro_fd);

    pid_t pid = fork();
    if (pid == 0) {
      // child process
      extinfo_.flags |= ANDROID_DLEXT_WRITE_RELRO;
      extinfo_.relro_fd = relro_fd;
      void* handle = android_dlopen_ext(lib, RTLD_NOW, &extinfo_);
      if (handle == nullptr) {
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

    // reopen file for reading so it can be used
    relro_fd = open(relro_file, O_RDONLY);
    ASSERT_NOERROR(relro_fd);
    extinfo_.flags |= ANDROID_DLEXT_USE_RELRO;
    extinfo_.relro_fd = relro_fd;
  }

  void TryUsingRelro(const char* lib) {
    handle_ = android_dlopen_ext(lib, RTLD_NOW, &extinfo_);
    ASSERT_DL_NOTNULL(handle_);
    fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
    ASSERT_DL_NOTNULL(f);
    EXPECT_EQ(4, f());
  }

  void SpawnChildrenAndMeasurePss(const char* lib, bool share_relro, size_t* pss_out);

  android_dlextinfo extinfo_;
};

TEST_F(DlExtRelroSharingTest, ChildWritesGoodData) {
  TemporaryFile tf; // Use tf to get an unique filename.
  ASSERT_NOERROR(close(tf.fd));

  ASSERT_NO_FATAL_FAILURE(CreateRelroFile(LIBNAME, tf.filename));
  ASSERT_NO_FATAL_FAILURE(TryUsingRelro(LIBNAME));

  // Use destructor of tf to close and unlink the file.
  tf.fd = extinfo_.relro_fd;
}

TEST_F(DlExtRelroSharingTest, ChildWritesNoRelro) {
  TemporaryFile tf; // // Use tf to get an unique filename.
  ASSERT_NOERROR(close(tf.fd));

  ASSERT_NO_FATAL_FAILURE(CreateRelroFile(LIBNAME_NORELRO, tf.filename));
  ASSERT_NO_FATAL_FAILURE(TryUsingRelro(LIBNAME_NORELRO));

  // Use destructor of tf to close and unlink the file.
  tf.fd = extinfo_.relro_fd;
}

TEST_F(DlExtRelroSharingTest, RelroFileEmpty) {
  ASSERT_NO_FATAL_FAILURE(TryUsingRelro(LIBNAME));
}

TEST_F(DlExtRelroSharingTest, VerifyMemorySaving) {
  if (geteuid() != 0) {
    GTEST_LOG_(INFO) << "This test must be run as root.\n";
    return;
  }

  TemporaryFile tf; // Use tf to get an unique filename.
  ASSERT_NOERROR(close(tf.fd));

  ASSERT_NO_FATAL_FAILURE(CreateRelroFile(LIBNAME, tf.filename));

  int pipefd[2];
  ASSERT_NOERROR(pipe(pipefd));

  size_t without_sharing, with_sharing;
  ASSERT_NO_FATAL_FAILURE(SpawnChildrenAndMeasurePss(LIBNAME, false, &without_sharing));
  ASSERT_NO_FATAL_FAILURE(SpawnChildrenAndMeasurePss(LIBNAME, true, &with_sharing));

  // We expect the sharing to save at least 10% of the total PSS. In practice
  // it saves 40%+ for this test.
  size_t expected_size = without_sharing - (without_sharing/10);
  EXPECT_LT(with_sharing, expected_size);

  // Use destructor of tf to close and unlink the file.
  tf.fd = extinfo_.relro_fd;
}

void getPss(pid_t pid, size_t* pss_out) {
  pm_kernel_t* kernel;
  ASSERT_EQ(0, pm_kernel_create(&kernel));

  pm_process_t* process;
  ASSERT_EQ(0, pm_process_create(kernel, pid, &process));

  pm_map_t** maps;
  size_t num_maps;
  ASSERT_EQ(0, pm_process_maps(process, &maps, &num_maps));

  size_t total_pss = 0;
  for (size_t i = 0; i < num_maps; i++) {
    pm_memusage_t usage;
    ASSERT_EQ(0, pm_map_usage(maps[i], &usage));
    total_pss += usage.pss;
  }
  *pss_out = total_pss;

  free(maps);
  pm_process_destroy(process);
  pm_kernel_destroy(kernel);
}

void DlExtRelroSharingTest::SpawnChildrenAndMeasurePss(const char* lib, bool share_relro,
                                                       size_t* pss_out) {
  const int CHILDREN = 20;

  // Create children
  pid_t childpid[CHILDREN];
  int childpipe[CHILDREN];
  for (int i=0; i<CHILDREN; ++i) {
    char read_buf;
    int child_done_pipe[2], parent_done_pipe[2];
    ASSERT_NOERROR(pipe(child_done_pipe));
    ASSERT_NOERROR(pipe(parent_done_pipe));

    pid_t child = fork();
    if (child == 0) {
      // close the 'wrong' ends of the pipes in the child
      close(child_done_pipe[0]);
      close(parent_done_pipe[1]);

      // open the library
      void* handle;
      if (share_relro) {
        handle = android_dlopen_ext(lib, RTLD_NOW, &extinfo_);
      } else {
        handle = dlopen(lib, RTLD_NOW);
      }
      if (handle == nullptr) {
        fprintf(stderr, "in child: %s\n", dlerror());
        exit(1);
      }

      // close write end of child_done_pipe to signal the parent that we're done.
      close(child_done_pipe[1]);

      // wait for the parent to close parent_done_pipe, then exit
      read(parent_done_pipe[0], &read_buf, 1);
      exit(0);
    }

    ASSERT_NOERROR(child);

    // close the 'wrong' ends of the pipes in the parent
    close(child_done_pipe[1]);
    close(parent_done_pipe[0]);

    // wait for the child to be done
    read(child_done_pipe[0], &read_buf, 1);
    close(child_done_pipe[0]);

    // save the child's pid and the parent_done_pipe
    childpid[i] = child;
    childpipe[i] = parent_done_pipe[1];
  }

  // Sum the PSS of all the children
  size_t total_pss = 0;
  for (int i=0; i<CHILDREN; ++i) {
    size_t child_pss;
    ASSERT_NO_FATAL_FAILURE(getPss(childpid[i], &child_pss));
    total_pss += child_pss;
  }
  *pss_out = total_pss;

  // Close pipes and wait for children to exit
  for (int i=0; i<CHILDREN; ++i) {
    ASSERT_NOERROR(close(childpipe[i]));
  }
  for (int i=0; i<CHILDREN; ++i) {
    int status;
    ASSERT_EQ(childpid[i], waitpid(childpid[i], &status, 0));
    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_EQ(0, WEXITSTATUS(status));
  }
}
