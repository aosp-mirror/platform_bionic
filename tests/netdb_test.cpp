/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <netdb.h>

#include <gtest/gtest.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// https://code.google.com/p/android/issues/detail?id=13228
TEST(netdb, freeaddrinfo_NULL) {
  freeaddrinfo(nullptr);
}

TEST(netdb, getaddrinfo_NULL_host) {
  // It's okay for the host argument to be NULL, as long as service isn't.
  addrinfo* ai = nullptr;
  ASSERT_EQ(0, getaddrinfo(nullptr, "smtp", nullptr, &ai));
  // (sockaddr_in::sin_port and sockaddr_in6::sin6_port overlap.)
  ASSERT_EQ(25U, ntohs(reinterpret_cast<sockaddr_in*>(ai->ai_addr)->sin_port));
  freeaddrinfo(ai);
}

TEST(netdb, getaddrinfo_NULL_service) {
  // It's okay for the service argument to be NULL, as long as host isn't.
  addrinfo* ai = nullptr;
  ASSERT_EQ(0, getaddrinfo("localhost", nullptr, nullptr, &ai));
  ASSERT_TRUE(ai != nullptr);
  freeaddrinfo(ai);
}

TEST(netdb, getaddrinfo_NULL_hints) {
  addrinfo* ai = nullptr;
  ASSERT_EQ(0, getaddrinfo("localhost", "9999", nullptr, &ai));

  bool saw_tcp = false;
  bool saw_udp = false;
  for (addrinfo* p = ai; p != nullptr; p = p->ai_next) {
    ASSERT_TRUE(p->ai_family == AF_INET || p->ai_family == AF_INET6);
    if (p->ai_socktype == SOCK_STREAM) {
      ASSERT_EQ(IPPROTO_TCP, p->ai_protocol);
      saw_tcp = true;
    } else if (p->ai_socktype == SOCK_DGRAM) {
      ASSERT_EQ(IPPROTO_UDP, p->ai_protocol);
      saw_udp = true;
    }
  }
  ASSERT_TRUE(saw_tcp);
  ASSERT_TRUE(saw_udp);

  freeaddrinfo(ai);
}

TEST(netdb, getaddrinfo_service_lookup) {
  addrinfo* ai = nullptr;
  ASSERT_EQ(0, getaddrinfo("localhost", "smtp", nullptr, &ai));
  ASSERT_EQ(SOCK_STREAM, ai->ai_socktype);
  ASSERT_EQ(IPPROTO_TCP, ai->ai_protocol);
  ASSERT_EQ(25, ntohs(reinterpret_cast<sockaddr_in*>(ai->ai_addr)->sin_port));
  freeaddrinfo(ai);
}

TEST(netdb, getaddrinfo_hints) {
  addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo* ai = nullptr;
  ASSERT_EQ(0, getaddrinfo( "localhost", "9999", &hints, &ai));
  ASSERT_TRUE(ai != nullptr);
  // In glibc, getaddrinfo() converts ::1 to 127.0.0.1 for localhost,
  // so one or two addrinfo may be returned.
  addrinfo* tai = ai;
  while (tai != nullptr) {
    ASSERT_EQ(AF_INET, tai->ai_family);
    ASSERT_EQ(SOCK_STREAM, tai->ai_socktype);
    ASSERT_EQ(IPPROTO_TCP, tai->ai_protocol);
    tai = tai->ai_next;
  }
  freeaddrinfo(ai);
}

TEST(netdb, getaddrinfo_ip6_localhost) {
  addrinfo* ai = nullptr;
  ASSERT_EQ(0, getaddrinfo("ip6-localhost", nullptr, nullptr, &ai));
  ASSERT_TRUE(ai != nullptr);
  ASSERT_GE(ai->ai_addrlen, static_cast<socklen_t>(sizeof(sockaddr_in6)));
  ASSERT_TRUE(ai->ai_addr != nullptr);
  sockaddr_in6 *addr = reinterpret_cast<sockaddr_in6*>(ai->ai_addr);
  ASSERT_EQ(addr->sin6_family, AF_INET6);
  ASSERT_EQ(0, memcmp(&addr->sin6_addr, &in6addr_loopback, sizeof(in6_addr)));
  freeaddrinfo(ai);
}

