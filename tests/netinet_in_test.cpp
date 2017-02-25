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

#include <netinet/in.h>

#include <errno.h>

#include <gtest/gtest.h>

#include <android-base/macros.h>

static constexpr uint16_t le16 = 0x1234;
static constexpr uint32_t le32 = 0x12345678;
static constexpr uint64_t le64 = 0x123456789abcdef0;

static constexpr uint16_t be16 = 0x3412;
static constexpr uint32_t be32 = 0x78563412;
static constexpr uint64_t be64 = 0xf0debc9a78563412;

TEST(netinet_in, bindresvport) {
  // This isn't something we can usually test, so just check the symbol's there.
  ASSERT_EQ(-1, bindresvport(-1, nullptr));
}

TEST(netinet_in, in6addr_any) {
  in6_addr any = IN6ADDR_ANY_INIT;
  ASSERT_EQ(0, memcmp(&any, &in6addr_any, sizeof(in6addr_any)));
}

TEST(netinet_in, in6addr_loopback) {
  in6_addr loopback = IN6ADDR_LOOPBACK_INIT;
  ASSERT_EQ(0, memcmp(&loopback, &in6addr_loopback, sizeof(in6addr_loopback)));
}

TEST(netinet_in, htons_function) {
  ASSERT_EQ(be16, (htons)(le16));
}

TEST(netinet_in, htonl_function) {
  ASSERT_EQ(be32, (htonl)(le32));
}

TEST(netinet_in, htonq_macro) {
#if defined(__BIONIC__)
  ASSERT_EQ(be64, htonq(le64));
#else
  UNUSED(be64);
#endif
}

TEST(netinet_in, ntohs_function) {
  ASSERT_EQ(le16, (ntohs)(be16));
}

TEST(netinet_in, ntohl_function) {
  ASSERT_EQ(le32, (ntohl)(be32));
}

TEST(netinet_in, ntohq_macro) {
#if defined(__BIONIC__)
  ASSERT_EQ(le64, ntohq(be64));
#else
  UNUSED(le64);
#endif
}
