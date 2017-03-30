/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <sys/ptrace.h>

#include <elf.h>
#include <err.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include <android-base/macros.h>
#include <android-base/unique_fd.h>

using namespace std::chrono_literals;

using android::base::unique_fd;

// Host libc does not define this.
#ifndef TRAP_HWBKPT
#define TRAP_HWBKPT 4
#endif

class ChildGuard {
 public:
  explicit ChildGuard(pid_t pid) : pid(pid) {}

  ~ChildGuard() {
    kill(pid, SIGKILL);
    int status;
    waitpid(pid, &status, 0);
  }

 private:
  pid_t pid;
};

enum class HwFeature { Watchpoint, Breakpoint };

static bool is_hw_feature_supported(pid_t child, HwFeature feature) {
#if defined(__arm__)
  long capabilities;
  long result = ptrace(PTRACE_GETHBPREGS, child, 0, &capabilities);
  if (result == -1) {
    EXPECT_EQ(EIO, errno);
    return false;
  }
  switch (feature) {
    case HwFeature::Watchpoint:
      return ((capabilities >> 8) & 0xff) > 0;
    case HwFeature::Breakpoint:
      return (capabilities & 0xff) > 0;
  }
#elif defined(__aarch64__)
  user_hwdebug_state dreg_state;
  iovec iov;
  iov.iov_base = &dreg_state;
  iov.iov_len = sizeof(dreg_state);

  long result = ptrace(PTRACE_GETREGSET, child,
                       feature == HwFeature::Watchpoint ? NT_ARM_HW_WATCH : NT_ARM_HW_BREAK, &iov);
  if (result == -1) {
    EXPECT_EQ(EINVAL, errno);
    return false;
  }
  return (dreg_state.dbg_info & 0xff) > 0;
#elif defined(__i386__) || defined(__x86_64__)
  // We assume watchpoints and breakpoints are always supported on x86.
  UNUSED(child);
  UNUSED(feature);
  return true;
#else
  // TODO: mips support.
  UNUSED(child);
  UNUSED(feature);
  return false;
#endif
}

static void set_watchpoint(pid_t child, uintptr_t address, size_t size) {
  ASSERT_EQ(0u, address & 0x7) << "address: " << address;
#if defined(__arm__) || defined(__aarch64__)
  const unsigned byte_mask = (1 << size) - 1;
  const unsigned type = 2; // Write.
  const unsigned enable = 1;
  const unsigned control = byte_mask << 5 | type << 3 | enable;

#ifdef __arm__
  ASSERT_EQ(0, ptrace(PTRACE_SETHBPREGS, child, -1, &address)) << strerror(errno);
  ASSERT_EQ(0, ptrace(PTRACE_SETHBPREGS, child, -2, &control)) << strerror(errno);
#else // aarch64
  user_hwdebug_state dreg_state;
  memset(&dreg_state, 0, sizeof dreg_state);
  dreg_state.dbg_regs[0].addr = address;
  dreg_state.dbg_regs[0].ctrl = control;

  iovec iov;
  iov.iov_base = &dreg_state;
  iov.iov_len = offsetof(user_hwdebug_state, dbg_regs) + sizeof(dreg_state.dbg_regs[0]);

  ASSERT_EQ(0, ptrace(PTRACE_SETREGSET, child, NT_ARM_HW_WATCH, &iov)) << strerror(errno);
#endif
#elif defined(__i386__) || defined(__x86_64__)
  ASSERT_EQ(0, ptrace(PTRACE_POKEUSER, child, offsetof(user, u_debugreg[0]), address)) << strerror(errno);
  errno = 0;
  unsigned data = ptrace(PTRACE_PEEKUSER, child, offsetof(user, u_debugreg[7]), nullptr);
  ASSERT_EQ(0, errno);

  const unsigned size_flag = (size == 8) ? 2 : size - 1;
  const unsigned enable = 1;
  const unsigned type = 1; // Write.

  const unsigned mask = 3 << 18 | 3 << 16 | 1;
  const unsigned value = size_flag << 18 | type << 16 | enable;
  data &= mask;
  data |= value;
  ASSERT_EQ(0, ptrace(PTRACE_POKEUSER, child, offsetof(user, u_debugreg[7]), data)) << strerror(errno);
#else
  UNUSED(child);
  UNUSED(address);
  UNUSED(size);
#endif
}

