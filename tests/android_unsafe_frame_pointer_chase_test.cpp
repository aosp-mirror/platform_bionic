/*
 * Copyright (C) 2020 The Android Open Source Project
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

#if defined(__BIONIC__)

#include <sys/mman.h>

#include "platform/bionic/android_unsafe_frame_pointer_chase.h"

// Prevent tail calls inside recurse.
__attribute__((weak, noinline)) size_t nop(size_t val) {
  return val;
}

// Call android_unsafe_frame_pointer_chase inside count recurse stack frames.
__attribute__((weak, noinline)) int recurse(int count, uintptr_t* buf, size_t num_entries) {
  if (count != 0) return nop(recurse(count - 1, buf, num_entries));
  return nop(android_unsafe_frame_pointer_chase(buf, num_entries));
}

static constexpr size_t kNumFrames = 32;

static bool CheckFrames(uintptr_t* frames, size_t num_frames) {
  // We expect one recurse frame calling android_unsafe_frame_pointer_chase, followed by kNumFrames identical
  // recurse frames calling themselves, followed by at least one frame (the first caller of
  // recurse).
  if (num_frames < kNumFrames + 2) {
    printf("num_frames (0x%zu) < kNumFrames + 2", num_frames);
    return false;
  }

  if (frames[0] == frames[1]) {
    printf("frames[0] == frames[1] (0x%zx)", frames[0]);
    return false;
  }

  for (size_t i = 2; i <= kNumFrames; ++i) {
    if (frames[i] != frames[1]) {
      printf("frames[i] (0x%zx) != frames[1] (0x%zx)", frames[i], frames[1]);
      return false;
    }
  }

  if (frames[kNumFrames] == frames[kNumFrames + 1]) {
    printf("frames[kNumFrames] == frames[kNumFrames + 1] (0x%zx)", frames[kNumFrames]);
    return false;
  }

  return true;
}

TEST(android_unsafe_frame_pointer_chase, main_thread) {
  size_t size = recurse(kNumFrames, 0, 0);

  uintptr_t frames[kNumFrames + 2];
  size_t size2 = recurse(kNumFrames, frames, kNumFrames + 2);
  EXPECT_EQ(size2, size);

  EXPECT_TRUE(CheckFrames(frames, size));
}

static const char* tester_func() {
  size_t size = recurse(kNumFrames, 0, 0);

  uintptr_t frames[kNumFrames + 2];
  size_t size2 = recurse(kNumFrames, frames, kNumFrames + 2);
  if (size2 != size) {
    return "size2 != size";
  }

  if (!CheckFrames(frames, size)) {
    return "CheckFrames failed";
  }
  return nullptr;
}

static void* BacktraceThread(void*) {
  return (void*)tester_func();
}

TEST(android_unsafe_frame_pointer_chase, pthread) {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, BacktraceThread, nullptr));
  void* retval;
  ASSERT_EQ(0, pthread_join(t, &retval));
  EXPECT_EQ(nullptr, reinterpret_cast<char*>(retval));
}

static bool g_handler_called;
static const char* g_handler_tester_result;

static void BacktraceHandler(int) {
  g_handler_called = true;
  g_handler_tester_result = tester_func();
}

static constexpr size_t kStackSize = 16384;

static void* SignalBacktraceThread(void* sp) {
  stack_t ss;
  ss.ss_sp = sp;
  ss.ss_flags = 0;
  ss.ss_size = kStackSize;
  sigaltstack(&ss, nullptr);

  struct sigaction s = {};
  s.sa_handler = BacktraceHandler;
  s.sa_flags = SA_ONSTACK;
  sigaction(SIGRTMIN, &s, nullptr);

  raise(SIGRTMIN);
  return nullptr;
}

TEST(android_unsafe_frame_pointer_chase, sigaltstack) {
  // Create threads where the alternate stack appears both after and before the regular stack, and
  // call android_unsafe_frame_pointer_chase from a signal handler. Without handling for the
  // alternate signal stack, this would cause false negatives or potential false positives in the
  // android_unsafe_frame_pointer_chase function.
  void* stacks =
      mmap(nullptr, kStackSize * 2, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  for (unsigned i = 0; i != 2; ++i) {
    pthread_t t;
    pthread_attr_t attr;
    ASSERT_EQ(0, pthread_attr_init(&attr));
    ASSERT_EQ(0, pthread_attr_setstack(&attr, reinterpret_cast<char*>(stacks) + kStackSize * i,
                                       kStackSize));

    ASSERT_EQ(0, pthread_create(&t, &attr, SignalBacktraceThread,
                                reinterpret_cast<char*>(stacks) + kStackSize * (1 - i)));
    void* retval;
    ASSERT_EQ(0, pthread_join(t, &retval));

    EXPECT_TRUE(g_handler_called);
    EXPECT_EQ(nullptr, g_handler_tester_result);
    g_handler_called = false;
  }

  munmap(stacks, kStackSize * 2);
}

#endif // __BIONIC__
