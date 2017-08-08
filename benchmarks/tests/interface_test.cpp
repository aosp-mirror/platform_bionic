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
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <regex>
#include <string>
#include <vector>

#include <gtest/gtest.h>

class SystemTests : public ::testing::Test {
 protected:
  void SetUp() override {
    raw_output_ = "";
    sanitized_output_ = "";
    exitcode_ = 0;
  }

  void SanitizeOutput();

  void Exec(std::vector<const char*> args);
  void ExecAndCapture(std::vector<const char*> args);
  void RunTest(std::vector<const char*> extra_args = {});
  void Verify(const std::string& expected_output, int expected_exitcode,
              std::vector<const char*> extra_args = {});

  std::string raw_output_;
  std::string sanitized_output_;
  int exitcode_;
  pid_t pid_;
  int fd_;
};

void SystemTests::SanitizeOutput() {
  // Cut off anything after the arguments, since that varies with time.
  sanitized_output_ = std::regex_replace(raw_output_, std::regex(".+(BM_\\S+) +.+"), "$1");

  // Remove everything before the header.
  sanitized_output_.erase(0, sanitized_output_.find("------------------------------------------------"));

  // Remove the header.
  sanitized_output_.erase(0, sanitized_output_.find("BM_"));

  // Remove any hanging output.
  sanitized_output_.erase(sanitized_output_.find_last_of("BM_\\S+\n") + 1);
}

static void GetExe(std::string* exe_name) {
  char path[PATH_MAX];
  ssize_t path_len = readlink("/proc/self/exe", path, sizeof(path));
  ASSERT_TRUE(path_len >= 0);
  *exe_name = std::string(std::regex_replace(path, std::regex("-tests"), ""));
}

void SystemTests::Exec(std::vector<const char*> args) {
  int fds[2];
  ASSERT_NE(-1, pipe(fds));
  ASSERT_NE(-1, fcntl(fds[0], F_SETFL, O_NONBLOCK));

  if ((pid_ = fork()) == 0) {
    // Run the test.
    close(fds[0]);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    ASSERT_NE(0, dup2(fds[1], STDOUT_FILENO));
    ASSERT_NE(0, dup2(fds[1], STDERR_FILENO));
    close(fds[1]);

    std::string exe_name;
    GetExe(&exe_name);
    args.insert(args.begin(), exe_name.c_str());
    args.push_back(nullptr);
    execv(args[0], reinterpret_cast<char* const*>(const_cast<char**>(args.data())));
    exit(1);
  }
  ASSERT_NE(-1, pid_);

  close(fds[1]);
  fd_ = fds[0];
}

void SystemTests::Verify(const std::string& expected_output,
                         int expected_exitcode, std::vector<const char*> extra_args) {
  std::vector<const char*> args;
  for (const auto& arg : extra_args) {
    args.push_back(arg);
  }

  Exec(args);

  raw_output_ = "";
  while (true) {
    char buffer[4097];
    ssize_t bytes = TEMP_FAILURE_RETRY(read(fd_, buffer, sizeof(buffer) - 1));
    if (bytes == -1 && errno == EAGAIN) {
      continue;
    }
    ASSERT_NE(-1, bytes);
    if (bytes == 0) {
      break;
    }
    buffer[bytes] = '\0';
    raw_output_ += buffer;
  }
  close(fd_);

  int status;
  ASSERT_EQ(pid_, TEMP_FAILURE_RETRY(waitpid(pid_, &status, 0))) << "Test output:\n" << raw_output_;
  exitcode_ = WEXITSTATUS(status);
  SanitizeOutput();

  ASSERT_EQ(expected_exitcode, exitcode_) << "Test output:\n" << raw_output_;
  if (!expected_output.empty()) {
    ASSERT_EQ(expected_output, sanitized_output_);
  }

}