template <typename T>
static void run_watchpoint_test(std::function<void(T&)> child_func, size_t offset, size_t size) {
  alignas(16) T data{};

  pid_t child = fork();
  ASSERT_NE(-1, child) << strerror(errno);
  if (child == 0) {
    // Extra precaution: make sure we go away if anything happens to our parent.
    if (prctl(PR_SET_PDEATHSIG, SIGKILL, 0, 0, 0) == -1) {
      perror("prctl(PR_SET_PDEATHSIG)");
      _exit(1);
    }

    if (ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) == -1) {
      perror("ptrace(PTRACE_TRACEME)");
      _exit(2);
    }

    child_func(data);
    _exit(0);
  }

  ChildGuard guard(child);

  int status;
  ASSERT_EQ(child, waitpid(child, &status, __WALL)) << strerror(errno);
  ASSERT_TRUE(WIFSTOPPED(status)) << "Status was: " << status;
  ASSERT_EQ(SIGSTOP, WSTOPSIG(status)) << "Status was: " << status;

  if (!is_hw_feature_supported(child, HwFeature::Watchpoint)) {
    GTEST_LOG_(INFO) << "Skipping test because hardware support is not available.\n";
    return;
  }

  set_watchpoint(child, uintptr_t(&data) + offset, size);

  ASSERT_EQ(0, ptrace(PTRACE_CONT, child, nullptr, nullptr)) << strerror(errno);
  ASSERT_EQ(child, waitpid(child, &status, __WALL)) << strerror(errno);
  ASSERT_TRUE(WIFSTOPPED(status)) << "Status was: " << status;
  ASSERT_EQ(SIGTRAP, WSTOPSIG(status)) << "Status was: " << status;

  siginfo_t siginfo;
  ASSERT_EQ(0, ptrace(PTRACE_GETSIGINFO, child, nullptr, &siginfo)) << strerror(errno);
  ASSERT_EQ(TRAP_HWBKPT, siginfo.si_code);
#if defined(__arm__) || defined(__aarch64__)
  ASSERT_LE(&data, siginfo.si_addr);
  ASSERT_GT((&data) + 1, siginfo.si_addr);
#endif
}

template <typename T>
static void watchpoint_stress_child(unsigned cpu, T& data) {
  cpu_set_t cpus;
  CPU_ZERO(&cpus);
  CPU_SET(cpu, &cpus);
  if (sched_setaffinity(0, sizeof cpus, &cpus) == -1) {
    perror("sched_setaffinity");
    _exit(3);
  }
  raise(SIGSTOP);  // Synchronize with the tracer, let it set the watchpoint.

  data = 1;  // Now trigger the watchpoint.
}

template <typename T>
static void run_watchpoint_stress(size_t cpu) {
  run_watchpoint_test<T>(std::bind(watchpoint_stress_child<T>, cpu, std::placeholders::_1), 0,
                         sizeof(T));
}

// Test watchpoint API. The test is considered successful if our watchpoints get hit OR the
// system reports that watchpoint support is not present. We run the test for different
// watchpoint sizes, while pinning the process to each cpu in turn, for better coverage.
TEST(sys_ptrace, watchpoint_stress) {
  cpu_set_t available_cpus;
  ASSERT_EQ(0, sched_getaffinity(0, sizeof available_cpus, &available_cpus));

  for (size_t cpu = 0; cpu < CPU_SETSIZE; ++cpu) {
    if (!CPU_ISSET(cpu, &available_cpus)) continue;

    run_watchpoint_stress<uint8_t>(cpu);
    run_watchpoint_stress<uint16_t>(cpu);
    run_watchpoint_stress<uint32_t>(cpu);
#if defined(__LP64__)
    run_watchpoint_stress<uint64_t>(cpu);
#endif
  }
}

