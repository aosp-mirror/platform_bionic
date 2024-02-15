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

#include <inttypes.h>

#include <errno.h>
#include <gtest/gtest.h>
#include <stdio.h>

#include "utils.h"

#define PRINTF_TYPED(FMT_SUFFIX, TYPE_SUFFIX) \
  do { \
    char buf[512]; \
    memset(buf, 0, sizeof(buf)); \
    snprintf(buf, sizeof(buf), "%" PRId##FMT_SUFFIX, int##TYPE_SUFFIX(123)); \
    EXPECT_STREQ("123", buf); \
    memset(buf, 0, sizeof(buf)); \
    snprintf(buf, sizeof(buf), "%" PRIi##FMT_SUFFIX, int##TYPE_SUFFIX(123)); \
    EXPECT_STREQ("123", buf); \
    memset(buf, 0, sizeof(buf)); \
    snprintf(buf, sizeof(buf), "%" PRIo##FMT_SUFFIX, int##TYPE_SUFFIX(123)); \
    EXPECT_STREQ("173", buf); \
    memset(buf, 0, sizeof(buf)); \
    snprintf(buf, sizeof(buf), "%" PRIu##FMT_SUFFIX, uint##TYPE_SUFFIX(123)); \
    EXPECT_STREQ("123", buf); \
    memset(buf, 0, sizeof(buf)); \
    snprintf(buf, sizeof(buf), "%" PRIx##FMT_SUFFIX, uint##TYPE_SUFFIX(123)); \
    EXPECT_STREQ("7b", buf); \
    memset(buf, 0, sizeof(buf)); \
    snprintf(buf, sizeof(buf), "%" PRIX##FMT_SUFFIX, uint##TYPE_SUFFIX(123)); \
    EXPECT_STREQ("7B", buf); \
  } while (false) \

#define PRINTF_SIZED(WIDTH) \
  PRINTF_TYPED(WIDTH, WIDTH##_t); \
  PRINTF_TYPED(FAST##WIDTH, _fast##WIDTH##_t); \
  PRINTF_TYPED(LEAST##WIDTH, _least##WIDTH##_t) \


#define SCANF_TYPED(FMT_SUFFIX, TYPE_SUFFIX) \
  do { \
    int##TYPE_SUFFIX dst_int##TYPE_SUFFIX = 0; \
    uint##TYPE_SUFFIX dst_uint##TYPE_SUFFIX = 0u; \
    \
    sscanf("123", "%" SCNd##FMT_SUFFIX, &dst_int##TYPE_SUFFIX); \
    EXPECT_EQ(123, dst_int##TYPE_SUFFIX); \
    dst_int##TYPE_SUFFIX = 0; \
    sscanf("123", "%" SCNi##FMT_SUFFIX, &dst_int##TYPE_SUFFIX); \
    EXPECT_EQ(123, dst_int##TYPE_SUFFIX); \
    dst_int##TYPE_SUFFIX = 0; \
    sscanf("173", "%" SCNo##FMT_SUFFIX, &dst_int##TYPE_SUFFIX); \
    EXPECT_EQ(123, dst_int##TYPE_SUFFIX); \
    dst_int##TYPE_SUFFIX = 0; \
    sscanf("123", "%" SCNu##FMT_SUFFIX, &dst_uint##TYPE_SUFFIX); \
    EXPECT_EQ(123u, dst_uint##TYPE_SUFFIX); \
    dst_uint##TYPE_SUFFIX = 0; \
    sscanf("7B", "%" SCNx##FMT_SUFFIX, &dst_uint##TYPE_SUFFIX); \
    EXPECT_EQ(123u, dst_uint##TYPE_SUFFIX); \
    dst_uint##TYPE_SUFFIX = 0; \
  } while (false) \

#define SCANF_SIZED(SIZE) \
  SCANF_TYPED(SIZE, SIZE##_t); \
  SCANF_TYPED(FAST##SIZE, _fast##SIZE##_t); \
  SCANF_TYPED(LEAST##SIZE, _least##SIZE##_t) \


TEST(inttypes, printf_macros) {
  PRINTF_SIZED(8);
  PRINTF_SIZED(16);
  PRINTF_SIZED(32);
  PRINTF_SIZED(64);

  PRINTF_TYPED(MAX, max_t);
  PRINTF_TYPED(PTR, ptr_t);
}

TEST(inttypes, scanf_macros) {
  SCANF_SIZED(8);
  SCANF_SIZED(16);
  SCANF_SIZED(32);
  SCANF_SIZED(64);

  SCANF_TYPED(MAX, max_t);
  SCANF_TYPED(PTR, ptr_t);
}

TEST(inttypes, wcstoimax) {
  wchar_t* end = nullptr;
  EXPECT_EQ(123, wcstoimax(L"  +123x", &end, 10));
  EXPECT_EQ(L'x', *end);
}

TEST(inttypes, wcstoumax) {
  wchar_t* end = nullptr;
  EXPECT_EQ(123U, wcstoumax(L"  +123x", &end, 10));
  EXPECT_EQ(L'x', *end);
}

TEST(inttypes, strtoimax_dec) {
  char* p;
  EXPECT_EQ(-18737357, strtoimax("-18737357foobar12", &p, 10));
  EXPECT_STREQ("foobar12", p);
}

TEST(inttypes, strtoimax_hex) {
  char* p;
  EXPECT_EQ(-0x18737357f, strtoimax("-18737357foobar12", &p, 16));
  EXPECT_STREQ("oobar12", p);
}

TEST(inttypes, strtoimax_EINVAL) {
  errno = 0;
  strtoimax("123", nullptr, -1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  strtoimax("123", nullptr, 1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  strtoimax("123", nullptr, 37);
  ASSERT_ERRNO(EINVAL);
}

TEST(inttypes, strtoumax_dec) {
  char* p;
  EXPECT_EQ(18737357U, strtoumax("18737357foobar12", &p, 10));
  EXPECT_STREQ("foobar12", p);
}

TEST(inttypes, strtoumax_hex) {
  char* p;
  EXPECT_EQ(0x18737357fU, strtoumax("18737357foobar12", &p, 16));
  EXPECT_STREQ("oobar12", p);
}

TEST(inttypes, strtoumax_negative) {
  char* p;
  EXPECT_EQ(UINTMAX_MAX - 18737357 + 1, strtoumax("-18737357foobar12", &p, 10));
  EXPECT_STREQ("foobar12", p);
}

TEST(inttypes, strtoumax_EINVAL) {
  errno = 0;
  strtoumax("123", nullptr, -1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  strtoumax("123", nullptr, 1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  strtoumax("123", nullptr, 37);
  ASSERT_ERRNO(EINVAL);
}

TEST(inttypes, wcstoimax_EINVAL) {
  errno = 0;
  wcstoimax(L"123", nullptr, -1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoimax(L"123", nullptr, 1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoimax(L"123", nullptr, 37);
  ASSERT_ERRNO(EINVAL);
}

TEST(inttypes, wcstoumax_EINVAL) {
  errno = 0;
  wcstoumax(L"123", nullptr, -1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoumax(L"123", nullptr, 1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoumax(L"123", nullptr, 37);
  ASSERT_ERRNO(EINVAL);
}

TEST(inttypes, div) {
  div_t r;

  r = div(5, 3);
  EXPECT_EQ(1, r.quot);
  EXPECT_EQ(2, r.rem);

  r = div(5, -3);
  EXPECT_EQ(-1, r.quot);
  EXPECT_EQ(2, r.rem);

  r = div(-5, 3);
  EXPECT_EQ(-1, r.quot);
  EXPECT_EQ(-2, r.rem);

  r = div(-5, -3);
  EXPECT_EQ(1, r.quot);
  EXPECT_EQ(-2, r.rem);
}

TEST(inttypes, ldiv) {
  ldiv_t r;

  r = ldiv(5, 3);
  EXPECT_EQ(1, r.quot);
  EXPECT_EQ(2, r.rem);

  r = ldiv(5, -3);
  EXPECT_EQ(-1, r.quot);
  EXPECT_EQ(2, r.rem);

  r = ldiv(-5, 3);
  EXPECT_EQ(-1, r.quot);
  EXPECT_EQ(-2, r.rem);

  r = ldiv(-5, -3);
  EXPECT_EQ(1, r.quot);
  EXPECT_EQ(-2, r.rem);
}

TEST(inttypes, lldiv) {
  lldiv_t r;

  r = lldiv(5, 3);
  EXPECT_EQ(1, r.quot);
  EXPECT_EQ(2, r.rem);

  r = lldiv(5, -3);
  EXPECT_EQ(-1, r.quot);
  EXPECT_EQ(2, r.rem);

  r = lldiv(-5, 3);
  EXPECT_EQ(-1, r.quot);
  EXPECT_EQ(-2, r.rem);

  r = lldiv(-5, -3);
  EXPECT_EQ(1, r.quot);
  EXPECT_EQ(-2, r.rem);
}

TEST(inttypes, imaxdiv) {
  imaxdiv_t r;

  r = imaxdiv(5, 3);
  EXPECT_EQ(1, r.quot);
  EXPECT_EQ(2, r.rem);

  r = imaxdiv(5, -3);
  EXPECT_EQ(-1, r.quot);
  EXPECT_EQ(2, r.rem);

  r = imaxdiv(-5, 3);
  EXPECT_EQ(-1, r.quot);
  EXPECT_EQ(-2, r.rem);

  r = imaxdiv(-5, -3);
  EXPECT_EQ(1, r.quot);
  EXPECT_EQ(-2, r.rem);
}

TEST(inttypes, imaxabs) {
  EXPECT_EQ(672423489, imaxabs(672423489));
  EXPECT_EQ(672423489, imaxabs(-672423489));
  EXPECT_EQ(INTMAX_MAX, imaxabs(-INTMAX_MAX));
  EXPECT_EQ(INTMAX_MAX, imaxabs(INTMAX_MAX));
}