TEST(netdb, getnameinfo_salen) {
  sockaddr_storage ss;
  memset(&ss, 0, sizeof(ss));
  sockaddr* sa = reinterpret_cast<sockaddr*>(&ss);
  char tmp[16];

  ss.ss_family = AF_INET;
  socklen_t too_much = sizeof(ss);
  socklen_t just_right = sizeof(sockaddr_in);
  socklen_t too_little = sizeof(sockaddr_in) - 1;

  ASSERT_EQ(0, getnameinfo(sa, too_much, tmp, sizeof(tmp), nullptr, 0, NI_NUMERICHOST));
  ASSERT_STREQ("0.0.0.0", tmp);
  ASSERT_EQ(0, getnameinfo(sa, just_right, tmp, sizeof(tmp), nullptr, 0, NI_NUMERICHOST));
  ASSERT_STREQ("0.0.0.0", tmp);
  ASSERT_EQ(EAI_FAMILY, getnameinfo(sa, too_little, tmp, sizeof(tmp), nullptr, 0, NI_NUMERICHOST));

  ss.ss_family = AF_INET6;
  just_right = sizeof(sockaddr_in6);
  too_little = sizeof(sockaddr_in6) - 1;
  too_much = just_right + 1;

  ASSERT_EQ(0, getnameinfo(sa, too_much, tmp, sizeof(tmp), nullptr, 0, NI_NUMERICHOST));
  ASSERT_STREQ("::", tmp);
  ASSERT_EQ(0, getnameinfo(sa, just_right, tmp, sizeof(tmp), nullptr, 0, NI_NUMERICHOST));
  ASSERT_STREQ("::", tmp);
  ASSERT_EQ(EAI_FAMILY, getnameinfo(sa, too_little, tmp, sizeof(tmp), nullptr, 0, NI_NUMERICHOST));
}

TEST(netdb, getnameinfo_localhost) {
  sockaddr_in addr;
  char host[NI_MAXHOST];
  memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(0x7f000001);
  ASSERT_EQ(0, getnameinfo(reinterpret_cast<sockaddr*>(&addr), sizeof(addr),
                           host, sizeof(host), nullptr, 0, 0));
  ASSERT_STREQ(host, "localhost");
}

static void VerifyLocalhostName(const char* name) {
  // Test possible localhost name and aliases, which depend on /etc/hosts or /system/etc/hosts.
  ASSERT_TRUE(strcmp(name, "localhost") == 0 ||
              strcmp(name, "ip6-localhost") == 0 ||
              strcmp(name, "ip6-loopback") == 0) << name;
}

TEST(netdb, getnameinfo_ip6_localhost) {
  sockaddr_in6 addr;
  char host[NI_MAXHOST];
  memset(&addr, 0, sizeof(sockaddr_in6));
  addr.sin6_family = AF_INET6;
  addr.sin6_addr = in6addr_loopback;
  ASSERT_EQ(0, getnameinfo(reinterpret_cast<sockaddr*>(&addr), sizeof(addr),
                           host, sizeof(host), nullptr, 0, 0));
  VerifyLocalhostName(host);
}

static void VerifyLocalhost(hostent *hent) {
  ASSERT_TRUE(hent != nullptr);
  VerifyLocalhostName(hent->h_name);
  for (size_t i = 0; hent->h_aliases[i] != nullptr; ++i) {
    VerifyLocalhostName(hent->h_aliases[i]);
  }
  ASSERT_EQ(hent->h_addrtype, AF_INET);
  ASSERT_EQ(hent->h_addr[0], 127);
  ASSERT_EQ(hent->h_addr[1], 0);
  ASSERT_EQ(hent->h_addr[2], 0);
  ASSERT_EQ(hent->h_addr[3], 1);
}

TEST(netdb, gethostbyname) {
  hostent* hp = gethostbyname("localhost");
  VerifyLocalhost(hp);
}

TEST(netdb, gethostbyname2) {
  hostent* hp = gethostbyname2("localhost", AF_INET);
  VerifyLocalhost(hp);
}

TEST(netdb, gethostbyname_r) {
  hostent hent;
  hostent *hp;
  char buf[512];
  int err;
  int result = gethostbyname_r("localhost", &hent, buf, sizeof(buf), &hp, &err);
  ASSERT_EQ(0, result);
  VerifyLocalhost(hp);

  // Change hp->h_addr to test reentrancy.
  hp->h_addr[0] = 0;

  hostent hent2;
  hostent *hp2;
  char buf2[512];
  result = gethostbyname_r("localhost", &hent2, buf2, sizeof(buf2), &hp2, &err);
  ASSERT_EQ(0, result);
  VerifyLocalhost(hp2);

  ASSERT_EQ(0, hp->h_addr[0]);
}

