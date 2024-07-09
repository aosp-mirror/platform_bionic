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
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <android-base/file.h>
#include <android-base/stringprintf.h>
#include <android-base/test_utils.h>
#include <gtest/gtest.h>
#include <log/log_read.h>

#include <atomic>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include <unwindstack/AndroidUnwinder.h>

#include <bionic/malloc.h>
#include <tests/utils.h>

// All DISABLED_ tests are designed to be executed after malloc debug
// is enabled. These tests don't run be default, and are executed
// by wrappers that will enable various malloc debug features.

extern "C" bool GetInitialArgs(const char*** args, size_t* num_args) {
  static const char* initial_args[] = {"--slow_threshold_ms=30000",
                                       "--deadline_threshold_ms=1200000"};
  *args = initial_args;
  *num_args = 2;
  return true;
}

class LogReader {
 public:
  LogReader(pid_t pid, log_id log) {
    std::call_once(log_start_time_flag_, []() {
      // Use this to figure out the point at which to start grabbing the log.
      // This avoids accidentally grabbing data from a previous process with
      // the same pid.
      log_start_time_ = {};
      logger_list* list = android_logger_list_open(LOG_ID_MAIN, ANDROID_LOG_NONBLOCK, INT_MAX, 0);
      if (list == nullptr) {
        return;
      }
      log_msg log_msg;
      int ret = android_logger_list_read(list, &log_msg);
      android_logger_list_close(list);
      if (ret <= 0) {
        return;
      }
      log_start_time_.tv_sec = log_msg.entry.sec;
      log_start_time_.tv_nsec = log_msg.entry.nsec;
    });

    std::call_once(jmp_data_key_flag_, []() {
      pthread_key_create(&jmp_data_key_, [](void* ptr) { free(ptr); });
      signal(SIGUSR1, [](int) {
        jmp_buf* jb = reinterpret_cast<jmp_buf*>(pthread_getspecific(jmp_data_key_));
        if (jb != nullptr) {
          // The thread reading the log is in a blocking read call that
          // cannot be interrupted. In order to get out of this read loop,
          // it's necessary to call longjmp when a SIGUSR1 signal is sent
          // to the thread.
          longjmp(*jb, 1);
        }
      });
    });

    reader_.reset(new std::thread([this, pid, log] {
      tid_.store(gettid(), std::memory_order_release);
      logger_list* list;
      while (true) {
        // Do not use non-blocking mode so that the two threads
        // are essentially asleep and not consuming any cpu.
        list = android_logger_list_open(log, 0, INT_MAX, pid);
        if (list != nullptr) {
          break;
        }
        // Wait for a short time for the log to become available.
        usleep(1000);
      }

      jmp_buf* jb = reinterpret_cast<jmp_buf*>(malloc(sizeof(jmp_buf)));
      if (jb == nullptr) {
        printf("Failed to allocate memory for jmp_buf\n");
        return;
      }
      pthread_setspecific(jmp_data_key_, jb);
      if (setjmp(*jb) != 0) {
        // SIGUSR1 signal hit, we need to terminate the thread.
        android_logger_list_free(list);
        return;
      }

      while (true) {
        log_msg msg;
        int actual = android_logger_list_read(list, &msg);
        if (actual < 0) {
          if (actual == -EINTR) {
            // Interrupted retry.
            continue;
          }
          // Unknown error.
          break;
        } else if (actual == 0) {
          // Nothing left to read.
          break;
        }
        // Do not allow SIGUSR1 while processing the log message.
        // This avoids a deadlock if the thread is being terminated
        // at this moment.
        sigset64_t mask_set;
        sigprocmask64(SIG_SETMASK, nullptr, &mask_set);
        sigaddset64(&mask_set, SIGUSR1);
        sigprocmask64(SIG_SETMASK, &mask_set, nullptr);

        {
          // Lock while appending to the data.
          std::lock_guard<std::mutex> guard(data_lock_);
          char* msg_str = msg.msg();
          // Make sure the message is not empty and recent.
          if (msg_str != nullptr && (msg.entry.sec > log_start_time_.tv_sec ||
                                     (msg.entry.sec == log_start_time_.tv_sec &&
                                      msg.entry.nsec > log_start_time_.tv_nsec))) {
            // Skip the tag part of the message.
            char* tag = msg_str + 1;
            msg_str = tag + strlen(tag) + 1;
            log_data_ += msg_str;
            if (log_data_.back() != '\n') {
              log_data_ += '\n';
            }
          }
        }

        // Re-enable SIGUSR1
        sigprocmask64(SIG_SETMASK, nullptr, &mask_set);
        sigdelset64(&mask_set, SIGUSR1);
        sigprocmask64(SIG_SETMASK, &mask_set, nullptr);
      }
      android_logger_list_free(list);
    }));
  }

