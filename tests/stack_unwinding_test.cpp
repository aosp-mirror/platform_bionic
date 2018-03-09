/*
 * Copyright (C) 2013 The Android Open Source Project
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

/*
 * Contributed by: Intel Corporation
 */

#include <gtest/gtest.h>

#include <dlfcn.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <unwind.h>

#include "SignalUtils.h"

#define noinline __attribute__((__noinline__))
#define __unused __attribute__((__unused__))

_Unwind_Reason_Code FrameCounter(_Unwind_Context* ctx __unused, void* arg) {
  int* count_ptr = reinterpret_cast<int*>(arg);

#if SHOW_FRAME_LOCATIONS
  void* ip = reinterpret_cast<void*>(_Unwind_GetIP(ctx));

  const char* symbol = "<unknown>";
  int offset = 0;

  Dl_info info;
  memset(&info, 0, sizeof(info));
  if (dladdr(ip, &info) != 0) {
    symbol = info.dli_sname;
    if (info.dli_saddr != nullptr) {
      offset = static_cast<int>(reinterpret_cast<char*>(ip) - reinterpret_cast<char*>(info.dli_saddr));
    }
  }

  fprintf(stderr, " #%02d %p %s%+d (%s)\n", *count_ptr, ip, symbol, offset, info.dli_fname ? info.dli_fname : "??");
  fflush(stderr);
#endif

  ++*count_ptr;
  return _URC_NO_REASON;
}

static int noinline unwind_one_frame_deeper() {
  int count = 0;
  _Unwind_Backtrace(FrameCounter, &count);
  return count;
}

TEST(stack_unwinding, easy) {
  int count = 0;
  _Unwind_Backtrace(FrameCounter, &count);
  int deeper_count = unwind_one_frame_deeper();
  ASSERT_EQ(count + 1, deeper_count);
}

struct UnwindData {
  volatile bool signal_handler_complete = false;
  int expected_frame_count = 0;
  int handler_frame_count = 0;
  int handler_one_deeper_frame_count = 0;
};

static UnwindData g_unwind_data;

static void noinline UnwindSignalHandler(int) {
  _Unwind_Backtrace(FrameCounter, &g_unwind_data.handler_frame_count);

  g_unwind_data.handler_one_deeper_frame_count = unwind_one_frame_deeper();
  g_unwind_data.signal_handler_complete = true;
}

static void verify_unwind_data(const UnwindData& unwind_data) {
  // In order to avoid a false positive, the caller must have at least 2 frames
  // outside of the signal handler. This avoids a case where the only frame
  // right after the signal handler winds up being garbage.
  EXPECT_GT(unwind_data.handler_frame_count, unwind_data.expected_frame_count + 1);

  EXPECT_EQ(unwind_data.handler_frame_count + 1, unwind_data.handler_one_deeper_frame_count);
}

static void noinline UnwindTest() {
  g_unwind_data = {};

  _Unwind_Backtrace(FrameCounter, &g_unwind_data.expected_frame_count);
  ASSERT_LE(2, g_unwind_data.expected_frame_count)
      << "The current call must contain at least 2 frames for the test to be valid.";

  ASSERT_EQ(0, kill(getpid(), SIGUSR1));
  while (!g_unwind_data.signal_handler_complete) {}

  verify_unwind_data(g_unwind_data);
}

TEST(stack_unwinding, unwind_through_signal_frame) {
  ScopedSignalHandler ssh(SIGUSR1, UnwindSignalHandler);

  UnwindTest();
}

// On LP32, the SA_SIGINFO flag gets you __restore_rt instead of __restore.
TEST(stack_unwinding, unwind_through_signal_frame_SA_SIGINFO) {
  ScopedSignalHandler ssh(SIGUSR1, UnwindSignalHandler, SA_SIGINFO);

  UnwindTest();
}
