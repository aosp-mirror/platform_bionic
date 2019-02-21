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
#include <android-base/file.h>
#include <android-base/strings.h>

#include <linux/memfd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <sys/wait.h>

#include <meminfo/procmeminfo.h>
#include <procinfo/process_map.h>
#include <ziparchive/zip_archive.h>

#include "gtest_globals.h"
#include "utils.h"
#include "dlext_private.h"
#include "dlfcn_symlink_support.h"

#define ASSERT_DL_NOTNULL(ptr) \
    ASSERT_TRUE((ptr) != nullptr) << "dlerror: " << dlerror()

#define ASSERT_DL_ZERO(i) \
    ASSERT_EQ(0, i) << "dlerror: " << dlerror()

#define ASSERT_NOERROR(i) \
    ASSERT_NE(-1, i) << "errno: " << strerror(errno)

#define ASSERT_SUBSTR(needle, haystack) \
    ASSERT_PRED_FORMAT2(::testing::IsSubstring, needle, haystack)


typedef int (*fn)(void);
constexpr const char* kLibName = "libdlext_test.so";
constexpr const char* kLibNameRecursive = "libdlext_test_recursive.so";
constexpr const char* kLibNameNoRelro = "libdlext_test_norelro.so";
constexpr const char* kLibZipSimpleZip = "libdir/libatest_simple_zip.so";
constexpr auto kLibSize = 1024 * 1024; // how much address space to reserve for it

class DlExtTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    handle_ = nullptr;
    // verify that we don't have the library loaded already
    void* h = dlopen(kLibName, RTLD_NOW | RTLD_NOLOAD);
    ASSERT_TRUE(h == nullptr);
    h = dlopen(kLibNameNoRelro, RTLD_NOW | RTLD_NOLOAD);
    ASSERT_TRUE(h == nullptr);
    // call dlerror() to swallow the error, and check it was the one we wanted
    ASSERT_EQ(std::string("dlopen failed: library \"") + kLibNameNoRelro + "\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());
  }

  virtual void TearDown() {
    if (handle_ != nullptr) {
      ASSERT_DL_ZERO(dlclose(handle_));
    }
  }

  void* handle_;
};

