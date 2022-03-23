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

  // 64-bit overflow.
  ASSERT_EQ(0, inet_aton("0x10000000000000000", nullptr));

  // Out of range octal.
  ASSERT_EQ(0, inet_aton("0400.0.0.1", nullptr));
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

TEST(arpa_inet, inet_nsap_addr) {
  // inet_nsap_addr() doesn't seem to be documented anywhere, but it's basically
  // text to binary for arbitrarily-long strings like "0xdeadbeef". Any
  // '.', '+', or '/' characters are ignored as punctuation. The return value is
  // the length in bytes, or 0 for all errors.
  u_char buf[32];

  // Missing "0x" prefix.
  ASSERT_EQ(0U, inet_nsap_addr("123", buf, sizeof(buf)));
  ASSERT_EQ(0U, inet_nsap_addr("012", buf, sizeof(buf)));

  // 1 byte.
  ASSERT_EQ(1U, inet_nsap_addr("0x12", buf, sizeof(buf)));
  ASSERT_EQ(0x12, buf[0]);

  // 10 bytes.
  ASSERT_EQ(10U, inet_nsap_addr("0x1234567890abcdef0011", buf, sizeof(buf)));
  ASSERT_EQ(0x12, buf[0]);
  ASSERT_EQ(0x34, buf[1]);
  ASSERT_EQ(0x56, buf[2]);
  ASSERT_EQ(0x78, buf[3]);
  ASSERT_EQ(0x90, buf[4]);
  ASSERT_EQ(0xab, buf[5]);
  ASSERT_EQ(0xcd, buf[6]);
  ASSERT_EQ(0xef, buf[7]);
  ASSERT_EQ(0x00, buf[8]);
  ASSERT_EQ(0x11, buf[9]);

  // Ignored punctuation.
  ASSERT_EQ(10U, inet_nsap_addr("0x1122.3344+5566/7788/99aa", buf, sizeof(buf)));
  ASSERT_EQ(0x11, buf[0]);
  ASSERT_EQ(0x22, buf[1]);
  ASSERT_EQ(0x33, buf[2]);
  ASSERT_EQ(0x44, buf[3]);
  ASSERT_EQ(0x55, buf[4]);
  ASSERT_EQ(0x66, buf[5]);
  ASSERT_EQ(0x77, buf[6]);
  ASSERT_EQ(0x88, buf[7]);
  ASSERT_EQ(0x99, buf[8]);
  ASSERT_EQ(0xaa, buf[9]);

  // Truncated.
  ASSERT_EQ(4U, inet_nsap_addr("0xdeadbeef666666666666", buf, 4));
  // Overwritten...
  ASSERT_EQ(0xde, buf[0]);
  ASSERT_EQ(0xad, buf[1]);
  ASSERT_EQ(0xbe, buf[2]);
  ASSERT_EQ(0xef, buf[3]);
  // Same as before...
  ASSERT_EQ(0x55, buf[4]);
  ASSERT_EQ(0x66, buf[5]);
  ASSERT_EQ(0x77, buf[6]);
  ASSERT_EQ(0x88, buf[7]);
  ASSERT_EQ(0x99, buf[8]);
  ASSERT_EQ(0xaa, buf[9]);

  // Case insensitivity.
  ASSERT_EQ(6U, inet_nsap_addr("0xaAbBcCdDeEfF", buf, 6));
  ASSERT_EQ(0xaa, buf[0]);
  ASSERT_EQ(0xbb, buf[1]);
  ASSERT_EQ(0xcc, buf[2]);
  ASSERT_EQ(0xdd, buf[3]);
  ASSERT_EQ(0xee, buf[4]);
  ASSERT_EQ(0xff, buf[5]);

  // Punctuation isn't allowed within a byte.
  ASSERT_EQ(0U, inet_nsap_addr("0x1.122", buf, sizeof(buf)));
  // Invalid punctuation.
  ASSERT_EQ(0U, inet_nsap_addr("0x11,22", buf, sizeof(buf)));
  // Invalid hex digit.
  ASSERT_EQ(0U, inet_nsap_addr("0x11.g2", buf, sizeof(buf)));
  ASSERT_EQ(0U, inet_nsap_addr("0x11.2g", buf, sizeof(buf)));
  // Invalid half-byte.
  ASSERT_EQ(0U, inet_nsap_addr("0x11.2", buf, sizeof(buf)));
}

TEST(arpa_inet, inet_nsap_ntoa) {
  // inet_nsap_ntoa() doesn't seem to be documented anywhere, but it's basically
  // binary to text for arbitrarily-long byte buffers.
  // The return value is a pointer to the buffer. No errors are possible.
  const unsigned char bytes[] = {0x01, 0x00, 0x02, 0x0e, 0xf0, 0x20};
  char dst[32];
  ASSERT_EQ(dst, inet_nsap_ntoa(6, bytes, dst));
  ASSERT_STREQ(dst, "0x01.0002.0EF0.20");
}

TEST(arpa_inet, inet_nsap_ntoa__nullptr) {
  // If you don't provide a destination, a static buffer is provided for you.
  const unsigned char bytes[] = {0x01, 0x00, 0x02, 0x0e, 0xf0, 0x20};
  ASSERT_STREQ("0x01.0002.0EF0.20", inet_nsap_ntoa(6, bytes, nullptr));
}
