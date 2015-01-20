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

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace testing {
namespace internal {

// Reuse of testing::internal::ColoredPrintf in gtest.
enum GTestColor {
  COLOR_DEFAULT,
  COLOR_RED,
  COLOR_GREEN,
  COLOR_YELLOW
};

void ColoredPrintf(GTestColor color, const char* fmt, ...);

}  // namespace internal
}  // namespace testing

using testing::internal::GTestColor;
using testing::internal::COLOR_DEFAULT;
using testing::internal::COLOR_RED;
using testing::internal::COLOR_GREEN;
using testing::internal::COLOR_YELLOW;
using testing::internal::ColoredPrintf;

constexpr int DEFAULT_GLOBAL_TEST_RUN_DEADLINE_IN_MS = 60000;
constexpr int DEFAULT_GLOBAL_TEST_RUN_WARNLINE_IN_MS = 2000;

// The time each test can run before killed for the reason of timeout.
// It takes effect only with --isolate option.
static int global_test_run_deadline_in_ms = DEFAULT_GLOBAL_TEST_RUN_DEADLINE_IN_MS;

// The time each test can run before be warned for too much running time.
// It takes effect only with --isolate option.
static int global_test_run_warnline_in_ms = DEFAULT_GLOBAL_TEST_RUN_WARNLINE_IN_MS;

// Return deadline duration for a test, in ms.
static int GetDeadlineInfo(const std::string& /*test_name*/) {
  return global_test_run_deadline_in_ms;
}

// Return warnline duration for a test, in ms.
static int GetWarnlineInfo(const std::string& /*test_name*/) {
  return global_test_run_warnline_in_ms;
}

static void PrintHelpInfo() {
  printf("Bionic Unit Test Options:\n"
         "  -j [JOB_COUNT]\n"
         "      Run up to JOB_COUNT tests in parallel.\n"
         "      Use isolation mode, Run each test in a separate process.\n"
         "      If JOB_COUNT is not given, it is set to the count of available processors.\n"
         "  --no-isolate\n"
         "      Don't use isolation mode, run all tests in a single process.\n"
         "  --deadline=[TIME_IN_MS]\n"
         "      Run each test in no longer than [TIME_IN_MS] time.\n"
         "      It takes effect only in isolation mode. Deafult deadline is 60000 ms.\n"
         "  --warnline=[TIME_IN_MS]\n"
         "      Test running longer than [TIME_IN_MS] will be warned.\n"
         "      It takes effect only in isolation mode. Default warnline is 2000 ms.\n"
         "\nDefault bionic unit test option is -j.\n"
         "\n");
}

enum TestResult {
  TEST_SUCCESS = 0,
  TEST_FAILED,
  TEST_TIMEOUT
};

class TestCase {
 public:
  TestCase() {} // For std::vector<TestCase>.
  explicit TestCase(const char* name) : name_(name) {}

  const std::string& GetName() const { return name_; }

  void AppendTest(const std::string& test_name) {
    test_list_.push_back(std::make_tuple(test_name, TEST_FAILED, 0LL));
  }

  size_t TestCount() const { return test_list_.size(); }

  std::string GetTestName(size_t test_id) const {
    VerifyTestId(test_id);
    return name_ + "." + std::get<0>(test_list_[test_id]);
  }

  void SetTestResult(size_t test_id, TestResult result) {
    VerifyTestId(test_id);
    std::get<1>(test_list_[test_id]) = result;
  }

  TestResult GetTestResult(size_t test_id) const {
    VerifyTestId(test_id);
    return std::get<1>(test_list_[test_id]);
  }

  void SetTestTime(size_t test_id, int64_t elapsed_time) {
    VerifyTestId(test_id);
    std::get<2>(test_list_[test_id]) = elapsed_time;
  }

  int64_t GetTestTime(size_t test_id) const {
    VerifyTestId(test_id);
    return std::get<2>(test_list_[test_id]);
  }

