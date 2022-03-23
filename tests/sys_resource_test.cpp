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

#include <sys/resource.h>

#include <gtest/gtest.h>

TEST(sys_resource, rlimit_struct_size) {
#if defined(__LP64__) || defined(__GLIBC__)
  ASSERT_EQ(sizeof(rlimit), sizeof(rlimit64));
  ASSERT_EQ(8U, sizeof(rlim_t));
#else
  ASSERT_NE(sizeof(rlimit), sizeof(rlimit64));
  ASSERT_EQ(4U, sizeof(rlim_t));
#endif
}

class SysResourceTest : public ::testing::Test {
 protected:
  void SetUp() override {
    ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32_));
    ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64_));
    ASSERT_EQ(0, prlimit(0, RLIMIT_CORE, nullptr, &pr_l32_));
    ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, nullptr, &pr_l64_));
  }

  void CheckResourceLimits();

 protected:
  rlimit l32_;
  rlimit64 l64_;
  rlimit pr_l32_;
  rlimit64 pr_l64_;
};

void SysResourceTest::CheckResourceLimits() {
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32_));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64_));
  ASSERT_EQ(0, prlimit(0, RLIMIT_CORE, nullptr, &pr_l32_));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, nullptr, &pr_l64_));

  ASSERT_EQ(l32_.rlim_cur, pr_l32_.rlim_cur);
  ASSERT_EQ(l64_.rlim_cur, pr_l64_.rlim_cur);

  if (l64_.rlim_cur == RLIM64_INFINITY) {
    ASSERT_EQ(RLIM_INFINITY, l32_.rlim_cur);
  } else {
    ASSERT_EQ(l64_.rlim_cur, l32_.rlim_cur);
  }

  ASSERT_EQ(l32_.rlim_max, pr_l32_.rlim_max);
  ASSERT_EQ(l64_.rlim_max, pr_l64_.rlim_max);

  if (l64_.rlim_max == RLIM64_INFINITY) {
    ASSERT_EQ(RLIM_INFINITY, l32_.rlim_max);
  } else {
    ASSERT_EQ(l64_.rlim_max, l32_.rlim_max);
  }
}

// Force rlim_max to be bigger than a constant so we can continue following test.
// Change resource limit setting with "ulimit -Hc" in the shell if this test fails.
TEST_F(SysResourceTest, RLIMIT_CORE_rlim_max_not_zero) {
  ASSERT_TRUE(l32_.rlim_max == RLIM_INFINITY || l32_.rlim_max >= 456U) <<
    "RLIMIT_CORE rlim_max = " << l32_.rlim_max;
}

TEST_F(SysResourceTest, get_resource_limit_equal) {
  CheckResourceLimits();
}

TEST_F(SysResourceTest, setrlimit) {
  l32_.rlim_cur = 123U;
  ASSERT_EQ(0, setrlimit(RLIMIT_CORE, &l32_));
  CheckResourceLimits();
  ASSERT_EQ(123U, l32_.rlim_cur);
}

TEST_F(SysResourceTest, setrlimit64) {
  l64_.rlim_cur = 456U;
  ASSERT_EQ(0, setrlimit64(RLIMIT_CORE, &l64_));
  CheckResourceLimits();
  ASSERT_EQ(456U, l64_.rlim_cur);
}

TEST_F(SysResourceTest, prlimit) {
  pr_l32_.rlim_cur = pr_l32_.rlim_max;
  ASSERT_EQ(0, prlimit(0, RLIMIT_CORE, &pr_l32_, nullptr));
  CheckResourceLimits();
  ASSERT_EQ(pr_l32_.rlim_max, pr_l32_.rlim_cur);
}

TEST_F(SysResourceTest, prlimit64) {
  pr_l64_.rlim_cur = pr_l64_.rlim_max;
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, &pr_l64_, nullptr));
  CheckResourceLimits();
  ASSERT_EQ(pr_l64_.rlim_max, pr_l64_.rlim_cur);
}
