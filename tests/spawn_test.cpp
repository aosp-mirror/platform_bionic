/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <spawn.h>

#include <errno.h>
#include <fcntl.h>
#include <gtest/gtest.h>

#include "SignalUtils.h"
#include "utils.h"

#include <android-base/file.h>
#include <android-base/strings.h>

// Old versions of glibc didn't have POSIX_SPAWN_SETSID.
#if __GLIBC__
# if !defined(POSIX_SPAWN_SETSID)
#  define POSIX_SPAWN_SETSID 0
# endif
#endif

TEST(spawn, posix_spawnattr_init_posix_spawnattr_destroy) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));
  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setflags_EINVAL) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));
  ASSERT_EQ(EINVAL, posix_spawnattr_setflags(&sa, ~0));
  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setflags_posix_spawnattr_getflags) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  ASSERT_EQ(0, posix_spawnattr_setflags(&sa, POSIX_SPAWN_RESETIDS));
  short flags;
  ASSERT_EQ(0, posix_spawnattr_getflags(&sa, &flags));
  ASSERT_EQ(POSIX_SPAWN_RESETIDS, flags);

  constexpr short all_flags = POSIX_SPAWN_RESETIDS | POSIX_SPAWN_SETPGROUP | POSIX_SPAWN_SETSIGDEF |
                              POSIX_SPAWN_SETSIGMASK | POSIX_SPAWN_SETSCHEDPARAM |
                              POSIX_SPAWN_SETSCHEDULER | POSIX_SPAWN_USEVFORK | POSIX_SPAWN_SETSID;
  ASSERT_EQ(0, posix_spawnattr_setflags(&sa, all_flags));
  ASSERT_EQ(0, posix_spawnattr_getflags(&sa, &flags));
  ASSERT_EQ(all_flags, flags);

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setpgroup_posix_spawnattr_getpgroup) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  ASSERT_EQ(0, posix_spawnattr_setpgroup(&sa, 123));
  pid_t g;
  ASSERT_EQ(0, posix_spawnattr_getpgroup(&sa, &g));
  ASSERT_EQ(123, g);

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setsigmask_posix_spawnattr_getsigmask) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  sigset_t sigs;
  ASSERT_EQ(0, posix_spawnattr_getsigmask(&sa, &sigs));
  ASSERT_FALSE(sigismember(&sigs, SIGALRM));

  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  ASSERT_EQ(0, posix_spawnattr_setsigmask(&sa, &just_SIGALRM));

  ASSERT_EQ(0, posix_spawnattr_getsigmask(&sa, &sigs));
  ASSERT_TRUE(sigismember(&sigs, SIGALRM));

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setsigmask64_posix_spawnattr_getsigmask64) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  sigset64_t sigs;
  ASSERT_EQ(0, posix_spawnattr_getsigmask64(&sa, &sigs));
  ASSERT_FALSE(sigismember64(&sigs, SIGRTMIN));

  sigset64_t just_SIGRTMIN;
  sigemptyset64(&just_SIGRTMIN);
  sigaddset64(&just_SIGRTMIN, SIGRTMIN);
  ASSERT_EQ(0, posix_spawnattr_setsigmask64(&sa, &just_SIGRTMIN));

  ASSERT_EQ(0, posix_spawnattr_getsigmask64(&sa, &sigs));
  ASSERT_TRUE(sigismember64(&sigs, SIGRTMIN));

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setsigdefault_posix_spawnattr_getsigdefault) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  sigset_t sigs;
  ASSERT_EQ(0, posix_spawnattr_getsigdefault(&sa, &sigs));
  ASSERT_FALSE(sigismember(&sigs, SIGALRM));

  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  ASSERT_EQ(0, posix_spawnattr_setsigdefault(&sa, &just_SIGALRM));

  ASSERT_EQ(0, posix_spawnattr_getsigdefault(&sa, &sigs));
  ASSERT_TRUE(sigismember(&sigs, SIGALRM));

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setsigdefault64_posix_spawnattr_getsigdefault64) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  sigset64_t sigs;
  ASSERT_EQ(0, posix_spawnattr_getsigdefault64(&sa, &sigs));
  ASSERT_FALSE(sigismember64(&sigs, SIGRTMIN));

  sigset64_t just_SIGRTMIN;
  sigemptyset64(&just_SIGRTMIN);
  sigaddset64(&just_SIGRTMIN, SIGRTMIN);
  ASSERT_EQ(0, posix_spawnattr_setsigdefault64(&sa, &just_SIGRTMIN));

  ASSERT_EQ(0, posix_spawnattr_getsigdefault64(&sa, &sigs));
  ASSERT_TRUE(sigismember64(&sigs, SIGRTMIN));

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setsschedparam_posix_spawnattr_getsschedparam) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  sched_param sp;
  ASSERT_EQ(0, posix_spawnattr_getschedparam(&sa, &sp));
  ASSERT_EQ(0, sp.sched_priority);

  sched_param sp123 = { .sched_priority = 123 };
  ASSERT_EQ(0, posix_spawnattr_setschedparam(&sa, &sp123));

  ASSERT_EQ(0, posix_spawnattr_getschedparam(&sa, &sp));
  ASSERT_EQ(123, sp.sched_priority);

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setschedpolicy_posix_spawnattr_getschedpolicy) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  int p;
  ASSERT_EQ(0, posix_spawnattr_getschedpolicy(&sa, &p));
  ASSERT_EQ(0, p);

  ASSERT_EQ(0, posix_spawnattr_setschedpolicy(&sa, SCHED_FIFO));

  ASSERT_EQ(0, posix_spawnattr_getschedpolicy(&sa, &p));
  ASSERT_EQ(SCHED_FIFO, p);

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawn) {
  ExecTestHelper eth;
  eth.SetArgs({BIN_DIR "true", nullptr});
  pid_t pid;
  ASSERT_EQ(0, posix_spawn(&pid, eth.GetArg0(), nullptr, nullptr, eth.GetArgs(), nullptr));
  AssertChildExited(pid, 0);
}

