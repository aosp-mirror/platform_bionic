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

#include <android-base/file.h>
#include <android-base/stringprintf.h>
#include <gtest/gtest.h>
#include <log/log.h>

#include <atomic>
#include <string>
#include <thread>
#include <vector>

#include <backtrace/Backtrace.h>
#include <backtrace/BacktraceMap.h>

#include <bionic/malloc.h>

// All DISABLED_ tests are designed to be executed after malloc debug
// is enabled. These tests don't run be default, and are executed
// by wrappers that will enable various malloc debug features.

static constexpr time_t kTimeoutSeconds = 10;

extern "C" bool GetInitialArgs(const char*** args, size_t* num_args) {
  static const char* initial_args[] = {"--slow_threshold_ms=30000",
                                       "--deadline_threshold_ms=1200000"};
  *args = initial_args;
  *num_args = 2;
  return true;
}

static void Exec(const char* test_name, const char* debug_options, pid_t* pid, int exit_code = 0,
                 time_t timeout_seconds = kTimeoutSeconds) {
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
    // Need this because some code depends on exit codes from the test run
    // but the isolation runner does not support that.
    args.push_back("--no_isolate");
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

    if ((time(nullptr) - start_time) > timeout_seconds) {
      kill(*pid, SIGINT);
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
    if ((time(nullptr) - start_time) > timeout_seconds) {
      break;
    }
  }
  if (!done) {
    kill(*pid, SIGKILL);
    start_time = time(nullptr);
    while (true) {
      int kill_status;
      int wait_pid = waitpid(*pid, &kill_status, WNOHANG);
      if (wait_pid == *pid || (time(nullptr) - start_time) > timeout_seconds) {
        break;
      }
    }
  }

  ASSERT_TRUE(done) << "Timed out waiting for waitpid, output:\n" << output;
  ASSERT_FALSE(WIFSIGNALED(status))
      << "Failed with signal " << WTERMSIG(status) << "\nOutput:\n" << output;
  ASSERT_EQ(exit_code, WEXITSTATUS(status)) << "Output:\n" << output;
}

