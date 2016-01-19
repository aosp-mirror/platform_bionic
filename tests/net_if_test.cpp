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
#include <ifaddrs.h>

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

TEST(net_if, if_nameindex) {
  struct if_nameindex* list = if_nameindex();
  ASSERT_TRUE(list != nullptr);

  ASSERT_TRUE(list->if_index != 0);

  std::set<std::string> if_nameindex_names;
  char buf[IF_NAMESIZE] = {};
  bool saw_lo = false;
  for (struct if_nameindex* it = list; it->if_index != 0; ++it) {
    fprintf(stderr, "\t%d\t%s\n", it->if_index, it->if_name);
    if_nameindex_names.insert(it->if_name);
    EXPECT_EQ(it->if_index, if_nametoindex(it->if_name));
    EXPECT_STREQ(it->if_name, if_indextoname(it->if_index, buf));
    if (strcmp(it->if_name, "lo") == 0) saw_lo = true;
  }
  ASSERT_TRUE(saw_lo);
  if_freenameindex(list);

  std::set<std::string> getifaddrs_names;
  ifaddrs* ifa;
  ASSERT_EQ(0, getifaddrs(&ifa));
  for (ifaddrs* it = ifa; it != nullptr; it = it->ifa_next) {
    getifaddrs_names.insert(it->ifa_name);
  }
  freeifaddrs(ifa);

  ASSERT_EQ(getifaddrs_names, if_nameindex_names);
}

TEST(net_if, if_freenameindex_nullptr) {
#if defined(__BIONIC__)
  if_freenameindex(nullptr);
#endif
}