  virtual ~LogReader() {
    tgkill(getpid(), tid_.load(std::memory_order_acquire), SIGUSR1);
    reader_->join();
  }

  std::string GetLog() {
    std::lock_guard<std::mutex> guard(data_lock_);
    return log_data_;
  }

 private:
  std::unique_ptr<std::thread> reader_;
  std::string log_data_;
  std::mutex data_lock_;
  std::atomic<pid_t> tid_;

  static std::once_flag jmp_data_key_flag_;
  static pthread_key_t jmp_data_key_;

  static std::once_flag log_start_time_flag_;
  static log_time log_start_time_;
};

std::once_flag LogReader::jmp_data_key_flag_;
pthread_key_t LogReader::jmp_data_key_;

std::once_flag LogReader::log_start_time_flag_;
log_time LogReader::log_start_time_;

class MallocDebugSystemTest : public ::testing::Test {
 protected:
  void SetUp() override {
    expected_log_strings_.clear();
    unexpected_log_strings_.clear();

    // All tests expect this message to be present.
    expected_log_strings_.push_back("malloc debug enabled");
  }

  void Exec(const char* test_name, const char* debug_options, int expected_exit_code = 0) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> rand_usleep_time(1, 10);
    std::srand(std::time(nullptr));

    for (size_t i = 0; i < kMaxRetries; i++) {
      ASSERT_NO_FATAL_FAILURE(InternalExec(test_name, debug_options, expected_exit_code));

      // Due to log messages sometimes getting lost, if a log message
      // is not present, allow retrying the test.
      std::string error_msg;
      bool found_expected = CheckExpectedLogStrings(&error_msg);
      if (!found_expected) {
        ASSERT_NE(i, kMaxRetries - 1) << error_msg;
        // Sleep a random amount of time to attempt to avoid tests syncing
        // up and sending the log messages at the same time.
        usleep(1000 * rand_usleep_time(generator));
      }

      // This doesn't need to be retried since if the log message is
      // present, that is an immediate fail.
      ASSERT_NO_FATAL_FAILURE(VerifyUnexpectedLogStrings());
      if (found_expected) {
        break;
      }
    }
  }

  void InternalExec(const char* test_name, const char* debug_options, int expected_exit_code) {
    int fds[2];
    ASSERT_NE(-1, pipe(fds));
    ASSERT_NE(-1, fcntl(fds[0], F_SETFL, O_NONBLOCK));
    if ((pid_ = fork()) == 0) {
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
    ASSERT_NE(-1, pid_);
    close(fds[1]);

    // Create threads to read the log output from the forked process as
    // soon as possible in case there is something flooding the log.
    log_main_.reset(new LogReader(pid_, LOG_ID_MAIN));
    log_crash_.reset(new LogReader(pid_, LOG_ID_CRASH));

    output_.clear();
    std::vector<char> buffer(4096);
    time_t start_time = time(nullptr);
    bool read_done = false;
    while (true) {
      struct pollfd read_fd = {.fd = fds[0], .events = POLLIN};
      if (TEMP_FAILURE_RETRY(poll(&read_fd, 1, 1)) > 0) {
        ssize_t bytes = TEMP_FAILURE_RETRY(read(fds[0], buffer.data(), sizeof(buffer) - 1));
        if (bytes == -1 && errno == EAGAIN) {
          continue;
        }
        ASSERT_NE(-1, bytes);
        if (bytes == 0) {
          read_done = true;
          break;
        }
        output_.append(buffer.data(), bytes);
      }

      if ((time(nullptr) - start_time) > kReadOutputTimeoutSeconds) {
        kill(pid_, SIGINT);
        break;
      }
    }

    bool done = false;
    int status;
    start_time = time(nullptr);
    while (true) {
      int wait_pid = waitpid(pid_, &status, WNOHANG);
      if (pid_ == wait_pid) {
        done = true;
        break;
      }
      if ((time(nullptr) - start_time) > kWaitpidTimeoutSeconds) {
        break;
      }
    }
    if (!done) {
      kill(pid_, SIGKILL);
      start_time = time(nullptr);
      while (true) {
        int kill_status;
        int wait_pid = waitpid(pid_, &kill_status, WNOHANG);
        if (wait_pid == pid_ || (time(nullptr) - start_time) > kWaitpidTimeoutSeconds) {
          break;
        }
      }
    }

    // Check timeout conditions first.
    ASSERT_TRUE(read_done) << "Timed out while reading data, output:\n" << output_;
    ASSERT_TRUE(done) << "Timed out waiting for waitpid, output:\n" << output_;

    ASSERT_FALSE(WIFSIGNALED(status)) << "Failed with signal " << WTERMSIG(status) << "\nOutput:\n"
                                      << output_;
    ASSERT_EQ(expected_exit_code, WEXITSTATUS(status)) << "Output:\n" << output_;
  }

  bool CheckExpectedLogStrings(std::string* error_msg) {
    time_t start = time(nullptr);
    std::string missing_match;
    std::string log_str;
    while (true) {
      log_str = log_main_->GetLog();
      missing_match.clear();
      // Look for the expected strings.
      for (auto str : expected_log_strings_) {
        if (log_str.find(str) == std::string::npos) {
          missing_match = str;
          break;
        }
      }
      if (missing_match.empty()) {
        return true;
      }
      if ((time(nullptr) - start) > kLogTimeoutSeconds) {
        break;
      }
    }

    *error_msg = android::base::StringPrintf("Didn't find string '%s' in log output:\n%s",
                                             missing_match.c_str(), log_str.c_str());
    return false;
  }

  void VerifyUnexpectedLogStrings() {
    std::string log_str = log_main_->GetLog();
    for (auto str : unexpected_log_strings_) {
      ASSERT_TRUE(log_str.find(str) == std::string::npos)
          << "Unexpectedly found string '" << str << "' in log output:\n"
          << log_str;
    }
  }

  void VerifyLeak(const char* test_prefix) {
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

    size_t match_len = expected_log_strings_.size() + 1;
    expected_log_strings_.resize(match_len);
    for (size_t i = 0; i < sizeof(functions) / sizeof(FunctionInfo); i++) {
      SCOPED_TRACE(testing::Message()
                   << functions[i].name << " expected size " << functions[i].size);

      expected_log_strings_[match_len - 1] =
          android::base::StringPrintf("leaked block of size %zu at", functions[i].size);

      std::string test = std::string("MallocTests.DISABLED_") + test_prefix + functions[i].name;
      ASSERT_NO_FATAL_FAILURE(Exec(test.c_str(), "verbose backtrace leak_track"));
    }
  }

  std::unique_ptr<LogReader> log_main_;
  std::unique_ptr<LogReader> log_crash_;
  pid_t pid_;
  std::string output_;
  std::vector<std::string> expected_log_strings_;
  std::vector<std::string> unexpected_log_strings_;

  static constexpr size_t kReadOutputTimeoutSeconds = 180;
  static constexpr size_t kWaitpidTimeoutSeconds = 10;
  static constexpr size_t kLogTimeoutSeconds = 5;
  static constexpr size_t kMaxRetries = 3;
};

