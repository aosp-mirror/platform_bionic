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

#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>  // for gettid

// Helper binary to use TLS-related functions in thread_properties

// Tests __get_static_tls_bound.
void test_static_tls_bounds() {
  void* start_addr;
  void* end_addr;

  __libc_get_static_tls_bounds(reinterpret_cast<void**>(&start_addr),
                               reinterpret_cast<void**>(&end_addr));
  printf("done_get_static_tls_bounds\n");
}

// Tests iterate_dynamic tls chunks.
// Export a var from the shared so.
__thread char large_tls_var[4 * 1024 * 1024];
void test_iter_tls() {
  void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);

  int i = 0;
  auto cb = [&](void* dtls_begin, void* dtls_end, size_t dso_id, void* arg) {
    printf("iterate_cb i = %d\n", i++);
  };
  __libc_iterate_dynamic_tls(gettid(), cb, nullptr);
  printf("done_iterate_dynamic_tls\n");
}

int main() {
  test_static_tls_bounds();
  test_iter_tls();
  return 0;
}

#else
int main() {
  return 0;
}
#endif  // __BIONIC__
