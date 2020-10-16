/*
 * Copyright (C) 2019 The Android Open Source Project
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

// Prevent tests from being compiled with glibc because thread_properties.h
// only exists in Bionic.
#if defined(__BIONIC__)

#include <sys/thread_properties.h>

#include <assert.h>
#include <dlfcn.h>
#include <elf.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

// Helper binary to use TLS-related functions in thread_properties

// Tests __get_static_tls_bound.
thread_local int local_var;
void test_static_tls_bounds() {
  local_var = 123;
  void* start_addr = nullptr;
  void* end_addr = nullptr;

  __libc_get_static_tls_bounds(reinterpret_cast<void**>(&start_addr),
                               reinterpret_cast<void**>(&end_addr));
  assert(start_addr != nullptr);
  assert(end_addr != nullptr);

  assert(&local_var >= start_addr && &local_var < end_addr);

  printf("done_get_static_tls_bounds\n");
}

// Tests iterate_dynamic tls chunks.
// Export a var from the shared so.
__thread char large_tls_var[4 * 1024 * 1024];
// found_count  has to be Global variable so that the non-capturing lambda
// can access it.
int found_count = 0;
void test_iter_tls() {
  void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);
  large_tls_var[1025] = 'a';
  auto cb = +[](void* dtls_begin, void* dtls_end, size_t dso_id, void* arg) {
    if (&large_tls_var >= dtls_begin && &large_tls_var < dtls_end) ++found_count;
  };
  __libc_iterate_dynamic_tls(gettid(), cb, nullptr);

  // It should be found exactly once.
  assert(found_count == 1);
  printf("done_iterate_dynamic_tls\n");
}

void* parent_addr = nullptr;
void test_iterate_another_thread_tls() {
  large_tls_var[1025] = 'b';
  parent_addr = &large_tls_var;
  found_count = 0;

  pid_t pid = fork();
  assert(pid != -1);
  int status;
  if (pid) {
    // Parent.
    assert(pid == wait(&status));
    assert(0 == status);
  } else {
    // Child.
    pid_t parent_pid = getppid();
    assert(0 == ptrace(PTRACE_ATTACH, parent_pid));
    assert(parent_pid == waitpid(parent_pid, &status, 0));

    auto cb = +[](void* dtls_begin, void* dtls_end, size_t dso_id, void* arg) {
      if (parent_addr >= dtls_begin && parent_addr < dtls_end) ++found_count;
    };
    __libc_iterate_dynamic_tls(parent_pid, cb, nullptr);
    // It should be found exactly once.
    assert(found_count == 1);
    printf("done_iterate_another_thread_tls\n");
  }
}
int main() {
  test_static_tls_bounds();
  test_iter_tls();
  test_iterate_another_thread_tls();
  return 0;
}

#else
int main() {
  return 0;
}
#endif  // __BIONIC__
