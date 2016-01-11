/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <net/if.h>

#include <errno.h>

#include <gtest/gtest.h>

TEST(net_if, if_nametoindex_if_indextoname) {
  unsigned index;
  index = if_nametoindex("lo");
  ASSERT_NE(index, 0U);

  char buf[IF_NAMESIZE] = {};
  char* name = if_indextoname(index, buf);
  ASSERT_STREQ("lo", name);
}

TEST(net_if, if_nametoindex_fail) {
  unsigned index = if_nametoindex("this-interface-does-not-exist");
  ASSERT_EQ(0U, index);
}