TEST_F(SystemTests, full_suite) {
  std::string expected =
    "BM_empty/iterations:1\n"
    "BM_load_relaxed/iterations:1\n"
    "BM_load_acquire/iterations:1\n"
    "BM_store_release/iterations:1\n"
    "BM_store_seq_cst/iterations:1\n"
    "BM_fetch_add_relaxed/iterations:1\n"
    "BM_fetch_add_seq_cst/iterations:1\n"
    "BM_acquire_fence/iterations:1\n"
    "BM_seq_cst_fence/iterations:1\n"
    "BM_fetch_add_cs/iterations:1\n"
    "BM_math_sqrt/iterations:1\n"
    "BM_math_log10/iterations:1\n"
    "BM_math_logb/iterations:1\n"
    "BM_math_isfinite_macro/0/iterations:1\n"
    "BM_math_isfinite/0/iterations:1\n"
    "BM_math_isinf_macro/0/iterations:1\n"
    "BM_math_isinf/0/iterations:1\n"
    "BM_math_isnan_macro/0/iterations:1\n"
    "BM_math_isnan/0/iterations:1\n"
    "BM_math_isnormal_macro/0/iterations:1\n"
    "BM_math_isnormal/0/iterations:1\n"
    "BM_math_sin_fast/iterations:1\n"
    "BM_math_sin_feupdateenv/iterations:1\n"
    "BM_math_sin_fesetenv/iterations:1\n"
    "BM_math_fpclassify/0/iterations:1\n"
    "BM_math_signbit_macro/0/iterations:1\n"
    "BM_math_signbit/0/iterations:1\n"
    "BM_math_fabs_macro/0/iterations:1\n"
    "BM_math_fabs/0/iterations:1\n"
    "BM_property_get/1/iterations:1\n"
    "BM_property_get/4/iterations:1\n"
    "BM_property_get/16/iterations:1\n"
    "BM_property_get/64/iterations:1\n"
    "BM_property_get/128/iterations:1\n"
    "BM_property_get/256/iterations:1\n"
    "BM_property_get/512/iterations:1\n"
    "BM_property_find/1/iterations:1\n"
    "BM_property_find/4/iterations:1\n"
    "BM_property_find/16/iterations:1\n"
    "BM_property_find/64/iterations:1\n"
    "BM_property_find/128/iterations:1\n"
    "BM_property_find/256/iterations:1\n"
    "BM_property_find/512/iterations:1\n"
    "BM_property_read/1/iterations:1\n"
    "BM_property_read/4/iterations:1\n"
    "BM_property_read/16/iterations:1\n"
    "BM_property_read/64/iterations:1\n"
    "BM_property_read/128/iterations:1\n"
    "BM_property_read/256/iterations:1\n"
    "BM_property_read/512/iterations:1\n"
    "BM_property_serial/1/iterations:1\n"
    "BM_property_serial/4/iterations:1\n"
    "BM_property_serial/16/iterations:1\n"
    "BM_property_serial/64/iterations:1\n"
    "BM_property_serial/128/iterations:1\n"
    "BM_property_serial/256/iterations:1\n"
    "BM_property_serial/512/iterations:1\n"
    "BM_pthread_self/iterations:1\n"
    "BM_pthread_getspecific/iterations:1\n"
    "BM_pthread_setspecific/iterations:1\n"
    "BM_pthread_once/iterations:1\n"
    "BM_pthread_mutex_lock/iterations:1\n"
    "BM_pthread_mutex_lock_ERRORCHECK/iterations:1\n"
    "BM_pthread_mutex_lock_RECURSIVE/iterations:1\n"
    "BM_pthread_rwlock_read/iterations:1\n"
    "BM_pthread_rwlock_write/iterations:1\n"
    "BM_pthread_create/iterations:1\n"
    "BM_pthread_create_and_run/iterations:1\n"
    "BM_pthread_exit_and_join/iterations:1\n"
    "BM_pthread_key_create/iterations:1\n"
    "BM_pthread_key_delete/iterations:1\n"
    "BM_semaphore_sem_getvalue/iterations:1\n"
    "BM_semaphore_sem_wait_sem_post/iterations:1\n"
    "BM_stdio_fread/8/iterations:1\n"
    "BM_stdio_fread/64/iterations:1\n"
    "BM_stdio_fread/512/iterations:1\n"
    "BM_stdio_fread/1024/iterations:1\n"
    "BM_stdio_fread/8192/iterations:1\n"
    "BM_stdio_fread/16384/iterations:1\n"
    "BM_stdio_fread/32768/iterations:1\n"
    "BM_stdio_fread/65536/iterations:1\n"
    "BM_stdio_fwrite/8/iterations:1\n"
    "BM_stdio_fwrite/64/iterations:1\n"
    "BM_stdio_fwrite/512/iterations:1\n"
    "BM_stdio_fwrite/1024/iterations:1\n"
    "BM_stdio_fwrite/8192/iterations:1\n"
    "BM_stdio_fwrite/16384/iterations:1\n"
    "BM_stdio_fwrite/32768/iterations:1\n"
    "BM_stdio_fwrite/65536/iterations:1\n"
    "BM_stdio_fread_unbuffered/8/iterations:1\n"
    "BM_stdio_fread_unbuffered/64/iterations:1\n"
    "BM_stdio_fread_unbuffered/512/iterations:1\n"
    "BM_stdio_fread_unbuffered/1024/iterations:1\n"
    "BM_stdio_fread_unbuffered/8192/iterations:1\n"
    "BM_stdio_fread_unbuffered/16384/iterations:1\n"
    "BM_stdio_fread_unbuffered/32768/iterations:1\n"
    "BM_stdio_fread_unbuffered/65536/iterations:1\n"
    "BM_stdio_fwrite_unbuffered/8/iterations:1\n"
    "BM_stdio_fwrite_unbuffered/64/iterations:1\n"
    "BM_stdio_fwrite_unbuffered/512/iterations:1\n"
    "BM_stdio_fwrite_unbuffered/1024/iterations:1\n"
    "BM_stdio_fwrite_unbuffered/8192/iterations:1\n"
    "BM_stdio_fwrite_unbuffered/16384/iterations:1\n"
    "BM_stdio_fwrite_unbuffered/32768/iterations:1\n"
    "BM_stdio_fwrite_unbuffered/65536/iterations:1\n"
    "BM_stdio_fopen_fgets_fclose_locking/iterations:1\n"
    "BM_stdio_fopen_fgets_fclose_no_locking/iterations:1\n"
    "BM_string_memcmp/8/0/0/iterations:1\n"
    "BM_string_memcmp/64/0/0/iterations:1\n"
    "BM_string_memcmp/512/0/0/iterations:1\n"
    "BM_string_memcmp/1024/0/0/iterations:1\n"
    "BM_string_memcmp/8192/0/0/iterations:1\n"
    "BM_string_memcmp/16384/0/0/iterations:1\n"
    "BM_string_memcmp/32768/0/0/iterations:1\n"
    "BM_string_memcmp/65536/0/0/iterations:1\n"
    "BM_string_memcpy/8/0/0/iterations:1\n"
    "BM_string_memcpy/64/0/0/iterations:1\n"
    "BM_string_memcpy/512/0/0/iterations:1\n"
    "BM_string_memcpy/1024/0/0/iterations:1\n"
    "BM_string_memcpy/8192/0/0/iterations:1\n"
    "BM_string_memcpy/16384/0/0/iterations:1\n"
    "BM_string_memcpy/32768/0/0/iterations:1\n"
    "BM_string_memcpy/65536/0/0/iterations:1\n"
    "BM_string_memmove_non_overlapping/8/0/iterations:1\n"
    "BM_string_memmove_non_overlapping/64/0/iterations:1\n"
    "BM_string_memmove_non_overlapping/512/0/iterations:1\n"
    "BM_string_memmove_non_overlapping/1024/0/iterations:1\n"
    "BM_string_memmove_non_overlapping/8192/0/iterations:1\n"
    "BM_string_memmove_non_overlapping/16384/0/iterations:1\n"
    "BM_string_memmove_non_overlapping/32768/0/iterations:1\n"
    "BM_string_memmove_non_overlapping/65536/0/iterations:1\n"
    "BM_string_memmove_overlap_dst_before_src/8/0/iterations:1\n"
    "BM_string_memmove_overlap_dst_before_src/64/0/iterations:1\n"
    "BM_string_memmove_overlap_dst_before_src/512/0/iterations:1\n"
    "BM_string_memmove_overlap_dst_before_src/1024/0/iterations:1\n"
    "BM_string_memmove_overlap_dst_before_src/8192/0/iterations:1\n"
    "BM_string_memmove_overlap_dst_before_src/16384/0/iterations:1\n"
    "BM_string_memmove_overlap_dst_before_src/32768/0/iterations:1\n"
    "BM_string_memmove_overlap_dst_before_src/65536/0/iterations:1\n"
    "BM_string_memmove_overlap_src_before_dst/8/0/iterations:1\n"
    "BM_string_memmove_overlap_src_before_dst/64/0/iterations:1\n"
    "BM_string_memmove_overlap_src_before_dst/512/0/iterations:1\n"
    "BM_string_memmove_overlap_src_before_dst/1024/0/iterations:1\n"
    "BM_string_memmove_overlap_src_before_dst/8192/0/iterations:1\n"
    "BM_string_memmove_overlap_src_before_dst/16384/0/iterations:1\n"
    "BM_string_memmove_overlap_src_before_dst/32768/0/iterations:1\n"
    "BM_string_memmove_overlap_src_before_dst/65536/0/iterations:1\n"
    "BM_string_memset/8/0/iterations:1\n"
    "BM_string_memset/64/0/iterations:1\n"
    "BM_string_memset/512/0/iterations:1\n"
    "BM_string_memset/1024/0/iterations:1\n"
    "BM_string_memset/8192/0/iterations:1\n"
    "BM_string_memset/16384/0/iterations:1\n"
    "BM_string_memset/32768/0/iterations:1\n"
    "BM_string_memset/65536/0/iterations:1\n"
    "BM_string_strlen/8/0/iterations:1\n"
    "BM_string_strlen/64/0/iterations:1\n"
    "BM_string_strlen/512/0/iterations:1\n"
    "BM_string_strlen/1024/0/iterations:1\n"
    "BM_string_strlen/8192/0/iterations:1\n"
    "BM_string_strlen/16384/0/iterations:1\n"
    "BM_string_strlen/32768/0/iterations:1\n"
    "BM_string_strlen/65536/0/iterations:1\n"
    "BM_string_strcat_copy_only/8/0/0/iterations:1\n"
    "BM_string_strcat_copy_only/64/0/0/iterations:1\n"
    "BM_string_strcat_copy_only/512/0/0/iterations:1\n"
    "BM_string_strcat_copy_only/1024/0/0/iterations:1\n"
    "BM_string_strcat_copy_only/8192/0/0/iterations:1\n"
    "BM_string_strcat_copy_only/16384/0/0/iterations:1\n"
    "BM_string_strcat_copy_only/32768/0/0/iterations:1\n"
    "BM_string_strcat_copy_only/65536/0/0/iterations:1\n"
    "BM_string_strcat_seek_only/8/0/0/iterations:1\n"
    "BM_string_strcat_seek_only/64/0/0/iterations:1\n"
    "BM_string_strcat_seek_only/512/0/0/iterations:1\n"
    "BM_string_strcat_seek_only/1024/0/0/iterations:1\n"
    "BM_string_strcat_seek_only/8192/0/0/iterations:1\n"
    "BM_string_strcat_seek_only/16384/0/0/iterations:1\n"
    "BM_string_strcat_seek_only/32768/0/0/iterations:1\n"
    "BM_string_strcat_seek_only/65536/0/0/iterations:1\n"
    "BM_string_strcat_half_copy_half_seek/8/0/0/iterations:1\n"
    "BM_string_strcat_half_copy_half_seek/64/0/0/iterations:1\n"
    "BM_string_strcat_half_copy_half_seek/512/0/0/iterations:1\n"
    "BM_string_strcat_half_copy_half_seek/1024/0/0/iterations:1\n"
    "BM_string_strcat_half_copy_half_seek/8192/0/0/iterations:1\n"
    "BM_string_strcat_half_copy_half_seek/16384/0/0/iterations:1\n"
    "BM_string_strcat_half_copy_half_seek/32768/0/0/iterations:1\n"
    "BM_string_strcat_half_copy_half_seek/65536/0/0/iterations:1\n"
    "BM_string_strcpy/8/0/0/iterations:1\n"
    "BM_string_strcpy/64/0/0/iterations:1\n"
    "BM_string_strcpy/512/0/0/iterations:1\n"
    "BM_string_strcpy/1024/0/0/iterations:1\n"
    "BM_string_strcpy/8192/0/0/iterations:1\n"
    "BM_string_strcpy/16384/0/0/iterations:1\n"
    "BM_string_strcpy/32768/0/0/iterations:1\n"
    "BM_string_strcpy/65536/0/0/iterations:1\n"
    "BM_string_strcmp/8/0/0/iterations:1\n"
    "BM_string_strcmp/64/0/0/iterations:1\n"
    "BM_string_strcmp/512/0/0/iterations:1\n"
    "BM_string_strcmp/1024/0/0/iterations:1\n"
    "BM_string_strcmp/8192/0/0/iterations:1\n"
    "BM_string_strcmp/16384/0/0/iterations:1\n"
    "BM_string_strcmp/32768/0/0/iterations:1\n"
    "BM_string_strcmp/65536/0/0/iterations:1\n"
    "BM_time_clock_gettime/iterations:1\n"
    "BM_time_clock_gettime_syscall/iterations:1\n"
    "BM_time_gettimeofday/iterations:1\n"
    "BM_time_gettimeofday_syscall/iterations:1\n"
    "BM_time_time/iterations:1\n"
    "BM_unistd_getpid/iterations:1\n"
    "BM_unistd_getpid_syscall/iterations:1\n"
    "BM_unistd_gettid/iterations:1\n"
    "BM_unistd_gettid_syscall/iterations:1\n";
  Verify(expected, 0, std::vector<const char *>{"--bionic_xml=suites/test_full.xml",
                                                "--bionic_iterations=1"});
}