TEST(MallocTests, DISABLED_smoke) {
  void* ptr = malloc(128);
  free(ptr);
}

TEST_F(MallocDebugSystemTest, smoke) {
  Exec("MallocTests.DISABLED_smoke", "verbose backtrace");
}

TEST_F(MallocDebugSystemTest, backtrace_full_smoke) {
  Exec("MallocTests.DISABLED_smoke", "verbose backtrace backtrace_full");
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

TEST_F(MallocDebugSystemTest, verify_leak) {
  VerifyLeak("leak_memory_");
}

TEST_F(MallocDebugSystemTest, verify_leak_allocation_limit) {
  SKIP_WITH_HWASAN;
  VerifyLeak("leak_memory_limit_");
}

static constexpr int kExpectedExitCode = 30;
static constexpr size_t kMaxThreads = sizeof(uint32_t) * 8;

TEST(MallocTests, DISABLED_exit_while_threads_allocating) {
  std::atomic_uint32_t thread_mask = {};

  for (size_t i = 0; i < kMaxThreads; i++) {
    std::thread malloc_thread([&thread_mask, i] {
      while (true) {
        void* ptr = malloc(100);
        if (ptr == nullptr) {
          exit(1000);
        }
        free(ptr);
        thread_mask.fetch_or(1U << i);
      }
    });
    malloc_thread.detach();
  }

  // Wait until each thread has done at least one allocation.
  while (thread_mask.load() != UINT32_MAX)
    ;
  exit(kExpectedExitCode);
}

// Verify that exiting while other threads are doing malloc operations,
// that there are no crashes.
TEST_F(MallocDebugSystemTest, exit_while_threads_allocating) {
  for (size_t i = 0; i < 100; i++) {
    SCOPED_TRACE(::testing::Message() << "Run " << i);
    ASSERT_NO_FATAL_FAILURE(Exec("MallocTests.DISABLED_exit_while_threads_allocating",
                                 "verbose backtrace", kExpectedExitCode));

    std::string log_str = log_crash_->GetLog();
    ASSERT_TRUE(log_str.find("Fatal signal") == std::string::npos)
        << "Found crash in log.\nLog message: " << log_str << " pid: " << pid_;
  }
}

TEST(MallocTests, DISABLED_exit_while_threads_freeing_allocs_with_header) {
  static constexpr size_t kMaxAllocsPerThread = 1000;
  std::atomic_uint32_t thread_mask = {};
  std::atomic_bool run;

  std::vector<std::vector<void*>> allocs(kMaxThreads);
  // Pre-allocate a bunch of memory so that we can try to trigger
  // the frees after the main thread finishes.
  for (size_t i = 0; i < kMaxThreads; i++) {
    for (size_t j = 0; j < kMaxAllocsPerThread; j++) {
      void* ptr = malloc(8);
      ASSERT_TRUE(ptr != nullptr);
      allocs[i].push_back(ptr);
    }
  }

  for (size_t i = 0; i < kMaxThreads; i++) {
    std::thread malloc_thread([&thread_mask, &run, &allocs, i] {
      thread_mask.fetch_or(1U << i);
      while (!run)
        ;
      for (auto ptr : allocs[i]) {
        free(ptr);
      }
    });
    malloc_thread.detach();
  }

  // Wait until all threads are running.
  while (thread_mask.load() != UINT32_MAX)
    ;
  run = true;
  exit(kExpectedExitCode);
}

TEST_F(MallocDebugSystemTest, exit_while_threads_freeing_allocs_with_header) {
  for (size_t i = 0; i < 50; i++) {
    SCOPED_TRACE(::testing::Message() << "Run " << i);
    // Enable guard to force the use of a header.
    ASSERT_NO_FATAL_FAILURE(
        Exec("MallocTests.DISABLED_exit_while_threads_freeing_allocs_with_header", "verbose guard",
             kExpectedExitCode));

    std::string log_str = log_crash_->GetLog();
    ASSERT_TRUE(log_str.find("Fatal signal") == std::string::npos)
        << "Found crash in log.\nLog message: " << log_str << " pid: " << pid_;
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

TEST_F(MallocDebugSystemTest, write_leak_info_no_header) {
  unexpected_log_strings_.push_back(" HAS INVALID TAG ");
  unexpected_log_strings_.push_back("USED AFTER FREE ");
  unexpected_log_strings_.push_back("UNKNOWN POINTER ");
  Exec("MallocTests.DISABLED_write_leak_info", "verbose backtrace");
}

TEST_F(MallocDebugSystemTest, write_leak_info_header) {
  unexpected_log_strings_.push_back(" HAS INVALID TAG ");
  unexpected_log_strings_.push_back("USED AFTER FREE ");
  unexpected_log_strings_.push_back("UNKNOWN POINTER ");
  Exec("MallocTests.DISABLED_write_leak_info", "verbose backtrace guard");
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
  unwindstack::AndroidLocalUnwinder unwinder;
  for (size_t i = 0; i < kNumUnwinds; i++) {
    // Only verify that there is at least one frame in the unwind.
    // This is not a test of the unwinder and clang for arm seems to
    // produces an increasing number of code that does not have unwind
    // information.
    unwindstack::AndroidUnwinderData data;
    ASSERT_TRUE(unwinder.Unwind(data)) << "Failed on unwind " << i;
  }
  running = false;
  thread.join();
}

TEST_F(MallocDebugSystemTest, malloc_and_backtrace_deadlock) {
  // Make sure that malloc debug is enabled and that no timeouts occur during
  // unwinds.
  unexpected_log_strings_.push_back("Timed out waiting for ");
  Exec("MallocTests.DISABLED_malloc_and_backtrace_deadlock", "verbose verify_pointers", 0);
}
