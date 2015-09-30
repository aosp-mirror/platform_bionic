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

#include <netinet/udp.h>

#include <gtest/gtest.h>

#if defined(__BIONIC__)
  #define UDPHDR_USES_ANON_UNION
#elif defined(__GLIBC_PREREQ)
  #if __GLIBC_PREREQ(2, 18)
    #define UDPHDR_USES_ANON_UNION
  #endif
#endif

TEST(netinet_udp, compat) {
#if defined(UDPHDR_USES_ANON_UNION)
    static_assert(offsetof(udphdr, uh_sport) == offsetof(udphdr, source), "udphdr::source");
    static_assert(offsetof(udphdr, uh_dport) == offsetof(udphdr, dest), "udphdr::dest");
    static_assert(offsetof(udphdr, uh_ulen) == offsetof(udphdr, len), "udphdr::len");
    static_assert(offsetof(udphdr, uh_sum) == offsetof(udphdr, check), "udphdr::check");

    udphdr u;
    u.uh_sport = 0x1111;
    u.uh_dport = 0x2222;
    u.uh_ulen = 0x3333;
    u.uh_sum = 0x4444;
    ASSERT_EQ(0x1111, u.source);
    ASSERT_EQ(0x2222, u.dest);
    ASSERT_EQ(0x3333, u.len);
    ASSERT_EQ(0x4444, u.check);
#endif
}