TEST_F(SystemTests, small) {
  std::string expected =
    "BM_string_memcmp/8/8/8\n"
    "BM_math_sqrt\n"
    "BM_property_get/1\n";
  Verify(expected, 0, std::vector<const char *>{"--bionic_xml=suites/test_small.xml"});
}

TEST_F(SystemTests, medium) {
  std::string expected =
    "BM_string_memcmp/8/0/0/iterations:1\n"
    "BM_string_memcmp/64/0/0/iterations:1\n"
    "BM_string_memcmp/512/0/0/iterations:1\n"
    "BM_string_memcmp/1024/0/0/iterations:1\n"
    "BM_string_memcmp/8192/0/0/iterations:1\n"
    "BM_string_memcmp/16384/0/0/iterations:1\n"
    "BM_string_memcmp/32768/0/0/iterations:1\n"
    "BM_string_memcmp/65536/0/0/iterations:1\n"
    "BM_math_sqrt/iterations:1\n"
    "BM_string_memcpy/512/4/4/iterations:25\n"
    "BM_property_get/1/iterations:1\n";
  Verify(expected, 0, std::vector<const char *>{"--bionic_xml=suites/test_medium.xml",
                                                "--bionic_iterations=1"});
}

TEST_F(SystemTests, from_each) {
  std::string expected =
    "BM_empty/iterations:1\n"
    "BM_math_sqrt/iterations:1\n"
    "BM_property_get/1/iterations:1\n"
    "BM_pthread_self/iterations:1\n"
    "BM_semaphore_sem_getvalue/iterations:1\n"
    "BM_stdio_fread/64/iterations:1\n"
    "BM_string_memcpy/512/4/4/iterations:1\n"
    "BM_time_clock_gettime/iterations:1\n"
    "BM_unistd_getpid/iterations:1\n";
  Verify(expected, 0, std::vector<const char *>{"--bionic_xml=suites/test_from_each.xml",
                                                "--bionic_iterations=1"});
}