static void GetLogStr(pid_t pid, std::string* log_str, log_id log = LOG_ID_MAIN) {
  log_str->clear();

  logger_list* list;
  list = android_logger_list_open(log, ANDROID_LOG_RDONLY | ANDROID_LOG_NONBLOCK, 1000, pid);
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

static void FindStrings(pid_t pid, std::vector<const char*> match_strings,
                        std::vector<const char*> no_match_strings = std::vector<const char*>{},
                        time_t timeout_seconds = kTimeoutSeconds) {
  std::string log_str;
  time_t start = time(nullptr);
  std::string missing_match;
  while (true) {
    GetLogStr(pid, &log_str);
    missing_match.clear();
    // Look for the expected strings.
    for (auto str : match_strings) {
      if (log_str.find(str) == std::string::npos) {
        missing_match = str;
        break;
      }
    }

    // Verify the unexpected strings are not present.
    for (auto str : no_match_strings) {
      ASSERT_TRUE(log_str.find(str) == std::string::npos) << "Unexpectedly found '" << str << "' in log output:\n" << log_str;
    }
    if (missing_match.empty()) {
      return;
    }
    if ((time(nullptr) - start) > timeout_seconds) {
      break;
    }
  }
  ASSERT_EQ("", missing_match) << "Didn't find expected log output:\n" << log_str;
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
    ASSERT_NO_FATAL_FAILURE(Exec(test.c_str(), "verbose backtrace leak_track", &pid));

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

static constexpr int kExpectedExitCode = 30;

TEST(MallocTests, DISABLED_exit_while_threads_allocating) {
  std::atomic_uint32_t thread_mask;
  thread_mask = 0;

  for (size_t i = 0; i < 32; i++) {
    std::thread malloc_thread([&thread_mask, i] {
      while (true) {
        void* ptr = malloc(100);
        if (ptr == nullptr) {
          exit(1000);
        }
        free(ptr);
        thread_mask.fetch_or(1 << i);
      }
    });
    malloc_thread.detach();
  }

  // Wait until each thread has done at least one allocation.
  while (thread_mask.load() != 0xffffffff)
    ;
  exit(kExpectedExitCode);
}

// Verify that exiting while other threads are doing malloc operations,
// that there are no crashes.
TEST(MallocDebugSystemTest, exit_while_threads_allocating) {
  for (size_t i = 0; i < 100; i++) {
    SCOPED_TRACE(::testing::Message() << "Run " << i);
    pid_t pid;
    ASSERT_NO_FATAL_FAILURE(Exec("MallocTests.DISABLED_exit_while_threads_allocating",
                                 "verbose backtrace", &pid, kExpectedExitCode));

    ASSERT_NO_FATAL_FAILURE(FindStrings(pid, std::vector<const char*>{"malloc debug enabled"}));

    std::string log_str;
    GetLogStr(pid, &log_str, LOG_ID_CRASH);
    ASSERT_TRUE(log_str.find("Fatal signal") == std::string::npos)
        << "Found crash in log.\nLog message: " << log_str;
  }
}

TEST(MallocTests, DISABLED_write_leak_info) {
  TemporaryFile tf;
  ASSERT_TRUE(tf.fd != -1);

  FILE* fp = fdopen(tf.fd, "w+");
  if (fp == nullptr) {
    printf("Unable to create %s\n", tf.path);
    _exit(1);
  }
  tf.release();

  void* ptr = malloc(1000);
  if (ptr == nullptr) {
    printf("malloc failed\n");
    _exit(1);
  }
  memset(ptr, 0, 1000);

  android_mallopt(M_WRITE_MALLOC_LEAK_INFO_TO_FILE, fp, sizeof(fp));

  fclose(fp);

  free(ptr);
}

TEST(MallocDebugSystemTest, write_leak_info_no_header) {
  pid_t pid;
  ASSERT_NO_FATAL_FAILURE(Exec("MallocTests.DISABLED_write_leak_info", "verbose backtrace", &pid, 0));

  ASSERT_NO_FATAL_FAILURE(FindStrings(pid, std::vector<const char*>{"malloc debug enabled"},

                          std::vector<const char*>{" HAS INVALID TAG ", "USED AFTER FREE ", "UNKNOWN POINTER "}));
}

TEST(MallocDebugSystemTest, write_leak_info_header) {
  pid_t pid;
  ASSERT_NO_FATAL_FAILURE(Exec("MallocTests.DISABLED_write_leak_info", "verbose backtrace guard", &pid, 0));

  ASSERT_NO_FATAL_FAILURE(FindStrings(pid, std::vector<const char*>{"malloc debug enabled"},
                          std::vector<const char*>{" HAS INVALID TAG ", "USED AFTER FREE ", "UNKNOWN POINTER "}));
}

TEST(MallocTests, DISABLED_malloc_and_backtrace_deadlock) {
  std::atomic_bool running(false);
  pid_t tid;
  std::thread thread([&tid, &running] {
    tid = gettid();
    running = true;
    while (running) {
      void* ptr = malloc(200);
      if (ptr == nullptr) {
        return;
      }
      free(ptr);
    }
  });

  while (!running) {
  }

  static constexpr size_t kNumUnwinds = 1000;
  for (size_t i = 0; i < kNumUnwinds; i++) {
    std::unique_ptr<Backtrace> backtrace(Backtrace::Create(getpid(), tid));
    ASSERT_TRUE(backtrace->Unwind(0)) << "Failed on unwind " << i;
    ASSERT_LT(1, backtrace->NumFrames()) << "Failed on unwind " << i;
  }
  running = false;
  thread.join();
}

TEST(MallocDebugSystemTest, malloc_and_backtrace_deadlock) {
  pid_t pid;
  ASSERT_NO_FATAL_FAILURE(Exec("MallocTests.DISABLED_malloc_and_backtrace_deadlock",
                               "verbose verify_pointers", &pid, 0, 180));

  // Make sure that malloc debug is enabled and that no timeouts occur during
  // unwinds.
  ASSERT_NO_FATAL_FAILURE(FindStrings(pid, std::vector<const char*>{"malloc debug enabled"},
                                      std::vector<const char*>{"Timed out waiting for "}));
}