 private:
  void VerifyTestId(size_t test_id) const {
    if(test_id >= test_list_.size()) {
      fprintf(stderr, "test_id %zu out of range [0, %zu)\n", test_id, test_list_.size());
      exit(1);
    }
  }

 private:
  const std::string name_;
  std::vector<std::tuple<std::string, TestResult, int64_t> > test_list_;
};

// TestResultPrinter is copied from part of external/gtest/src/gtest.cc:PrettyUnitTestResultPrinter.
// The reason for copy is that PrettyUnitTestResultPrinter is defined and used in gtest.cc, which
// is hard to reuse.
// TestResultPrinter only print information for a single test, which is used in child process.
// The information of test_iteration/environment/testcase is left for parent process to print.
class TestResultPrinter : public testing::EmptyTestEventListener {
 public:
  TestResultPrinter() : pinfo_(NULL) {}
  virtual void OnTestStart(const testing::TestInfo& test_info) {
    pinfo_ = &test_info; // Record test_info for use in OnTestPartResult.
  }
  virtual void OnTestPartResult(const testing::TestPartResult& result);
  virtual void OnTestEnd(const testing::TestInfo& test_info);

 private:
  const testing::TestInfo* pinfo_;
};

// Called after an assertion failure.
void TestResultPrinter::OnTestPartResult(const testing::TestPartResult& result) {
  // If the test part succeeded, we don't need to do anything.
  if (result.type() == testing::TestPartResult::kSuccess)
    return;

  // Print failure message from the assertion (e.g. expected this and got that).
  char buf[1024];
  snprintf(buf, sizeof(buf), "%s:(%d) Failure in test %s.%s\n%s\n", result.file_name(),
                                                                    result.line_number(),
                                                                    pinfo_->test_case_name(),
                                                                    pinfo_->name(),
                                                                    result.message());

  // Use write() to skip line buffer of printf, thus can avoid getting interleaved when
  // several processes are printing at the same time.
  int towrite = strlen(buf);
  char* p = buf;
  while (towrite > 0) {
    ssize_t write_count = write(fileno(stdout), p, towrite);
    if (write_count == -1) {
      if (errno != EINTR) {
        fprintf(stderr, "write, errno = %d\n", errno);
        break;
      }
    } else {
      towrite -= write_count;
      p += write_count;
    }
  }
}

void TestResultPrinter::OnTestEnd(const testing::TestInfo& test_info) {
  if (test_info.result()->Passed()) {
    ColoredPrintf(COLOR_GREEN, "[    OK    ] ");
  } else {
    ColoredPrintf(COLOR_RED, "[  FAILED  ] ");
  }
  printf("%s.%s", test_info.test_case_name(), test_info.name());
  if (test_info.result()->Failed()) {
    const char* const type_param = test_info.type_param();
    const char* const value_param = test_info.value_param();
    if (type_param != NULL || value_param != NULL) {
      printf(", where ");
      if (type_param != NULL) {
        printf("TypeParam = %s", type_param);
        if (value_param != NULL) {
          printf(" and ");
        }
      }
      if (value_param != NULL) {
        printf("GetParam() = %s", value_param);
      }
    }
  }

  if (testing::GTEST_FLAG(print_time)) {
    printf(" (%lld ms)\n", test_info.result()->elapsed_time());
  } else {
    printf("\n");
  }
  fflush(stdout);
}

static int64_t NanoTime() {
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return static_cast<int64_t>(t.tv_sec) * 1000000000LL + t.tv_nsec;
}

static bool EnumerateTests(int argc, char** argv, std::vector<TestCase>& testcase_list) {
  std::string command;
  for (int i = 0; i < argc; ++i) {
    command += argv[i];
    command += " ";
  }
  command += "--gtest_list_tests";
  FILE* fp = popen(command.c_str(), "r");
  if (fp == NULL) {
    perror("popen");
    return false;
  }

  char buf[200];
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    char* p = buf;

    while (*p != '\0' && isspace(*p)) {
      ++p;
    }
    if (*p == '\0') continue;
    char* start = p;
    while (*p != '\0' && !isspace(*p)) {
      ++p;
    }
    char* end = p;
    while (*p != '\0' && isspace(*p)) {
      ++p;
    }
    if (*p != '\0') {
      // This is not we want, gtest must meet with some error when parsing the arguments.
      fprintf(stderr, "argument error, check with --help\n");
      return false;
    }
    *end = '\0';
    if (*(end - 1) == '.') {
      *(end - 1) = '\0';
      testcase_list.push_back(TestCase(start));
    } else {
      testcase_list.back().AppendTest(start);
    }
  }
  int result = pclose(fp);
  return (result != -1 && WEXITSTATUS(result) == 0);
}

