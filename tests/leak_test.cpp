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

static void WaitUntilAllExited(pid_t* pids, size_t pid_count) {
  // Wait until all children have exited.
  bool alive = true;
  while (alive) {
    alive = false;
    for (size_t i = 0; i < pid_count; ++i) {
      if (pids[i] != 0) {
        if (kill(pids[i], 0) == 0) {
          alive = true;
        } else {
          EXPECT_EQ(errno, ESRCH);
          pids[i] = 0;  // Skip in next loop.
        }
      }
    }
  }
}

class LeakChecker {
 public:
  LeakChecker() {
    Reset();
  }

  ~LeakChecker() {
    Check();
  }

  void Reset() {
    previous_size_ = GetMappingSize();
  }

  void DumpTo(std::ostream& os) const {
    os << previous_size_;
  }

 private:
  size_t previous_size_;

  void Check() {
    auto current_size = GetMappingSize();
    if (current_size > previous_size_) {
      FAIL() << "increase in process map size: " << previous_size_ << " -> " << current_size;
    }
  }
};

std::ostream& operator<<(std::ostream& os, const LeakChecker& lc) {
  lc.DumpTo(os);
  return os;
}

// http://b/36045112
TEST(pthread_leak, join) {
  LeakChecker lc;
  for (int i = 0; i < 100; ++i) {
    pthread_t thread;
    ASSERT_EQ(0, pthread_create(&thread, nullptr, [](void*) -> void* { return nullptr; }, nullptr));
    ASSERT_EQ(0, pthread_join(thread, nullptr));
  }
}

// http://b/36045112
TEST(pthread_leak, detach) {
  LeakChecker lc;

  for (size_t pass = 0; pass < 2; ++pass) {
    pthread_barrier_t barrier;
    constexpr int thread_count = 100;
    ASSERT_EQ(pthread_barrier_init(&barrier, nullptr, thread_count + 1), 0);

    // Start child threads.
    struct thread_data { pthread_barrier_t* barrier; pid_t* tid; };
    pid_t tids[thread_count];
    for (int i = 0; i < thread_count; ++i) {
      thread_data* td = new thread_data{&barrier, &tids[i]};
      const auto thread_function = +[](void* ptr) -> void* {
        thread_data* data = static_cast<thread_data*>(ptr);
        *data->tid = gettid();
        pthread_barrier_wait(data->barrier);
        // Doing this delete allocates new VMAs for jemalloc bookkeeping,
        // but the two-pass nature of this test means we can check that
        // it's a pool rather than an unbounded leak.
        delete data;
        return nullptr;
      };
      pthread_t thread;
      ASSERT_EQ(0, pthread_create(&thread, nullptr, thread_function, td));
      ASSERT_EQ(0, pthread_detach(thread));
    }

    pthread_barrier_wait(&barrier);
    ASSERT_EQ(pthread_barrier_destroy(&barrier), 0);

    WaitUntilAllExited(tids, arraysize(tids));

    // houdini keeps a thread pool, so we ignore the first pass while the
    // pool fills, but then on the second pass require that the "pool" isn't
    // actually an unbounded leak. https://issuetracker.google.com/37920774.
    if (pass == 0) lc.Reset();
  }
}
