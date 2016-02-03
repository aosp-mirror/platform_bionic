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