TEST(spawn, posix_spawn_not_found) {
  ExecTestHelper eth;
  eth.SetArgs({"true", nullptr});
  pid_t pid;
  ASSERT_EQ(0, posix_spawn(&pid, eth.GetArg0(), nullptr, nullptr, eth.GetArgs(), nullptr));
  AssertChildExited(pid, 127);
}

TEST(spawn, posix_spawnp) {
  ExecTestHelper eth;
  eth.SetArgs({"true", nullptr});
  pid_t pid;
  ASSERT_EQ(0, posix_spawnp(&pid, eth.GetArg0(), nullptr, nullptr, eth.GetArgs(), nullptr));
  AssertChildExited(pid, 0);
}

TEST(spawn, posix_spawnp_not_found) {
  ExecTestHelper eth;
  eth.SetArgs({"does-not-exist", nullptr});
  pid_t pid;
  ASSERT_EQ(0, posix_spawnp(&pid, eth.GetArg0(), nullptr, nullptr, eth.GetArgs(), nullptr));
  AssertChildExited(pid, 127);
}

TEST(spawn, posix_spawn_environment) {
  ExecTestHelper eth;
  eth.SetArgs({"sh", "-c", "exit $posix_spawn_environment_test", nullptr});
  eth.SetEnv({"posix_spawn_environment_test=66", nullptr});
  pid_t pid;
  ASSERT_EQ(0, posix_spawnp(&pid, eth.GetArg0(), nullptr, nullptr, eth.GetArgs(), eth.GetEnv()));
  AssertChildExited(pid, 66);
}

TEST(spawn, posix_spawn_file_actions) {
  int fds[2];
  ASSERT_NE(-1, pipe(fds));

  posix_spawn_file_actions_t fa;
  ASSERT_EQ(0, posix_spawn_file_actions_init(&fa));

  ASSERT_EQ(0, posix_spawn_file_actions_addclose(&fa, fds[0]));
  ASSERT_EQ(0, posix_spawn_file_actions_adddup2(&fa, fds[1], 1));
  ASSERT_EQ(0, posix_spawn_file_actions_addclose(&fa, fds[1]));
  // Check that close(2) failures are ignored by closing the same fd again.
  ASSERT_EQ(0, posix_spawn_file_actions_addclose(&fa, fds[1]));
  ASSERT_EQ(0, posix_spawn_file_actions_addopen(&fa, 56, "/proc/version", O_RDONLY, 0));

  ExecTestHelper eth;
  eth.SetArgs({"ls", "-l", "/proc/self/fd", nullptr});
  pid_t pid;
  ASSERT_EQ(0, posix_spawnp(&pid, eth.GetArg0(), &fa, nullptr, eth.GetArgs(), eth.GetEnv()));
  ASSERT_EQ(0, posix_spawn_file_actions_destroy(&fa));

  ASSERT_EQ(0, close(fds[1]));
  std::string content;
  ASSERT_TRUE(android::base::ReadFdToString(fds[0], &content));
  ASSERT_EQ(0, close(fds[0]));

  AssertChildExited(pid, 0);

  // We'll know the dup2 worked if we see any ls(1) output in our pipe.
  // The open we can check manually...
  bool open_to_fd_56_worked = false;
  for (const auto& line : android::base::Split(content, "\n")) {
    if (line.find(" 56 -> /proc/version") != std::string::npos) open_to_fd_56_worked = true;
  }
  ASSERT_TRUE(open_to_fd_56_worked);
}