TEST_F(SystemTests, cmd_args) {
  std::string expected =
    "BM_string_memcpy/8/8/8/iterations:1\n"
    "BM_math_log10/iterations:1\n";
  Verify(expected, 0, std::vector<const char *>{"--bionic_extra=BM_string_memcpy 8 8 8",
                                                "--bionic_extra=BM_math_log10",
                                                "--bionic_iterations=1"});
}

TEST_F(SystemTests, cmd_args_no_iter) {
  std::string expected =
    "BM_string_memcpy/8/8/8\n"
    "BM_math_log10\n";
  Verify(expected, 0, std::vector<const char *>{"--bionic_extra=BM_string_memcpy 8 8 8",
                                                "--bionic_extra=BM_math_log10"});
}

TEST_F(SystemTests, xml_and_args) {
  std::string expected =
    "BM_string_memcmp/8/0/0/iterations:1\n"
    "BM_string_memcmp/64/0/0/iterations:1\n"
    "BM_string_memcmp/512/0/0/iterations:1\n"
    "BM_string_memcmp/1024/0/0/iterations:1\n"
    "BM_string_memcmp/8192/0/0/iterations:1\n"
    "BM_string_memcmp/16384/0/0/iterations:1\n"
    "BM_string_memcmp/32768/0/0/iterations:1\n"
    "BM_string_memcmp/65536/0/0/iterations:1\n"
    "BM_math_sqrt/iterations:1\n"
    "BM_string_memcpy/512/4/4/iterations:25\n"
    "BM_property_get/1/iterations:1\n"
    "BM_string_memcpy/8/0/0/iterations:1\n"
    "BM_string_memcpy/64/0/0/iterations:1\n"
    "BM_string_memcpy/512/0/0/iterations:1\n"
    "BM_string_memcpy/1024/0/0/iterations:1\n"
    "BM_string_memcpy/8192/0/0/iterations:1\n"
    "BM_string_memcpy/16384/0/0/iterations:1\n"
    "BM_string_memcpy/32768/0/0/iterations:1\n"
    "BM_string_memcpy/65536/0/0/iterations:1\n"
    "BM_math_log10/iterations:1\n";
  Verify(expected, 0, std::vector<const char *>{"--bionic_extra=BM_string_memcpy AT_ALIGNED_TWOBUF",
                                                "--bionic_extra=BM_math_log10",
                                                "--bionic_cpu -1",
                                                "--bionic_xml=suites/test_medium.xml",
                                                "--bionic_iterations=1"});
}

