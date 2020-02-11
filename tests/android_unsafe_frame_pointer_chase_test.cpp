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

static void *BacktraceThread(void *) {
  size_t size = recurse(kNumFrames, 0, 0);

  uintptr_t frames[kNumFrames + 2];
  size_t size2 = recurse(kNumFrames, frames, kNumFrames + 2);
  if (size2 != size) {
    return (void*)"size2 != size";
  }

  if (!CheckFrames(frames, size)) {
    return (void*)"CheckFrames failed";
  }
  return nullptr;
}

TEST(android_unsafe_frame_pointer_chase, pthread) {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, BacktraceThread, nullptr));
  void* retval;
  ASSERT_EQ(0, pthread_join(t, &retval));
  EXPECT_EQ(nullptr, reinterpret_cast<char*>(retval));
}

#endif // __BIONIC__
