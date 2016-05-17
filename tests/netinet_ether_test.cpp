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

#include <gtest/gtest.h>

#include <netinet/ether.h>

TEST(netinet_ether, ether_aton__ether_ntoa) {
  ether_addr* a = ether_aton("12:34:56:78:9a:bc");
  ASSERT_NE(nullptr, a);
  ASSERT_EQ(0x12, a->ether_addr_octet[0]);
  ASSERT_EQ(0x34, a->ether_addr_octet[1]);
  ASSERT_EQ(0x56, a->ether_addr_octet[2]);
  ASSERT_EQ(0x78, a->ether_addr_octet[3]);
  ASSERT_EQ(0x9a, a->ether_addr_octet[4]);
  ASSERT_EQ(0xbc, a->ether_addr_octet[5]);

  ASSERT_STREQ("12:34:56:78:9a:bc", ether_ntoa(a));
}

TEST(netinet_ether, ether_aton_r__ether_ntoa_r) {
  ether_addr addr;
  memset(&addr, 0, sizeof(addr));
  ether_addr* a = ether_aton_r("12:34:56:78:9a:bc", &addr);
  ASSERT_EQ(&addr, a);
  ASSERT_EQ(0x12, addr.ether_addr_octet[0]);
  ASSERT_EQ(0x34, addr.ether_addr_octet[1]);
  ASSERT_EQ(0x56, addr.ether_addr_octet[2]);
  ASSERT_EQ(0x78, addr.ether_addr_octet[3]);
  ASSERT_EQ(0x9a, addr.ether_addr_octet[4]);
  ASSERT_EQ(0xbc, addr.ether_addr_octet[5]);

  char buf[32];
  memset(buf, 0, sizeof(buf));
  char* p = ether_ntoa_r(&addr, buf);
  ASSERT_EQ(buf, p);
  ASSERT_STREQ("12:34:56:78:9a:bc", buf);
}
