/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <sys/resource.h>

#if defined(__GLIBC__)
/* The host glibc we're currently building with doesn't have prlimit64 yet. */
static int prlimit64(pid_t, int resource, const struct rlimit64* new_limit, struct rlimit64* old_limit) {
  if (new_limit != NULL) {
    return setrlimit64(resource, new_limit);
  } else {
    return getrlimit64(resource, old_limit);
  }
}
#endif

TEST(sys_resource, smoke) {
#if defined(__LP64__) || defined(__GLIBC__)
  ASSERT_EQ(sizeof(rlimit), sizeof(rlimit64));
  ASSERT_EQ(8U, sizeof(rlim_t));
#else
  ASSERT_NE(sizeof(rlimit), sizeof(rlimit64));
  ASSERT_EQ(4U, sizeof(rlim_t));
#endif

  // Read with getrlimit, getrlimit64, and prlimit64.
  // (prlimit is prlimit64 on LP64 and unimplemented on 32-bit.)
  rlimit l32;
  rlimit64 l64;
  rlimit64 pr_l64;
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(l64.rlim_cur, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);
  ASSERT_EQ(l64.rlim_max, pr_l64.rlim_max);
  if (l64.rlim_max == RLIM64_INFINITY) {
    ASSERT_EQ(RLIM_INFINITY, l32.rlim_max);
  } else {
    ASSERT_EQ(l64.rlim_max, l32.rlim_max);
  }

  // Write with setrlimit and read back with everything.
  l32.rlim_cur = 123;
  ASSERT_EQ(0, setrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(123U, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);

  // Write with setrlimit64 and read back with everything.
  l64.rlim_cur = 456;
  ASSERT_EQ(0, setrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(456U, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);

  // Write with prlimit64 and read back with everything.
  l64.rlim_cur = 789;
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, &l64, NULL));
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(789U, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);
}
