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

#include <linux/if_packet.h>
#include <netinet/in.h>

TEST(ifaddrs, freeifaddrs_null) {
  freeifaddrs(nullptr);
}

TEST(ifaddrs, getifaddrs_smoke) {
  ifaddrs* addrs = nullptr;

  ASSERT_EQ(0, getifaddrs(&addrs));
  ASSERT_TRUE(addrs != nullptr);

  // We can't say much about what network interfaces are available, but we can be pretty
  // sure there's a loopback interface, and that it has IPv4, IPv6, and AF_PACKET entries.
  ifaddrs* lo_inet4 = nullptr;
  ifaddrs* lo_inet6 = nullptr;
  ifaddrs* lo_packet = nullptr;
  for (ifaddrs* addr = addrs; addr != nullptr; addr = addr->ifa_next) {
    if (addr->ifa_name && strcmp(addr->ifa_name, "lo") == 0) {
      if (addr->ifa_addr && addr->ifa_addr->sa_family == AF_INET) lo_inet4 = addr;
      else if (addr->ifa_addr && addr->ifa_addr->sa_family == AF_INET6) lo_inet6 = addr;
      else if (addr->ifa_addr && addr->ifa_addr->sa_family == AF_PACKET) lo_packet = addr;
    }
  }

  // Does the IPv4 entry look right?
  ASSERT_TRUE(lo_inet4 != nullptr);
  const sockaddr_in* sa_inet4 = reinterpret_cast<const sockaddr_in*>(lo_inet4->ifa_addr);
  ASSERT_TRUE(ntohl(sa_inet4->sin_addr.s_addr) == INADDR_LOOPBACK);

  // Does the IPv6 entry look right?
  ASSERT_TRUE(lo_inet6 != nullptr);
  const sockaddr_in6* sa_inet6 = reinterpret_cast<const sockaddr_in6*>(lo_inet6->ifa_addr);
  ASSERT_TRUE(IN6_IS_ADDR_LOOPBACK(&sa_inet6->sin6_addr));

  // Does the AF_PACKET entry look right?
  ASSERT_TRUE(lo_packet != nullptr);
  const sockaddr_ll* sa_ll = reinterpret_cast<const sockaddr_ll*>(lo_packet->ifa_addr);
  ASSERT_EQ(6, sa_ll->sll_halen);

  freeifaddrs(addrs);
}
