/*
 * Copyright (C) 2020 The Android Open Source Project
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

#if defined(LIBC_STATIC)
#error This file should not be compiled for static targets.
#endif

#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ucontext.h>
#include <sys/un.h>

#include <async_safe/log.h>
#include <platform/bionic/malloc.h>
#include <platform/bionic/reserved_signals.h>
#include <private/ErrnoRestorer.h>
#include <private/ScopedFd.h>

#include "malloc_heapprofd.h"

// This file defines the handler for the reserved signal sent by the Android
// platform's profilers. The accompanying signal value discriminates between
// specific requestors:
//  0: heapprofd heap profiler.
//  1: traced_perf perf profiler.
static constexpr int kHeapprofdSignalValue = 0;
static constexpr int kTracedPerfSignalValue = 1;

static void HandleProfilingSignal(int, siginfo_t*, void*);

// Called during dynamic libc preinit.
__LIBC_HIDDEN__ void __libc_init_profiling_handlers() {
  struct sigaction action = {};
  action.sa_flags = SA_SIGINFO | SA_RESTART;
  action.sa_sigaction = HandleProfilingSignal;
  sigaction(BIONIC_SIGNAL_PROFILER, &action, nullptr);

  // The perfetto_hprof ART plugin installs a signal handler to handle this signal. That plugin
  // does not get loaded for a) non-apps, b) non-profilable apps on user. The default signal
  // disposition is to crash. We do not want the target to crash if we accidentally target a
  // non-app or non-profilable process.
  //
  // This does *not* get run for processes that statically link libc, and those will still crash.
  signal(BIONIC_SIGNAL_ART_PROFILER, SIG_IGN);
}

static void HandleSigsysSeccompOverride(int, siginfo_t*, void*);
static void HandleTracedPerfSignal();

static void HandleProfilingSignal(int /*signal_number*/, siginfo_t* info, void* /*ucontext*/) {
  ErrnoRestorer errno_restorer;

  if (info->si_code != SI_QUEUE) {
    return;
  }

  int signal_value = info->si_value.sival_int;
  async_safe_format_log(ANDROID_LOG_INFO, "libc", "%s: received profiling signal with si_value: %d",
                        getprogname(), signal_value);

  // Proceed only if the process is considered profileable.
  bool profileable = false;
  android_mallopt(M_GET_PROCESS_PROFILEABLE, &profileable, sizeof(profileable));
  if (!profileable) {
    async_safe_write_log(ANDROID_LOG_ERROR, "libc", "profiling signal rejected (not profileable)");
    return;
  }

  // Temporarily override SIGSYS handling, in a best-effort attempt at not
  // crashing if we happen to be running in a process with a seccomp filter that
  // disallows some of the syscalls done by this signal handler. This protects
  // against SECCOMP_RET_TRAP with a crashing SIGSYS handler (typical of android
  // minijails). Won't help if the filter is using SECCOMP_RET_KILL_*.
  // Note: the override is process-wide, but short-lived. The syscalls are still
  // blocked, but the overridden handler recovers from SIGSYS, and fakes the
  // syscall return value as ENOSYS.
  struct sigaction sigsys_override = {};
  sigsys_override.sa_sigaction = &HandleSigsysSeccompOverride;
  sigsys_override.sa_flags = SA_SIGINFO;

  struct sigaction old_act = {};
  sigaction(SIGSYS, &sigsys_override, &old_act);

  if (signal_value == kHeapprofdSignalValue) {
    HandleHeapprofdSignal();
  } else if (signal_value == kTracedPerfSignalValue) {
    HandleTracedPerfSignal();
  } else {
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", "unrecognized profiling signal si_value: %d",
                          signal_value);
  }
  sigaction(SIGSYS, &old_act, nullptr);
}

// Open /proc/self/{maps,mem}, connect to traced_perf, send the fds over the
// socket. Everything happens synchronously within the signal handler. Socket
// is made non-blocking, and we do not retry.
static void HandleTracedPerfSignal() {
  ScopedFd sock_fd{ socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0 /*protocol*/) };
  if (sock_fd.get() == -1) {
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", "failed to create socket: %s", strerror(errno));
    return;
  }

  sockaddr_un saddr{ AF_UNIX, "/dev/socket/traced_perf" };
  size_t addrlen = sizeof(sockaddr_un);
  if (connect(sock_fd.get(), reinterpret_cast<const struct sockaddr*>(&saddr), addrlen) == -1) {
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", "failed to connect to traced_perf socket: %s",
                          strerror(errno));
    return;
  }

  ScopedFd maps_fd{ open("/proc/self/maps", O_RDONLY | O_CLOEXEC) };
  if (maps_fd.get() == -1) {
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", "failed to open /proc/self/maps: %s",
                          strerror(errno));
    return;
  }
  ScopedFd mem_fd{ open("/proc/self/mem", O_RDONLY | O_CLOEXEC) };
  if (mem_fd.get() == -1) {
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", "failed to open /proc/self/mem: %s",
                          strerror(errno));
    return;
  }

  // Send 1 byte with auxiliary data carrying two fds.
  int send_fds[2] = { maps_fd.get(), mem_fd.get() };
  int num_fds = 2;
  char iobuf[1] = {};
  msghdr msg_hdr = {};
  iovec iov = { reinterpret_cast<void*>(iobuf), sizeof(iobuf) };
  msg_hdr.msg_iov = &iov;
  msg_hdr.msg_iovlen = 1;
  alignas(cmsghdr) char control_buf[256] = {};
  const auto raw_ctl_data_sz = num_fds * sizeof(int);
  const size_t control_buf_len = static_cast<size_t>(CMSG_SPACE(raw_ctl_data_sz));
  msg_hdr.msg_control = control_buf;
  msg_hdr.msg_controllen = control_buf_len;  // used by CMSG_FIRSTHDR
  struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg_hdr);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = static_cast<size_t>(CMSG_LEN(raw_ctl_data_sz));
  memcpy(CMSG_DATA(cmsg), send_fds, num_fds * sizeof(int));

  if (sendmsg(sock_fd.get(), &msg_hdr, 0) == -1) {
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", "failed to sendmsg: %s", strerror(errno));
  }
}

static void HandleSigsysSeccompOverride(int /*signal_number*/, siginfo_t* info,
                                        void* void_context) {
  ErrnoRestorer errno_restorer;
  if (info->si_code != SYS_SECCOMP) {
    return;
  }

  async_safe_format_log(
      ANDROID_LOG_WARN, "libc",
      "Profiling setup: trapped seccomp SIGSYS for syscall %d. Returning ENOSYS to caller.",
      info->si_syscall);

  // The handler is responsible for setting the return value as if the system
  // call happened (which is arch-specific). Use a plausible unsuccessful value.
  auto ret = -ENOSYS;
  ucontext_t* ctx = reinterpret_cast<ucontext_t*>(void_context);

#if defined(__arm__)
  ctx->uc_mcontext.arm_r0 = ret;
#elif defined(__aarch64__)
  ctx->uc_mcontext.regs[0] = ret;  // x0
#elif defined(__i386__)
  ctx->uc_mcontext.gregs[REG_EAX] = ret;
#elif defined(__x86_64__)
  ctx->uc_mcontext.gregs[REG_RAX] = ret;
#else
#error "unsupported architecture"
#endif
}
