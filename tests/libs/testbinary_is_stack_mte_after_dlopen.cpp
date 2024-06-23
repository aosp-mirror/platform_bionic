/*
 * Copyright (C) 2024 The Android Open Source Project
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <condition_variable>
#include <thread>

#include <dlfcn.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mte_utils.h"
#include "CHECK.h"

#if defined(__BIONIC__) && defined(__aarch64__)

enum State { kInit, kThreadStarted, kStackRemapped };

// We can't use pthread_getattr_np because that uses the rlimit rather than the actual mapping
// bounds.
static void find_main_stack_limits(uintptr_t* low, uintptr_t* high) {
  uintptr_t startstack = reinterpret_cast<uintptr_t>(__builtin_frame_address(0));

  // Hunt for the region that contains that address.
  FILE* fp = fopen("/proc/self/maps", "re");
  if (fp == nullptr) {
    abort();
  }
  char line[BUFSIZ];
  while (fgets(line, sizeof(line), fp) != nullptr) {
    uintptr_t lo, hi;
    if (sscanf(line, "%" SCNxPTR "-%" SCNxPTR, &lo, &hi) == 2) {
      if (lo <= startstack && startstack <= hi) {
        *low = lo;
        *high = hi;
        fclose(fp);
        return;
      }
    }
  }
  abort();
}

template <typename Fn>
unsigned int fault_new_stack_page(uintptr_t low, Fn f) {
  uintptr_t new_low;
  uintptr_t new_high;
  volatile char buf[4096];
  buf[4095] = 1;
  find_main_stack_limits(&new_low, &new_high);
  if (new_low < low) {
    f();
    return new_high;
  }
  // Useless, but should defeat TCO.
  return new_low + fault_new_stack_page(low, f);
}
extern "C" int main(int argc, char** argv) {
  if (argc < 2) {
    return 1;
  }
  const char* path = argv[1];
  CHECK(access(path, F_OK) == 0);  // Verify test setup.
  CHECK(!is_stack_mte_on());
  std::mutex m;
  std::condition_variable cv;
  State state = kInit;

  bool is_early_thread_mte_on = false;
  std::thread early_th([&] {
    {
      std::lock_guard lk(m);
      state = kThreadStarted;
    }
    cv.notify_one();
    {
      std::unique_lock lk(m);
      cv.wait(lk, [&] { return state == kStackRemapped; });
    }
    is_early_thread_mte_on = is_stack_mte_on();
  });
  {
    std::unique_lock lk(m);
    cv.wait(lk, [&] { return state == kThreadStarted; });
  }
  void* handle = dlopen(path, RTLD_NOW);
  {
    std::lock_guard lk(m);
    state = kStackRemapped;
  }
  cv.notify_one();
  CHECK(handle != nullptr);
  CHECK(is_stack_mte_on());

  bool new_stack_page_mte_on = false;
  uintptr_t low;
  uintptr_t high;
  find_main_stack_limits(&low, &high);
  fault_new_stack_page(low, [&] { new_stack_page_mte_on = is_stack_mte_on(); });
  CHECK(new_stack_page_mte_on);

  bool is_late_thread_mte_on = false;
  std::thread late_th([&] { is_late_thread_mte_on = is_stack_mte_on(); });
  late_th.join();
  early_th.join();
  CHECK(is_late_thread_mte_on);
  CHECK(is_early_thread_mte_on);
  printf("RAN\n");
  return 0;
}

#else
extern "C" int main(int, char**) {
  return 1;
}
#endif
