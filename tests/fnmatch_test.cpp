/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <fnmatch.h>

TEST(fnmatch, basic) {
  EXPECT_EQ(0, fnmatch("abc", "abc", 0));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("abc", "abd", 0));
}

TEST(fnmatch, casefold) {
  EXPECT_EQ(FNM_NOMATCH, fnmatch("abc", "aBc", 0));
  EXPECT_EQ(0, fnmatch("abc", "aBc", FNM_CASEFOLD));
}

TEST(fnmatch, character_class) {
  // Literal.
  EXPECT_EQ(0, fnmatch("ab[cd]", "abc", 0));
  EXPECT_EQ(0, fnmatch("ab[cd]", "abd", 0));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab[cd]", "abe", 0));

  // Inverted literal.
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab[^cd]", "abc", 0));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab[^cd]", "abd", 0));
  EXPECT_EQ(0, fnmatch("ab[^cd]", "abe", 0));

  // Range.
  EXPECT_EQ(0, fnmatch("a[0-9]b", "a0b", 0));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("a[0-9]b", "aOb", 0));

  // Inverted range.
  EXPECT_EQ(0, fnmatch("a[^0-9]b", "aOb", 0));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("a[^0-9]b", "a0b", 0));

  // Named.
  EXPECT_EQ(0, fnmatch("a[[:digit:]]b", "a0b", 0));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("a[[:digit:]]b", "aOb", 0));

  // Inverted named.
  EXPECT_EQ(FNM_NOMATCH, fnmatch("a[^[:digit:]]b", "a0b", 0));
  EXPECT_EQ(0, fnmatch("a[^[:digit:]]b", "aOb", 0));
}

TEST(fnmatch, wild_any) {
  EXPECT_EQ(0, fnmatch("ab*", "ab", 0));
  EXPECT_EQ(0, fnmatch("ab*", "abc", 0));
  EXPECT_EQ(0, fnmatch("ab*", "abcd", 0));
  EXPECT_EQ(0, fnmatch("ab*", "ab/cd", 0));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab*", "ab/cd", FNM_PATHNAME));
}

TEST(fnmatch, wild_one) {
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab?", "ab", 0));
  EXPECT_EQ(0, fnmatch("ab?", "abc", 0));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab?", "abcd", 0));
  EXPECT_EQ(0, fnmatch("ab?d", "abcd", 0));
  EXPECT_EQ(0, fnmatch("ab?cd", "ab/cd", 0));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab?cd", "ab/cd", FNM_PATHNAME));
}

TEST(fnmatch, leading_dir) {
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab", "abcd", FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("ab*", "abcd", FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("*ab*", "1/2/3/4/abcd", FNM_LEADING_DIR));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("*ab*", "1/2/3/4/abcd", FNM_PATHNAME | FNM_LEADING_DIR));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab?", "abcd", FNM_LEADING_DIR));

  EXPECT_EQ(0, fnmatch("ab", "ab/cd", FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("ab", "ab/cd", FNM_PATHNAME | FNM_LEADING_DIR));
  // TODO(b/175302045) fix this case and enable this test.
  // EXPECT_EQ(0, fnmatch("*ab", "1/2/3/4/ab/cd", FNM_LEADING_DIR));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("*ab", "1/2/3/4/ab/cd", FNM_PATHNAME | FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("ab*", "ab/cd/ef", FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("ab*", "ab/cd/ef", FNM_PATHNAME | FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("*ab*", "1/2/3/4/ab/cd/ef", FNM_LEADING_DIR));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("*ab*", "1/2/3/4/ab/cd/ef", FNM_PATHNAME | FNM_LEADING_DIR));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab?", "ab/cd/ef", FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("ab?", "abx/cd/ef", FNM_LEADING_DIR));

  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab/", "ab/cd/ef", FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("ab/*", "ab/cd/ef", FNM_LEADING_DIR));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab/?", "ab/cd/ef", FNM_LEADING_DIR));

  // TODO(b/175302045) fix this case and enable this test.
  // EXPECT_EQ(0, fnmatch("ab*c", "ab/1/2/3/c/d/e", FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("ab?c", "ab/c/ef", FNM_LEADING_DIR));

  EXPECT_EQ(0, fnmatch("ab*c*", "ab/1/2/3/c/d/e", FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("ab?c*", "ab/c/ef", FNM_LEADING_DIR));

  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab*c/", "ab/1/2/3/c/d/e", FNM_LEADING_DIR));
  EXPECT_EQ(FNM_NOMATCH, fnmatch("ab?c/", "ab/c/ef", FNM_LEADING_DIR));

  EXPECT_EQ(0, fnmatch("ab*c/*", "ab/1/2/3/c/d/e", FNM_LEADING_DIR));
  EXPECT_EQ(0, fnmatch("ab?c/*", "ab/c/ef", FNM_LEADING_DIR));
}
