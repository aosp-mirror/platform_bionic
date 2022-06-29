/*
 * Copyright (C) 2021 The Android Open Source Project
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

#include <gtest/gtest.h>
#include <stdio.h>
#include <sys/file.h>
#include <string>

#if defined(__BIONIC__)

#include "android-base/file.h"
#include "gwp_asan/options.h"
#include "platform/bionic/malloc.h"
#include "sys/system_properties.h"
#include "utils.h"

// basename is a mess, use gnu basename explicitly to avoid the need for string
// mutation.
extern "C" const char* __gnu_basename(const char* path);

// This file implements "torture testing" under GWP-ASan, where we sample every
// single allocation. The upper limit for the number of GWP-ASan allocations in
// the torture mode is is generally 40,000, so that svelte devices don't
// explode, as this uses ~163MiB RAM (4KiB per live allocation).
TEST(gwp_asan_integration, malloc_tests_under_torture) {
  RunGwpAsanTest("malloc.*:-malloc.mallinfo*");
}

class SyspropRestorer {
 private:
  std::vector<std::pair<std::string, std::string>> props_to_restore_;
  // System properties are global for a device, so the tests that mutate the
  // GWP-ASan system properties must be run mutually exclusive. Because
  // bionic-unit-tests is run in an isolated gtest fashion (each test is run in
  // its own process), we have to use flocks to synchronise between tests.
  int flock_fd_;

 public:
  SyspropRestorer() {
    std::string path = testing::internal::GetArgvs()[0];
    flock_fd_ = open(path.c_str(), O_RDONLY);
    EXPECT_NE(flock_fd_, -1) << "failed to open self for a flock";
    EXPECT_NE(flock(flock_fd_, LOCK_EX), -1) << "failed to flock myself";

    const char* basename = __gnu_basename(path.c_str());
    std::vector<std::string> props = {
        std::string("libc.debug.gwp_asan.sample_rate.") + basename,
        std::string("libc.debug.gwp_asan.process_sampling.") + basename,
        std::string("libc.debug.gwp_asan.max_allocs.") + basename,
        "libc.debug.gwp_asan.sample_rate.system_default",
        "libc.debug.gwp_asan.sample_rate.app_default",
        "libc.debug.gwp_asan.process_sampling.system_default",
        "libc.debug.gwp_asan.process_sampling.app_default",
        "libc.debug.gwp_asan.max_allocs.system_default",
        "libc.debug.gwp_asan.max_allocs.app_default",
    };

    size_t base_props_size = props.size();
    for (size_t i = 0; i < base_props_size; ++i) {
      props.push_back("persist." + props[i]);
    }

    std::string reset_log;

    for (const std::string& prop : props) {
      std::string value = GetSysprop(prop);
      props_to_restore_.emplace_back(prop, value);
      if (!value.empty()) {
        __system_property_set(prop.c_str(), "");
      }
    }
  }

  ~SyspropRestorer() {
    for (const auto& kv : props_to_restore_) {
      if (kv.second != GetSysprop(kv.first)) {
        __system_property_set(kv.first.c_str(), kv.second.c_str());
      }
    }
    close(flock_fd_);
  }

  static std::string GetSysprop(const std::string& name) {
    std::string value;
    const prop_info* pi = __system_property_find(name.c_str());
    if (pi == nullptr) return value;
    __system_property_read_callback(
        pi,
        [](void* cookie, const char* /* name */, const char* value, uint32_t /* serial */) {
          std::string* v = static_cast<std::string*>(cookie);
          *v = value;
        },
        &value);
    return value;
  }
};

TEST(gwp_asan_integration, DISABLED_assert_gwp_asan_enabled) {
  std::string maps;
  EXPECT_TRUE(android::base::ReadFileToString("/proc/self/maps", &maps));
  EXPECT_TRUE(maps.find("GWP-ASan") != std::string::npos) << maps;

  volatile int* x = new int;
  delete x;
  EXPECT_DEATH({ *x = 7; }, "");
}

TEST(gwp_asan_integration, DISABLED_assert_gwp_asan_disabled) {
  std::string maps;
  EXPECT_TRUE(android::base::ReadFileToString("/proc/self/maps", &maps));
  EXPECT_TRUE(maps.find("GWP-ASan") == std::string::npos);
}

