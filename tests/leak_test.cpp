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

#include <err.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <vector>

#include <android-base/macros.h>

#include "utils.h"

using namespace std::chrono_literals;

static size_t GetMappingSize() {
  std::vector<map_record> maps;
  if (!Maps::parse_maps(&maps)) {
    err(1, "failed to parse maps");
  }

  size_t result = 0;
  for (const map_record& map : maps) {
    result += map.addr_end - map.addr_start;
  }

  return result;
}

#define LEAK_TEST(test_case_name, test_name)                                                 \
  static void __leak_test__##test_case_name##__##test_name();                                \
  TEST(test_case_name, test_name) {                                                          \
    auto previous_size = GetMappingSize();                                                   \
    __leak_test__##test_case_name##__##test_name();                                          \
    auto current_size = GetMappingSize();                                                    \
    if (current_size > previous_size) {                                                      \
      FAIL() << "increase in process map size: " << previous_size << " -> " << current_size; \
    }                                                                                        \
  }                                                                                          \
  static void __leak_test__##test_case_name##__##test_name()

LEAK_TEST(leak, smoke) {
  // Do nothing.
}

LEAK_TEST(leak, xfail) {
  UNUSED(mmap(nullptr, PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

// http://b/36045112
LEAK_TEST(pthread_leak, join) {
  for (int i = 0; i < 100; ++i) {
    pthread_t thread;
    ASSERT_EQ(0, pthread_create(&thread, nullptr, [](void*) -> void* { return nullptr; }, nullptr));
    ASSERT_EQ(0, pthread_join(thread, nullptr));
  }
}

// http://b/36045112
LEAK_TEST(pthread_leak, detach) {
  pthread_barrier_t barrier;
  constexpr int thread_count = 100;
  ASSERT_EQ(0, pthread_barrier_init(&barrier, nullptr, thread_count + 1));
  for (int i = 0; i < thread_count; ++i) {
    pthread_t thread;
    const auto thread_function = +[](void* barrier) -> void* {
      pthread_barrier_wait(static_cast<pthread_barrier_t*>(barrier));
      return nullptr;
    };
    ASSERT_EQ(0, pthread_create(&thread, nullptr, thread_function, &barrier));
    ASSERT_EQ(0, pthread_detach(thread));
  }

  pthread_barrier_wait(&barrier);

  // Give the threads some time to exit.
  std::this_thread::sleep_for(100ms);
}
