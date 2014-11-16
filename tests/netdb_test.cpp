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

#include <gtest/gtest.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

TEST(netdb, getaddrinfo_NULL_host) {
  // It's okay for the host argument to be NULL, as long as service isn't.
  addrinfo* ai = NULL;
  ASSERT_EQ(0, getaddrinfo(NULL, "smtp", NULL, &ai));
  // (sockaddr_in::sin_port and sockaddr_in6::sin6_port overlap.)
  ASSERT_EQ(25U, ntohs(reinterpret_cast<sockaddr_in*>(ai->ai_addr)->sin_port));
  freeaddrinfo(ai);
}

TEST(netdb, getaddrinfo_NULL_service) {
  // It's okay for the service argument to be NULL, as long as host isn't.
  addrinfo* ai = NULL;
  ASSERT_EQ(0, getaddrinfo("localhost", NULL, NULL, &ai));
  ASSERT_TRUE(ai != NULL);
  freeaddrinfo(ai);
}

TEST(netdb, getaddrinfo_NULL_hints) {
  addrinfo* ai = NULL;
  ASSERT_EQ(0, getaddrinfo("localhost", "9999", NULL, &ai));

  bool saw_tcp = false;
  bool saw_udp = false;
  for (addrinfo* p = ai; p != NULL; p = p->ai_next) {
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
  addrinfo* ai = NULL;
  ASSERT_EQ(0, getaddrinfo("localhost", "smtp", NULL, &ai));
  ASSERT_EQ(SOCK_STREAM, ai->ai_socktype);
  ASSERT_EQ(IPPROTO_TCP, ai->ai_protocol);
  ASSERT_EQ(25, ntohs(reinterpret_cast<sockaddr_in*>(ai->ai_addr)->sin_port));
  freeaddrinfo(ai);
}

TEST(netdb, getaddrinfo_hints) {
  addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo* ai = NULL;
  ASSERT_EQ(0, getaddrinfo( "localhost", "9999", &hints, &ai));
  ASSERT_EQ(AF_INET, ai->ai_family);
  ASSERT_EQ(SOCK_STREAM, ai->ai_socktype);
  ASSERT_EQ(IPPROTO_TCP, ai->ai_protocol);
  ASSERT_TRUE(ai->ai_next == NULL);
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

  ASSERT_EQ(0, getnameinfo(sa, too_much, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));
  ASSERT_STREQ("0.0.0.0", tmp);
  ASSERT_EQ(0, getnameinfo(sa, just_right, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));
  ASSERT_STREQ("0.0.0.0", tmp);
  ASSERT_EQ(EAI_FAMILY, getnameinfo(sa, too_little, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));

  ss.ss_family = AF_INET6;
  just_right = sizeof(sockaddr_in6);
  too_little = sizeof(sockaddr_in6) - 1;
  too_much = just_right + 1;

  ASSERT_EQ(0, getnameinfo(sa, too_much, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));
  ASSERT_STREQ("::", tmp);
  ASSERT_EQ(0, getnameinfo(sa, just_right, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));
  ASSERT_STREQ("::", tmp);
  ASSERT_EQ(EAI_FAMILY, getnameinfo(sa, too_little, tmp, sizeof(tmp), NULL, 0, NI_NUMERICHOST));
}

TEST(netdb, gethostbyname) {
  hostent* hent = gethostbyname("localhost");
  ASSERT_TRUE(hent != NULL);
  ASSERT_EQ(hent->h_addrtype, AF_INET);
  ASSERT_EQ(hent->h_addr[0], 127);
  ASSERT_EQ(hent->h_addr[1], 0);
  ASSERT_EQ(hent->h_addr[2], 0);
  ASSERT_EQ(hent->h_addr[3], 1);
}

TEST(netdb, getservbyname) {
  // smtp is TCP-only, so we know we'll get 25/tcp back.
  servent* s = getservbyname("smtp", NULL);
  ASSERT_TRUE(s != NULL);
  ASSERT_EQ(25, ntohs(s->s_port));
  ASSERT_STREQ("tcp", s->s_proto);

  // We get the same result by explicitly asking for tcp.
  s = getservbyname("smtp", "tcp");
  ASSERT_TRUE(s != NULL);
  ASSERT_EQ(25, ntohs(s->s_port));
  ASSERT_STREQ("tcp", s->s_proto);

  // And we get a failure if we explicitly ask for udp.
  s = getservbyname("smtp", "udp");
  ASSERT_TRUE(s == NULL);

  // But there are actually udp services.
  s = getservbyname("echo", "udp");
  ASSERT_TRUE(s != NULL);
  ASSERT_EQ(7, ntohs(s->s_port));
  ASSERT_STREQ("udp", s->s_proto);
}