static void CatFileToString(posix_spawnattr_t* sa, const char* path, std::string* content) {
  int fds[2];
  ASSERT_NE(-1, pipe(fds));

  posix_spawn_file_actions_t fa;
  ASSERT_EQ(0, posix_spawn_file_actions_init(&fa));
  ASSERT_EQ(0, posix_spawn_file_actions_addclose(&fa, fds[0]));
  ASSERT_EQ(0, posix_spawn_file_actions_adddup2(&fa, fds[1], 1));
  ASSERT_EQ(0, posix_spawn_file_actions_addclose(&fa, fds[1]));

  ExecTestHelper eth;
  eth.SetArgs({"cat", path, nullptr});
  pid_t pid;
  ASSERT_EQ(0, posix_spawnp(&pid, eth.GetArg0(), &fa, sa, eth.GetArgs(), nullptr));
  ASSERT_EQ(0, posix_spawn_file_actions_destroy(&fa));

  ASSERT_EQ(0, close(fds[1]));
  ASSERT_TRUE(android::base::ReadFdToString(fds[0], content));
  ASSERT_EQ(0, close(fds[0]));
  AssertChildExited(pid, 0);
}

struct ProcStat {
  pid_t pid;
  pid_t ppid;
  pid_t pgrp;
  pid_t sid;
};

static void GetChildStat(posix_spawnattr_t* sa, ProcStat* ps) {
  std::string content;
  CatFileToString(sa, "/proc/self/stat", &content);

  ASSERT_EQ(4, sscanf(content.c_str(), "%d (cat) %*c %d %d %d", &ps->pid, &ps->ppid, &ps->pgrp,
                      &ps->sid));

  ASSERT_EQ(getpid(), ps->ppid);
}

struct ProcStatus {
  uint64_t sigblk;
  uint64_t sigign;
};

static void GetChildStatus(posix_spawnattr_t* sa, ProcStatus* ps) {
  std::string content;
  CatFileToString(sa, "/proc/self/status", &content);

  bool saw_blk = false;
  bool saw_ign = false;
  for (const auto& line : android::base::Split(content, "\n")) {
    if (sscanf(line.c_str(), "SigBlk: %" SCNx64, &ps->sigblk) == 1) saw_blk = true;
    if (sscanf(line.c_str(), "SigIgn: %" SCNx64, &ps->sigign) == 1) saw_ign = true;
  }
  ASSERT_TRUE(saw_blk);
  ASSERT_TRUE(saw_ign);
}

