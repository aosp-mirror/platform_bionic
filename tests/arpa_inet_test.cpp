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

#include <arpa/inet.h>

TEST(arpa_inet, inet_addr) {
  ASSERT_EQ((htonl)(0x7f000001), inet_addr("127.0.0.1"));
}

TEST(arpa_inet, inet_aton) {
  in_addr a;

  // a.b.c.d
  a.s_addr = 0;
  ASSERT_EQ(1, inet_aton("127.1.2.3", &a));
  ASSERT_EQ((htonl)(0x7f010203), a.s_addr);

  // a.b.c
  a.s_addr = 0;
  ASSERT_EQ(1, inet_aton("127.1.2", &a));
  ASSERT_EQ((htonl)(0x7f010002), a.s_addr);

  // a.b
  a.s_addr = 0;
  ASSERT_EQ(1, inet_aton("127.1", &a));
  ASSERT_EQ((htonl)(0x7f000001), a.s_addr);

  // a
  a.s_addr = 0;
  ASSERT_EQ(1, inet_aton("0x7f000001", &a));
  ASSERT_EQ((htonl)(0x7f000001), a.s_addr);

  // Hex (0x) and mixed-case hex digits.
  a.s_addr = 0;
  ASSERT_EQ(1, inet_aton("0xFf.0.0.1", &a));
  ASSERT_EQ((htonl)(0xff000001), a.s_addr);

  // Hex (0X) and mixed-case hex digits.
  a.s_addr = 0;
  ASSERT_EQ(1, inet_aton("0XfF.0.0.1", &a));
  ASSERT_EQ((htonl)(0xff000001), a.s_addr);

  // Octal.
  a.s_addr = 0;
  ASSERT_EQ(1, inet_aton("0177.0.0.1", &a));
  ASSERT_EQ((htonl)(0x7f000001), a.s_addr);

  a.s_addr = 0;
  ASSERT_EQ(1, inet_aton("036", &a));
  ASSERT_EQ((htonl)(036U), a.s_addr);
}

TEST(arpa_inet, inet_aton_nullptr) {
  ASSERT_EQ(0, inet_aton("", nullptr));
  ASSERT_EQ(1, inet_aton("127.0.0.1", nullptr));
}

TEST(arpa_inet, inet_aton_invalid) {
  ASSERT_EQ(0, inet_aton("", nullptr)); // Empty.
  ASSERT_EQ(0, inet_aton("x", nullptr)); // Leading junk.
  ASSERT_EQ(0, inet_aton("127.0.0.1x", nullptr)); // Trailing junk.
  ASSERT_EQ(0, inet_aton("09.0.0.1", nullptr)); // Invalid octal.
  ASSERT_EQ(0, inet_aton("0xg.0.0.1", nullptr)); // Invalid hex.

  ASSERT_EQ(0, inet_aton("1.2.3.4.5", nullptr)); // Too many dots.
  ASSERT_EQ(0, inet_aton("1.2.3.4.", nullptr)); // Trailing dot.

  // Out of range a.b.c.d form.
  ASSERT_EQ(0, inet_aton("999.0.0.1", nullptr));
  ASSERT_EQ(0, inet_aton("0.999.0.1", nullptr));
  ASSERT_EQ(0, inet_aton("0.0.999.1", nullptr));
  ASSERT_EQ(0, inet_aton("0.0.0.999", nullptr));

  // Out of range a.b.c form.
  ASSERT_EQ(0, inet_aton("256.0.0", nullptr));
  ASSERT_EQ(0, inet_aton("0.256.0", nullptr));
  ASSERT_EQ(0, inet_aton("0.0.0x10000", nullptr));

  // Out of range a.b form.
  ASSERT_EQ(0, inet_aton("256.0", nullptr));
  ASSERT_EQ(0, inet_aton("0.0x1000000", nullptr));

  // Out of range a form.
  ASSERT_EQ(0, inet_aton("0x100000000", nullptr));

  ASSERT_EQ(0, inet_aton("0400.0.0.1", nullptr)); // Out of range octal.
}

TEST(arpa_inet, inet_lnaof) {
  in_addr a = { htonl(0x12345678) };
  ASSERT_EQ(0x00345678U, inet_lnaof(a));
}

TEST(arpa_inet, inet_makeaddr) {
  in_addr a = inet_makeaddr(0x12U, 0x345678);
  ASSERT_EQ((htonl)(0x12345678), a.s_addr);
}

TEST(arpa_inet, inet_netof) {
  in_addr a = { htonl(0x12345678) };
  ASSERT_EQ(0x12U, inet_netof(a));
}

TEST(arpa_inet, inet_network) {
  ASSERT_EQ(0x7f000001U, inet_network("127.0.0.1"));
  ASSERT_EQ(0x7fU, inet_network("0x7f"));
  ASSERT_EQ(~0U, inet_network(""));
}

TEST(arpa_inet, inet_ntoa) {
  in_addr a = { (htonl)(0x7f000001) };
  ASSERT_STREQ("127.0.0.1", inet_ntoa(a));
}

TEST(arpa_inet, inet_pton__inet_ntop) {
  sockaddr_storage ss;
  ASSERT_EQ(1, inet_pton(AF_INET, "127.0.0.1", &ss));

  char s[INET_ADDRSTRLEN];
  ASSERT_STREQ("127.0.0.1", inet_ntop(AF_INET, &ss, s, INET_ADDRSTRLEN));
}

TEST(arpa_inet, inet_ntop_overflow) {
  // OpenBSD's inet_ntop had a bug where passing a 'size' larger than INET_ADDRSTRLEN
  // for AF_INET or INET6_ADDRSTRLEN for AF_INET6 would cause inet_ntop to overflow an
  // internal buffer.

  sockaddr_storage ss4;
  ASSERT_EQ(1, inet_pton(AF_INET, "127.0.0.1", &ss4));

  sockaddr_storage ss6;
  ASSERT_EQ(1, inet_pton(AF_INET6, "::1", &ss6));

  char s4[INET_ADDRSTRLEN];
  char s6[INET6_ADDRSTRLEN];
  ASSERT_STREQ("127.0.0.1", inet_ntop(AF_INET, &ss4, s4, INET_ADDRSTRLEN));
  ASSERT_STREQ("127.0.0.1", inet_ntop(AF_INET, &ss4, s4, 2*INET_ADDRSTRLEN));
  ASSERT_STREQ("::1", inet_ntop(AF_INET6, &ss6, s6, INET_ADDRSTRLEN));
  ASSERT_STREQ("::1", inet_ntop(AF_INET6, &ss6, s6, INET6_ADDRSTRLEN));
  ASSERT_STREQ("::1", inet_ntop(AF_INET6, &ss6, s6, 2*INET6_ADDRSTRLEN));
}
