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
#include <sched.h>
#include <sys/prctl.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <unistd.h>

#include <gtest/gtest.h>

// Host libc does not define this.
#ifndef TRAP_HWBKPT
#define TRAP_HWBKPT 4
#endif

template<typename T>
static void __attribute__((noreturn)) fork_child(unsigned cpu, T &data) {
  // Extra precaution: make sure we go away if anything happens to our parent.
  if (prctl(PR_SET_PDEATHSIG, SIGKILL, 0, 0, 0) == -1) {
    perror("prctl(PR_SET_PDEATHSIG)");
    _exit(1);
  }

  cpu_set_t cpus;
  CPU_ZERO(&cpus);
  CPU_SET(cpu, &cpus);
  if (sched_setaffinity(0, sizeof cpus, &cpus) == -1) {
    perror("sched_setaffinity");
    _exit(2);
  }
  if (ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) == -1) {
    perror("ptrace(PTRACE_TRACEME)");
    _exit(3);
  }

  raise(SIGSTOP); // Synchronize with the tracer, let it set the watchpoint.

  data = 1; // Now trigger the watchpoint.

  _exit(0);
}

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

static bool are_watchpoints_supported(pid_t child) {
#if defined(__arm__)
  long capabilities;
  long result = ptrace(PTRACE_GETHBPREGS, child, 0, &capabilities);
  if (result == -1) {
    EXPECT_EQ(EIO, errno);
    return false;
  }
  return ((capabilities >> 8) & 0xff) > 0;
#elif defined(__aarch64__)
  user_hwdebug_state dreg_state;
  iovec iov;
  iov.iov_base = &dreg_state;
  iov.iov_len = sizeof(dreg_state);

  long result = ptrace(PTRACE_GETREGSET, child, NT_ARM_HW_WATCH, &iov);
  if (result == -1) {
    EXPECT_EQ(EINVAL, errno);
    return false;
  }
  return (dreg_state.dbg_info & 0xff) > 0;
#elif defined(__i386__) || defined(__x86_64__)
  // We assume watchpoints are always supported on x86.
  (void) child;
  return true;
#else
  // TODO: mips support.
  (void) child;
  return false;
#endif
}

static void set_watchpoint(pid_t child, const void *address, size_t size) {
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
  dreg_state.dbg_regs[0].addr = reinterpret_cast<uintptr_t>(address);
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
  (void) child;
  (void) address;
  (void) size;
#endif
}

template<typename T>
static void run_watchpoint_test_impl(unsigned cpu) {
  alignas(8) T data = 0;

  pid_t child = fork();
  ASSERT_NE(-1, child) << strerror(errno);
  if (child == 0) fork_child(cpu, data);

  ChildGuard guard(child);

  int status;
  ASSERT_EQ(child, waitpid(child, &status, __WALL)) << strerror(errno);
  ASSERT_TRUE(WIFSTOPPED(status)) << "Status was: " << status;
  ASSERT_EQ(SIGSTOP, WSTOPSIG(status)) << "Status was: " << status;

  if (!are_watchpoints_supported(child)) return;

  set_watchpoint(child, &data, sizeof data);

  ASSERT_EQ(0, ptrace(PTRACE_CONT, child, nullptr, nullptr)) << strerror(errno);
  ASSERT_EQ(child, waitpid(child, &status, __WALL)) << strerror(errno);
  ASSERT_TRUE(WIFSTOPPED(status)) << "Status was: " << status;
  ASSERT_EQ(SIGTRAP, WSTOPSIG(status)) << "Status was: " << status;

  siginfo_t siginfo;
  ASSERT_EQ(0, ptrace(PTRACE_GETSIGINFO, child, nullptr, &siginfo)) << strerror(errno);
  ASSERT_EQ(TRAP_HWBKPT, siginfo.si_code);
#if defined(__arm__) || defined(__aarch64__)
  ASSERT_EQ(&data, siginfo.si_addr);
#endif
}

static void run_watchpoint_test(unsigned cpu) {
  run_watchpoint_test_impl<uint8_t>(cpu);
  run_watchpoint_test_impl<uint16_t>(cpu);
  run_watchpoint_test_impl<uint32_t>(cpu);
#if defined(__LP64__)
  run_watchpoint_test_impl<uint64_t>(cpu);
#endif
}

// Test watchpoint API. The test is considered successful if our watchpoints get hit OR the
// system reports that watchpoint support is not present. We run the test for different
// watchpoint sizes, while pinning the process to each cpu in turn, for better coverage.
TEST(ptrace, watchpoint_stress) {
  cpu_set_t available_cpus;
  ASSERT_EQ(0, sched_getaffinity(0, sizeof available_cpus, &available_cpus));

  for (size_t cpu = 0; cpu < CPU_SETSIZE; ++cpu) {
    if (!CPU_ISSET(cpu, &available_cpus)) continue;
    run_watchpoint_test(cpu);
  }
}