TEST(spawn, posix_spawn_POSIX_SPAWN_SETSID_clear) {
  pid_t parent_sid = getsid(0);

  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));
  ASSERT_EQ(0, posix_spawnattr_setflags(&sa, 0));

  ProcStat ps = {};
  GetChildStat(&sa, &ps);
  ASSERT_EQ(parent_sid, ps.sid);
  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawn_POSIX_SPAWN_SETSID_set) {
  pid_t parent_sid = getsid(0);

  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));
  ASSERT_EQ(0, posix_spawnattr_setflags(&sa, POSIX_SPAWN_SETSID));

  ProcStat ps = {};
  GetChildStat(&sa, &ps);
  ASSERT_NE(parent_sid, ps.sid);
  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawn_POSIX_SPAWN_SETPGROUP_clear) {
  pid_t parent_pgrp = getpgrp();

  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));
  ASSERT_EQ(0, posix_spawnattr_setflags(&sa, 0));

  ProcStat ps = {};
  GetChildStat(&sa, &ps);
  ASSERT_EQ(parent_pgrp, ps.pgrp);
  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawn_POSIX_SPAWN_SETPGROUP_set) {
  pid_t parent_pgrp = getpgrp();

  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));
  ASSERT_EQ(0, posix_spawnattr_setpgroup(&sa, 0));
  ASSERT_EQ(0, posix_spawnattr_setflags(&sa, POSIX_SPAWN_SETPGROUP));

  ProcStat ps = {};
  GetChildStat(&sa, &ps);
  ASSERT_NE(parent_pgrp, ps.pgrp);
  // Setting pgid 0 means "the same as the caller's pid".
  ASSERT_EQ(ps.pid, ps.pgrp);
  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawn_POSIX_SPAWN_SETSIGMASK) {
  // Block SIGBUS in the parent...
  sigset_t just_SIGBUS;
  sigemptyset(&just_SIGBUS);
  sigaddset(&just_SIGBUS, SIGBUS);
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, &just_SIGBUS, nullptr));

  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  // Ask for only SIGALRM to be blocked in the child...
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  ASSERT_EQ(0, posix_spawnattr_setsigmask(&sa, &just_SIGALRM));
  ASSERT_EQ(0, posix_spawnattr_setflags(&sa, POSIX_SPAWN_SETSIGMASK));

  // Check that's what happens...
  ProcStatus ps = {};
  GetChildStatus(&sa, &ps);

  // TIMER_SIGNAL should also be blocked.
  uint64_t expected_blocked = 0;
  SignalSetAdd(&expected_blocked, SIGALRM);
  SignalSetAdd(&expected_blocked, __SIGRTMIN + 0);
  EXPECT_EQ(expected_blocked, ps.sigblk);

  EXPECT_EQ(static_cast<uint64_t>(0), ps.sigign);

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawn_POSIX_SPAWN_SETSIGDEF) {
  // Ignore SIGALRM and SIGCONT in the parent...
  ASSERT_NE(SIG_ERR, signal(SIGALRM, SIG_IGN));
  ASSERT_NE(SIG_ERR, signal(SIGCONT, SIG_IGN));

  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  // Ask for SIGALRM to be defaulted in the child...
  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);

  ASSERT_EQ(0, posix_spawnattr_setsigdefault(&sa, &just_SIGALRM));
  ASSERT_EQ(0, posix_spawnattr_setflags(&sa, POSIX_SPAWN_SETSIGDEF));

  // Check that's what happens...
  ProcStatus ps = {};
  GetChildStatus(&sa, &ps);

  // TIMER_SIGNAL should be blocked.
  uint64_t expected_blocked = 0;
  SignalSetAdd(&expected_blocked, __SIGRTMIN + 0);
  EXPECT_EQ(expected_blocked, ps.sigblk);

  uint64_t expected_ignored = 0;
  SignalSetAdd(&expected_ignored, SIGCONT);
  EXPECT_EQ(expected_ignored, ps.sigign);

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, signal_stress) {
  // Ensure that posix_spawn doesn't restore the caller's signal mask in the
  // child without first defaulting any caught signals (http://b/68707996).
  static pid_t parent = getpid();

  setpgid(0, 0);
  signal(SIGRTMIN, SIG_IGN);

  pid_t pid = fork();
  ASSERT_NE(-1, pid);

  if (pid == 0) {
    for (size_t i = 0; i < 1024; ++i) {
      kill(0, SIGRTMIN);
      usleep(10);
    }
    _exit(99);
  }

  // We test both with and without attributes, because they used to be
  // different codepaths. We also test with an empty `sigdefault` set.
  posix_spawnattr_t attr1;
  posix_spawnattr_init(&attr1);

  sigset_t empty_mask = {};
  posix_spawnattr_t attr2;
  posix_spawnattr_init(&attr2);
  posix_spawnattr_setflags(&attr2, POSIX_SPAWN_SETSIGDEF);
  posix_spawnattr_setsigdefault(&attr2, &empty_mask);

  posix_spawnattr_t* attrs[] = { nullptr, &attr1, &attr2 };

  // We use a real-time signal because that's a tricky case for LP32
  // because our sigset_t was too small.
  ScopedSignalHandler ssh(SIGRTMIN, [](int) { ASSERT_EQ(getpid(), parent); });

  const size_t pid_count = 128;
  pid_t spawned_pids[pid_count];

  ExecTestHelper eth;
  eth.SetArgs({"true", nullptr});
  for (size_t i = 0; i < pid_count; ++i) {
    pid_t spawned_pid;
    ASSERT_EQ(0, posix_spawn(&spawned_pid, "true", nullptr, attrs[i % 3], eth.GetArgs(), nullptr));
    spawned_pids[i] = spawned_pid;
  }

  for (pid_t spawned_pid : spawned_pids) {
    ASSERT_EQ(spawned_pid, TEMP_FAILURE_RETRY(waitpid(spawned_pid, nullptr, 0)));
  }

  AssertChildExited(pid, 99);
}