TEST(gwp_asan_integration, sysprops_program_specific) {
  SyspropRestorer restorer;

  std::string path = testing::internal::GetArgvs()[0];
  const char* basename = __gnu_basename(path.c_str());
  __system_property_set((std::string("libc.debug.gwp_asan.sample_rate.") + basename).c_str(), "1");
  __system_property_set((std::string("libc.debug.gwp_asan.process_sampling.") + basename).c_str(),
                        "1");
  __system_property_set((std::string("libc.debug.gwp_asan.max_allocs.") + basename).c_str(),
                        "40000");

  RunSubtestNoEnv("gwp_asan_integration.DISABLED_assert_gwp_asan_enabled");
}

TEST(gwp_asan_integration, sysprops_persist_program_specific) {
  SyspropRestorer restorer;

  std::string path = testing::internal::GetArgvs()[0];
  const char* basename = __gnu_basename(path.c_str());
  __system_property_set(
      (std::string("persist.libc.debug.gwp_asan.sample_rate.") + basename).c_str(), "1");
  __system_property_set(
      (std::string("persist.libc.debug.gwp_asan.process_sampling.") + basename).c_str(), "1");
  __system_property_set((std::string("persist.libc.debug.gwp_asan.max_allocs.") + basename).c_str(),
                        "40000");

  RunSubtestNoEnv("gwp_asan_integration.DISABLED_assert_gwp_asan_enabled");
}

TEST(gwp_asan_integration, sysprops_system) {
  SyspropRestorer restorer;

  __system_property_set("libc.debug.gwp_asan.sample_rate.system_default", "1");
  __system_property_set("libc.debug.gwp_asan.process_sampling.system_default", "1");
  __system_property_set("libc.debug.gwp_asan.max_allocs.system_default", "40000");

  RunSubtestNoEnv("gwp_asan_integration.DISABLED_assert_gwp_asan_enabled");
}

TEST(gwp_asan_integration, sysprops_persist_system) {
  SyspropRestorer restorer;

  __system_property_set("persist.libc.debug.gwp_asan.sample_rate.system_default", "1");
  __system_property_set("persist.libc.debug.gwp_asan.process_sampling.system_default", "1");
  __system_property_set("persist.libc.debug.gwp_asan.max_allocs.system_default", "40000");

  RunSubtestNoEnv("gwp_asan_integration.DISABLED_assert_gwp_asan_enabled");
}

TEST(gwp_asan_integration, sysprops_non_persist_overrides_persist) {
  SyspropRestorer restorer;

  __system_property_set("libc.debug.gwp_asan.sample_rate.system_default", "1");
  __system_property_set("libc.debug.gwp_asan.process_sampling.system_default", "1");
  __system_property_set("libc.debug.gwp_asan.max_allocs.system_default", "40000");

  __system_property_set("persist.libc.debug.gwp_asan.sample_rate.system_default", "0");
  __system_property_set("persist.libc.debug.gwp_asan.process_sampling.system_default", "0");
  __system_property_set("persist.libc.debug.gwp_asan.max_allocs.system_default", "0");

  RunSubtestNoEnv("gwp_asan_integration.DISABLED_assert_gwp_asan_enabled");
}

TEST(gwp_asan_integration, sysprops_program_specific_overrides_default) {
  SyspropRestorer restorer;

  std::string path = testing::internal::GetArgvs()[0];
  const char* basename = __gnu_basename(path.c_str());
  __system_property_set(
      (std::string("persist.libc.debug.gwp_asan.sample_rate.") + basename).c_str(), "1");
  __system_property_set(
      (std::string("persist.libc.debug.gwp_asan.process_sampling.") + basename).c_str(), "1");
  __system_property_set((std::string("persist.libc.debug.gwp_asan.max_allocs.") + basename).c_str(),
                        "40000");

  __system_property_set("libc.debug.gwp_asan.sample_rate.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.process_sampling.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.max_allocs.system_default", "0");

  RunSubtestNoEnv("gwp_asan_integration.DISABLED_assert_gwp_asan_enabled");
}

TEST(gwp_asan_integration, sysprops_can_disable) {
  SyspropRestorer restorer;

  __system_property_set("libc.debug.gwp_asan.sample_rate.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.process_sampling.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.max_allocs.system_default", "0");

  RunSubtestNoEnv("gwp_asan_integration.DISABLED_assert_gwp_asan_disabled");
}

TEST(gwp_asan_integration, env_overrides_sysprop) {
  SyspropRestorer restorer;

  __system_property_set("libc.debug.gwp_asan.sample_rate.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.process_sampling.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.max_allocs.system_default", "0");

  RunGwpAsanTest("gwp_asan_integration.DISABLED_assert_gwp_asan_enabled");
}

#endif  // defined(__BIONIC__)
