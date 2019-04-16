/*
 * Copyright (C) 2018 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <android-base/stringprintf.h>
#include <gtest/gtest.h>
#include <log/log.h>

#include <string>
#include <vector>

#include "private/bionic_malloc.h"

static constexpr time_t kTimeoutSeconds = 5;

static void Exec(const char* test_name, const char* debug_options, pid_t* pid) {
  int fds[2];
  ASSERT_NE(-1, pipe(fds));
  ASSERT_NE(-1, fcntl(fds[0], F_SETFL, O_NONBLOCK));
  if ((*pid = fork()) == 0) {
    ASSERT_EQ(0, setenv("LIBC_DEBUG_MALLOC_OPTIONS", debug_options, 1));
    close(fds[0]);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    ASSERT_NE(0, dup2(fds[1], STDOUT_FILENO));
    ASSERT_NE(0, dup2(fds[1], STDERR_FILENO));

    std::vector<const char*> args;
    // Get a copy of this argument so it doesn't disappear on us.
    std::string exec(testing::internal::GetArgvs()[0]);
    args.push_back(exec.c_str());
    args.push_back("--gtest_also_run_disabled_tests");
    std::string filter_arg = std::string("--gtest_filter=") + test_name;
    args.push_back(filter_arg.c_str());
    args.push_back(nullptr);
    execv(args[0], reinterpret_cast<char* const*>(const_cast<char**>(args.data())));
    exit(20);
  }
  ASSERT_NE(-1, *pid);
  close(fds[1]);

  std::string output;
  std::vector<char> buffer(4096);
  time_t start_time = time(nullptr);
  bool done = false;
  while (true) {
    struct pollfd read_fd = {.fd = fds[0], .events = POLLIN};
    if (TEMP_FAILURE_RETRY(poll(&read_fd, 1, 1)) > 0) {
      ssize_t bytes = TEMP_FAILURE_RETRY(read(fds[0], buffer.data(), sizeof(buffer) - 1));
      if (bytes == -1 && errno == EAGAIN) {
        continue;
      }
      ASSERT_NE(-1, bytes);
      if (bytes == 0) {
        done = true;
        break;
      }
      output.append(buffer.data(), bytes);
    }

    if ((time(nullptr) - start_time) > kTimeoutSeconds) {
      break;
    }
  }
  EXPECT_TRUE(done) << "Timed out while reading data, output:\n" << output;

  done = false;
  int status;
  start_time = time(nullptr);
  while (true) {
    int wait_pid = waitpid(*pid, &status, WNOHANG);
    if (*pid == wait_pid) {
      done = true;
      break;
    }
    if ((time(nullptr) - start_time) > kTimeoutSeconds) {
      break;
    }
  }
  if (!done) {
    kill(*pid, SIGKILL);
    start_time = time(nullptr);
    while (true) {
      int kill_status;
      int wait_pid = waitpid(*pid, &kill_status, WNOHANG);
      if (wait_pid == *pid || (time(nullptr) - start_time) > kTimeoutSeconds) {
        break;
      }
    }
  }

  ASSERT_TRUE(done) << "Timed out waiting for waitpid, output:\n" << output;
  ASSERT_EQ(0, WEXITSTATUS(status)) << "Output:\n" << output;
}

static void GetLogStr(pid_t pid, std::string* log_str) {
  log_str->clear();

  logger_list* list;
  list = android_logger_list_open(LOG_ID_MAIN, ANDROID_LOG_RDONLY | ANDROID_LOG_NONBLOCK, 1000, pid);
  ASSERT_TRUE(list != nullptr);

  while (true) {
    log_msg msg;
    ssize_t actual = android_logger_list_read(list, &msg);
    if (actual < 0) {
      if (actual == -EINTR) {
        // Interrupted retry.
        continue;
      } else if (actual == -EAGAIN) {
        // Non-blocking EOF, finished.
        break;
      } else {
        break;
      }
    } else if (actual == 0) {
      break;
    }
    ASSERT_EQ(msg.entry.pid, pid);

    char* msg_str = msg.msg();
    if (msg_str != nullptr) {
      char* tag = msg_str + 1;
      msg_str = tag + strlen(tag) + 1;
      *log_str += msg_str;
      if (log_str->back() != '\n') {
        *log_str += '\n';
      }
    }
  }

  android_logger_list_close(list);
}

static void FindStrings(pid_t pid, std::vector<const char*> match_strings) {
  std::string log_str;
  time_t start = time(nullptr);
  bool found_all;
  while (true) {
    GetLogStr(pid, &log_str);
    found_all = true;
    for (auto str : match_strings) {
      if (log_str.find(str) == std::string::npos) {
        found_all = false;
        break;
      }
    }
    if (found_all) {
      return;
    }
    if ((time(nullptr) - start) > kTimeoutSeconds) {
      break;
    }
  }
  ASSERT_TRUE(found_all) << "Didn't find expected log output:\n" + log_str;
}

TEST(MallocTests, DISABLED_smoke) {}

TEST(MallocDebugSystemTest, smoke) {
  pid_t pid;
  ASSERT_NO_FATAL_FAILURE(Exec("MallocTests.DISABLED_smoke", "verbose backtrace", &pid));

  ASSERT_NO_FATAL_FAILURE(FindStrings(pid, std::vector<const char*>{"malloc debug enabled"}));
}

static void SetAllocationLimit() {
  // Set to a large value, this is only to enable the limit code and
  // verify that malloc debug is still called properly.
  size_t limit = 500 * 1024 * 1024;
  ASSERT_TRUE(android_mallopt(M_SET_ALLOCATION_LIMIT_BYTES, &limit, sizeof(limit)));
}

static void AlignedAlloc() {
  void* ptr = aligned_alloc(64, 1152);
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 0, 1152);
}

TEST(MallocTests, DISABLED_leak_memory_aligned_alloc) {
  AlignedAlloc();
}

TEST(MallocTests, DISABLED_leak_memory_limit_aligned_alloc) {
  SetAllocationLimit();
  AlignedAlloc();
}

static void Calloc() {
  void* ptr = calloc(1, 1123);
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 1, 1123);
}

TEST(MallocTests, DISABLED_leak_memory_calloc) {
  Calloc();
}

TEST(MallocTests, DISABLED_leak_memory_limit_calloc) {
  SetAllocationLimit();
  Calloc();
}

static void Malloc() {
  void* ptr = malloc(1123);
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 0, 1123);
}

TEST(MallocTests, DISABLED_leak_memory_malloc) {
  Malloc();
}

TEST(MallocTests, DISABLED_leak_memory_limit_malloc) {
  SetAllocationLimit();
  Malloc();
}

static void Memalign() {
  void* ptr = memalign(64, 1123);
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 0, 1123);
}

TEST(MallocTests, DISABLED_leak_memory_memalign) {
  Memalign();
}

TEST(MallocTests, DISABLED_leak_memory_limit_memalign) {
  SetAllocationLimit();
  Memalign();
}

static void PosixMemalign() {
  void* ptr;
  ASSERT_EQ(0, posix_memalign(&ptr, 64, 1123));
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 0, 1123);
}

TEST(MallocTests, DISABLED_leak_memory_posix_memalign) {
  PosixMemalign();
}

TEST(MallocTests, DISABLED_leak_memory_limit_posix_memalign) {
  SetAllocationLimit();
  PosixMemalign();
}

static void Reallocarray() {
  void* ptr = reallocarray(nullptr, 1, 1123);
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 0, 1123);
}

TEST(MallocTests, DISABLED_leak_memory_reallocarray) {
  Reallocarray();
}

TEST(MallocTests, DISABLED_leak_memory_limit_reallocarray) {
  SetAllocationLimit();
  Reallocarray();
}

static void Realloc() {
  void* ptr = realloc(nullptr, 1123);
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 0, 1123);
}

TEST(MallocTests, DISABLED_leak_memory_realloc) {
  Realloc();
}

TEST(MallocTests, DISABLED_leak_memory_limit_realloc) {
  SetAllocationLimit();
  Realloc();
}

#if !defined(__LP64__)
extern "C" void* pvalloc(size_t);

static void Pvalloc() {
  void* ptr = pvalloc(1123);
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 0, 1123);
}

TEST(MallocTests, DISABLED_leak_memory_pvalloc) {
  Pvalloc();
}

TEST(MallocTests, DISABLED_leak_memory_limit_pvalloc) {
  SetAllocationLimit();
  Pvalloc();
}

extern "C" void* valloc(size_t);

static void Valloc() {
  void* ptr = valloc(1123);
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 0, 1123);
}

TEST(MallocTests, DISABLED_leak_memory_valloc) {
  Valloc();
}

TEST(MallocTests, DISABLED_leak_memory_limit_valloc) {
  SetAllocationLimit();
  Valloc();
}
#endif

static void VerifyLeak(const char* test_prefix) {
  struct FunctionInfo {
    const char* name;
    size_t size;
  };
  static FunctionInfo functions[] = {
    {
      "aligned_alloc",
      1152,
    },
    {
      "calloc",
      1123,
    },
    {
      "malloc",
      1123,
    },
    {
      "memalign",
      1123,
    },
    {
      "posix_memalign",
      1123,
    },
    {
      "reallocarray",
      1123,
    },
    {
      "realloc",
      1123,
    },
#if !defined(__LP64__)
    {
      "pvalloc",
      4096,
    },
    {
      "valloc",
      1123,
    }
#endif
  };

  for (size_t i = 0; i < sizeof(functions) / sizeof(FunctionInfo); i++) {
    pid_t pid;
    SCOPED_TRACE(testing::Message() << functions[i].name << " expected size " << functions[i].size);
    std::string test = std::string("MallocTests.DISABLED_") + test_prefix + functions[i].name;
    EXPECT_NO_FATAL_FAILURE(Exec(test.c_str(), "verbose backtrace leak_track", &pid));

    std::string expected_leak = android::base::StringPrintf("leaked block of size %zu at", functions[i].size);
    EXPECT_NO_FATAL_FAILURE(FindStrings(
        pid, std::vector<const char*>{"malloc debug enabled", expected_leak.c_str()}));
  }
}

TEST(MallocDebugSystemTest, verify_leak) {
  VerifyLeak("leak_memory_");
}

TEST(MallocDebugSystemTest, verify_leak_allocation_limit) {
  VerifyLeak("leak_memory_limit_");
}