// Part of the following *Print functions are copied from external/gtest/src/gtest.cc:
// PrettyUnitTestResultPrinter. The reason for copy is that PrettyUnitTestResultPrinter
// is defined and used in gtest.cc, which is hard to reuse.
static void OnTestIterationStartPrint(const std::vector<TestCase>& testcase_list, size_t iteration,
                                      size_t iteration_count) {
  if (iteration_count > 1) {
    printf("\nRepeating all tests (iteration %zu) . . .\n\n", iteration);
  }
  ColoredPrintf(COLOR_GREEN,  "[==========] ");

  size_t testcase_count = testcase_list.size();
  size_t test_count = 0;
  for (const auto& testcase : testcase_list) {
    test_count += testcase.TestCount();
  }

  printf("Running %zu %s from %zu %s.\n",
         test_count, (test_count == 1) ? "test" : "tests",
         testcase_count, (testcase_count == 1) ? "test case" : "test cases");
  fflush(stdout);
}

static void OnTestTerminatedPrint(const TestCase& testcase, size_t test_id, int sig) {
  ColoredPrintf(COLOR_RED, "[  FAILED  ] ");
  printf("%s terminated by signal: %s\n", testcase.GetTestName(test_id).c_str(),
                                          strsignal(sig));
  fflush(stdout);
}

static void OnTestTimeoutPrint(const TestCase& testcase, size_t test_id) {
  ColoredPrintf(COLOR_RED, "[ TIMEOUT  ] ");
  printf("%s (killed by timeout at %lld ms)\n", testcase.GetTestName(test_id).c_str(),
                                                testcase.GetTestTime(test_id) / 1000000LL);
  fflush(stdout);
}