TEST(netdb, gethostbyname2_r) {
  hostent hent;
  hostent *hp;
  char buf[512];
  int err;
  int result = gethostbyname2_r("localhost", AF_INET, &hent, buf, sizeof(buf), &hp, &err);
  ASSERT_EQ(0, result);
  VerifyLocalhost(hp);

  // Change hp->h_addr to test reentrancy.
  hp->h_addr[0] = 0;

  hostent hent2;
  hostent *hp2;
  char buf2[512];
  result = gethostbyname2_r("localhost", AF_INET, &hent2, buf2, sizeof(buf2), &hp2, &err);
  ASSERT_EQ(0, result);
  VerifyLocalhost(hp2);

  ASSERT_EQ(0, hp->h_addr[0]);
}

TEST(netdb, gethostbyaddr) {
  in_addr addr = { htonl(0x7f000001) };
  hostent *hp = gethostbyaddr(&addr, sizeof(addr), AF_INET);
  VerifyLocalhost(hp);
}

TEST(netdb, gethostbyaddr_r) {
  in_addr addr = { htonl(0x7f000001) };
  hostent hent;
  hostent *hp;
  char buf[512];
  int err;
  int result = gethostbyaddr_r(&addr, sizeof(addr), AF_INET, &hent, buf, sizeof(buf), &hp, &err);
  ASSERT_EQ(0, result);
  VerifyLocalhost(hp);

  // Change hp->h_addr to test reentrancy.
  hp->h_addr[0] = 0;

  hostent hent2;
  hostent *hp2;
  char buf2[512];
  result = gethostbyaddr_r(&addr, sizeof(addr), AF_INET, &hent2, buf2, sizeof(buf2), &hp2, &err);
  ASSERT_EQ(0, result);
  VerifyLocalhost(hp2);

  ASSERT_EQ(0, hp->h_addr[0]);
}

TEST(netdb, gethostbyname_r_ERANGE) {
  hostent hent;
  hostent *hp;
  char buf[4]; // Use too small buffer.
  int err;
  int result = gethostbyname_r("localhost", &hent, buf, sizeof(buf), &hp, &err);
  EXPECT_EQ(NETDB_INTERNAL, err);
  EXPECT_EQ(ERANGE, result);
  EXPECT_EQ(nullptr, hp);
}

TEST(netdb, gethostbyname2_r_ERANGE) {
  hostent hent;
  hostent *hp;
  char buf[4]; // Use too small buffer.
  int err;
  int result = gethostbyname2_r("localhost", AF_INET, &hent, buf, sizeof(buf), &hp, &err);
  EXPECT_EQ(NETDB_INTERNAL, err);
  EXPECT_EQ(ERANGE, result);
  EXPECT_EQ(nullptr, hp);
}

TEST(netdb, gethostbyaddr_r_ERANGE) {
  in_addr addr = { htonl(0x7f000001) };
  hostent hent;
  hostent *hp;
  char buf[4]; // Use too small buffer.
  int err;
  int result = gethostbyaddr_r(&addr, sizeof(addr), AF_INET, &hent, buf, sizeof(buf), &hp, &err);
  EXPECT_EQ(NETDB_INTERNAL, err);
  EXPECT_EQ(ERANGE, result);
  EXPECT_EQ(nullptr, hp);
}

TEST(netdb, gethostbyname_r_HOST_NOT_FOUND) {
  hostent hent;
  hostent *hp;
  char buf[BUFSIZ];
  int err;
  int result = gethostbyname_r("does.not.exist.google.com", &hent, buf, sizeof(buf), &hp, &err);
  EXPECT_EQ(HOST_NOT_FOUND, err);
  EXPECT_EQ(0, result);
  EXPECT_EQ(nullptr, hp);
}

TEST(netdb, gethostbyname2_r_HOST_NOT_FOUND) {
  hostent hent;
  hostent *hp;
  char buf[BUFSIZ];
  int err;
  int result = gethostbyname2_r("does.not.exist.google.com", AF_INET, &hent, buf, sizeof(buf), &hp, &err);
  EXPECT_EQ(HOST_NOT_FOUND, err);
  EXPECT_EQ(0, result);
  EXPECT_EQ(nullptr, hp);
}

TEST(netdb, gethostbyaddr_r_HOST_NOT_FOUND) {
  in_addr addr = { htonl(0xffffffff) };
  hostent hent;
  hostent *hp;
  char buf[BUFSIZ];
  int err;
  int result = gethostbyaddr_r(&addr, sizeof(addr), AF_INET, &hent, buf, sizeof(buf), &hp, &err);
  EXPECT_EQ(HOST_NOT_FOUND, err);
  EXPECT_EQ(0, result);
  EXPECT_EQ(nullptr, hp);
}

