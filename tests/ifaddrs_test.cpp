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
#include <net/ethernet.h>
#include <net/if.h>
#include <netdb.h>
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

static void print_sockaddr_ll(const char* what, const sockaddr* p) {
  const sockaddr_ll* s = reinterpret_cast<const sockaddr_ll*>(p);
  printf("\t\t%s\t", what);
  for (int i = 0; i < s->sll_halen; ++i) {
    if (i > 0) printf(":");
    printf("%02X", s->sll_addr[i]);
  }
  printf(" (%d bytes)\n", s->sll_halen);
}

static void print_sockaddr_inet(const char* what, const sockaddr* addr) {
  char host[NI_MAXHOST];
  int family = addr->sa_family;
  int error = getnameinfo(addr,
                          (family == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6),
                          host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);
  if (error != 0) {
    printf("%d getnameinfo() failed: %s\n", family, gai_strerror(error));
    strcpy(host, "???");
  }
  printf("\t\t%s: <%s>\n", what, host);
}

static const char* family_to_name(int family) {
  if (family == AF_INET) return "AF_INET";
  if (family == AF_INET6) return "AF_INET6";
  if (family == AF_PACKET) return "AF_PACKET";
  if (family == AF_UNSPEC) return "AF_UNSPEC";
  return "?";
}

// Not really a test, but a useful debugging tool.
TEST(ifaddrs, dump) {
  ifaddrs* addrs;
  ASSERT_EQ(0, getifaddrs(&addrs));

  for (ifaddrs* ifa = addrs; ifa != nullptr; ifa = ifa->ifa_next) {
    int family = ifa->ifa_addr ? ifa->ifa_addr->sa_family :
                                 ifa->ifa_broadaddr ? ifa->ifa_broadaddr->sa_family : AF_UNSPEC;

    printf("\t%s\n"
           "\t\t%s (%d) flags=%#x\n",
           ifa->ifa_name, family_to_name(family), family, ifa->ifa_flags);

    if (family == AF_PACKET) {
      if (ifa->ifa_addr) print_sockaddr_ll("hwaddr", ifa->ifa_addr);
      if (ifa->ifa_broadaddr) print_sockaddr_ll("hwbroad", ifa->ifa_addr);
    } else if (family == AF_INET || family == AF_INET6) {
      if (ifa->ifa_addr) print_sockaddr_inet("address", ifa->ifa_addr);
      if (ifa->ifa_broadaddr && (ifa->ifa_flags & (IFF_BROADCAST | IFF_POINTOPOINT)) != 0) {
        print_sockaddr_inet((ifa->ifa_flags & IFF_BROADCAST) ? "broadcast" : "destination",
                            ifa->ifa_broadaddr);
      }
    }

    fflush(stdout);
  }

  freeifaddrs(addrs);
}
