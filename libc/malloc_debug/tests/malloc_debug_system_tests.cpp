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

#include <gtest/gtest.h>

#include <log/log.h>

#include <string>
#include <vector>

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
    args.push_back(testing::internal::GetArgvs()[0].c_str());
    args.push_back("--gtest_also_run_disabled_tests");
    std::string filter_arg = std::string("--gtest_filter=") + test_name;
    args.push_back(filter_arg.c_str());
    args.push_back(nullptr);
    execv(args[0], reinterpret_cast<char* const*>(const_cast<char**>(args.data())));
    exit(1);
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
  ASSERT_NO_FATAL_FAILURE(Exec("MallocTests.DISABLED_smoke", "backtrace", &pid));

  ASSERT_NO_FATAL_FAILURE(FindStrings(pid, std::vector<const char*>{"malloc debug enabled"}));
}

TEST(MallocTests, DISABLED_leak_memory) {
  void* ptr = malloc(1123);
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 0, 1123);
}

TEST(MallocDebugSystemTest, verify_leak) {
  pid_t pid;
  ASSERT_NO_FATAL_FAILURE(Exec("MallocTests.DISABLED_leak_memory", "backtrace leak_track", &pid));

  ASSERT_NO_FATAL_FAILURE(FindStrings(
      pid, std::vector<const char*>{"malloc debug enabled", "leaked block of size 1123 at"}));
}