static void OnTestIterationEndPrint(const std::vector<TestCase>& testcase_list, size_t /*iteration*/,
                                    int64_t elapsed_time) {

  std::vector<std::string> fail_test_name_list;
  std::vector<std::pair<std::string, int64_t>> timeout_test_list;

  // For tests run exceed warnline but not timeout.
  std::vector<std::tuple<std::string, int64_t, int>> timewarn_test_list;
  size_t testcase_count = testcase_list.size();
  size_t test_count = 0;
  size_t success_test_count = 0;

  for (const auto& testcase : testcase_list) {
    test_count += testcase.TestCount();
    for (size_t i = 0; i < testcase.TestCount(); ++i) {
      TestResult result = testcase.GetTestResult(i);
      if (result == TEST_SUCCESS) {
        ++success_test_count;
      } else if (result == TEST_FAILED) {
        fail_test_name_list.push_back(testcase.GetTestName(i));
      } else if (result == TEST_TIMEOUT) {
        timeout_test_list.push_back(std::make_pair(testcase.GetTestName(i),
                                                   testcase.GetTestTime(i)));
      }
      if (result != TEST_TIMEOUT &&
          testcase.GetTestTime(i) / 1000000 >= GetWarnlineInfo(testcase.GetTestName(i))) {
        timewarn_test_list.push_back(std::make_tuple(testcase.GetTestName(i),
                                                     testcase.GetTestTime(i),
                                                     GetWarnlineInfo(testcase.GetTestName(i))));
      }
    }
  }

  ColoredPrintf(COLOR_GREEN,  "[==========] ");
  printf("%zu %s from %zu %s ran.", test_count, (test_count == 1) ? "test" : "tests",
                                    testcase_count, (testcase_count == 1) ? "test case" : "test cases");
  if (testing::GTEST_FLAG(print_time)) {
    printf(" (%lld ms total)", elapsed_time / 1000000LL);
  }
  printf("\n");
  ColoredPrintf(COLOR_GREEN,  "[  PASSED  ] ");
  printf("%zu %s.\n", success_test_count, (success_test_count == 1) ? "test" : "tests");

  // Print tests failed.
  size_t fail_test_count = fail_test_name_list.size();
  if (fail_test_count > 0) {
    ColoredPrintf(COLOR_RED,  "[  FAILED  ] ");
    printf("%zu %s, listed below:\n", fail_test_count, (fail_test_count == 1) ? "test" : "tests");
    for (const auto& name : fail_test_name_list) {
      ColoredPrintf(COLOR_RED, "[  FAILED  ] ");
      printf("%s\n", name.c_str());
    }
  }

  // Print tests run timeout.
  size_t timeout_test_count = timeout_test_list.size();
  if (timeout_test_count > 0) {
    ColoredPrintf(COLOR_RED, "[ TIMEOUT  ] ");
    printf("%zu %s, listed below:\n", timeout_test_count, (timeout_test_count == 1) ? "test" : "tests");
    for (const auto& timeout_pair : timeout_test_list) {
      ColoredPrintf(COLOR_RED, "[ TIMEOUT  ] ");
      printf("%s (stopped at %lld ms)\n", timeout_pair.first.c_str(),
                                          timeout_pair.second / 1000000LL);
    }
  }

  // Print tests run exceed warnline.
  size_t timewarn_test_count = timewarn_test_list.size();
  if (timewarn_test_count > 0) {
    ColoredPrintf(COLOR_YELLOW, "[ TIMEWARN ] ");
    printf("%zu %s, listed below:\n", timewarn_test_count, (timewarn_test_count == 1) ? "test" : "tests");
    for (const auto& timewarn_tuple : timewarn_test_list) {
      ColoredPrintf(COLOR_YELLOW, "[ TIMEWARN ] ");
      printf("%s (%lld ms, exceed warnline %d ms)\n", std::get<0>(timewarn_tuple).c_str(),
                                                      std::get<1>(timewarn_tuple) / 1000000LL,
                                                      std::get<2>(timewarn_tuple));
    }
  }

  if (fail_test_count > 0) {
    printf("\n%2zu FAILED %s\n", fail_test_count, (fail_test_count == 1) ? "TEST" : "TESTS");
  }
  if (timeout_test_count > 0) {
    printf("%2zu TIMEOUT %s\n", timeout_test_count, (timeout_test_count == 1) ? "TEST" : "TESTS");
  }
  if (timewarn_test_count > 0) {
    printf("%2zu TIMEWARN %s\n", timewarn_test_count, (timewarn_test_count == 1) ? "TEST" : "TESTS");
  }
  fflush(stdout);
}

// Forked Child process, run the single test.
static void ChildProcessFn(int argc, char** argv, const std::string& test_name) {
  char** new_argv = new char*[argc + 1];
  memcpy(new_argv, argv, sizeof(char*) * argc);

  char* filter_arg = new char [test_name.size() + 20];
  strcpy(filter_arg, "--gtest_filter=");
  strcat(filter_arg, test_name.c_str());
  new_argv[argc] = filter_arg;

  int new_argc = argc + 1;
  testing::InitGoogleTest(&new_argc, new_argv);
  int result = RUN_ALL_TESTS();
  exit(result);
}

struct ChildProcInfo {
  pid_t pid;
  int64_t start_time;
  int64_t deadline_time;
  size_t testcase_id, test_id;
  bool done_flag;
  bool timeout_flag;
  int exit_status;
  ChildProcInfo() : pid(0) {}
};

