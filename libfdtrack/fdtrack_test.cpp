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

#include <dlfcn.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <map>
#include <vector>

#include <gtest/gtest.h>

struct FdtrackFrame {
  const char* function_name;
  uint64_t function_offset;
};

std::map<int, std::vector<FdtrackFrame>> RunFdtrack(std::function<void()> f) {
  void* libfdtrack = dlopen("libfdtrack.so", RTLD_NOW);
  if (!libfdtrack) {
    errx(1, "failed to dlopen libfdtrack.so: %s", dlerror());
  }

  using fdtrack_callback_t = bool (*)(int fd, const char* const* function_names,
                                      const uint64_t* function_offsets, size_t count, void* arg);
  auto fdtrack_iterate = reinterpret_cast<void (*)(fdtrack_callback_t, void* arg)>(
      dlsym(libfdtrack, "fdtrack_iterate"));
  if (!fdtrack_iterate) {
    errx(1, "failed to dlsym fdtrack_iterate");
  }

  f();

  std::map<int, std::vector<FdtrackFrame>> result;
  fdtrack_iterate(
      [](int fd, const char* const* function_names, const uint64_t* function_offsets, size_t count,
         void* arg) {
        auto& map = *static_cast<decltype(result)*>(arg);
        for (size_t i = 0; i < count; ++i) {
          map[fd].push_back(FdtrackFrame{
              .function_name = function_names[i],
              .function_offset = function_offsets[i],
          });
        }

        return true;
      },
      &result);

  return result;
}

TEST(fdtrack, open) {
  static int fd = -1;
  auto result = RunFdtrack([]() { fd = open("/dev/null", O_RDONLY | O_CLOEXEC); });

  ASSERT_NE(-1, fd);
  ASSERT_EQ(1, result.size());
  ASSERT_EQ(fd, result.begin()->first);
  ASSERT_NE(nullptr, strstr(result.begin()->second.at(0).function_name, "open"));
}

TEST(fdtrack, close) {
  static int fd1 = -1;
  static int fd2 = -1;
  static int fd3 = -1;
  auto result = RunFdtrack([]() {
    fd1 = open("/dev/null", O_RDONLY | O_CLOEXEC);
    fd2 = open("/dev/null", O_RDONLY | O_CLOEXEC);
    fd3 = open("/dev/null", O_RDONLY | O_CLOEXEC);
    close(fd2);
  });

  ASSERT_NE(-1, fd1);
  ASSERT_NE(-1, fd2);
  ASSERT_NE(-1, fd3);

  ASSERT_EQ(2, result.size());
  ASSERT_EQ(1, result.count(fd1));
  ASSERT_EQ(1, result.count(fd3));

  ASSERT_NE(nullptr, strstr(result[fd1].at(0).function_name, "open"));
  ASSERT_NE(nullptr, strstr(result[fd3].at(0).function_name, "open"));
}