struct Uint128_t {
  uint64_t data[2];
};
static void watchpoint_imprecise_child(Uint128_t& data) {
  raise(SIGSTOP);  // Synchronize with the tracer, let it set the watchpoint.

#if defined(__i386__) || defined(__x86_64__)
  asm volatile("movdqa %%xmm0, %0" : : "m"(data));
#elif defined(__arm__)
  asm volatile("stm %0, { r0, r1, r2, r3 }" : : "r"(&data));
#elif defined(__aarch64__)
  asm volatile("stp x0, x1, %0" : : "m"(data));
#elif defined(__mips__)
// TODO
  UNUSED(data);
#endif
}

// Test that the kernel is able to handle the case when the instruction writes
// to a larger block of memory than the one we are watching. If you see this
// test fail on arm64, you will likely need to cherry-pick fdfeff0f into your
// kernel.
TEST(sys_ptrace, watchpoint_imprecise) {
  // Make sure we get interrupted in case a buggy kernel does not report the
  // watchpoint hit correctly.
  struct sigaction action, oldaction;
  action.sa_handler = [](int) {};
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  ASSERT_EQ(0, sigaction(SIGALRM, &action, &oldaction)) << strerror(errno);
  alarm(5);

  run_watchpoint_test<Uint128_t>(watchpoint_imprecise_child, 8, 8);

  ASSERT_EQ(0, sigaction(SIGALRM, &oldaction, nullptr)) << strerror(errno);
}

static void __attribute__((noinline)) breakpoint_func() {
  asm volatile("");
}

static void __attribute__((noreturn)) breakpoint_fork_child() {
  // Extra precaution: make sure we go away if anything happens to our parent.
  if (prctl(PR_SET_PDEATHSIG, SIGKILL, 0, 0, 0) == -1) {
    perror("prctl(PR_SET_PDEATHSIG)");
    _exit(1);
  }

  if (ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) == -1) {
    perror("ptrace(PTRACE_TRACEME)");
    _exit(2);
  }

  raise(SIGSTOP);  // Synchronize with the tracer, let it set the breakpoint.

  breakpoint_func();  // Now trigger the breakpoint.

  _exit(0);
}

static void set_breakpoint(pid_t child) {
  uintptr_t address = uintptr_t(breakpoint_func);
#if defined(__arm__) || defined(__aarch64__)
  address &= ~3;
  const unsigned byte_mask = 0xf;
  const unsigned enable = 1;
  const unsigned control = byte_mask << 5 | enable;

#ifdef __arm__
  ASSERT_EQ(0, ptrace(PTRACE_SETHBPREGS, child, 1, &address)) << strerror(errno);
  ASSERT_EQ(0, ptrace(PTRACE_SETHBPREGS, child, 2, &control)) << strerror(errno);
#else  // aarch64
  user_hwdebug_state dreg_state;
  memset(&dreg_state, 0, sizeof dreg_state);
  dreg_state.dbg_regs[0].addr = reinterpret_cast<uintptr_t>(address);
  dreg_state.dbg_regs[0].ctrl = control;

  iovec iov;
  iov.iov_base = &dreg_state;
  iov.iov_len = offsetof(user_hwdebug_state, dbg_regs) + sizeof(dreg_state.dbg_regs[0]);

  ASSERT_EQ(0, ptrace(PTRACE_SETREGSET, child, NT_ARM_HW_BREAK, &iov)) << strerror(errno);
#endif
#elif defined(__i386__) || defined(__x86_64__)
  ASSERT_EQ(0, ptrace(PTRACE_POKEUSER, child, offsetof(user, u_debugreg[0]), address))
      << strerror(errno);
  errno = 0;
  unsigned data = ptrace(PTRACE_PEEKUSER, child, offsetof(user, u_debugreg[7]), nullptr);
  ASSERT_EQ(0, errno);

  const unsigned size = 0;
  const unsigned enable = 1;
  const unsigned type = 0;  // Execute

  const unsigned mask = 3 << 18 | 3 << 16 | 1;
  const unsigned value = size << 18 | type << 16 | enable;
  data &= mask;
  data |= value;
  ASSERT_EQ(0, ptrace(PTRACE_POKEUSER, child, offsetof(user, u_debugreg[7]), data))
      << strerror(errno);
#else
  UNUSED(child);
  UNUSED(address);
#endif
}

