/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <ifaddrs.h>

TEST(ifaddrs, freeifaddrs_null) {
  freeifaddrs(nullptr);
}

TEST(ifaddrs, getifaddrs_smoke) {
  ifaddrs* addrs = nullptr;

  ASSERT_EQ(0, getifaddrs(&addrs));
  ASSERT_TRUE(addrs != nullptr);

  bool saw_loopback = false;
  for (ifaddrs* addr = addrs; addr != nullptr; addr = addr->ifa_next) {
    if (addr->ifa_name && strcmp(addr->ifa_name, "lo") == 0) saw_loopback = true;
  }
  ASSERT_TRUE(saw_loopback);

  freeifaddrs(addrs);
}