static void WaitChildProcs(std::vector<ChildProcInfo>& child_proc_list) {
  pid_t result;
  int status;
  bool loop_flag = true;

  while (true) {
    while ((result = waitpid(-1, &status, WNOHANG)) == -1) {
      if (errno != EINTR) {
        break;
      }
    }

    if (result == -1) {
      perror("waitpid");
      exit(1);
    } else if (result == 0) {
      // Check child timeout.
      int64_t current_time = NanoTime();
      for (size_t i = 0; i < child_proc_list.size(); ++i) {
        if (child_proc_list[i].deadline_time <= current_time) {
          child_proc_list[i].done_flag = true;
          child_proc_list[i].timeout_flag = true;
          loop_flag = false;
        }
      }
    } else {
      // Check child finish.
      for (size_t i = 0; i < child_proc_list.size(); ++i) {
        if (child_proc_list[i].pid == result) {
          child_proc_list[i].done_flag = true;
          child_proc_list[i].timeout_flag = false;
          child_proc_list[i].exit_status = status;
          loop_flag = false;
          break;
        }
      }
    }

    if (!loop_flag) break;
    // sleep 1 ms to avoid busy looping.
    timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 1000000;
    nanosleep(&sleep_time, NULL);
  }
}

static TestResult WaitChildProc(pid_t pid) {
  pid_t result;
  int exit_status;

  while ((result = waitpid(pid, &exit_status, 0)) == -1) {
    if (errno != EINTR) {
      break;
    }
  }

  TestResult test_result = TEST_SUCCESS;
  if (result != pid || WEXITSTATUS(exit_status) != 0) {
    test_result = TEST_FAILED;
  }
  return test_result;
}

// We choose to use multi-fork and multi-wait here instead of multi-thread, because it always
// makes deadlock to use fork in multi-thread.
static void RunTestInSeparateProc(int argc, char** argv, std::vector<TestCase>& testcase_list,
                                  size_t iteration_count, size_t job_count) {
  // Stop default result printer to avoid environment setup/teardown information for each test.
  testing::UnitTest::GetInstance()->listeners().Release(
                        testing::UnitTest::GetInstance()->listeners().default_result_printer());
  testing::UnitTest::GetInstance()->listeners().Append(new TestResultPrinter);

  for (size_t iteration = 1; iteration <= iteration_count; ++iteration) {
    OnTestIterationStartPrint(testcase_list, iteration, iteration_count);
    int64_t iteration_start_time = NanoTime();

    // Run up to job_count tests in parallel, each test in a child process.
    std::vector<ChildProcInfo> child_proc_list(job_count);

    // Next test to run is [next_testcase_id:next_test_id].
    size_t next_testcase_id = 0;
    size_t next_test_id = 0;

    // Record how many tests are finished.
    std::vector<size_t> finished_test_count_list(testcase_list.size(), 0);
    size_t finished_testcase_count = 0;

    while (finished_testcase_count < testcase_list.size()) {
      // Fork up to job_count child processes.
      for (auto& child_proc : child_proc_list) {
        if (child_proc.pid == 0 && next_testcase_id < testcase_list.size()) {
          std::string test_name = testcase_list[next_testcase_id].GetTestName(next_test_id);
          pid_t pid = fork();
          if (pid == -1) {
            perror("fork in RunTestInSeparateProc");
            exit(1);
          } else if (pid == 0) {
            // Run child process test, never return.
            ChildProcessFn(argc, argv, test_name);
          }
          // Parent process
          child_proc.pid = pid;
          child_proc.start_time = NanoTime();
          child_proc.deadline_time = child_proc.start_time + GetDeadlineInfo(test_name) * 1000000LL;
          child_proc.testcase_id = next_testcase_id;
          child_proc.test_id = next_test_id;
          child_proc.done_flag = false;
          if (++next_test_id == testcase_list[next_testcase_id].TestCount()) {
            next_test_id = 0;
            ++next_testcase_id;
          }
        }
      }

      // Wait for any child proc finish or timeout.
      WaitChildProcs(child_proc_list);

      // Collect result.
      for (auto& child_proc : child_proc_list) {
        if (child_proc.pid != 0 && child_proc.done_flag == true) {
          size_t testcase_id = child_proc.testcase_id;
          size_t test_id = child_proc.test_id;
          TestCase& testcase = testcase_list[testcase_id];
          testcase.SetTestTime(test_id, NanoTime() - child_proc.start_time);

          if (child_proc.timeout_flag) {
            // Kill and wait the timeout child process.
            kill(child_proc.pid, SIGKILL);
            WaitChildProc(child_proc.pid);
            testcase.SetTestResult(test_id, TEST_TIMEOUT);
            OnTestTimeoutPrint(testcase, test_id);

          } else if (WIFSIGNALED(child_proc.exit_status)) {
            // Record signal terminated test as failed.
            testcase.SetTestResult(test_id, TEST_FAILED);
            OnTestTerminatedPrint(testcase, test_id, WTERMSIG(child_proc.exit_status));

          } else {
            testcase.SetTestResult(test_id, WEXITSTATUS(child_proc.exit_status) == 0 ?
                                   TEST_SUCCESS : TEST_FAILED);
            // TestResultPrinter::OnTestEnd has already printed result for normal exit.
          }

          if (++finished_test_count_list[testcase_id] == testcase.TestCount()) {
            ++finished_testcase_count;
          }
          child_proc.pid = 0;
          child_proc.done_flag = false;
        }
      }
    }

    OnTestIterationEndPrint(testcase_list, iteration, NanoTime() - iteration_start_time);
  }
}