// Test hardware breakpoint API. The test is considered successful if the breakpoints get hit OR the
// system reports that hardware breakpoint support is not present.
TEST(sys_ptrace, hardware_breakpoint) {
  pid_t child = fork();
  ASSERT_NE(-1, child) << strerror(errno);
  if (child == 0) breakpoint_fork_child();

  ChildGuard guard(child);

  int status;
  ASSERT_EQ(child, waitpid(child, &status, __WALL)) << strerror(errno);
  ASSERT_TRUE(WIFSTOPPED(status)) << "Status was: " << status;
  ASSERT_EQ(SIGSTOP, WSTOPSIG(status)) << "Status was: " << status;

  if (!is_hw_feature_supported(child, HwFeature::Breakpoint)) {
    GTEST_LOG_(INFO) << "Skipping test because hardware support is not available.\n";
    return;
  }

  set_breakpoint(child);

  ASSERT_EQ(0, ptrace(PTRACE_CONT, child, nullptr, nullptr)) << strerror(errno);
  ASSERT_EQ(child, waitpid(child, &status, __WALL)) << strerror(errno);
  ASSERT_TRUE(WIFSTOPPED(status)) << "Status was: " << status;
  ASSERT_EQ(SIGTRAP, WSTOPSIG(status)) << "Status was: " << status;

  siginfo_t siginfo;
  ASSERT_EQ(0, ptrace(PTRACE_GETSIGINFO, child, nullptr, &siginfo)) << strerror(errno);
  ASSERT_EQ(TRAP_HWBKPT, siginfo.si_code);
}

class PtraceResumptionTest : public ::testing::Test {
 public:
  unique_fd worker_pipe_write;

  pid_t worker = -1;
  pid_t tracer = -1;

  PtraceResumptionTest() {
    unique_fd worker_pipe_read;
    int pipefd[2];
    if (pipe2(pipefd, O_CLOEXEC) != 0) {
      err(1, "failed to create pipe");
    }

    worker_pipe_read.reset(pipefd[0]);
    worker_pipe_write.reset(pipefd[1]);

    worker = fork();
    if (worker == -1) {
      err(1, "failed to fork worker");
    } else if (worker == 0) {
      char buf;
      worker_pipe_write.reset();
      TEMP_FAILURE_RETRY(read(worker_pipe_read.get(), &buf, sizeof(buf)));
      exit(0);
    }
  }

  ~PtraceResumptionTest() {
  }

  void AssertDeath(int signo);

  void StartTracer(std::function<void()> f) {
    tracer = fork();
    ASSERT_NE(-1, tracer);
    if (tracer == 0) {
      f();
      if (HasFatalFailure()) {
        exit(1);
      }
      exit(0);
    }
  }

  bool WaitForTracer() {
    if (tracer == -1) {
      errx(1, "tracer not started");
    }

    int result;
    pid_t rc = waitpid(tracer, &result, 0);
    if (rc != tracer) {
      printf("waitpid returned %d (%s)\n", rc, strerror(errno));
      return false;
    }

    if (!WIFEXITED(result) && !WIFSIGNALED(result)) {
      printf("!WIFEXITED && !WIFSIGNALED\n");
      return false;
    }

    if (WIFEXITED(result)) {
      if (WEXITSTATUS(result) != 0) {
        printf("tracer failed\n");
        return false;
      }
    }

    return true;
  }

  bool WaitForWorker() {
    if (worker == -1) {
      errx(1, "worker not started");
    }

    int result;
    pid_t rc = waitpid(worker, &result, WNOHANG);
    if (rc != 0) {
      printf("worker exited prematurely\n");
      return false;
    }

    worker_pipe_write.reset();

    rc = waitpid(worker, &result, 0);
    if (rc != worker) {
      printf("waitpid for worker returned %d (%s)\n", rc, strerror(errno));
      return false;
    }

    if (!WIFEXITED(result)) {
      printf("worker didn't exit\n");
      return false;
    }

    if (WEXITSTATUS(result) != 0) {
      printf("worker exited with status %d\n", WEXITSTATUS(result));
      return false;
    }

    return true;
  }
};