TEST(netdb, getservbyname) {
  // smtp is TCP-only, so we know we'll get 25/tcp back.
  servent* s = getservbyname("smtp", nullptr);
  ASSERT_TRUE(s != nullptr);
  ASSERT_STREQ("smtp", s->s_name);
  ASSERT_EQ(25, ntohs(s->s_port));
  ASSERT_STREQ("tcp", s->s_proto);

  // We get the same result by explicitly asking for tcp.
  s = getservbyname("smtp", "tcp");
  ASSERT_TRUE(s != nullptr);
  ASSERT_STREQ("smtp", s->s_name);
  ASSERT_EQ(25, ntohs(s->s_port));
  ASSERT_STREQ("tcp", s->s_proto);

  // And we get a failure if we explicitly ask for udp.
  s = getservbyname("smtp", "udp");
  ASSERT_TRUE(s == nullptr);

  // But there are actually udp services.
  s = getservbyname("echo", "udp");
  ASSERT_TRUE(s != nullptr);
  ASSERT_STREQ("echo", s->s_name);
  ASSERT_EQ(7, ntohs(s->s_port));
  ASSERT_STREQ("udp", s->s_proto);
}

TEST(netdb, getservbyport) {
  // smtp is TCP-only, so we know we'll get 25/tcp back.
  servent* s = getservbyport(htons(25), nullptr);
  ASSERT_TRUE(s != nullptr);
  ASSERT_STREQ("smtp", s->s_name);
  ASSERT_EQ(25, ntohs(s->s_port));
  ASSERT_STREQ("tcp", s->s_proto);

  // We get the same result by explicitly asking for tcp.
  s = getservbyport(htons(25), "tcp");
  ASSERT_TRUE(s != nullptr);
  ASSERT_STREQ("smtp", s->s_name);
  ASSERT_EQ(25, ntohs(s->s_port));
  ASSERT_STREQ("tcp", s->s_proto);

  // And we get a failure if we explicitly ask for udp.
  s = getservbyport(htons(25), "udp");
  ASSERT_TRUE(s == nullptr);

  // But there are actually udp services.
  s = getservbyport(htons(7), "udp");
  ASSERT_TRUE(s != nullptr);
  ASSERT_STREQ("echo", s->s_name);
  ASSERT_EQ(7, ntohs(s->s_port));
  ASSERT_STREQ("udp", s->s_proto);
}

TEST(netdb, endnetent_getnetent_setnetent) {
  setnetent(0);
  setnetent(1);
  endnetent();
  while (getnetent() != nullptr) {
  }
}

TEST(netdb, getnetbyaddr) {
  getnetbyaddr(0, 0);
}

TEST(netdb, getnetbyname) {
  getnetbyname("x");
}

TEST(netdb, endprotoent_getprotoent_setprotoent) {
  setprotoent(0);
  setprotoent(1);
  endprotoent();
  while (getprotoent() != nullptr) {
  }
}

TEST(netdb, getprotobyname) {
  getprotobyname("tcp");
}

TEST(netdb, getprotobynumber) {
  getprotobynumber(6);
}

TEST(netdb, endservent_getservent_setservent) {
  setservent(0);
  setservent(1);
  endservent();
  size_t service_count = 0;
  while (getservent() != nullptr) {
    ++service_count;
  }
  ASSERT_GT(service_count, 0U);
}

TEST(netdb, getservbyname_getservent_conflicts) {
  // Calling getservbyname shouldn't affect getservent's iteration order.
  endservent();
  while (getservent() != nullptr) {
    ASSERT_TRUE(getservbyname("smtp", "tcp") != nullptr);
  }
}

TEST(netdb, getservbyport_getservent_conflicts) {
  // Calling getservbyport shouldn't affect getservent's iteration order.
  endservent();
  while (getservent() != nullptr) {
    ASSERT_TRUE(getservbyport(htons(25), "tcp") != nullptr);
  }
}

TEST(netdb, endservent_resets) {
  endservent();
  std::string first_service(getservent()->s_name);
  endservent();
  ASSERT_EQ(first_service, std::string(getservent()->s_name));
}

TEST(netdb, setservent_resets) {
  endservent();
  std::string first_service(getservent()->s_name);
  setservent(0);
  ASSERT_EQ(first_service, std::string(getservent()->s_name));
}

TEST(netdb, endhostent_gethostent_sethostent) {
  sethostent(0);
  sethostent(1);
  endhostent();
  size_t host_count = 0;
  while (gethostent() != nullptr) {
    ++host_count;
  }
  ASSERT_GT(host_count, 0U);
}

TEST(netdb, endhostent_resets) {
  endhostent();
  std::string first_host(gethostent()->h_name);
  endhostent();
  ASSERT_EQ(first_host, std::string(gethostent()->h_name));
}

TEST(netdb, sethostent_resets) {
  endhostent();
  std::string first_host(gethostent()->h_name);
  sethostent(0);
  ASSERT_EQ(first_host, std::string(gethostent()->h_name));
}
