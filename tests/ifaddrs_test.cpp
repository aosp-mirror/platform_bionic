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

#include <dirent.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#include <algorithm>
#include <map>
#include <vector>

TEST(ifaddrs, freeifaddrs_null) {
  freeifaddrs(nullptr);
}

// We can't statically say much about what network interfaces are available, but we can be pretty
// sure there's a loopback interface, and that it has IPv4, IPv6, and AF_PACKET entries.
TEST(ifaddrs, getifaddrs_lo) {
  ifaddrs* addrs = nullptr;

  ASSERT_EQ(0, getifaddrs(&addrs));
  ASSERT_TRUE(addrs != nullptr);

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

// Check that getifaddrs sees the same list of interfaces as /sys/class/net.
TEST(ifaddrs, getifaddrs_interfaces) {
  std::vector<std::string> ifaddrs_socks;
  {
    ifaddrs* addrs;
    ASSERT_EQ(0, getifaddrs(&addrs));

    for (ifaddrs* addr = addrs; addr != nullptr; addr = addr->ifa_next) {
      int family = addr->ifa_addr ? addr->ifa_addr->sa_family :
          addr->ifa_broadaddr ? addr->ifa_broadaddr->sa_family :
          AF_UNSPEC;

      if (family == AF_PACKET || family == AF_UNSPEC) {
        ifaddrs_socks.push_back(std::string(addr->ifa_name));
      }
    }

    freeifaddrs(addrs);
  }

  std::vector<std::string> sys_class_net;
  {
    std::unique_ptr<DIR, decltype(&closedir)> d(opendir("/sys/class/net"), closedir);
    ASSERT_TRUE(d != nullptr);
    dirent* dir;
    while ((dir = readdir(d.get())) != nullptr) {
      if (dir->d_type == DT_LNK) {
        sys_class_net.push_back(std::string(dir->d_name));
      }
    }
  }

  ASSERT_TRUE(std::is_permutation(ifaddrs_socks.begin(), ifaddrs_socks.end(),
                                  sys_class_net.begin()));
}

static void CheckAddressIsInSet(const std::string& if_name, bool unicast,
                                const std::set<in_addr_t>& addrs) {
  ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_addr.sa_family = AF_INET;
  if_name.copy(ifr.ifr_name, IFNAMSIZ - 1);

  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  ASSERT_TRUE(fd != -1);

  int request = SIOCGIFADDR;
  if (!unicast) {
    // For non-unicast, the specific ioctl to use depends on whether the IFF_BROADCAST flag is set.
    ASSERT_EQ(0, ioctl(fd, SIOCGIFFLAGS, &ifr)) << if_name << ' ' << strerror(errno);
    request = ((ifr.ifr_flags & IFF_BROADCAST) != 0) ? SIOCGIFBRDADDR : SIOCGIFDSTADDR;
  }

  ASSERT_EQ(0, ioctl(fd, request, &ifr)) << if_name << ' ' << strerror(errno);
  close(fd);

  sockaddr_in* sock = reinterpret_cast<sockaddr_in*>(&ifr.ifr_addr);
  in_addr_t addr = sock->sin_addr.s_addr;

  EXPECT_TRUE(addrs.find(addr) != addrs.end()) << if_name << ' ' << std::hex << ntohl(addr);
}

TEST(ifaddrs, getifaddrs_INET) {
  std::map<std::string, std::set<in_addr_t>> inet_addrs;
  std::map<std::string, std::set<in_addr_t>> broad_addrs;

  // Collect the IPv4 addresses for each interface.
  ifaddrs* addrs;
  ASSERT_EQ(0, getifaddrs(&addrs));
  for (ifaddrs* addr = addrs; addr != nullptr; addr = addr->ifa_next) {
    if (addr->ifa_name && addr->ifa_addr && addr->ifa_addr->sa_family == AF_INET) {
      auto sock = reinterpret_cast<sockaddr_in*>(addr->ifa_addr);
      inet_addrs[addr->ifa_name].insert(sock->sin_addr.s_addr);
    }
    if (addr->ifa_name && addr->ifa_broadaddr && addr->ifa_broadaddr->sa_family == AF_INET) {
      auto sock = reinterpret_cast<sockaddr_in*>(addr->ifa_broadaddr);
      broad_addrs[addr->ifa_name].insert(sock->sin_addr.s_addr);
    }
  }
  freeifaddrs(addrs);

  // Check that the addresses returned by the SIOCGIFADDR and SIOCGIFBRDADDR/SIOCGIFDSTADDR ioctls
  // are in our collections.
  for (const auto& it : inet_addrs) CheckAddressIsInSet(it.first, true, it.second);
  for (const auto& it : broad_addrs) CheckAddressIsInSet(it.first, false, it.second);
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

static const char* FamilyToName(int family) {
  if (family == AF_INET) return "AF_INET";
  if (family == AF_INET6) return "AF_INET6";
  if (family == AF_PACKET) return "AF_PACKET";
  if (family == AF_UNSPEC) return "AF_UNSPEC";
  return "?";
}

static std::string FlagsToString(short flags) {
  std::string result;
  if ((flags & IFF_UP) != 0) result += " UP";
  if ((flags & IFF_BROADCAST) != 0) result += " BROADCAST";
  if ((flags & IFF_DEBUG) != 0) result += " DEBUG";
  if ((flags & IFF_LOOPBACK) != 0) result += " LOOPBACK";
  if ((flags & IFF_POINTOPOINT) != 0) result += " POINTOPOINT";
  if ((flags & IFF_NOTRAILERS) != 0) result += " NOTRAILERS";
  if ((flags & IFF_RUNNING) != 0) result += " RUNNING";
  if ((flags & IFF_NOARP) != 0) result += " NOARP";
  if ((flags & IFF_PROMISC) != 0) result += " PROMISC";
  if ((flags & IFF_ALLMULTI) != 0) result += " ALLMULTI";
  if ((flags & IFF_MASTER) != 0) result += " MASTER";
  if ((flags & IFF_SLAVE) != 0) result += " SLAVE";
  if ((flags & IFF_MULTICAST) != 0) result += " MULTICAST";
  if ((flags & IFF_PORTSEL) != 0) result += " PORTSEL";
  if ((flags & IFF_AUTOMEDIA) != 0) result += " AUTOMEDIA";
  if ((flags & IFF_DYNAMIC) != 0) result += " DYNAMIC";
#if defined(IFF_LOWER_UP)
  if ((flags & IFF_LOWER_UP) != 0) result += " LOWER_UP";
#endif
#if defined(IFF_DORMANT)
  if ((flags & IFF_DORMANT) != 0) result += " DORMANT";
#endif
#if defined(IFF_ECHO)
  if ((flags & IFF_ECHO) != 0) result += " ECHO";
#endif
  return result;
}

// Not really a test, but a useful debugging tool.
TEST(ifaddrs, dump) {
  ifaddrs* addrs;
  ASSERT_EQ(0, getifaddrs(&addrs));

  for (ifaddrs* ifa = addrs; ifa != nullptr; ifa = ifa->ifa_next) {
    int family = ifa->ifa_addr ? ifa->ifa_addr->sa_family :
                                 ifa->ifa_broadaddr ? ifa->ifa_broadaddr->sa_family : AF_UNSPEC;

    printf("\t%s\n"
           "\t\t%s (%d) flags=%#x%s\n",
           ifa->ifa_name, FamilyToName(family), family,
           ifa->ifa_flags, FlagsToString(ifa->ifa_flags).c_str());

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

TEST(ifaddrs, inet6_scope_ids) {
  ifaddrs* addrs;
  ASSERT_EQ(0, getifaddrs(&addrs));

  for (ifaddrs* ifa = addrs; ifa != nullptr; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET6) {
      sockaddr_in6* sa6 = reinterpret_cast<sockaddr_in6*>(ifa->ifa_addr);
      // Any link-local IPv6 address should have a scope id. (http://b/27219454.)
      // 0 isn't a valid interface index, so that would mean the scope id wasn't set.
      if (IN6_IS_ADDR_LINKLOCAL(&sa6->sin6_addr) || IN6_IS_ADDR_MC_LINKLOCAL(&sa6->sin6_addr)) {
        ASSERT_NE(sa6->sin6_scope_id, 0U);
      }
    }
  }

  freeifaddrs(addrs);
}