TEST_F(SystemTests, alignment) {
  std::string expected =
    "BM_string_memcmp/8/2/2/iterations:1\n"
    "BM_string_memcmp/64/2/2/iterations:1\n"
    "BM_string_memcmp/512/2/2/iterations:1\n"
    "BM_string_memcmp/1024/2/2/iterations:1\n"
    "BM_string_memcmp/8192/2/2/iterations:1\n"
    "BM_string_memcmp/16384/2/2/iterations:1\n"
    "BM_string_memcmp/32768/2/2/iterations:1\n"
    "BM_string_memcmp/65536/2/2/iterations:1\n"
    "BM_string_memcmp/8/4/4/iterations:1\n"
    "BM_string_memcmp/64/4/4/iterations:1\n"
    "BM_string_memcmp/512/4/4/iterations:1\n"
    "BM_string_memcmp/1024/4/4/iterations:1\n"
    "BM_string_memcmp/8192/4/4/iterations:1\n"
    "BM_string_memcmp/16384/4/4/iterations:1\n"
    "BM_string_memcmp/32768/4/4/iterations:1\n"
    "BM_string_memcmp/65536/4/4/iterations:1\n"
    "BM_string_memcmp/8/16/16/iterations:1\n"
    "BM_string_memcmp/64/16/16/iterations:1\n"
    "BM_string_memcmp/512/16/16/iterations:1\n"
    "BM_string_memcmp/1024/16/16/iterations:1\n"
    "BM_string_memcmp/8192/16/16/iterations:1\n"
    "BM_string_memcmp/16384/16/16/iterations:1\n"
    "BM_string_memcmp/32768/16/16/iterations:1\n"
    "BM_string_memcmp/65536/16/16/iterations:1\n"
    "BM_string_memcmp/8/512/512/iterations:1\n"
    "BM_string_memcmp/64/512/512/iterations:1\n"
    "BM_string_memcmp/512/512/512/iterations:1\n"
    "BM_string_memcmp/1024/512/512/iterations:1\n"
    "BM_string_memcmp/8192/512/512/iterations:1\n"
    "BM_string_memcmp/16384/512/512/iterations:1\n"
    "BM_string_memcmp/32768/512/512/iterations:1\n"
    "BM_string_memcmp/65536/512/512/iterations:1\n"
    "BM_string_memcmp/8/2048/2048/iterations:1\n"
    "BM_string_memcmp/64/2048/2048/iterations:1\n"
    "BM_string_memcmp/512/2048/2048/iterations:1\n"
    "BM_string_memcmp/1024/2048/2048/iterations:1\n"
    "BM_string_memcmp/8192/2048/2048/iterations:1\n"
    "BM_string_memcmp/16384/2048/2048/iterations:1\n"
    "BM_string_memcmp/32768/2048/2048/iterations:1\n"
    "BM_string_memcmp/65536/2048/2048/iterations:1\n"
    "BM_string_strlen/8/2/iterations:1\n"
    "BM_string_strlen/64/2/iterations:1\n"
    "BM_string_strlen/512/2/iterations:1\n"
    "BM_string_strlen/1024/2/iterations:1\n"
    "BM_string_strlen/8192/2/iterations:1\n"
    "BM_string_strlen/16384/2/iterations:1\n"
    "BM_string_strlen/32768/2/iterations:1\n"
    "BM_string_strlen/65536/2/iterations:1\n"
    "BM_string_strlen/8/4/iterations:1\n"
    "BM_string_strlen/64/4/iterations:1\n"
    "BM_string_strlen/512/4/iterations:1\n"
    "BM_string_strlen/1024/4/iterations:1\n"
    "BM_string_strlen/8192/4/iterations:1\n"
    "BM_string_strlen/16384/4/iterations:1\n"
    "BM_string_strlen/32768/4/iterations:1\n"
    "BM_string_strlen/65536/4/iterations:1\n"
    "BM_string_strlen/8/16/iterations:1\n"
    "BM_string_strlen/64/16/iterations:1\n"
    "BM_string_strlen/512/16/iterations:1\n"
    "BM_string_strlen/1024/16/iterations:1\n"
    "BM_string_strlen/8192/16/iterations:1\n"
    "BM_string_strlen/16384/16/iterations:1\n"
    "BM_string_strlen/32768/16/iterations:1\n"
    "BM_string_strlen/65536/16/iterations:1\n"
    "BM_string_strlen/8/512/iterations:1\n"
    "BM_string_strlen/64/512/iterations:1\n"
    "BM_string_strlen/512/512/iterations:1\n"
    "BM_string_strlen/1024/512/iterations:1\n"
    "BM_string_strlen/8192/512/iterations:1\n"
    "BM_string_strlen/16384/512/iterations:1\n"
    "BM_string_strlen/32768/512/iterations:1\n"
    "BM_string_strlen/65536/512/iterations:1\n"
    "BM_string_strlen/8/2048/iterations:1\n"
    "BM_string_strlen/64/2048/iterations:1\n"
    "BM_string_strlen/512/2048/iterations:1\n"
    "BM_string_strlen/1024/2048/iterations:1\n"
    "BM_string_strlen/8192/2048/iterations:1\n"
    "BM_string_strlen/16384/2048/iterations:1\n"
    "BM_string_strlen/32768/2048/iterations:1\n"
    "BM_string_strlen/65536/2048/iterations:1\n";
  Verify(expected, 0, std::vector<const char *>{"--bionic_xml=suites/test_alignment.xml",
                                                "--bionic_iterations=100"});
}
