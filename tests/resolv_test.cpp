/*
 * Copyright (C) 2016 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <resolv.h>

#include <gtest/gtest.h>

TEST(resolv, b64_pton_28035006) {
#if !defined(MUSL)
  // Test data from https://groups.google.com/forum/#!topic/mailing.openbsd.tech/w3ACIlklJkI.
  const char* data =
      "p1v3+nehH3N3n+/OokzXpsyGF2VVpxIxkjSn3Mv/Sq74OE1iFuVU+K4bQImuVj"
      "S55RB2fpCpbB8Nye7tzrt6h9YPP3yyJfqORDETGmIB4lveZXA4KDxx50F9rYrO"
      "dFbTLyWfNBb/8Q2TnD72eY/3Y5P9qwtJwyDL25Tleic8G3g=";

  // This buffer is exactly the right size, but old versions of the BSD code
  // incorrectly required an extra byte. http://b/28035006.
  uint8_t buf[128];
  ASSERT_EQ(128, b64_pton(data, buf, sizeof(buf)));
#else
  GTEST_SKIP() << "musl doesn't have b64_pton";
#endif
}

TEST(resolv, b64_ntop) {
#if !defined(MUSL)
  char buf[128];
  memset(buf, 'x', sizeof(buf));
  ASSERT_EQ(static_cast<int>(strlen("aGVsbG8=")),
            b64_ntop(reinterpret_cast<u_char const*>("hello"), strlen("hello"),
                     buf, sizeof(buf)));
  ASSERT_STREQ(buf, "aGVsbG8=");
#else
  GTEST_SKIP() << "musl doesn't have b64_ntop";
#endif
}

TEST(resolv, b64_pton) {
#if !defined(MUSL)
  u_char buf[128];
  memset(buf, 'x', sizeof(buf));
  ASSERT_EQ(static_cast<int>(strlen("hello")), b64_pton("aGVsbG8=", buf, sizeof(buf)));
  ASSERT_STREQ(reinterpret_cast<char*>(buf), "hello");
#else
  GTEST_SKIP() << "musl doesn't have b64_pton";
#endif
}

TEST(resolv, p_class) {
#if !defined(MUSL)
  ASSERT_STREQ("IN", p_class(ns_c_in));
  ASSERT_STREQ("BADCLASS", p_class(-1));
#else
  GTEST_SKIP() << "musl doesn't have p_class";
#endif
}

TEST(resolv, p_type) {
#if !defined(MUSL)
  ASSERT_STREQ("AAAA", p_type(ns_t_aaaa));
  ASSERT_STREQ("BADTYPE", p_type(-1));
#else
  GTEST_SKIP() << "musl doesn't have p_type";
#endif
}

TEST(resolv, res_init) {
  ASSERT_EQ(0, res_init());
}

TEST(resolv, res_randomid) {
#if !defined(MUSL)
  res_randomid();
#else
  GTEST_SKIP() << "musl doesn't have res_randomid";
#endif
}
