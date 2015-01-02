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

} // namespace internal
} // namespace testing

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

  int NumTests() const { return test_list_.size(); }

  std::string GetTestName(int test_id) const {
    VerifyTestId(test_id);
    return name_ + "." + std::get<0>(test_list_[test_id]);
  }

  void SetTestResult(int test_id, TestResult result) {
    VerifyTestId(test_id);
    std::get<1>(test_list_[test_id]) = result;
  }

  TestResult GetTestResult(int test_id) const {
    VerifyTestId(test_id);
    return std::get<1>(test_list_[test_id]);
  }

  void SetTestTime(int test_id, int64_t elapsed_time) {
    VerifyTestId(test_id);
    std::get<2>(test_list_[test_id]) = elapsed_time;
  }

  int64_t GetTestTime(int test_id) const {
    VerifyTestId(test_id);
    return std::get<2>(test_list_[test_id]);
  }

 private:
  void VerifyTestId(int test_id) const {
    if(test_id < 0 || test_id >= static_cast<int>(test_list_.size())) {
      fprintf(stderr, "test_id %d out of range [0, %zu)\n", test_id, test_list_.size());
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
    ssize_t num_write = write(fileno(stdout), p, towrite);
    if (num_write == -1) {
      if (errno != EINTR) {
        fprintf(stderr, "write, errno = %d\n", errno);
        break;
      }
    } else {
      towrite -= num_write;
      p += num_write;
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

static void PrintHelpInfo() {
  printf("Bionic Unit Test Options:\n"
         "  --isolate\n"
         "      Use isolation mode, Run each test in a separate process.\n"
         "  --deadline=[TIME_IN_MS]\n"
         "      Run each test in no longer than [TIME_IN_MS] time.\n"
         "      It takes effect only in isolation mode. Deafult deadline is 60000 ms.\n"
         "  --warnline=[TIME_IN_MS]\n"
         "      Test running longer than [TIME_IN_MS] will be warned.\n"
         "      It takes effect only in isolation mode. Default warnline is 2000 ms.\n"
         "  -j [JOB_NUM]\n"
         "      Run up to JOB_NUM tests in parallel.\n"
         "      Use isolation mode, Run each test in a separate process.\n"
         "      If JOB_NUM is not given, it is set to the number of cpus available.\n"
         "\n");
}

// Part of the following *Print functions are copied from external/gtest/src/gtest.cc:
// PrettyUnitTestResultPrinter. The reason for copy is that PrettyUnitTestResultPrinter
// is defined and used in gtest.cc, which is hard to reuse.
static void OnTestIterationStartPrint(const std::vector<TestCase>& testcase_list, int iteration,
                                      int num_iterations) {
  if (num_iterations > 1) {
    printf("\nRepeating all tests (iteration %d) . . .\n\n", iteration);
  }
  ColoredPrintf(COLOR_GREEN,  "[==========] ");

  int num_testcases = testcase_list.size();
  int num_tests = 0;
  for (const auto& testcase : testcase_list) {
    num_tests += testcase.NumTests();
  }

  printf("Running %d %s from %d %s.\n",
         num_tests, (num_tests == 1) ? "test" : "tests",
         num_testcases, (num_testcases == 1) ? "test case" : "test cases");
  fflush(stdout);
}

static void OnTestTimeoutPrint(const TestCase& testcase, int test_id) {
  ColoredPrintf(COLOR_RED, "[ TIMEOUT  ] ");
  printf("%s (killed by timeout at %lld ms)\n", testcase.GetTestName(test_id).c_str(),
                                                testcase.GetTestTime(test_id) / 1000000LL);
  fflush(stdout);
}

static void TestcaseTimePrint(const TestCase& testcase) {
  int64_t testcase_time = 0;
  for (int i = 0; i < testcase.NumTests(); ++i) {
    testcase_time += testcase.GetTestTime(i);
  }
  printf("%d %s from %s (%lld ms total)\n", testcase.NumTests(),
                                            (testcase.NumTests() == 1) ? "test" : "tests",
                                            testcase.GetName().c_str(),
                                            testcase_time / 1000000LL);
  fflush(stdout);
}

static void OnTestIterationEndPrint(const std::vector<TestCase>& testcase_list, int /*iteration*/,
                                    int64_t elapsed_time) {

  std::vector<std::string> fail_test_name_list;
  std::vector<std::pair<std::string, int64_t>> timeout_test_list;

  // For tests run exceed warnline but not timeout.
  std::vector<std::tuple<std::string, int64_t, int>> timewarn_test_list;
  int num_testcases = testcase_list.size();
  int num_tests = 0;
  int num_success_tests = 0;

  for (const auto& testcase : testcase_list) {
    num_tests += testcase.NumTests();
    for (int i = 0; i < testcase.NumTests(); ++i) {
      TestResult result = testcase.GetTestResult(i);
      if (result == TEST_SUCCESS) {
        ++num_success_tests;
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

  for (auto const& testcase : testcase_list) {
    TestcaseTimePrint(testcase);
  }

  ColoredPrintf(COLOR_GREEN,  "[==========] ");
  printf("%d %s from %d %s ran.", num_tests, (num_tests == 1) ? "test" : "tests",
                                  num_testcases, (num_testcases == 1) ? "test case" : "test cases");
  if (testing::GTEST_FLAG(print_time)) {
    printf(" (%lld ms total)", elapsed_time / 1000000LL);
  }
  printf("\n");
  ColoredPrintf(COLOR_GREEN,  "[  PASSED  ] ");
  printf("%d %s.\n", num_success_tests, (num_success_tests == 1) ? "test" : "tests");

  // Print tests failed.
  int num_fail_tests = fail_test_name_list.size();
  if (num_fail_tests > 0) {
    ColoredPrintf(COLOR_RED,  "[  FAILED  ] ");
    printf("%d %s, listed below:\n", num_fail_tests, (num_fail_tests == 1) ? "test" : "tests");
    for (const auto& name : fail_test_name_list) {
      ColoredPrintf(COLOR_RED, "[  FAILED  ] ");
      printf("%s\n", name.c_str());
    }
  }

  // Print tests run timeout.
  int num_timeout_tests = timeout_test_list.size();
  if (num_timeout_tests > 0) {
    ColoredPrintf(COLOR_RED, "[ TIMEOUT  ] ");
    printf("%d %s, listed below:\n", num_timeout_tests, (num_timeout_tests == 1) ? "test" : "tests");
    for (const auto& timeout_pair : timeout_test_list) {
      ColoredPrintf(COLOR_RED, "[ TIMEOUT  ] ");
      printf("%s (stopped at %lld ms)\n", timeout_pair.first.c_str(),
                                          timeout_pair.second / 1000000LL);
    }
  }

  // Print tests run exceed warnline.
  int num_timewarn_tests = timewarn_test_list.size();
  if (num_timewarn_tests > 0) {
    ColoredPrintf(COLOR_YELLOW, "[ TIMEWARN ] ");
    printf("%d %s, listed below:\n", num_timewarn_tests, (num_timewarn_tests == 1) ? "test" : "tests");
    for (const auto& timewarn_tuple : timewarn_test_list) {
      ColoredPrintf(COLOR_YELLOW, "[ TIMEWARN ] ");
      printf("%s (%lld ms, exceed warnline %d ms)\n", std::get<0>(timewarn_tuple).c_str(),
                                                      std::get<1>(timewarn_tuple) / 1000000LL,
                                                      std::get<2>(timewarn_tuple));
    }
  }

  if (num_fail_tests > 0) {
    printf("\n%2d FAILED %s\n", num_fail_tests, (num_fail_tests == 1) ? "TEST" : "TESTS");
  }
  if (num_timeout_tests > 0) {
    printf("%2d TIMEOUT %s\n", num_timeout_tests, (num_timeout_tests == 1) ? "TEST" : "TESTS");
  }
  if (num_timewarn_tests > 0) {
    printf("%2d TIMEWARN %s\n", num_timewarn_tests, (num_timewarn_tests == 1) ? "TEST" : "TESTS");
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
  int testcase_id, test_id;
  bool done_flag;
  TestResult test_result;
  ChildProcInfo() : pid(0) {}
};

static void WaitChildProcs(std::vector<ChildProcInfo>& child_proc_list) {
  pid_t result;
  int exit_status;
  bool loop_flag = true;

  while (true) {
    while ((result = waitpid(-1, &exit_status, WNOHANG)) == -1) {
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
          child_proc_list[i].test_result = TEST_TIMEOUT;
          loop_flag = false;
        }
      }
    } else {
      // Check child finish.
      for (size_t i = 0; i < child_proc_list.size(); ++i) {
        if (child_proc_list[i].pid == result) {
          child_proc_list[i].done_flag = true;
          child_proc_list[i].test_result = (WEXITSTATUS(exit_status) == 0) ? TEST_SUCCESS :
                                                                             TEST_FAILED;
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
                                  int num_iterations, int num_jobs) {
  // Stop default result printer to avoid environment setup/teardown information for each test.
  testing::UnitTest::GetInstance()->listeners().Release(
                        testing::UnitTest::GetInstance()->listeners().default_result_printer());
  testing::UnitTest::GetInstance()->listeners().Append(new TestResultPrinter);

  for (int iteration = 1; iteration <= num_iterations; ++iteration) {
    OnTestIterationStartPrint(testcase_list, iteration, num_iterations);
    int64_t iteration_start_time = NanoTime();

    std::vector<ChildProcInfo> child_proc_list(num_jobs);
    int assign_testcase = 0, assign_test = 0;
    std::vector<int> finish_test_num_list(testcase_list.size(), 0);
    int num_finish_testcases = 0;

    while (num_finish_testcases < static_cast<int>(testcase_list.size())) {
      // Fork child process up to num_jobs.
      for (auto& child_proc : child_proc_list) {
        if (child_proc.pid == 0 && assign_testcase < static_cast<int>(testcase_list.size())) {
          std::string test_name = testcase_list[assign_testcase].GetTestName(assign_test);
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
          child_proc.testcase_id = assign_testcase;
          child_proc.test_id = assign_test;
          child_proc.done_flag = false;
          if (++assign_test == testcase_list[assign_testcase].NumTests()) {
            assign_test = 0;
            ++assign_testcase;
          }
        }
      }

      // Wait for any child proc finish or timeout.
      WaitChildProcs(child_proc_list);

      // Collect result.
      for (auto& child_proc : child_proc_list) {
        if (child_proc.pid != 0 && child_proc.done_flag == true) {
          int testcase_id = child_proc.testcase_id;
          int test_id = child_proc.test_id;
          TestResult test_result = child_proc.test_result;
          if (test_result == TEST_TIMEOUT) {
            // Kill and wait the child process.
            kill(child_proc.pid, SIGKILL);
            WaitChildProc(child_proc.pid);
          }
          TestCase& testcase = testcase_list[testcase_id];
          testcase.SetTestResult(test_id, test_result);
          testcase.SetTestTime(test_id, NanoTime() - child_proc.start_time);
          if (test_result == TEST_TIMEOUT) {
            OnTestTimeoutPrint(testcase, test_id);
          }
          if (++finish_test_num_list[testcase_id] == testcase.NumTests()) {
            ++num_finish_testcases;
          }
          child_proc.pid = 0;
          child_proc.done_flag = false;
        }
      }
    }

    OnTestIterationEndPrint(testcase_list, iteration, NanoTime() - iteration_start_time);
  }
}

static int GetCpuNumber() {
  FILE* fp = popen("cat /proc/cpuinfo | grep processor | wc -l", "r");
  int result = 1; // Return 1 if we can't get the exact cpu number.
  if (fp != NULL) {
    fscanf(fp, "%d", &result);
    if (result < 1) {
      result = 1;
    }
    pclose(fp);
  }
  return result;
}

// Pick options not for gtest; Return false if run error.
// Use exit_flag to indicate whether we need to run gtest flow after PickOptions.
static bool PickOptions(int* pargc, char*** pargv, bool* exit_flag) {
  int argc = *pargc;
  char** argv = *pargv;
  *exit_flag = false;
  for (int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      PrintHelpInfo();
      return true;
    }
  }

  // Move --gtest_filter option to last, and add "-bionic_gtest*" to disable self test.
  int gtest_filter_option_pos = 0;
  for (int i = argc - 1; i >= 1; --i) {
    if (strncmp(argv[i], "--gtest_filter=", sizeof("--gtest_filter=") - 1) == 0) {
      gtest_filter_option_pos = i;
      break;
    }
  }
  if (gtest_filter_option_pos != 0) {
    char* gtest_filter_arg = argv[gtest_filter_option_pos];
    for (int i = gtest_filter_option_pos; i < argc - 1; ++i) {
      argv[i] = argv[i + 1];
    }
    char* new_arg = new char[strlen(gtest_filter_arg) + sizeof(":-bionic_gtest*")];
    strcpy(new_arg, gtest_filter_arg);
    strcat(new_arg, ":-bionic_gtest*");
    argv[argc - 1] = new_arg;
  } else {
    char** new_argv = new char* [argc + 1];
    for (int i = 0; i < argc; ++i) {
      new_argv[i] = argv[i];
    }
    new_argv[argc++] = const_cast<char*>("--gtest_filter=-bionic_gtest*");
    *pargv = argv = new_argv;
  }

  // Parse bionic_gtest specific options.
  bool isolate_option = false;
  int job_num_option = 1;

  int deadline_option_len = sizeof("--deadline=") - 1;
  int warnline_option_len = sizeof("--warnline=") - 1;
  int gtest_color_option_len = sizeof("--gtest_color=") - 1;

  for (int i = 1; i < argc; ++i) {
    int remove_arg_num = 0;

    // If running in isolation mode, main process doesn't call testing::InitGoogleTest(&argc, argv).
    // So we should parse gtest options for printing here.
    if (strncmp(argv[i], "--gtest_color=", gtest_color_option_len) == 0) {
      testing::GTEST_FLAG(color) = argv[i] + gtest_color_option_len;

    } else if (strcmp(argv[i], "--gtest_print_time=0") == 0) {
      testing::GTEST_FLAG(print_time) = false;

    } else if (strcmp(argv[i], "--isolate") == 0) {
      isolate_option = true;
      remove_arg_num = 1;

    } else if (strncmp(argv[i], "--deadline=", deadline_option_len) == 0) {
      global_test_run_deadline_in_ms = atoi(argv[i] + deadline_option_len);
      if (global_test_run_deadline_in_ms <= 0) {
        fprintf(stderr, "value for --deadline option should be positive: %s\n",
                argv[i] + deadline_option_len);
        exit(1);
      }
      remove_arg_num = 1;

    } else if (strncmp(argv[i], "--warnline=", warnline_option_len) == 0) {
      global_test_run_warnline_in_ms = atoi(argv[i] + warnline_option_len);
      if (global_test_run_warnline_in_ms <= 0) {
        fprintf(stderr, "value for --warnline option should be positive: %s\n",
                argv[i] + warnline_option_len);
        exit(1);
      }
      remove_arg_num = 1;

    } else if (strcmp(argv[i], "--bionic_gtest") == 0) {
      // Enable "bionic_gtest*" for self test.
      // Don't remove this option from argument list.
      argv[argc - 1] = const_cast<char*>("--gtest_filter=bionic_gtest*");

    } else if (strcmp(argv[i], "-j") == 0) {
      isolate_option = true; // Enable isolation mode when -j is used.
      if (i + 1 < argc && (job_num_option = atoi(argv[i + 1])) > 0) {
        remove_arg_num = 2;
      } else {
        job_num_option = GetCpuNumber();
        remove_arg_num = 1;
      }
    }

    if (remove_arg_num != 0) {
      for (int j = i; j < argc - remove_arg_num; ++j) {
        argv[j] = argv[j + remove_arg_num];
      }
      argc -= remove_arg_num;
      --i;
    }
  }

  // Handle --gtest_repeat=[COUNT] option if we are in isolation mode.
  // We should check and remove this option to avoid child process running single test for several
  // iterations.
  int gtest_repeat_num = 1;
  if (isolate_option == true) {
    int len = sizeof("--gtest_repeat=") - 1;
    for (int i = 1; i < argc; ++i) {
      if (strncmp(argv[i], "--gtest_repeat=", len) == 0) {
        gtest_repeat_num = atoi(argv[i] + len);
        if (gtest_repeat_num < 0) {
          fprintf(stderr, "error count for option --gtest_repeat=[COUNT]\n");
          return false;
        }
        for (int j = i; j < argc - 1; ++j) {
          argv[j] = argv[j + 1];
        }
        --argc;
        break;
      }
    }
  }

  *pargc = argc;

  // Run tests in isolation mode.
  if (isolate_option) {
    *exit_flag = true;

    std::vector<TestCase> testcase_list;
    if (EnumerateTests(argc, argv, testcase_list) == false) {
      return false;
    }
    RunTestInSeparateProc(argc, argv, testcase_list, gtest_repeat_num, job_num_option);
    return true;
  }
  return true;
}

int main(int argc, char** argv) {
  bool exit_flag;
  int return_result = 0;

  if (PickOptions(&argc, &argv, &exit_flag) == false) {
    return_result = 1;
  } else if (!exit_flag) {
    testing::InitGoogleTest(&argc, argv);
    return_result = RUN_ALL_TESTS();
  }
  return return_result;
}

//################################################################################
// Bionic Gtest self test, run this by --bionic_gtest --isolate option.

TEST(bionic_gtest, test_success) {
  ASSERT_EQ(1, 1);
}

TEST(bionic_gtest, test_fail) {
  ASSERT_EQ(0, 1);
}

TEST(bionic_gtest, test_time_warn) {
  sleep(4);
}

TEST(bionic_gtest, test_timeout) {
  while (1) {}
}