TEST_F(DlExtTest, ExtInfoNull) {
  handle_ = android_dlopen_ext(kLibName, RTLD_NOW, nullptr);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, ExtInfoNoFlags) {
  android_dlextinfo extinfo;
  extinfo.flags = 0;
  handle_ = android_dlopen_ext(kLibName, RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, ExtInfoUseFd) {
  const std::string lib_path = GetTestlibRoot() + "/libdlext_test_fd/libdlext_test_fd.so";

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_LIBRARY_FD;
  extinfo.library_fd = TEMP_FAILURE_RETRY(open(lib_path.c_str(), O_RDONLY | O_CLOEXEC));
  ASSERT_TRUE(extinfo.library_fd != -1);
  handle_ = android_dlopen_ext(lib_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_EQ(4, f());

  uint32_t* taxicab_number = reinterpret_cast<uint32_t*>(dlsym(handle_, "dlopen_testlib_taxicab_number"));
  ASSERT_DL_NOTNULL(taxicab_number);
  EXPECT_EQ(1729U, *taxicab_number);
}

TEST_F(DlExtTest, ExtInfoUseFdWithOffset) {
  const std::string lib_path = GetTestlibRoot() + "/libdlext_test_zip/libdlext_test_zip_zipaligned.zip";

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_LIBRARY_FD | ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET;
  extinfo.library_fd = TEMP_FAILURE_RETRY(open(lib_path.c_str(), O_RDONLY | O_CLOEXEC));

  // Find the offset of the shared library in the zip.
  ZipArchiveHandle handle;
  ASSERT_EQ(0, OpenArchive(lib_path.c_str(), &handle));
  ZipEntry zip_entry;
  ZipString zip_name;
  zip_name.name = reinterpret_cast<const uint8_t*>(kLibZipSimpleZip);
  zip_name.name_length = strlen(kLibZipSimpleZip);
  ASSERT_EQ(0, FindEntry(handle, zip_name, &zip_entry));
  extinfo.library_fd_offset = zip_entry.offset;
  CloseArchive(handle);

  handle_ = android_dlopen_ext(lib_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);

  uint32_t* taxicab_number = reinterpret_cast<uint32_t*>(dlsym(handle_, "dlopen_testlib_taxicab_number"));
  ASSERT_DL_NOTNULL(taxicab_number);
  EXPECT_EQ(1729U, *taxicab_number);
}

TEST_F(DlExtTest, ExtInfoUseFdWithInvalidOffset) {
  const std::string lib_path = GetTestlibRoot() + "/libdlext_test_zip/libdlext_test_zip_zipaligned.zip";

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

  extinfo.library_fd_offset = 0;
  handle_ = android_dlopen_ext("libname_ignored", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle_ == nullptr);
  ASSERT_EQ("dlopen failed: \"" + lib_path + "\" has bad ELF magic: 504b0304", dlerror());

  // Check if dlsym works after unsuccessful dlopen().
  // Supply non-exiting one to make linker visit every soinfo.
  void* sym = dlsym(RTLD_DEFAULT, "this_symbol_does_not_exist___");
  ASSERT_TRUE(sym == nullptr);

  close(extinfo.library_fd);
}

TEST_F(DlExtTest, ExtInfoUseOffsetWithoutFd) {
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET;
  // This offset will not be used, so it doesn't matter.
  extinfo.library_fd_offset = 0;

  handle_ = android_dlopen_ext("/some/lib/that/does_not_exist", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle_ == nullptr);
  ASSERT_STREQ("dlopen failed: invalid extended flag combination (ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET without ANDROID_DLEXT_USE_LIBRARY_FD): 0x20", dlerror());
}

TEST(dlext, android_dlopen_ext_force_load_smoke) {
  DlfcnSymlink symlink("android_dlopen_ext_force_load_smoke");
  const std::string symlink_name = basename(symlink.get_symlink_path().c_str());
  // 1. Open actual file
  void* handle = dlopen("libdlext_test.so", RTLD_NOW);
  ASSERT_DL_NOTNULL(handle);
  // 2. Open link with force_load flag set
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_FORCE_LOAD;
  void* handle2 = android_dlopen_ext(symlink_name.c_str(), RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle2);
  ASSERT_TRUE(handle != handle2);

  dlclose(handle2);
  dlclose(handle);
}

TEST(dlext, android_dlopen_ext_force_load_soname_exception) {
  DlfcnSymlink symlink("android_dlopen_ext_force_load_soname_exception");
  const std::string symlink_name = basename(symlink.get_symlink_path().c_str());
  // Check if soname lookup still returns already loaded library
  // when ANDROID_DLEXT_FORCE_LOAD flag is specified.
  void* handle = dlopen(symlink_name.c_str(), RTLD_NOW);
  ASSERT_DL_NOTNULL(handle);

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_FORCE_LOAD;

  // Note that 'libdlext_test.so' is dt_soname for the symlink_name
  void* handle2 = android_dlopen_ext("libdlext_test.so", RTLD_NOW, &extinfo);

  ASSERT_DL_NOTNULL(handle2);
  ASSERT_TRUE(handle == handle2);

  dlclose(handle2);
  dlclose(handle);
}

TEST(dlfcn, dlopen_from_nullptr_android_api_level) {
  // Regression test for http://b/123972211. Testing dlopen(nullptr) when target sdk is P
  android_set_application_target_sdk_version(__ANDROID_API_P__);
  ASSERT_TRUE(dlopen(nullptr, RTLD_NOW) != nullptr);
}

TEST(dlfcn, dlopen_from_zip_absolute_path) {
  const std::string lib_zip_path = "/libdlext_test_zip/libdlext_test_zip_zipaligned.zip";
  const std::string lib_path = GetTestlibRoot() + lib_zip_path;

  void* handle = dlopen((lib_path + "!/libdir/libatest_simple_zip.so").c_str(), RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  uint32_t* taxicab_number = reinterpret_cast<uint32_t*>(dlsym(handle, "dlopen_testlib_taxicab_number"));
  ASSERT_DL_NOTNULL(taxicab_number);
  EXPECT_EQ(1729U, *taxicab_number);

  dlclose(handle);
}

TEST(dlfcn, dlopen_from_zip_with_dt_runpath) {
  const std::string lib_zip_path = "/libdlext_test_runpath_zip/libdlext_test_runpath_zip_zipaligned.zip";
  const std::string lib_path = GetTestlibRoot() + lib_zip_path;

  void* handle = dlopen((lib_path + "!/libdir/libtest_dt_runpath_d_zip.so").c_str(), RTLD_NOW);

  ASSERT_TRUE(handle != nullptr) << dlerror();

  typedef void *(* dlopen_b_fn)();
  dlopen_b_fn fn = (dlopen_b_fn)dlsym(handle, "dlopen_b");
  ASSERT_TRUE(fn != nullptr) << dlerror();

  void *p = fn();
  ASSERT_TRUE(p != nullptr) << dlerror();

  dlclose(p);
  dlclose(handle);
}

TEST(dlfcn, dlopen_from_zip_ld_library_path) {
  const std::string lib_zip_path = "/libdlext_test_zip/libdlext_test_zip_zipaligned.zip";
  const std::string lib_path = GetTestlibRoot() + lib_zip_path + "!/libdir";

  typedef void (*fn_t)(const char*);
  fn_t android_update_LD_LIBRARY_PATH =
      reinterpret_cast<fn_t>(dlsym(RTLD_DEFAULT, "android_update_LD_LIBRARY_PATH"));

  ASSERT_TRUE(android_update_LD_LIBRARY_PATH != nullptr) << dlerror();

  void* handle = dlopen("libdlext_test_zip.so", RTLD_NOW);
  ASSERT_TRUE(handle == nullptr);

  android_update_LD_LIBRARY_PATH(lib_path.c_str());

  handle = dlopen("libdlext_test_zip.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  int (*fn)(void);
  fn = reinterpret_cast<int (*)(void)>(dlsym(handle, "getRandomNumber"));
  ASSERT_TRUE(fn != nullptr);
  EXPECT_EQ(4, fn());

  uint32_t* taxicab_number =
          reinterpret_cast<uint32_t*>(dlsym(handle, "dlopen_testlib_taxicab_number"));
  ASSERT_DL_NOTNULL(taxicab_number);
  EXPECT_EQ(1729U, *taxicab_number);

  dlclose(handle);
}


TEST_F(DlExtTest, Reserved) {
  void* start = mmap(nullptr, kLibSize, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = kLibSize;
  handle_ = android_dlopen_ext(kLibName, RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_GE(reinterpret_cast<void*>(f), start);
  EXPECT_LT(reinterpret_cast<void*>(f),
            reinterpret_cast<char*>(start) + kLibSize);
  EXPECT_EQ(4, f());

  // Check that after dlclose reserved address space is unmapped (and can be reused)
  dlclose(handle_);
  handle_ = nullptr;

  void* new_start = mmap(start, PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_NE(start, new_start) << "dlclose unmapped reserved space";
}

TEST_F(DlExtTest, ReservedTooSmall) {
  void* start = mmap(nullptr, PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = PAGE_SIZE;
  handle_ = android_dlopen_ext(kLibName, RTLD_NOW, &extinfo);
  EXPECT_EQ(nullptr, handle_);
}

TEST_F(DlExtTest, ReservedRecursive) {
  void* start = mmap(nullptr, kLibSize, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS | ANDROID_DLEXT_RESERVED_ADDRESS_RECURSIVE;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = kLibSize;
  handle_ = android_dlopen_ext(kLibNameRecursive, RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);

  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_GE(reinterpret_cast<void*>(f), start);
  EXPECT_LT(reinterpret_cast<void*>(f),
            reinterpret_cast<char*>(start) + kLibSize);
  EXPECT_EQ(4, f());

  f = reinterpret_cast<fn>(dlsym(handle_, "getBiggerRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_GE(reinterpret_cast<void*>(f), start);
  EXPECT_LT(reinterpret_cast<void*>(f),
            reinterpret_cast<char*>(start) + kLibSize);
  EXPECT_EQ(8, f());

  uint32_t* taxicab_number = reinterpret_cast<uint32_t*>(dlsym(handle_, "dlopen_testlib_taxicab_number"));
  ASSERT_DL_NOTNULL(taxicab_number);
  EXPECT_GE(reinterpret_cast<void*>(taxicab_number), start);
  EXPECT_LT(reinterpret_cast<void*>(taxicab_number), reinterpret_cast<char*>(start) + kLibSize);
  EXPECT_EQ(1729U, *taxicab_number);
}

TEST_F(DlExtTest, ReservedRecursiveTooSmall) {
  void* start = mmap(nullptr, kLibSize/2, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS | ANDROID_DLEXT_RESERVED_ADDRESS_RECURSIVE;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = kLibSize/2;
  handle_ = android_dlopen_ext(kLibNameRecursive, RTLD_NOW, &extinfo);
  EXPECT_EQ(nullptr, handle_);
}

TEST_F(DlExtTest, ReservedHint) {
  void* start = mmap(nullptr, kLibSize, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS_HINT;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = kLibSize;
  handle_ = android_dlopen_ext(kLibName, RTLD_NOW, &extinfo);
  ASSERT_DL_NOTNULL(handle_);
  fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
  ASSERT_DL_NOTNULL(f);
  EXPECT_GE(reinterpret_cast<void*>(f), start);
  EXPECT_LT(reinterpret_cast<void*>(f),
            reinterpret_cast<char*>(start) + kLibSize);
  EXPECT_EQ(4, f());
}

TEST_F(DlExtTest, ReservedHintTooSmall) {
  void* start = mmap(nullptr, PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_TRUE(start != MAP_FAILED);
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_RESERVED_ADDRESS_HINT;
  extinfo.reserved_addr = start;
  extinfo.reserved_size = PAGE_SIZE;
  handle_ = android_dlopen_ext(kLibName, RTLD_NOW, &extinfo);
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
    void* start = mmap(nullptr, kLibSize, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_TRUE(start != MAP_FAILED);
    extinfo_.flags = ANDROID_DLEXT_RESERVED_ADDRESS;
    extinfo_.reserved_addr = start;
    extinfo_.reserved_size = kLibSize;
    extinfo_.relro_fd = -1;
  }

  virtual void TearDown() {
    DlExtTest::TearDown();
  }

  void CreateRelroFile(const char* lib, const char* relro_file, bool recursive) {
    int relro_fd = open(relro_file, O_RDWR | O_TRUNC | O_CLOEXEC);
    ASSERT_NOERROR(relro_fd);

    if (recursive) {
      extinfo_.flags |= ANDROID_DLEXT_RESERVED_ADDRESS_RECURSIVE;
    }

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
    AssertChildExited(pid, 0);

    // reopen file for reading so it can be used
    relro_fd = open(relro_file, O_RDONLY | O_CLOEXEC);
    ASSERT_NOERROR(relro_fd);
    extinfo_.flags |= ANDROID_DLEXT_USE_RELRO;
    extinfo_.relro_fd = relro_fd;
  }

  void TryUsingRelro(const char* lib, bool recursive) {
    handle_ = android_dlopen_ext(lib, RTLD_NOW, &extinfo_);
    ASSERT_DL_NOTNULL(handle_);
    fn f = reinterpret_cast<fn>(dlsym(handle_, "getRandomNumber"));
    ASSERT_DL_NOTNULL(f);
    EXPECT_EQ(4, f());

    if (recursive) {
      fn f = reinterpret_cast<fn>(dlsym(handle_, "getBiggerRandomNumber"));
      ASSERT_DL_NOTNULL(f);
      EXPECT_EQ(8, f());
    }

    uint32_t* taxicab_number =
            reinterpret_cast<uint32_t*>(dlsym(handle_, "dlopen_testlib_taxicab_number"));
    ASSERT_DL_NOTNULL(taxicab_number);
    EXPECT_EQ(1729U, *taxicab_number);
  }

  void SpawnChildrenAndMeasurePss(const char* lib, const char* relro_file, bool share_relro,
                                  size_t* pss_out);

  std::string FindMappingName(void* ptr);

  android_dlextinfo extinfo_;
};

TEST_F(DlExtRelroSharingTest, ChildWritesGoodData) {
  TemporaryFile tf; // Use tf to get an unique filename.
  ASSERT_NOERROR(close(tf.fd));

  ASSERT_NO_FATAL_FAILURE(CreateRelroFile(kLibName, tf.path, false));
  ASSERT_NO_FATAL_FAILURE(TryUsingRelro(kLibName, false));
  void* relro_data = dlsym(handle_, "lots_of_relro");
  ASSERT_DL_NOTNULL(relro_data);
  EXPECT_EQ(tf.path, FindMappingName(relro_data));

  // Use destructor of tf to close and unlink the file.
  tf.fd = extinfo_.relro_fd;
}

TEST_F(DlExtRelroSharingTest, ChildWritesGoodDataRecursive) {
  TemporaryFile tf; // Use tf to get an unique filename.
  ASSERT_NOERROR(close(tf.fd));

  ASSERT_NO_FATAL_FAILURE(CreateRelroFile(kLibNameRecursive, tf.path, true));
  ASSERT_NO_FATAL_FAILURE(TryUsingRelro(kLibNameRecursive, true));
  void* relro_data = dlsym(handle_, "lots_of_relro");
  ASSERT_DL_NOTNULL(relro_data);
  EXPECT_EQ(tf.path, FindMappingName(relro_data));
  void* recursive_relro_data = dlsym(handle_, "lots_more_relro");
  ASSERT_DL_NOTNULL(recursive_relro_data);
  EXPECT_EQ(tf.path, FindMappingName(recursive_relro_data));


  // Use destructor of tf to close and unlink the file.
  tf.fd = extinfo_.relro_fd;
}

TEST_F(DlExtRelroSharingTest, ChildWritesNoRelro) {
  TemporaryFile tf; // // Use tf to get an unique filename.
  ASSERT_NOERROR(close(tf.fd));

  ASSERT_NO_FATAL_FAILURE(CreateRelroFile(kLibNameNoRelro, tf.path, false));
  ASSERT_NO_FATAL_FAILURE(TryUsingRelro(kLibNameNoRelro, false));

  // Use destructor of tf to close and unlink the file.
  tf.fd = extinfo_.relro_fd;
}

TEST_F(DlExtRelroSharingTest, RelroFileEmpty) {
  ASSERT_NO_FATAL_FAILURE(TryUsingRelro(kLibName, false));
}

TEST_F(DlExtRelroSharingTest, VerifyMemorySaving) {
  if (geteuid() != 0) {
    GTEST_LOG_(INFO) << "This test must be run as root.\n";
    return;
  }

  TemporaryFile tf; // Use tf to get an unique filename.
  ASSERT_NOERROR(close(tf.fd));

  ASSERT_NO_FATAL_FAILURE(CreateRelroFile(kLibName, tf.path, false));

  int pipefd[2];
  ASSERT_NOERROR(pipe(pipefd));

  size_t without_sharing, with_sharing;
  ASSERT_NO_FATAL_FAILURE(SpawnChildrenAndMeasurePss(kLibName, tf.path, false, &without_sharing));
  ASSERT_NO_FATAL_FAILURE(SpawnChildrenAndMeasurePss(kLibName, tf.path, true, &with_sharing));
  ASSERT_LT(with_sharing, without_sharing);

  // We expect the sharing to save at least 50% of the library's total PSS.
  // In practice it saves 80%+ for this library in the test.
  size_t pss_saved = without_sharing - with_sharing;
  size_t expected_min_saved = without_sharing / 2;

  EXPECT_LT(expected_min_saved, pss_saved);

  // Use destructor of tf to close and unlink the file.
  tf.fd = extinfo_.relro_fd;
}

void GetPss(bool shared_relro, const char* lib, const char* relro_file, pid_t pid,
            size_t* total_pss) {
  android::meminfo::ProcMemInfo proc_mem(pid);
  const std::vector<android::meminfo::Vma>& maps = proc_mem.Maps();
  ASSERT_GT(maps.size(), 0UL);

  // Calculate total PSS of the library.
  *total_pss = 0;
  bool saw_relro_file = false;
  for (auto& vma : maps) {
    if (android::base::EndsWith(vma.name, lib) || (vma.name == relro_file)) {
      if (vma.name == relro_file) {
          saw_relro_file = true;
      }

      *total_pss += vma.usage.pss;
    }
  }

  if (shared_relro) ASSERT_TRUE(saw_relro_file);
}

void DlExtRelroSharingTest::SpawnChildrenAndMeasurePss(const char* lib, const char* relro_file,
                                                       bool share_relro, size_t* pss_out) {
  const int CHILDREN = 20;

  // Create children
  pid_t child_pids[CHILDREN];
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
    child_pids[i] = child;
    childpipe[i] = parent_done_pipe[1];
  }

  // Sum the PSS of tested library of all the children
  size_t total_pss = 0;
  for (int i=0; i<CHILDREN; ++i) {
    size_t child_pss;
    ASSERT_NO_FATAL_FAILURE(GetPss(share_relro, lib, relro_file, child_pids[i], &child_pss));
    total_pss += child_pss;
  }
  *pss_out = total_pss;

  // Close pipes and wait for children to exit
  for (int i=0; i<CHILDREN; ++i) {
    ASSERT_NOERROR(close(childpipe[i]));
  }
  for (int i = 0; i < CHILDREN; ++i) {
    AssertChildExited(child_pids[i], 0);
  }
}

std::string DlExtRelroSharingTest::FindMappingName(void* ptr) {
  uint64_t addr = reinterpret_cast<uint64_t>(ptr);
  std::string found_name = "<not found>";

  EXPECT_TRUE(android::procinfo::ReadMapFile(
      "/proc/self/maps",
      [&](uint64_t start, uint64_t end, uint16_t, uint16_t, ino_t, const char* name) {
        if (addr >= start && addr < end) {
          found_name = name;
        }
      }));

  return found_name;
}

// Testing namespaces
static const char* g_public_lib = "libnstest_public.so";

// These are libs shared with default namespace
static const std::string g_core_shared_libs = "libc.so:libc++.so:libdl.so:libm.so";

TEST(dlext, ns_smoke) {
  static const char* root_lib = "libnstest_root.so";
  std::string shared_libs = g_core_shared_libs + ":" + g_public_lib;

  ASSERT_FALSE(android_init_anonymous_namespace("", nullptr));
  ASSERT_STREQ("android_init_anonymous_namespace failed: error linking namespaces"
               " \"(anonymous)\"->\"(default)\": the list of shared libraries is empty.",
               dlerror());

  const std::string lib_public_path = GetTestlibRoot() + "/public_namespace_libs/" + g_public_lib;
  void* handle_public = dlopen(lib_public_path.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle_public != nullptr) << dlerror();

  ASSERT_TRUE(android_init_anonymous_namespace(shared_libs.c_str(), nullptr)) << dlerror();

  // Check that libraries added to public namespace are not NODELETE
  dlclose(handle_public);
  handle_public = dlopen(lib_public_path.c_str(), RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle_public == nullptr);
  ASSERT_EQ(std::string("dlopen failed: library \"") + lib_public_path +
               "\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());

  handle_public = dlopen(lib_public_path.c_str(), RTLD_NOW);

  // create "public namespace", share limited set of public libraries with

  android_namespace_t* ns1 =
          android_create_namespace("private",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_REGULAR,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns1 != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns1, nullptr, shared_libs.c_str())) << dlerror();

  android_namespace_t* ns2 =
          android_create_namespace("private_isolated",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns2 != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns2, nullptr, shared_libs.c_str())) << dlerror();

  // This should not have affect search path for default namespace:
  ASSERT_TRUE(dlopen(root_lib, RTLD_NOW) == nullptr);
  void* handle = dlopen(g_public_lib, RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  dlclose(handle);

  // dlopen for a public library using an absolute path should work
  // 1. For isolated namespaces
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns2;
  handle = android_dlopen_ext(lib_public_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  ASSERT_TRUE(handle == handle_public);

  dlclose(handle);

  // 1.1 even if it wasn't loaded before
  dlclose(handle_public);

  handle_public = dlopen(lib_public_path.c_str(), RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle_public == nullptr);
  ASSERT_EQ(std::string("dlopen failed: library \"") + lib_public_path +
               "\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());

  handle = android_dlopen_ext(lib_public_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  handle_public = dlopen(lib_public_path.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle == handle_public);

  dlclose(handle);

  // 2. And for regular namespaces (make sure it does not load second copy of the library)
  extinfo.library_namespace = ns1;
  handle = android_dlopen_ext(lib_public_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  ASSERT_TRUE(handle == handle_public);

  dlclose(handle);

  // 2.1 Unless it was not loaded before - in which case it will load a duplicate.
  // TODO(dimitry): This is broken. Maybe we need to deprecate non-isolated namespaces?
  dlclose(handle_public);

  handle_public = dlopen(lib_public_path.c_str(), RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle_public == nullptr);
  ASSERT_EQ(std::string("dlopen failed: library \"") + lib_public_path +
               "\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());

  handle = android_dlopen_ext(lib_public_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  handle_public = dlopen(lib_public_path.c_str(), RTLD_NOW);

  ASSERT_TRUE(handle != handle_public);

  dlclose(handle);

  extinfo.library_namespace = ns1;

  void* handle1 = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle1 != nullptr) << dlerror();

  extinfo.library_namespace = ns2;
  void* handle2 = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle2 != nullptr) << dlerror();

  ASSERT_TRUE(handle1 != handle2);

  typedef const char* (*fn_t)();

  fn_t ns_get_local_string1 = reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_local_string"));
  ASSERT_TRUE(ns_get_local_string1 != nullptr) << dlerror();
  fn_t ns_get_local_string2 = reinterpret_cast<fn_t>(dlsym(handle2, "ns_get_local_string"));
  ASSERT_TRUE(ns_get_local_string2 != nullptr) << dlerror();

  EXPECT_STREQ("This string is local to root library", ns_get_local_string1());
  EXPECT_STREQ("This string is local to root library", ns_get_local_string2());

  ASSERT_TRUE(ns_get_local_string1() != ns_get_local_string2());

  fn_t ns_get_private_extern_string1 =
          reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_private_extern_string"));
  ASSERT_TRUE(ns_get_private_extern_string1 != nullptr) << dlerror();
  fn_t ns_get_private_extern_string2 =
          reinterpret_cast<fn_t>(dlsym(handle2, "ns_get_private_extern_string"));
  ASSERT_TRUE(ns_get_private_extern_string2 != nullptr) << dlerror();

  EXPECT_STREQ("This string is from private namespace", ns_get_private_extern_string1());
  EXPECT_STREQ("This string is from private namespace", ns_get_private_extern_string2());

  ASSERT_TRUE(ns_get_private_extern_string1() != ns_get_private_extern_string2());

  fn_t ns_get_public_extern_string1 =
          reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_public_extern_string"));
  ASSERT_TRUE(ns_get_public_extern_string1 != nullptr) << dlerror();
  fn_t ns_get_public_extern_string2 =
          reinterpret_cast<fn_t>(dlsym(handle2, "ns_get_public_extern_string"));
  ASSERT_TRUE(ns_get_public_extern_string2 != nullptr) << dlerror();

  EXPECT_STREQ("This string is from public namespace", ns_get_public_extern_string1());
  ASSERT_TRUE(ns_get_public_extern_string1() == ns_get_public_extern_string2());

  // and now check that dlopen() does the right thing in terms of preserving namespace
  fn_t ns_get_dlopened_string1 = reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_dlopened_string"));
  ASSERT_TRUE(ns_get_dlopened_string1 != nullptr) << dlerror();
  fn_t ns_get_dlopened_string2 = reinterpret_cast<fn_t>(dlsym(handle2, "ns_get_dlopened_string"));
  ASSERT_TRUE(ns_get_dlopened_string2 != nullptr) << dlerror();

  EXPECT_STREQ("This string is from private namespace (dlopened library)", ns_get_dlopened_string1());
  EXPECT_STREQ("This string is from private namespace (dlopened library)", ns_get_dlopened_string2());

  ASSERT_TRUE(ns_get_dlopened_string1() != ns_get_dlopened_string2());

  // Check that symbols from non-shared libraries a shared library depends on are not visible
  // from original namespace.

  fn_t ns_get_internal_extern_string =
          reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_internal_extern_string"));
  ASSERT_TRUE(ns_get_internal_extern_string != nullptr) << dlerror();
  ASSERT_TRUE(ns_get_internal_extern_string() == nullptr) <<
      "ns_get_internal_extern_string() expected to return null but returns \"" <<
      ns_get_internal_extern_string() << "\"";

  dlclose(handle1);

  // Check if handle2 is still alive (and well)
  ASSERT_STREQ("This string is local to root library", ns_get_local_string2());
  ASSERT_STREQ("This string is from private namespace", ns_get_private_extern_string2());
  ASSERT_STREQ("This string is from public namespace", ns_get_public_extern_string2());
  ASSERT_STREQ("This string is from private namespace (dlopened library)", ns_get_dlopened_string2());

  dlclose(handle2);
}

TEST(dlext, dlopen_ext_use_o_tmpfile_fd) {
  const std::string lib_path = GetTestlibRoot() + "/libtest_simple.so";

  int tmpfd = TEMP_FAILURE_RETRY(
        open(GetTestlibRoot().c_str(), O_TMPFILE | O_CLOEXEC | O_RDWR | O_EXCL, 0));

  // Ignore kernels without O_TMPFILE flag support
  if (tmpfd == -1 && (errno == EISDIR || errno == EINVAL || errno == EOPNOTSUPP)) {
    return;
  }

  ASSERT_TRUE(tmpfd != -1) << strerror(errno);

  android_namespace_t* ns =
          android_create_namespace("testing-o_tmpfile",
                                   nullptr,
                                   GetTestlibRoot().c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_DL_NOTNULL(ns);

  ASSERT_TRUE(android_link_namespaces(ns, nullptr, g_core_shared_libs.c_str())) << dlerror();

  std::string content;
  ASSERT_TRUE(android::base::ReadFileToString(lib_path, &content)) << strerror(errno);
  ASSERT_TRUE(android::base::WriteStringToFd(content, tmpfd)) << strerror(errno);

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_LIBRARY_FD | ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_fd = tmpfd;
  extinfo.library_namespace = ns;

  void* handle = android_dlopen_ext("foobar", RTLD_NOW, &extinfo);

  ASSERT_DL_NOTNULL(handle);

  uint32_t* taxicab_number = reinterpret_cast<uint32_t*>(dlsym(handle, "dlopen_testlib_taxicab_number"));
  ASSERT_DL_NOTNULL(taxicab_number);
  EXPECT_EQ(1729U, *taxicab_number);
  dlclose(handle);
}

TEST(dlext, dlopen_ext_use_memfd) {
  const std::string lib_path = GetTestlibRoot() + "/libtest_simple.so";

  // create memfd
  int memfd = syscall(__NR_memfd_create, "foobar", MFD_CLOEXEC);
  if (memfd == -1 && errno == ENOSYS) {
    return;
  }

  ASSERT_TRUE(memfd != -1) << strerror(errno);

  // Check st.f_type is TMPFS_MAGIC for memfd
  struct statfs st;
  ASSERT_TRUE(TEMP_FAILURE_RETRY(fstatfs(memfd, &st)) == 0) << strerror(errno);
  ASSERT_EQ(static_cast<decltype(st.f_type)>(TMPFS_MAGIC), st.f_type);

  android_namespace_t* ns =
          android_create_namespace("testing-memfd",
                                   nullptr,
                                   GetTestlibRoot().c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_DL_NOTNULL(ns);

  ASSERT_TRUE(android_link_namespaces(ns, nullptr, g_core_shared_libs.c_str())) << dlerror();

  // read file into memfd backed one.
  std::string content;
  ASSERT_TRUE(android::base::ReadFileToString(lib_path, &content)) << strerror(errno);
  ASSERT_TRUE(android::base::WriteStringToFd(content, memfd)) << strerror(errno);

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_LIBRARY_FD | ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_fd = memfd;
  extinfo.library_namespace = ns;

  void* handle = android_dlopen_ext("foobar", RTLD_NOW, &extinfo);

  ASSERT_DL_NOTNULL(handle);

  uint32_t* taxicab_number = reinterpret_cast<uint32_t*>(dlsym(handle, "dlopen_testlib_taxicab_number"));
  ASSERT_DL_NOTNULL(taxicab_number);
  EXPECT_EQ(1729U, *taxicab_number);
  dlclose(handle);
}

TEST(dlext, ns_symbol_visibilty_one_namespace) {
  static const char* root_lib = "libnstest_root.so";
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  const std::string ns_search_path = GetTestlibRoot() + "/public_namespace_libs:" +
                                     GetTestlibRoot() + "/private_namespace_libs";

  android_namespace_t* ns =
          android_create_namespace("one",
                                   nullptr,
                                   ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns;

  void* handle = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  typedef const char* (*fn_t)();

  // Check that relocation worked correctly
  fn_t ns_get_internal_extern_string =
          reinterpret_cast<fn_t>(dlsym(handle, "ns_get_internal_extern_string"));
  ASSERT_TRUE(ns_get_internal_extern_string != nullptr) << dlerror();
  ASSERT_STREQ("This string is from a library a shared library depends on", ns_get_internal_extern_string());

  fn_t internal_extern_string_fn =
          reinterpret_cast<fn_t>(dlsym(handle, "internal_extern_string"));
  ASSERT_TRUE(internal_extern_string_fn != nullptr) << dlerror();
  ASSERT_STREQ("This string is from a library a shared library depends on", internal_extern_string_fn());
}

TEST(dlext, ns_symbol_visibilty_between_namespaces) {
  static const char* root_lib = "libnstest_root.so";
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  const std::string public_ns_search_path =  GetTestlibRoot() + "/public_namespace_libs";
  const std::string private_ns_search_path = GetTestlibRoot() + "/private_namespace_libs";

  android_namespace_t* ns_public =
          android_create_namespace("public",
                                   nullptr,
                                   public_ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns_public, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_namespace_t* ns_private =
          android_create_namespace("private",
                                   nullptr,
                                   private_ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns_private, ns_public, g_public_lib)) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_private, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns_private;

  void* handle = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  typedef const char* (*fn_t)();

  // Check that relocation worked correctly
  fn_t ns_get_internal_extern_string =
          reinterpret_cast<fn_t>(dlsym(handle, "ns_get_internal_extern_string"));
  ASSERT_TRUE(ns_get_internal_extern_string != nullptr) << dlerror();
  ASSERT_TRUE(ns_get_internal_extern_string() == nullptr) <<
      "ns_get_internal_extern_string() expected to return null but returns \"" <<
      ns_get_internal_extern_string() << "\"";

  fn_t internal_extern_string_fn =
          reinterpret_cast<fn_t>(dlsym(handle, "internal_extern_string"));
  ASSERT_TRUE(internal_extern_string_fn == nullptr);
  ASSERT_STREQ("undefined symbol: internal_extern_string", dlerror());
}

TEST(dlext, ns_unload_between_namespaces) {
  static const char* root_lib = "libnstest_root.so";
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  const std::string public_ns_search_path =  GetTestlibRoot() + "/public_namespace_libs";
  const std::string private_ns_search_path = GetTestlibRoot() + "/private_namespace_libs";

  android_namespace_t* ns_public =
          android_create_namespace("public",
                                   nullptr,
                                   public_ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns_public, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_namespace_t* ns_private =
          android_create_namespace("private",
                                   nullptr,
                                   private_ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns_private, ns_public, g_public_lib)) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_private, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns_private;

  void* handle = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  dlclose(handle);
  // Check that root_lib was unloaded
  handle = android_dlopen_ext(root_lib, RTLD_NOW | RTLD_NOLOAD, &extinfo);
  ASSERT_TRUE(handle == nullptr);
  ASSERT_EQ(std::string("dlopen failed: library \"") + root_lib +
            "\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());

  // Check that shared library was unloaded in public ns
  extinfo.library_namespace = ns_public;
  handle = android_dlopen_ext(g_public_lib, RTLD_NOW | RTLD_NOLOAD, &extinfo);
  ASSERT_TRUE(handle == nullptr);
  ASSERT_EQ(std::string("dlopen failed: library \"") + g_public_lib +
            "\" wasn't loaded and RTLD_NOLOAD prevented it", dlerror());
}

TEST(dlext, ns_unload_between_namespaces_missing_symbol_direct) {
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  const std::string public_ns_search_path =  GetTestlibRoot() + "/public_namespace_libs";
  const std::string private_ns_search_path = GetTestlibRoot() + "/private_namespace_libs";

  android_namespace_t* ns_public =
          android_create_namespace("public",
                                   nullptr,
                                   public_ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns_public, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_namespace_t* ns_private =
          android_create_namespace("private",
                                   nullptr,
                                   private_ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns_private, ns_public, "libtest_missing_symbol.so")) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_private, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns_private;

  void* handle = android_dlopen_ext((public_ns_search_path + "/libtest_missing_symbol.so").c_str(),
                                    RTLD_NOW,
                                    &extinfo);
  ASSERT_TRUE(handle == nullptr);
  ASSERT_EQ(std::string("dlopen failed: cannot locate symbol \"dlopen_testlib_missing_symbol\" referenced by \"") +
            public_ns_search_path + "/libtest_missing_symbol.so\"...",
            dlerror());
}

TEST(dlext, ns_unload_between_namespaces_missing_symbol_indirect) {
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  const std::string public_ns_search_path =  GetTestlibRoot() + "/public_namespace_libs";
  const std::string private_ns_search_path = GetTestlibRoot() + "/private_namespace_libs";

  android_namespace_t* ns_public =
          android_create_namespace("public",
                                   nullptr,
                                   public_ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns_public, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_namespace_t* ns_private =
          android_create_namespace("private",
                                   nullptr,
                                   private_ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns_private,
                                      ns_public,
                                      "libnstest_public.so:libtest_missing_symbol_child_public.so")
              ) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_private, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns_private;

  void* handle = android_dlopen_ext("libtest_missing_symbol_root.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle == nullptr);
  ASSERT_EQ(std::string("dlopen failed: cannot locate symbol \"dlopen_testlib_missing_symbol\" referenced by \"") +
            private_ns_search_path + "/libtest_missing_symbol_root.so\"...",
            dlerror());
}

TEST(dlext, ns_greylist_enabled) {
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  const std::string ns_search_path = GetTestlibRoot() + "/private_namespace_libs";

  android_namespace_t* ns =
          android_create_namespace("namespace",
                                   nullptr,
                                   ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED | ANDROID_NAMESPACE_TYPE_GREYLIST_ENABLED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns;

  // An app targeting M can open libnativehelper.so because it's on the greylist.
  android_set_application_target_sdk_version(__ANDROID_API_M__);
  void* handle = android_dlopen_ext("libnativehelper.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  // Check that loader did not load another copy of libdl.so while loading greylisted library.
  void* dlsym_ptr = dlsym(handle, "dlsym");
  ASSERT_TRUE(dlsym_ptr != nullptr) << dlerror();
  ASSERT_EQ(&dlsym, dlsym_ptr);

  dlclose(handle);

  // An app targeting N no longer has the greylist.
  android_set_application_target_sdk_version(__ANDROID_API_N__);
  handle = android_dlopen_ext("libnativehelper.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle == nullptr);
  ASSERT_STREQ("dlopen failed: library \"libnativehelper.so\" not found", dlerror());
}

TEST(dlext, ns_greylist_disabled_by_default) {
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  const std::string ns_search_path = GetTestlibRoot() + "/private_namespace_libs";

  android_namespace_t* ns =
          android_create_namespace("namespace",
                                   nullptr,
                                   ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns;

  android_set_application_target_sdk_version(__ANDROID_API_M__);
  void* handle = android_dlopen_ext("libnativehelper.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle == nullptr);
  ASSERT_STREQ("dlopen failed: library \"libnativehelper.so\" not found", dlerror());
}

TEST(dlext, ns_cyclic_namespaces) {
  // Test that ns1->ns2->ns1 link does not break the loader
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));
  std::string shared_libs = g_core_shared_libs + ":libthatdoesnotexist.so";

  const std::string ns_search_path =  GetTestlibRoot() + "/public_namespace_libs";

  android_namespace_t* ns1 =
          android_create_namespace("ns1",
                                   nullptr,
                                   ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns1, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_namespace_t* ns2 =
          android_create_namespace("ns1",
                                   nullptr,
                                   ns_search_path.c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(android_link_namespaces(ns2, nullptr, g_core_shared_libs.c_str())) << dlerror();

  ASSERT_TRUE(android_link_namespaces(ns2, ns1, shared_libs.c_str())) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns1, ns2, shared_libs.c_str())) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns1;

  void* handle = android_dlopen_ext("libthatdoesnotexist.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle == nullptr);
  ASSERT_STREQ("dlopen failed: library \"libthatdoesnotexist.so\" not found", dlerror());
}

TEST(dlext, ns_isolated) {
  static const char* root_lib = "libnstest_root_not_isolated.so";
  std::string shared_libs = g_core_shared_libs + ":" + g_public_lib;

  const std::string lib_public_path = GetTestlibRoot() + "/public_namespace_libs/" + g_public_lib;
  void* handle_public = dlopen(lib_public_path.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle_public != nullptr) << dlerror();

  android_set_application_target_sdk_version(42U); // something > 23

  ASSERT_TRUE(android_init_anonymous_namespace(shared_libs.c_str(), nullptr)) << dlerror();

  android_namespace_t* ns_not_isolated =
          android_create_namespace("private",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_REGULAR,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns_not_isolated != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_not_isolated, nullptr, shared_libs.c_str())) << dlerror();

  android_namespace_t* ns_isolated =
          android_create_namespace("private_isolated1",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns_isolated != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_isolated, nullptr, shared_libs.c_str())) << dlerror();

  android_namespace_t* ns_isolated2 =
          android_create_namespace("private_isolated2",
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   nullptr,
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   GetTestlibRoot().c_str(),
                                   nullptr);
  ASSERT_TRUE(ns_isolated2 != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_isolated2, nullptr, shared_libs.c_str())) << dlerror();

  ASSERT_TRUE(dlopen(root_lib, RTLD_NOW) == nullptr);
  ASSERT_STREQ("dlopen failed: library \"libnstest_root_not_isolated.so\" not found", dlerror());

  std::string lib_private_external_path =
      GetTestlibRoot() + "/private_namespace_libs_external/libnstest_private_external.so";

  // Load lib_private_external_path to default namespace
  // (it should remain invisible for the isolated namespaces after this)
  void* handle = dlopen(lib_private_external_path.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns_not_isolated;

  void* handle1 = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle1 != nullptr) << dlerror();

  extinfo.library_namespace = ns_isolated;

  void* handle2 = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle2 == nullptr);
  ASSERT_STREQ("dlopen failed: library \"libnstest_private_external.so\" not found", dlerror());

  // Check dlopen by absolute path
  handle2 = android_dlopen_ext(lib_private_external_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle2 == nullptr);
  ASSERT_EQ("dlopen failed: library \"" + lib_private_external_path + "\" needed"
            " or dlopened by \"" + android::base::GetExecutablePath() +  "\" is not accessible"
            " for the namespace \"private_isolated1\"", dlerror());

  extinfo.library_namespace = ns_isolated2;

  // this should work because isolation_path for private_isolated2 includes GetTestlibRoot()
  handle2 = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle2 != nullptr) << dlerror();
  dlclose(handle2);

  // Check dlopen by absolute path
  handle2 = android_dlopen_ext(lib_private_external_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle2 != nullptr) << dlerror();
  dlclose(handle2);

  typedef const char* (*fn_t)();
  fn_t ns_get_local_string = reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_local_string"));
  ASSERT_TRUE(ns_get_local_string != nullptr) << dlerror();

  ASSERT_STREQ("This string is local to root library", ns_get_local_string());

  fn_t ns_get_private_extern_string =
          reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_private_extern_string"));
  ASSERT_TRUE(ns_get_private_extern_string != nullptr) << dlerror();

  ASSERT_STREQ("This string is from private namespace", ns_get_private_extern_string());

  fn_t ns_get_public_extern_string =
          reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_public_extern_string"));
  ASSERT_TRUE(ns_get_public_extern_string != nullptr) << dlerror();

  ASSERT_STREQ("This string is from public namespace", ns_get_public_extern_string());

  fn_t ns_get_dlopened_string = reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_dlopened_string"));
  ASSERT_TRUE(ns_get_dlopened_string != nullptr) << dlerror();

  ASSERT_STREQ("This string is from private namespace (dlopened library)", ns_get_dlopened_string());

  dlclose(handle1);
}

TEST(dlext, ns_shared) {
  static const char* root_lib = "libnstest_root_not_isolated.so";
  static const char* root_lib_isolated = "libnstest_root.so";

  std::string shared_libs = g_core_shared_libs + ":" + g_public_lib;

  // create a parent namespace to use instead of the default namespace. This is
  // to make this test be independent from the configuration of the default
  // namespace.
  android_namespace_t* ns_parent =
          android_create_namespace("parent",
                                   nullptr,
                                   nullptr,
                                   ANDROID_NAMESPACE_TYPE_REGULAR,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns_parent != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_parent, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns_parent;

  const std::string lib_public_path = GetTestlibRoot() + "/public_namespace_libs/" + g_public_lib;
  void* handle_public = android_dlopen_ext(lib_public_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle_public != nullptr) << dlerror();

  android_set_application_target_sdk_version(42U); // something > 23

  ASSERT_TRUE(android_init_anonymous_namespace(shared_libs.c_str(), nullptr)) << dlerror();

  // preload this library to the parent namespace to check if it
  // is shared later on.
  void* handle_dlopened =
          android_dlopen_ext((GetTestlibRoot() + "/private_namespace_libs/libnstest_dlopened.so").c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle_dlopened != nullptr) << dlerror();

  // create two child namespaces of 'ns_parent'. One with regular, the other
  // with isolated & shared.
  android_namespace_t* ns_not_isolated =
          android_create_namespace("private",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_REGULAR,
                                   nullptr,
                                   ns_parent);
  ASSERT_TRUE(ns_not_isolated != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_not_isolated, ns_parent, g_public_lib)) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_not_isolated, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_namespace_t* ns_isolated_shared =
          android_create_namespace("private_isolated_shared",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED | ANDROID_NAMESPACE_TYPE_SHARED,
                                   nullptr,
                                   ns_parent);
  ASSERT_TRUE(ns_isolated_shared != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_isolated_shared, ns_parent, g_public_lib)) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_isolated_shared, nullptr, g_core_shared_libs.c_str())) << dlerror();

  ASSERT_TRUE(android_dlopen_ext(root_lib, RTLD_NOW, &extinfo) == nullptr);
  ASSERT_STREQ("dlopen failed: library \"libnstest_root_not_isolated.so\" not found", dlerror());

  std::string lib_private_external_path =
      GetTestlibRoot() + "/private_namespace_libs_external/libnstest_private_external.so";

  // Load lib_private_external_path to the parent namespace
  // (it should remain invisible for the isolated namespaces after this)
  void* handle = android_dlopen_ext(lib_private_external_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  extinfo.library_namespace = ns_not_isolated;

  void* handle1 = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle1 != nullptr) << dlerror();

  extinfo.library_namespace = ns_isolated_shared;

  void* handle2 = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle2 == nullptr);
  ASSERT_STREQ("dlopen failed: library \"libnstest_private_external.so\" not found", dlerror());

  // Check dlopen by absolute path
  handle2 = android_dlopen_ext(lib_private_external_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle2 == nullptr);
  ASSERT_EQ("dlopen failed: library \"" + lib_private_external_path + "\" needed"
            " or dlopened by \"" + android::base::GetExecutablePath() + "\" is not accessible"
            " for the namespace \"private_isolated_shared\"", dlerror());

  // load libnstest_root.so to shared namespace in order to check that everything is different
  // except shared libnstest_dlopened.so

  handle2 = android_dlopen_ext(root_lib_isolated, RTLD_NOW, &extinfo);

  typedef const char* (*fn_t)();
  fn_t ns_get_local_string = reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_local_string"));
  ASSERT_TRUE(ns_get_local_string != nullptr) << dlerror();
  fn_t ns_get_local_string_shared = reinterpret_cast<fn_t>(dlsym(handle2, "ns_get_local_string"));
  ASSERT_TRUE(ns_get_local_string_shared != nullptr) << dlerror();

  ASSERT_STREQ("This string is local to root library", ns_get_local_string());
  ASSERT_STREQ("This string is local to root library", ns_get_local_string_shared());
  ASSERT_TRUE(ns_get_local_string() != ns_get_local_string_shared());

  fn_t ns_get_private_extern_string =
          reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_private_extern_string"));
  ASSERT_TRUE(ns_get_private_extern_string != nullptr) << dlerror();
  fn_t ns_get_private_extern_string_shared =
          reinterpret_cast<fn_t>(dlsym(handle2, "ns_get_private_extern_string"));
  ASSERT_TRUE(ns_get_private_extern_string_shared() != nullptr) << dlerror();

  ASSERT_STREQ("This string is from private namespace", ns_get_private_extern_string());
  ASSERT_STREQ("This string is from private namespace", ns_get_private_extern_string_shared());
  ASSERT_TRUE(ns_get_private_extern_string() != ns_get_private_extern_string_shared());

  fn_t ns_get_public_extern_string =
          reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_public_extern_string"));
  ASSERT_TRUE(ns_get_public_extern_string != nullptr) << dlerror();
  fn_t ns_get_public_extern_string_shared =
          reinterpret_cast<fn_t>(dlsym(handle2, "ns_get_public_extern_string"));
  ASSERT_TRUE(ns_get_public_extern_string_shared != nullptr) << dlerror();

  ASSERT_STREQ("This string is from public namespace", ns_get_public_extern_string());
  ASSERT_STREQ("This string is from public namespace", ns_get_public_extern_string_shared());
  ASSERT_TRUE(ns_get_public_extern_string() == ns_get_public_extern_string_shared());

  fn_t ns_get_dlopened_string = reinterpret_cast<fn_t>(dlsym(handle1, "ns_get_dlopened_string"));
  ASSERT_TRUE(ns_get_dlopened_string != nullptr) << dlerror();
  fn_t ns_get_dlopened_string_shared = reinterpret_cast<fn_t>(dlsym(handle2, "ns_get_dlopened_string"));
  ASSERT_TRUE(ns_get_dlopened_string_shared != nullptr) << dlerror();
  const char** ns_dlopened_string = static_cast<const char**>(dlsym(handle_dlopened, "g_private_dlopened_string"));
  ASSERT_TRUE(ns_dlopened_string != nullptr) << dlerror();

  ASSERT_STREQ("This string is from private namespace (dlopened library)", ns_get_dlopened_string());
  ASSERT_STREQ("This string is from private namespace (dlopened library)", *ns_dlopened_string);
  ASSERT_STREQ("This string is from private namespace (dlopened library)", ns_get_dlopened_string_shared());
  ASSERT_TRUE(ns_get_dlopened_string() != ns_get_dlopened_string_shared());
  ASSERT_TRUE(*ns_dlopened_string == ns_get_dlopened_string_shared());

  dlclose(handle1);
  dlclose(handle2);
}

TEST(dlext, ns_shared_links_and_paths) {
  // Create parent namespace (isolated, not shared)
  android_namespace_t* ns_isolated =
          android_create_namespace("private_isolated",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   (GetTestlibRoot() + "/public_namespace_libs").c_str(),
                                   nullptr);
  ASSERT_TRUE(ns_isolated != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_isolated, nullptr, g_core_shared_libs.c_str())) << dlerror();

  // Create shared namespace with ns_isolated parent
  android_namespace_t* ns_shared =
          android_create_namespace("private_shared",
                                   nullptr,
                                   nullptr,
                                   ANDROID_NAMESPACE_TYPE_SHARED | ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   ns_isolated);
  ASSERT_TRUE(ns_shared != nullptr) << dlerror();

  // 1. Load a library in ns_shared to check that it has inherited
  // search path and the link to the default namespace.
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns_shared;

  {
    void* handle = android_dlopen_ext("libnstest_private.so", RTLD_NOW, &extinfo);
    ASSERT_TRUE(handle != nullptr) << dlerror();
    const char** ns_private_extern_string = static_cast<const char**>(dlsym(handle, "g_private_extern_string"));
    ASSERT_TRUE(ns_private_extern_string != nullptr) << dlerror();
    ASSERT_STREQ("This string is from private namespace", *ns_private_extern_string);

    dlclose(handle);
  }
  // 2. Load another test library by absolute path to check that
  // it has inherited permitted_when_isolated_path
  {
    void* handle = android_dlopen_ext(
            (GetTestlibRoot() + "/public_namespace_libs/libnstest_public.so").c_str(),
            RTLD_NOW,
            &extinfo);

    ASSERT_TRUE(handle != nullptr) << dlerror();
    const char** ns_public_extern_string = static_cast<const char**>(dlsym(handle, "g_public_extern_string"));
    ASSERT_TRUE(ns_public_extern_string != nullptr) << dlerror();
    ASSERT_STREQ("This string is from public namespace", *ns_public_extern_string);

    dlclose(handle);
  }

  // 3. Check that it is still isolated.
  {
    void* handle = android_dlopen_ext(
            (GetTestlibRoot() + "/libtest_empty.so").c_str(),
            RTLD_NOW,
            &extinfo);

    ASSERT_TRUE(handle == nullptr);
  }
}

TEST(dlext, ns_shared_dlclose) {
  android_set_application_target_sdk_version(42U); // something > 23

  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr)) << dlerror();

  // preload this library to the default namespace to check if it
  // is shared later on.
  void* handle_dlopened =
          dlopen((GetTestlibRoot() + "/private_namespace_libs/libnstest_dlopened.so").c_str(), RTLD_NOW);
  ASSERT_TRUE(handle_dlopened != nullptr) << dlerror();

  android_namespace_t* ns_isolated_shared =
          android_create_namespace("private_isolated_shared",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED | ANDROID_NAMESPACE_TYPE_SHARED,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns_isolated_shared != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_isolated_shared, nullptr, g_core_shared_libs.c_str())) << dlerror();

  // Check if "libnstest_dlopened.so" is loaded (and the same)
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns_isolated_shared;

  void* handle = android_dlopen_ext("libnstest_dlopened.so", RTLD_NOW | RTLD_NOLOAD, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  ASSERT_TRUE(handle == handle_dlopened);
  dlclose(handle);
  dlclose(handle_dlopened);

  // And now check that the library cannot be found by soname (and is no longer loaded)
  handle = android_dlopen_ext("libnstest_dlopened.so", RTLD_NOW | RTLD_NOLOAD, &extinfo);
  ASSERT_TRUE(handle == nullptr)
      << "Error: libnstest_dlopened.so is still accessible in shared namespace";

  handle = android_dlopen_ext((GetTestlibRoot() + "/private_namespace_libs/libnstest_dlopened.so").c_str(),
                              RTLD_NOW | RTLD_NOLOAD, &extinfo);
  ASSERT_TRUE(handle == nullptr)
      << "Error: libnstest_dlopened.so is still accessible in shared namespace";

  handle = dlopen("libnstest_dlopened.so", RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr)
      << "Error: libnstest_dlopened.so is still accessible in default namespace";

  handle = dlopen((GetTestlibRoot() + "/private_namespace_libs/libnstest_dlopened.so").c_str(),
                  RTLD_NOW | RTLD_NOLOAD);
  ASSERT_TRUE(handle == nullptr)
      << "Error: libnstest_dlopened.so is still accessible in default namespace";

  // Now lets see if the soinfo area gets reused in the wrong way:
  // load a library to default namespace.
  const std::string lib_public_path = GetTestlibRoot() + "/public_namespace_libs/" + g_public_lib;
  void* handle_public = dlopen(lib_public_path.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle_public != nullptr) << dlerror();

  // try to find it in shared namespace
  handle = android_dlopen_ext(g_public_lib, RTLD_NOW | RTLD_NOLOAD, &extinfo);
  ASSERT_TRUE(handle == nullptr)
      << "Error: " << g_public_lib << " is accessible in shared namespace";
}

TEST(dlext, ns_isolated_rtld_global) {
  static const char* root_lib = "libnstest_root.so";
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  const std::string lib_public_path = GetTestlibRoot() + "/public_namespace_libs";

  android_namespace_t* ns1 =
          android_create_namespace("isolated1",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   lib_public_path.c_str(),
                                   nullptr);
  ASSERT_TRUE(ns1 != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns1, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_namespace_t* ns2 =
          android_create_namespace("isolated2",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   lib_public_path.c_str(),
                                   nullptr);
  ASSERT_TRUE(ns2 != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns2, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns1;

  void* handle_global = android_dlopen_ext((lib_public_path + "/" + g_public_lib).c_str(),
                                           RTLD_GLOBAL,
                                           &extinfo);

  ASSERT_TRUE(handle_global != nullptr) << dlerror();

  android_namespace_t* ns1_child =
          android_create_namespace("isolated1_child",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   ns1);

  ASSERT_TRUE(ns1_child != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns1_child, nullptr, g_core_shared_libs.c_str())) << dlerror();

  // Now - only ns1 and ns1 child should be able to dlopen root_lib
  // attempt to use ns2 should result in dlerror()

  // Check ns1_child first.
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns1_child;

  void* handle1 = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle1 != nullptr) << dlerror();

  // now ns1
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns1;

  handle1 = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle1 != nullptr) << dlerror();

  // and ns2 should fail
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns2;

  handle1 = android_dlopen_ext(root_lib, RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle1 == nullptr);
  ASSERT_STREQ("dlopen failed: library \"libnstest_public.so\" not found", dlerror());
}

TEST(dlext, ns_inaccessible_error_message) {
  // We set up 2 namespaces (a and b) and link a->b with a shared library
  // libtestshared.so. Then try to dlopen different library with the same
  // name from in namespace a. Note that library should not be accessible
  // in either namespace but since it's soname is in the list of shared libs
  // the linker will attempt to find it in linked namespace.
  //
  // Check the error message and make sure it mentions correct namespace name.
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  android_namespace_t* ns_a =
          android_create_namespace("ns_a",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns_a != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_a, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_namespace_t* ns_b =
          android_create_namespace("ns_b",
                                   nullptr,
                                   GetTestlibRoot().c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns_b != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_b, nullptr, g_core_shared_libs.c_str())) << dlerror();

  ASSERT_TRUE(android_link_namespaces(ns_a, ns_b, "libtestshared.so")) << dlerror();

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns_a;

  std::string library_path = GetTestlibRoot() + "/inaccessible_libs/libtestshared.so";

  void* handle = android_dlopen_ext(library_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle == nullptr);
  std::string expected_dlerror =
      android::base::StringPrintf("dlopen failed: library \"%s\" needed or dlopened by \"%s\""
                                  " is not accessible for the namespace \"ns_a\"",
                                  library_path.c_str(),
                                  android::base::GetExecutablePath().c_str());
  ASSERT_EQ(expected_dlerror, dlerror());
}

extern "C" bool __loader_android_link_namespaces_all_libs(android_namespace_t* namespace_from,
                                                          android_namespace_t* namespace_to);

TEST(dlext, ns_link_namespaces_invalid_arguments) {
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  android_namespace_t* ns =
          android_create_namespace("private",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_REGULAR,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns != nullptr) << dlerror();

  // Test android_link_namespaces()
  ASSERT_FALSE(android_link_namespaces(nullptr, nullptr, "libc.so"));
  ASSERT_STREQ("android_link_namespaces failed: error linking namespaces: namespace_from is null.",
               dlerror());

  ASSERT_FALSE(android_link_namespaces(ns, nullptr, nullptr));
  ASSERT_STREQ("android_link_namespaces failed: "
               "error linking namespaces \"private\"->\"(default)\": "
               "the list of shared libraries is empty.", dlerror());

  ASSERT_FALSE(android_link_namespaces(ns, nullptr, ""));
  ASSERT_STREQ("android_link_namespaces failed: "
               "error linking namespaces \"private\"->\"(default)\": "
               "the list of shared libraries is empty.", dlerror());

  // Test __loader_android_link_namespaces_all_libs()
  ASSERT_FALSE(__loader_android_link_namespaces_all_libs(nullptr, nullptr));
  ASSERT_STREQ("android_link_namespaces_all_libs failed: "
               "error linking namespaces: namespace_from is null.", dlerror());

  ASSERT_FALSE(__loader_android_link_namespaces_all_libs(nullptr, ns));
  ASSERT_STREQ("android_link_namespaces_all_libs failed: "
               "error linking namespaces: namespace_from is null.", dlerror());

  ASSERT_FALSE(__loader_android_link_namespaces_all_libs(ns, nullptr));
  ASSERT_STREQ("android_link_namespaces_all_libs failed: "
               "error linking namespaces: namespace_to is null.", dlerror());
}

TEST(dlext, ns_allow_all_shared_libs) {
  ASSERT_TRUE(android_init_anonymous_namespace(g_core_shared_libs.c_str(), nullptr));

  android_namespace_t* ns_a =
          android_create_namespace("ns_a",
                                   nullptr,
                                   (GetTestlibRoot() + "/ns_a").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns_a != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_a, nullptr, g_core_shared_libs.c_str())) << dlerror();

  android_namespace_t* ns_b =
          android_create_namespace("ns_b",
                                   nullptr,
                                   (GetTestlibRoot() + "/ns_b").c_str(),
                                   ANDROID_NAMESPACE_TYPE_ISOLATED,
                                   nullptr,
                                   nullptr);
  ASSERT_TRUE(ns_b != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns_b, nullptr, g_core_shared_libs.c_str())) << dlerror();

  ASSERT_TRUE(android_link_namespaces(ns_b, ns_a, "libnstest_ns_a_public1.so")) << dlerror();
  ASSERT_TRUE(__loader_android_link_namespaces_all_libs(ns_a, ns_b)) << dlerror();

  // Load libs with android_dlopen_ext() from namespace b
  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns_b;

  void* ns_b_handle1 = android_dlopen_ext("libnstest_ns_a_public1.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(ns_b_handle1 != nullptr) << dlerror();

  void* ns_b_handle1_internal =
      android_dlopen_ext("libnstest_ns_a_public1_internal.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(ns_b_handle1_internal == nullptr);

  void* ns_b_handle2 = android_dlopen_ext("libnstest_ns_b_public2.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(ns_b_handle2 != nullptr) << dlerror();

  void* ns_b_handle3 = android_dlopen_ext("libnstest_ns_b_public3.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(ns_b_handle3 != nullptr) << dlerror();

  // Load libs with android_dlopen_ext() from namespace a
  extinfo.library_namespace = ns_a;

  void* ns_a_handle1 = android_dlopen_ext("libnstest_ns_a_public1.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(ns_a_handle1 != nullptr) << dlerror();

  void* ns_a_handle1_internal =
      android_dlopen_ext("libnstest_ns_a_public1_internal.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(ns_a_handle1_internal != nullptr) << dlerror();

  void* ns_a_handle2 = android_dlopen_ext("libnstest_ns_b_public2.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(ns_a_handle2 != nullptr) << dlerror();

  void* ns_a_handle3 = android_dlopen_ext("libnstest_ns_b_public3.so", RTLD_NOW, &extinfo);
  ASSERT_TRUE(ns_a_handle3 != nullptr) << dlerror();

  // Compare the dlopen handle
  ASSERT_EQ(ns_b_handle1, ns_a_handle1);
  ASSERT_EQ(ns_b_handle2, ns_a_handle2);
  ASSERT_EQ(ns_b_handle3, ns_a_handle3);

  // Close libs
  dlclose(ns_b_handle1);
  dlclose(ns_b_handle2);
  dlclose(ns_b_handle3);

  dlclose(ns_a_handle1);
  dlclose(ns_a_handle1_internal);
  dlclose(ns_a_handle2);
  dlclose(ns_a_handle3);
}

TEST(dlext, ns_anonymous) {
  static const char* root_lib = "libnstest_root.so";
  std::string shared_libs = g_core_shared_libs + ":" + g_public_lib;

  const std::string lib_public_path = GetTestlibRoot() + "/public_namespace_libs/" + g_public_lib;
  void* handle_public = dlopen(lib_public_path.c_str(), RTLD_NOW);

  ASSERT_TRUE(handle_public != nullptr) << dlerror();

  ASSERT_TRUE(
          android_init_anonymous_namespace(shared_libs.c_str(),
                                           (GetTestlibRoot() + "/private_namespace_libs").c_str())
      ) << dlerror();

  android_namespace_t* ns =
          android_create_namespace("private",
                                   nullptr,
                                   (GetTestlibRoot() + "/private_namespace_libs").c_str(),
                                   ANDROID_NAMESPACE_TYPE_REGULAR,
                                   nullptr,
                                   nullptr);

  ASSERT_TRUE(ns != nullptr) << dlerror();
  ASSERT_TRUE(android_link_namespaces(ns, nullptr, shared_libs.c_str())) << dlerror();

  std::string private_library_absolute_path = GetTestlibRoot() + "/private_namespace_libs/" + root_lib;

  android_dlextinfo extinfo;
  extinfo.flags = ANDROID_DLEXT_USE_NAMESPACE;
  extinfo.library_namespace = ns;

  // we are going to copy this library to anonymous mmap and call the copy of ns_get_dlopened_string
  void* handle = android_dlopen_ext(private_library_absolute_path.c_str(), RTLD_NOW, &extinfo);
  ASSERT_TRUE(handle != nullptr) << dlerror();

  uintptr_t ns_get_dlopened_string_addr =
      reinterpret_cast<uintptr_t>(dlsym(handle, "ns_get_dlopened_string"));
  ASSERT_TRUE(ns_get_dlopened_string_addr != 0) << dlerror();
  typedef const char* (*fn_t)();
  fn_t ns_get_dlopened_string_private = reinterpret_cast<fn_t>(ns_get_dlopened_string_addr);

  std::vector<map_record> maps;
  Maps::parse_maps(&maps);

  uintptr_t addr_start = 0;
  uintptr_t addr_end = 0;
  bool has_executable_segment = false;
  std::vector<map_record> maps_to_copy;

  for (const auto& rec : maps) {
    if (rec.pathname == private_library_absolute_path) {
      if (addr_start == 0) {
        addr_start = rec.addr_start;
      }
      addr_end = rec.addr_end;
      has_executable_segment = has_executable_segment || (rec.perms & PROT_EXEC) != 0;

      maps_to_copy.push_back(rec);
    }
  }

  // some sanity checks..
  ASSERT_TRUE(addr_start > 0);
  ASSERT_TRUE(addr_end > 0);
  ASSERT_TRUE(maps_to_copy.size() > 0);
  ASSERT_TRUE(ns_get_dlopened_string_addr > addr_start);
  ASSERT_TRUE(ns_get_dlopened_string_addr < addr_end);

  if (!has_executable_segment) {
    // For some natively bridged environments this code might be missing
    // the executable flag. This is because the guest code is not supposed
    // to be executed directly and making it non-executable is more secure.
    // If this is the case we assume that the first segment is the one that
    // has this flag.
    ASSERT_TRUE((maps_to_copy[0].perms & PROT_WRITE) == 0);
    maps_to_copy[0].perms |= PROT_EXEC;
  }

  // copy
  uintptr_t reserved_addr = reinterpret_cast<uintptr_t>(mmap(nullptr, addr_end - addr_start,
                                                             PROT_NONE, MAP_ANON | MAP_PRIVATE,
                                                             -1, 0));
  ASSERT_TRUE(reinterpret_cast<void*>(reserved_addr) != MAP_FAILED);

  for (const auto& rec : maps_to_copy) {
    uintptr_t offset = rec.addr_start - addr_start;
    size_t size = rec.addr_end - rec.addr_start;
    void* addr = reinterpret_cast<void*>(reserved_addr + offset);
    void* map = mmap(addr, size, PROT_READ | PROT_WRITE,
                     MAP_ANON | MAP_PRIVATE | MAP_FIXED, -1, 0);
    ASSERT_TRUE(map != MAP_FAILED);
    memcpy(map, reinterpret_cast<void*>(rec.addr_start), size);
    mprotect(map, size, rec.perms);
  }

  // call the function copy
  uintptr_t ns_get_dlopened_string_offset  = ns_get_dlopened_string_addr - addr_start;
  fn_t ns_get_dlopened_string_anon = reinterpret_cast<fn_t>(reserved_addr + ns_get_dlopened_string_offset);
  ASSERT_STREQ("This string is from private namespace (dlopened library)",
               ns_get_dlopened_string_anon());

  // They should belong to different namespaces (private and anonymous)
  ASSERT_STREQ("This string is from private namespace (dlopened library)",
               ns_get_dlopened_string_private());

  ASSERT_TRUE(ns_get_dlopened_string_anon() != ns_get_dlopened_string_private());
}

TEST(dlext, dlopen_handle_value_platform) {
  void* handle = dlopen("libtest_dlsym_from_this.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE((reinterpret_cast<uintptr_t>(handle) & 1) != 0)
          << "dlopen should return odd value for the handle";
  dlclose(handle);
}

TEST(dlext, dlopen_handle_value_app_compat) {
  android_set_application_target_sdk_version(__ANDROID_API_M__);
  void* handle = dlopen("libtest_dlsym_from_this.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(reinterpret_cast<uintptr_t>(handle) % sizeof(uintptr_t) == 0)
          << "dlopen should return valid pointer";
  dlclose(handle);
}
