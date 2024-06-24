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

#include <stdio.h>

#if defined(__BIONIC__)
#include "bionic/pthread_internal.h"

// Ensure that the layout of these data structures is architecture independent and only depends on
// the bitness of the architecture.
template <typename CheckSize, typename CheckOffset>
void tests(CheckSize check_size, CheckOffset check_offset) {
#define CHECK_SIZE(name, size) \
    check_size(#name, sizeof(name), size);
#define CHECK_OFFSET(name, field, offset) \
    check_offset(#name, #field, offsetof(name, field), offset);
#ifdef __LP64__
  CHECK_SIZE(pthread_internal_t, 776);
  CHECK_OFFSET(pthread_internal_t, next, 0);
  CHECK_OFFSET(pthread_internal_t, prev, 8);
  CHECK_OFFSET(pthread_internal_t, tid, 16);
  CHECK_OFFSET(pthread_internal_t, attr, 24);
  CHECK_OFFSET(pthread_internal_t, join_state, 80);
  CHECK_OFFSET(pthread_internal_t, cleanup_stack, 88);
  CHECK_OFFSET(pthread_internal_t, start_routine, 96);
  CHECK_OFFSET(pthread_internal_t, start_routine_arg, 104);
  CHECK_OFFSET(pthread_internal_t, return_value, 112);
  CHECK_OFFSET(pthread_internal_t, start_mask, 120);
  CHECK_OFFSET(pthread_internal_t, alternate_signal_stack, 128);
  CHECK_OFFSET(pthread_internal_t, shadow_call_stack_guard_region, 136);
  CHECK_OFFSET(pthread_internal_t, stack_top, 144);
  CHECK_OFFSET(pthread_internal_t, startup_handshake_lock, 156);
  CHECK_OFFSET(pthread_internal_t, mmap_base, 168);
  CHECK_OFFSET(pthread_internal_t, mmap_size, 176);
  CHECK_OFFSET(pthread_internal_t, mmap_base_unguarded, 184);
  CHECK_OFFSET(pthread_internal_t, mmap_size_unguarded, 192);
  CHECK_OFFSET(pthread_internal_t, vma_name_buffer, 200);
  CHECK_OFFSET(pthread_internal_t, thread_local_dtors, 232);
  CHECK_OFFSET(pthread_internal_t, current_dlerror, 240);
  CHECK_OFFSET(pthread_internal_t, dlerror_buffer, 248);
  CHECK_OFFSET(pthread_internal_t, bionic_tls, 760);
  CHECK_OFFSET(pthread_internal_t, errno_value, 768);
  CHECK_SIZE(bionic_tls, 12200);
  CHECK_OFFSET(bionic_tls, key_data, 0);
  CHECK_OFFSET(bionic_tls, locale, 2080);
  CHECK_OFFSET(bionic_tls, basename_buf, 2088);
  CHECK_OFFSET(bionic_tls, dirname_buf, 6184);
  CHECK_OFFSET(bionic_tls, mntent_buf, 10280);
  CHECK_OFFSET(bionic_tls, mntent_strings, 10320);
  CHECK_OFFSET(bionic_tls, ptsname_buf, 11344);
  CHECK_OFFSET(bionic_tls, ttyname_buf, 11376);
  CHECK_OFFSET(bionic_tls, strerror_buf, 11440);
  CHECK_OFFSET(bionic_tls, strsignal_buf, 11695);
  CHECK_OFFSET(bionic_tls, group, 11952);
  CHECK_OFFSET(bionic_tls, passwd, 12040);
  CHECK_OFFSET(bionic_tls, fdtrack_disabled, 12192);
  CHECK_OFFSET(bionic_tls, bionic_systrace_disabled, 12193);
  CHECK_OFFSET(bionic_tls, padding, 12194);
#else
  CHECK_SIZE(pthread_internal_t, 668);
  CHECK_OFFSET(pthread_internal_t, next, 0);
  CHECK_OFFSET(pthread_internal_t, prev, 4);
  CHECK_OFFSET(pthread_internal_t, tid, 8);
  CHECK_OFFSET(pthread_internal_t, attr, 16);
  CHECK_OFFSET(pthread_internal_t, join_state, 40);
  CHECK_OFFSET(pthread_internal_t, cleanup_stack, 44);
  CHECK_OFFSET(pthread_internal_t, start_routine, 48);
  CHECK_OFFSET(pthread_internal_t, start_routine_arg, 52);
  CHECK_OFFSET(pthread_internal_t, return_value, 56);
  CHECK_OFFSET(pthread_internal_t, start_mask, 60);
  CHECK_OFFSET(pthread_internal_t, alternate_signal_stack, 68);
  CHECK_OFFSET(pthread_internal_t, shadow_call_stack_guard_region, 72);
  CHECK_OFFSET(pthread_internal_t, stack_top, 76);
  CHECK_OFFSET(pthread_internal_t, startup_handshake_lock, 84);
  CHECK_OFFSET(pthread_internal_t, mmap_base, 92);
  CHECK_OFFSET(pthread_internal_t, mmap_size, 96);
  CHECK_OFFSET(pthread_internal_t, mmap_base_unguarded, 100);
  CHECK_OFFSET(pthread_internal_t, mmap_size_unguarded, 104);
  CHECK_OFFSET(pthread_internal_t, vma_name_buffer, 108);
  CHECK_OFFSET(pthread_internal_t, thread_local_dtors, 140);
  CHECK_OFFSET(pthread_internal_t, current_dlerror, 144);
  CHECK_OFFSET(pthread_internal_t, dlerror_buffer, 148);
  CHECK_OFFSET(pthread_internal_t, bionic_tls, 660);
  CHECK_OFFSET(pthread_internal_t, errno_value, 664);
  CHECK_SIZE(bionic_tls, 11080);
  CHECK_OFFSET(bionic_tls, key_data, 0);
  CHECK_OFFSET(bionic_tls, locale, 1040);
  CHECK_OFFSET(bionic_tls, basename_buf, 1044);
  CHECK_OFFSET(bionic_tls, dirname_buf, 5140);
  CHECK_OFFSET(bionic_tls, mntent_buf, 9236);
  CHECK_OFFSET(bionic_tls, mntent_strings, 9260);
  CHECK_OFFSET(bionic_tls, ptsname_buf, 10284);
  CHECK_OFFSET(bionic_tls, ttyname_buf, 10316);
  CHECK_OFFSET(bionic_tls, strerror_buf, 10380);
  CHECK_OFFSET(bionic_tls, strsignal_buf, 10635);
  CHECK_OFFSET(bionic_tls, group, 10892);
  CHECK_OFFSET(bionic_tls, passwd, 10952);
  CHECK_OFFSET(bionic_tls, fdtrack_disabled, 11076);
  CHECK_OFFSET(bionic_tls, bionic_systrace_disabled, 11077);
  CHECK_OFFSET(bionic_tls, padding, 11078);
#endif  // __LP64__
#undef CHECK_SIZE
#undef CHECK_OFFSET
}
#endif  // defined(__BIONIC__)

TEST(struct_layout, sizes_offsets) {
#if defined(__BIONIC__)
  bool failed = false;

  auto check_size = [&](const char* name, size_t size, size_t expected_size) {
    EXPECT_EQ(expected_size, size) << "sizeof(" << name << ")";
    if (size != expected_size) {
      failed = true;
    }
  };
  auto check_offset = [&](const char* name, const char* field, size_t offset,
                          size_t expected_offset) {
    EXPECT_EQ(expected_offset, offset) << "offsetof(" << name << ", " << field << ")";
    if (offset != expected_offset) {
      failed = true;
    }
  };
  tests(check_size, check_offset);

  if (failed) {
    printf(
        "Please update the tests function in bionic/tests/struct_layout_test.cpp with the "
        "following contents:\n");

    auto print_size = [&](const char* name, size_t size, size_t expected_size) {
      (void)expected_size;
      printf("  CHECK_SIZE(%s, %zu);\n", name, size);
    };
    auto print_offset = [&](const char* name, const char* field, size_t offset,
                            size_t expected_offset) {
      (void)expected_offset;
      printf("  CHECK_OFFSET(%s, %s, %zu);\n", name, field, offset);
    };
    tests(print_size, print_offset);
  }
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}