static void wait_for_ptrace_stop(pid_t pid) {
  while (true) {
    int status;
    pid_t rc = TEMP_FAILURE_RETRY(waitpid(pid, &status, __WALL));
    if (rc != pid) {
      abort();
    }
    if (WIFSTOPPED(status)) {
      return;
    }
  }
}

TEST_F(PtraceResumptionTest, smoke) {
  // Make sure that the worker doesn't exit before the tracer stops tracing.
  StartTracer([this]() {
    ASSERT_EQ(0, ptrace(PTRACE_SEIZE, worker, 0, 0)) << strerror(errno);
    ASSERT_EQ(0, ptrace(PTRACE_INTERRUPT, worker, 0, 0)) << strerror(errno);
    wait_for_ptrace_stop(worker);
    std::this_thread::sleep_for(500ms);
  });

  worker_pipe_write.reset();
  std::this_thread::sleep_for(250ms);

  int result;
  ASSERT_EQ(0, waitpid(worker, &result, WNOHANG));
  ASSERT_TRUE(WaitForTracer());
  ASSERT_EQ(worker, waitpid(worker, &result, 0));
}

TEST_F(PtraceResumptionTest, seize) {
  StartTracer([this]() { ASSERT_EQ(0, ptrace(PTRACE_SEIZE, worker, 0, 0)) << strerror(errno); });
  ASSERT_TRUE(WaitForTracer());
  ASSERT_TRUE(WaitForWorker());
}

TEST_F(PtraceResumptionTest, seize_interrupt) {
  StartTracer([this]() {
    ASSERT_EQ(0, ptrace(PTRACE_SEIZE, worker, 0, 0)) << strerror(errno);
    ASSERT_EQ(0, ptrace(PTRACE_INTERRUPT, worker, 0, 0)) << strerror(errno);
    wait_for_ptrace_stop(worker);
  });
  ASSERT_TRUE(WaitForTracer());
  ASSERT_TRUE(WaitForWorker());
}

TEST_F(PtraceResumptionTest, seize_interrupt_cont) {
  StartTracer([this]() {
    ASSERT_EQ(0, ptrace(PTRACE_SEIZE, worker, 0, 0)) << strerror(errno);
    ASSERT_EQ(0, ptrace(PTRACE_INTERRUPT, worker, 0, 0)) << strerror(errno);
    wait_for_ptrace_stop(worker);
    ASSERT_EQ(0, ptrace(PTRACE_CONT, worker, 0, 0)) << strerror(errno);
  });
  ASSERT_TRUE(WaitForTracer());
  ASSERT_TRUE(WaitForWorker());
}

TEST_F(PtraceResumptionTest, zombie_seize) {
  StartTracer([this]() { ASSERT_EQ(0, ptrace(PTRACE_SEIZE, worker, 0, 0)) << strerror(errno); });
  ASSERT_TRUE(WaitForWorker());
  ASSERT_TRUE(WaitForTracer());
}

TEST_F(PtraceResumptionTest, zombie_seize_interrupt) {
  StartTracer([this]() {
    ASSERT_EQ(0, ptrace(PTRACE_SEIZE, worker, 0, 0)) << strerror(errno);
    ASSERT_EQ(0, ptrace(PTRACE_INTERRUPT, worker, 0, 0)) << strerror(errno);
    wait_for_ptrace_stop(worker);
  });
  ASSERT_TRUE(WaitForWorker());
  ASSERT_TRUE(WaitForTracer());
}

TEST_F(PtraceResumptionTest, zombie_seize_interrupt_cont) {
  StartTracer([this]() {
    ASSERT_EQ(0, ptrace(PTRACE_SEIZE, worker, 0, 0)) << strerror(errno);
    ASSERT_EQ(0, ptrace(PTRACE_INTERRUPT, worker, 0, 0)) << strerror(errno);
    wait_for_ptrace_stop(worker);
    ASSERT_EQ(0, ptrace(PTRACE_CONT, worker, 0, 0)) << strerror(errno);
  });
  ASSERT_TRUE(WaitForWorker());
  ASSERT_TRUE(WaitForTracer());
}