static size_t GetProcessorCount() {
  return static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
}

// Pick options not for gtest: There are two parts in argv, one part is handled by PickOptions()
// as described in PrintHelpInfo(), the other part is handled by testing::InitGoogleTest() in
// gtest. PickOptions() picks the first part of options and change them into flags and operations,
// lefting the second part in argv.
// Arguments:
//  argv is used to pass in all command arguments, and pass out only the part of options for gtest.
//  exit_flag is to indicate whether we need to run gtest workflow after PickOptions.
// Return false if run error.
static bool PickOptions(std::vector<char*>& argv, bool* exit_flag) {
  *exit_flag = false;
  for (size_t i = 1; i < argv.size() - 1; ++i) {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      PrintHelpInfo();
      return true;
    }
  }

  // Move --gtest_filter option to last, and add "-bionic_selftest*" to disable self test.
  std::string gtest_filter_str = "--gtest_filter=-bionic_selftest*";
  for (size_t i = argv.size() - 2; i >= 1; --i) {
    if (strncmp(argv[i], "--gtest_filter=", sizeof("--gtest_filter=") - 1) == 0) {
      gtest_filter_str = std::string(argv[i]) + ":-bionic_selftest*";
      argv.erase(argv.begin() + i);
      break;
    }
  }
  argv.insert(argv.end() - 1, strdup(gtest_filter_str.c_str()));

  // Init default bionic_gtest option.
  bool isolate_option = true;
  size_t job_count_option = GetProcessorCount();

  size_t deadline_option_len = strlen("--deadline=");
  size_t warnline_option_len = strlen("--warnline=");
  size_t gtest_color_option_len = strlen("--gtest_color=");

  // Parse bionic_gtest specific options in arguments.
  for (size_t i = 1; i < argv.size() - 1; ++i) {
    if (strcmp(argv[i], "-j") == 0) {
      isolate_option = true; // Enable isolation mode when -j is used.
      int tmp;
      if (argv[i + 1] != NULL && (tmp = atoi(argv[i + 1])) > 0) {
        job_count_option = tmp;
        argv.erase(argv.begin() + i);
      } else {
        job_count_option = GetProcessorCount();
      }
      argv.erase(argv.begin() + i);
      --i;

    } else if (strcmp(argv[i], "--no-isolate") == 0) {
      isolate_option = false;
      argv.erase(argv.begin() + i);
      --i;

    } else if (strncmp(argv[i], "--deadline=", deadline_option_len) == 0) {
      global_test_run_deadline_in_ms = atoi(argv[i] + deadline_option_len);
      if (global_test_run_deadline_in_ms <= 0) {
        fprintf(stderr, "value for --deadline option should be positive: %s\n",
                argv[i] + deadline_option_len);
        exit(1);
      }
      argv.erase(argv.begin() + i);
      --i;

    } else if (strncmp(argv[i], "--warnline=", warnline_option_len) == 0) {
      global_test_run_warnline_in_ms = atoi(argv[i] + warnline_option_len);
      if (global_test_run_warnline_in_ms <= 0) {
        fprintf(stderr, "value for --warnline option should be positive: %s\n",
                argv[i] + warnline_option_len);
        exit(1);
      }
      argv.erase(argv.begin() + i);
      --i;

    } else if (strncmp(argv[i], "--gtest_color=", gtest_color_option_len) == 0) {
      // If running in isolation mode, main process doesn't call testing::InitGoogleTest(&argc, argv).
    // So we should parse gtest options for printing by ourselves.
      testing::GTEST_FLAG(color) = argv[i] + gtest_color_option_len;

    } else if (strcmp(argv[i], "--gtest_print_time=0") == 0) {
      testing::GTEST_FLAG(print_time) = false;

    } else if (strcmp(argv[i], "--gtest_list_tests") == 0) {
      // Disable isolation mode in gtest_list_tests option.
      isolate_option = false;

    } else if (strcmp(argv[i], "--bionic-selftest") == 0) {
      // This option is to enable "bionic_selftest*" for self test, and not shown in help informantion.
      // Don't remove this option from argument list.
      argv[argv.size() - 2] = strdup("--gtest_filter=bionic_selftest*");
    }
  }

  // Handle --gtest_repeat=[COUNT] option if we are in isolation mode.
  // We should check and remove this option to avoid child process running single test for several
  // iterations.
  size_t gtest_repeat_count = 1;
  if (isolate_option == true) {
    int len = sizeof("--gtest_repeat=") - 1;
    for (size_t i = 1; i < argv.size() - 1; ++i) {
      if (strncmp(argv[i], "--gtest_repeat=", len) == 0) {
        int tmp = atoi(argv[i] + len);
        if (tmp < 0) {
          fprintf(stderr, "error count for option --gtest_repeat=[COUNT]\n");
          return false;
        }
        gtest_repeat_count = tmp;
        argv.erase(argv.begin() + i);
        break;
      }
    }
  }

  // Add --no-isolate option in argv to suppress subprocess running in isolation mode again.
  // As DeathTest will try to execve again, this option should always be set.
  argv.insert(argv.begin() + 1, strdup("--no-isolate"));

  // Run tests in isolation mode.
  if (isolate_option) {
    *exit_flag = true;

    std::vector<TestCase> testcase_list;
    int argc = static_cast<int>(argv.size()) - 1;
    if (EnumerateTests(argc, argv.data(), testcase_list) == false) {
      return false;
    }
    RunTestInSeparateProc(argc, argv.data(), testcase_list, gtest_repeat_count, job_count_option);
    return true;
  }
  return true;
}

int main(int argc, char** argv) {
  std::vector<char*> arg_list;
  for (int i = 0; i < argc; ++i) {
    arg_list.push_back(argv[i]);
  }
  arg_list.push_back(NULL);

  bool exit_flag;
  int return_result = 0;

  if (PickOptions(arg_list, &exit_flag) == false) {
    return_result = 1;
  } else if (!exit_flag) {
    argc = static_cast<int>(arg_list.size()) - 1;
    testing::InitGoogleTest(&argc, arg_list.data());
    return_result = RUN_ALL_TESTS();
  }
  return return_result;
}

//################################################################################
// Bionic Gtest self test, run this by --bionic-selftest option.

TEST(bionic_selftest, test_success) {
  ASSERT_EQ(1, 1);
}

TEST(bionic_selftest, test_fail) {
  ASSERT_EQ(0, 1);
}

TEST(bionic_selftest, test_time_warn) {
  sleep(4);
}

TEST(bionic_selftest, test_timeout) {
  while (1) {}
}

TEST(bionic_selftest, test_signal_SEGV_terminated) {
  char* p = reinterpret_cast<char*>(static_cast<intptr_t>(atoi("0")));
  *p = 3;
}
