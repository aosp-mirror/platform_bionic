/*
 * Copyright (C) 2012 The Android Open Source Project
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

#define _GNU_SOURCE 1

#include <string.h>

#include <errno.h>
#include <gtest/gtest.h>
#include <malloc.h>
#include <math.h>
#include <stdint.h>

#include <algorithm>
#include <vector>

#include "buffer_tests.h"

#if defined(NOFORTIFY)
#define STRING_TEST string_nofortify
#else
#define STRING_TEST string
#endif

#if defined(__BIONIC__)
#define STRLCPY_SUPPORTED
#define STRLCAT_SUPPORTED
#endif

constexpr auto KB = 1024;
constexpr auto SMALL = 1 * KB;
constexpr auto MEDIUM = 4 * KB;
constexpr auto LARGE = 64 * KB;

static int signum(int i) {
  if (i < 0) {
    return -1;
  } else if (i > 0) {
    return 1;
  }
  return 0;
}

TEST(STRING_TEST, strerror) {
  // Valid.
  ASSERT_STREQ("Success", strerror(0));
  ASSERT_STREQ("Operation not permitted", strerror(1));

  // Invalid.
  ASSERT_STREQ("Unknown error -1", strerror(-1));
  ASSERT_STREQ("Unknown error 134", strerror(EHWPOISON + 1));
}

#if defined(__BIONIC__)
static void* ConcurrentStrErrorFn(void*) {
  bool equal = (strcmp("Unknown error 2002", strerror(2002)) == 0);
  return reinterpret_cast<void*>(equal);
}
#endif // __BIONIC__

// glibc's strerror isn't thread safe, only its strsignal.
TEST(STRING_TEST, strerror_concurrent) {
#if defined(__BIONIC__)
  const char* strerror1001 = strerror(1001);
  ASSERT_STREQ("Unknown error 1001", strerror1001);

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, ConcurrentStrErrorFn, nullptr));
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_TRUE(static_cast<bool>(result));

  ASSERT_STREQ("Unknown error 1001", strerror1001);
#else // __BIONIC__
  GTEST_SKIP() << "thread-safe strerror not available";
#endif // __BIONIC__
}

TEST(STRING_TEST, gnu_strerror_r) {
  char buf[256];

  // Note that glibc doesn't necessarily write into the buffer.

  // Valid.
  ASSERT_STREQ("Success", strerror_r(0, buf, sizeof(buf)));
#if defined(__BIONIC__)
  ASSERT_STREQ("Success", buf);
#endif
  ASSERT_STREQ("Operation not permitted", strerror_r(1, buf, sizeof(buf)));
#if defined(__BIONIC__)
  ASSERT_STREQ("Operation not permitted", buf);
#endif

  // Invalid.
  ASSERT_STREQ("Unknown error -1", strerror_r(-1, buf, sizeof(buf)));
  ASSERT_STREQ("Unknown error -1", buf);
  ASSERT_STREQ("Unknown error 1234", strerror_r(1234, buf, sizeof(buf)));
  ASSERT_STREQ("Unknown error 1234", buf);

  // Buffer too small.
  errno = 0;
  memset(buf, 0, sizeof(buf));
  ASSERT_EQ(buf, strerror_r(4567, buf, 2));
  ASSERT_STREQ("U", buf);
  // The GNU strerror_r doesn't set errno (the POSIX one sets it to ERANGE).
  ASSERT_EQ(0, errno);
}

TEST(STRING_TEST, strsignal) {
  // A regular signal.
  ASSERT_STREQ("Hangup", strsignal(1));

  // A real-time signal.
  ASSERT_STREQ("Real-time signal 14", strsignal(SIGRTMIN + 14));
  // One of the signals the C library keeps to itself.
  ASSERT_STREQ("Unknown signal 32", strsignal(__SIGRTMIN));

  // Errors.
  ASSERT_STREQ("Unknown signal -1", strsignal(-1)); // Too small.
  ASSERT_STREQ("Unknown signal 0", strsignal(0)); // Still too small.
  ASSERT_STREQ("Unknown signal 1234", strsignal(1234)); // Too large.
}

static void* ConcurrentStrSignalFn(void*) {
  bool equal = (strcmp("Unknown signal 2002", strsignal(2002)) == 0);
  return reinterpret_cast<void*>(equal);
}

TEST(STRING_TEST, strsignal_concurrent) {
  const char* strsignal1001 = strsignal(1001);
  ASSERT_STREQ("Unknown signal 1001", strsignal1001);

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, ConcurrentStrSignalFn, nullptr));
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_TRUE(static_cast<bool>(result));

  ASSERT_STREQ("Unknown signal 1001", strsignal1001);
}

// TODO: where did this number come from?
#define ITER        500

// For every length we want to test, vary and change alignment
// of allocated memory, fill it with some values, calculate
// expected result and then run function and compare what we got.
// These tests contributed by Intel Corporation.
// TODO: make these tests more intention-revealing and less random.
template<class Character>
class StringTestState {
 public:
  explicit StringTestState(size_t MAX_LEN) : MAX_LEN(MAX_LEN), align1_index_(0), align2_index_(0) {
    int max_alignment = 64;

    // TODO: fix the tests to not sometimes use twice their specified "MAX_LEN".
    glob_ptr = reinterpret_cast<Character*>(memalign(sysconf(_SC_PAGESIZE), 2 * sizeof(Character) * MAX_LEN + max_alignment));
    glob_ptr1 = reinterpret_cast<Character*>(memalign(sysconf(_SC_PAGESIZE), 2 * sizeof(Character) * MAX_LEN + max_alignment));
    glob_ptr2 = reinterpret_cast<Character*>(memalign(sysconf(_SC_PAGESIZE), 2 * sizeof(Character) * MAX_LEN + max_alignment));

    InitLenArray();

    srandom(1234);
  }

  ~StringTestState() {
    free(glob_ptr);
    free(glob_ptr1);
    free(glob_ptr2);
  }

  void BeginIterations() {
    align1_index_ = 0;
    align2_index_ = 0;

    ResetPointers();
  }

  bool HasNextIteration() {
    return (align1_index_ != (alignments_size - 1) || align2_index_ != (alignments_size - 1));
  }

  void NextIteration() {
    if (align1_index_ == (alignments_size - 1) && align2_index_ == (alignments_size - 1)) {
      return;
    }

    if (align1_index_ == (alignments_size - 1)) {
      align1_index_ = 0;
      align2_index_++;
    } else {
      align1_index_++;
    }

    ResetPointers();
  }

  const size_t MAX_LEN;
  Character *ptr, *ptr1, *ptr2;
  size_t n;
  size_t len[ITER + 1];

 private:
  static size_t alignments[];
  static size_t alignments_size;
  Character *glob_ptr, *glob_ptr1, *glob_ptr2;
  size_t align1_index_, align2_index_;

  // Calculate input lengths and fill state.len with them.
  // Test small lengths with more density than big ones. Manually push
  // smallest (0) and biggest (MAX_LEN) lengths. Avoid repeats.
  // Return number of lengths to test.
  void InitLenArray() {
    n = 0;
    len[n++] = 0;
    for (size_t i = 1; i < ITER; ++i) {
      size_t l = static_cast<size_t>(exp(log(static_cast<double>(MAX_LEN)) * i / ITER));
      if (l != len[n - 1]) {
        len[n++] = l;
      }
    }
    len[n++] = MAX_LEN;
  }

  void ResetPointers() {
    if (align1_index_ == alignments_size || align2_index_ == alignments_size) {
      ptr = ptr1 = ptr2 = nullptr;
    } else {
      ptr = glob_ptr + alignments[align1_index_];
      ptr1 = glob_ptr1 + alignments[align1_index_];
      ptr2 = glob_ptr2 + alignments[align2_index_];
    }
  }
};

template<class Character>
size_t StringTestState<Character>::alignments[] = { 24, 32, 16, 48, 0, 1, 2, 3, 4, 5, 6, 7, 11 };

template<class Character>
size_t StringTestState<Character>::alignments_size = sizeof(alignments)/sizeof(size_t);

TEST(STRING_TEST, strcat) {
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr2, '\2', state.MAX_LEN);
      state.ptr2[state.MAX_LEN - 1] = '\0';
      memcpy(state.ptr, state.ptr2, 2 * state.MAX_LEN);

      memset(state.ptr1, 'L', state.len[i]);
      state.ptr1[random() % state.len[i]] = '\0';
      state.ptr1[state.len[i] - 1] = '\0';

      strcpy(state.ptr + state.MAX_LEN - 1, state.ptr1);

      EXPECT_TRUE(strcat(state.ptr2, state.ptr1) == state.ptr2);
      EXPECT_TRUE(memcmp(state.ptr, state.ptr2, 2 * state.MAX_LEN) == 0);
    }
  }
}

// one byte target with "\0" source
TEST(STRING_TEST, strcpy2) {
  char buf[1];
  char* orig = strdup("");
  ASSERT_EQ(buf, strcpy(buf, orig));
  ASSERT_EQ('\0', buf[0]);
  free(orig);
}

// multibyte target where we under fill target
TEST(STRING_TEST, strcpy3) {
  char buf[10];
  char* orig = strdup("12345");
  memset(buf, 'A', sizeof(buf));
  ASSERT_EQ(buf, strcpy(buf, orig));
  ASSERT_STREQ("12345", buf);
  ASSERT_EQ('A',  buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
  free(orig);
}

// multibyte target where we fill target exactly
TEST(STRING_TEST, strcpy4) {
  char buf[10];
  char* orig = strdup("123456789");
  memset(buf, 'A', sizeof(buf));
  ASSERT_EQ(buf, strcpy(buf, orig));
  ASSERT_STREQ("123456789", buf);
  free(orig);
}

// one byte target with "\0" source
TEST(STRING_TEST, stpcpy2) {
  char buf[1];
  char* orig = strdup("");
  ASSERT_EQ(buf, stpcpy(buf, orig));
  ASSERT_EQ('\0', buf[0]);
  free(orig);
}

// multibyte target where we under fill target
TEST(STRING_TEST, stpcpy3) {
  char buf[10];
  char* orig = strdup("12345");
  memset(buf, 'A', sizeof(buf));
  ASSERT_EQ(buf+strlen(orig), stpcpy(buf, orig));
  ASSERT_STREQ("12345", buf);
  ASSERT_EQ('A',  buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
  free(orig);
}

// multibyte target where we fill target exactly
TEST(STRING_TEST, stpcpy4) {
  char buf[10];
  char* orig = strdup("123456789");
  memset(buf, 'A', sizeof(buf));
  ASSERT_EQ(buf+strlen(orig), stpcpy(buf, orig));
  ASSERT_STREQ("123456789", buf);
  free(orig);
}

TEST(STRING_TEST, strcat2) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strcat(buf, "01234");
  ASSERT_EQ(buf, res);
  ASSERT_STREQ("a01234", buf);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
}

TEST(STRING_TEST, strcat3) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strcat(buf, "01234567");
  ASSERT_EQ(buf, res);
  ASSERT_STREQ("a01234567", buf);
}

TEST(STRING_TEST, strncat2) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strncat(buf, "01234", sizeof(buf) - strlen(buf) - 1);
  ASSERT_EQ(buf, res);
  ASSERT_STREQ("a01234", buf);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
}

TEST(STRING_TEST, strncat3) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strncat(buf, "0123456789", 5);
  ASSERT_EQ(buf, res);
  ASSERT_STREQ("a01234", buf);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
}

TEST(STRING_TEST, strncat4) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strncat(buf, "01234567", 8);
  ASSERT_EQ(buf, res);
  ASSERT_STREQ("a01234567", buf);
}

TEST(STRING_TEST, strncat5) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strncat(buf, "01234567", 9);
  ASSERT_EQ(buf, res);
  ASSERT_STREQ("a01234567", buf);
}

TEST(STRING_TEST, strchr_with_0) {
  char buf[10];
  const char* s = "01234";
  memcpy(buf, s, strlen(s) + 1);
  EXPECT_TRUE(strchr(buf, '\0') == (buf + strlen(s)));
}

TEST(STRING_TEST, strchr_multiple) {
  char str[128];
  memset(str, 'a', sizeof(str) - 1);
  str[sizeof(str)-1] = '\0';

  // Verify that strchr finds the first occurrence of 'a' in a string
  // filled with 'a' characters. Iterate over the string putting
  // non 'a' characters at the front of the string during each iteration
  // and continue to verify that strchr can find the first occurrence
  // properly. The idea is to cover all possible alignments of the location
  // of the first occurrence of the 'a' character and which includes
  // other 'a' characters close by.
  for (size_t i = 0; i < sizeof(str) - 1; i++) {
    EXPECT_EQ(&str[i], strchr(str, 'a'));
    str[i] = 'b';
  }
}

TEST(STRING_TEST, strchr) {
  int seek_char = 'R';

  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      if (~seek_char > 0) {
        memset(state.ptr1, ~seek_char, state.len[i]);
      } else {
        memset(state.ptr1, '\1', state.len[i]);
      }
      state.ptr1[state.len[i] - 1] = '\0';

      size_t pos = random() % state.MAX_LEN;
      char* expected;
      if (pos >= state.len[i] - 1) {
        if (seek_char == 0) {
          expected = state.ptr1 + state.len[i] - 1;
        } else {
          expected = nullptr;
        }
      } else {
        state.ptr1[pos] = seek_char;
        expected = state.ptr1 + pos;
      }

      ASSERT_TRUE(strchr(state.ptr1, seek_char) == expected);
    }
  }
}

TEST(STRING_TEST, strchrnul) {
  const char* s = "01234222";
  EXPECT_TRUE(strchrnul(s, '2') == &s[2]);
  EXPECT_TRUE(strchrnul(s, '8') == (s + strlen(s)));
  EXPECT_TRUE(strchrnul(s, '\0') == (s + strlen(s)));
}

TEST(STRING_TEST, strcmp) {
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr1, 'v', state.MAX_LEN);
      memset(state.ptr2, 'n', state.MAX_LEN);
      state.ptr1[state.len[i] - 1] = '\0';
      state.ptr2[state.len[i] - 1] = '\0';

      size_t pos = 1 + (random() % (state.MAX_LEN - 1));
      int actual;
      int expected;
      if (pos >= state.len[i] - 1) {
        memcpy(state.ptr1, state.ptr2, state.len[i]);
        expected = 0;
        actual = strcmp(state.ptr1, state.ptr2);
      } else {
        memcpy(state.ptr1, state.ptr2, pos);
        if (state.ptr1[pos] > state.ptr2[pos]) {
          expected = 1;
        } else if (state.ptr1[pos] == state.ptr2[pos]) {
          state.ptr1[pos + 1] = '\0';
          state.ptr2[pos + 1] = '\0';
          expected = 0;
        } else {
          expected = -1;
        }
        actual = strcmp(state.ptr1, state.ptr2);
      }

      ASSERT_EQ(expected, signum(actual));
    }
  }
}

TEST(STRING_TEST, stpcpy) {
  StringTestState<char> state(SMALL);
  for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
    size_t pos = random() % state.MAX_LEN;

    memset(state.ptr1, '\2', pos);
    state.ptr1[pos] = '\0';
    state.ptr1[state.MAX_LEN - 1] = '\0';

    memcpy(state.ptr, state.ptr1, state.MAX_LEN);

    memset(state.ptr2, '\1', state.MAX_LEN);
    state.ptr2[state.MAX_LEN - 1] = '\0';

    memset(state.ptr + state.MAX_LEN, '\1', state.MAX_LEN);
    memcpy(state.ptr + state.MAX_LEN, state.ptr1, pos + 1);
    state.ptr[2 * state.MAX_LEN - 1] = '\0';

    ASSERT_TRUE(stpcpy(state.ptr2, state.ptr1) == state.ptr2 + strlen(state.ptr1));
    ASSERT_FALSE((memcmp(state.ptr1, state.ptr, state.MAX_LEN)) != 0 ||
                 (memcmp(state.ptr2, state.ptr + state.MAX_LEN, state.MAX_LEN) != 0));
  }
}

TEST(STRING_TEST, strcpy) {
  StringTestState<char> state(SMALL);
  for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
    size_t pos = random() % state.MAX_LEN;

    memset(state.ptr1, '\2', pos);
    state.ptr1[pos] = '\0';
    state.ptr1[state.MAX_LEN - 1] = '\0';

    memcpy(state.ptr, state.ptr1, state.MAX_LEN);

    memset(state.ptr2, '\1', state.MAX_LEN);
    state.ptr2[state.MAX_LEN - 1] = '\0';

    memset(state.ptr + state.MAX_LEN, '\1', state.MAX_LEN);
    memcpy(state.ptr + state.MAX_LEN, state.ptr1, pos + 1);
    state.ptr[2 * state.MAX_LEN - 1] = '\0';

    ASSERT_TRUE(strcpy(state.ptr2, state.ptr1) == state.ptr2);
    ASSERT_FALSE((memcmp(state.ptr1, state.ptr, state.MAX_LEN)) != 0 ||
                 (memcmp(state.ptr2, state.ptr + state.MAX_LEN, state.MAX_LEN) != 0));
  }
}

TEST(STRING_TEST, strlcat) {
#if defined(STRLCAT_SUPPORTED)
  StringTestState<char> state(SMALL);
  for (size_t i = 0; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr2, '\2', state.MAX_LEN + state.len[i]);
      state.ptr2[state.MAX_LEN - 1] = '\0';
      memcpy(state.ptr, state.ptr2, state.MAX_LEN + state.len[i]);

      size_t pos = random() % state.MAX_LEN;
      memset(state.ptr1, '\3', pos);
      state.ptr1[pos] = '\0';
      if (pos < state.len[i]) {
        memcpy(state.ptr + state.MAX_LEN - 1, state.ptr1, pos + 1);
      } else {
        memcpy(state.ptr + state.MAX_LEN - 1, state.ptr1, state.len[i]);
        state.ptr[state.MAX_LEN + state.len[i] - 1] = '\0';
      }

      strlcat(state.ptr2, state.ptr1, state.MAX_LEN + state.len[i]);

      ASSERT_TRUE(memcmp(state.ptr, state.ptr2, state.MAX_LEN + state.len[i]) == 0);
    }
  }
#else
  GTEST_SKIP() << "strlcat not available";
#endif
}

TEST(STRING_TEST, strlcpy) {
#if defined(STRLCPY_SUPPORTED)
  StringTestState<char> state(SMALL);
  for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
    int rand = 'O';
    memset(state.ptr1, rand, state.MAX_LEN);

    size_t pos = random() % state.MAX_LEN;
    if (pos < state.MAX_LEN) {
      state.ptr1[pos] = '\0';
    }
    memcpy(state.ptr, state.ptr1, state.MAX_LEN);

    memset(state.ptr2, 'I', state.MAX_LEN);
    memcpy(state.ptr + state.MAX_LEN, state.ptr2, state.MAX_LEN);

    if (pos > state.MAX_LEN - 1) {
      memcpy(state.ptr + state.MAX_LEN, state.ptr1, state.MAX_LEN);
      state.ptr[2 * state.MAX_LEN - 1] = '\0';
    } else {
      memcpy(state.ptr + state.MAX_LEN, state.ptr1, pos + 1);
    }

    ASSERT_EQ(strlcpy(state.ptr2, state.ptr1, state.MAX_LEN), strlen(state.ptr1));
    ASSERT_FALSE((memcmp(state.ptr1, state.ptr, state.MAX_LEN) != 0) ||
                 (memcmp(state.ptr2, state.ptr + state.MAX_LEN, state.MAX_LEN) != 0));
  }
#else
  GTEST_SKIP() << "strlcpy not available";
#endif
}

TEST(STRING_TEST, strncat) {
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr2, '\2', state.MAX_LEN);
      state.ptr2[state.MAX_LEN - 1] = '\0';
      memcpy(state.ptr, state.ptr2, 2 * state.MAX_LEN);

      memset(state.ptr1, 'I', state.len[i]);
      state.ptr1[random() % state.len[i]] = '\0';
      state.ptr1[state.len[i] - 1] = '\0';

      size_t pos = strlen(state.ptr1);

      size_t actual = random() % state.len[i];
      strncpy(state.ptr + state.MAX_LEN - 1, state.ptr1, std::min(actual, pos));
      state.ptr[state.MAX_LEN + std::min(actual, pos) - 1] = '\0';

      ASSERT_TRUE(strncat(state.ptr2, state.ptr1, actual) == state.ptr2);
      ASSERT_EQ(memcmp(state.ptr, state.ptr2, 2 * state.MAX_LEN), 0);
    }
  }
}

TEST(STRING_TEST, strncmp) {
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr1, 'v', state.MAX_LEN);
      memset(state.ptr2, 'n', state.MAX_LEN);
      state.ptr1[state.len[i] - 1] = '\0';
      state.ptr2[state.len[i] - 1] = '\0';

      size_t pos = 1 + (random() % (state.MAX_LEN - 1));
      int actual;
      int expected;
      if (pos >= state.len[i] - 1) {
        memcpy(state.ptr1, state.ptr2, state.len[i]);
        expected = 0;
        actual = strncmp(state.ptr1, state.ptr2, state.len[i]);
      } else {
        memcpy(state.ptr1, state.ptr2, pos);
        if (state.ptr1[pos] > state.ptr2[pos]) {
          expected = 1;
        } else if (state.ptr1[pos] == state.ptr2[pos]) {
          state.ptr1[pos + 1] = '\0';
          state.ptr2[pos + 1] = '\0';
          expected = 0;
        } else {
          expected = -1;
        }
        actual = strncmp(state.ptr1, state.ptr2, state.len[i]);
      }

      ASSERT_EQ(expected, signum(actual));
    }
  }
}

TEST(STRING_TEST, stpncpy) {
  StringTestState<char> state(SMALL);
  for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
    memset(state.ptr1, 'J', state.MAX_LEN);
    // Choose a random size for our src buffer.
    size_t ptr1_len = random() % state.MAX_LEN;
    state.ptr1[ptr1_len] = '\0';
    // Copy ptr1 into ptr, used to verify that ptr1 does not get modified.
    memcpy(state.ptr, state.ptr1, state.MAX_LEN);
    // Init ptr2 to a set value.
    memset(state.ptr2, '\1', state.MAX_LEN);

    // Choose a random amount of data to copy.
    size_t copy_len = random() % state.MAX_LEN;

    // Set the second half of ptr to the expected pattern in ptr2.
    memset(state.ptr + state.MAX_LEN, '\1', state.MAX_LEN);
    memcpy(state.ptr + state.MAX_LEN, state.ptr1, copy_len);
    size_t expected_end;
    if (copy_len > ptr1_len) {
      memset(state.ptr + state.MAX_LEN + ptr1_len, '\0', copy_len - ptr1_len);
      expected_end = ptr1_len;
    } else {
      expected_end = copy_len;
    }

    ASSERT_EQ(state.ptr2 + expected_end, stpncpy(state.ptr2, state.ptr1, copy_len));

    // Verify ptr1 was not modified.
    ASSERT_EQ(0, memcmp(state.ptr1, state.ptr, state.MAX_LEN));
    // Verify ptr2 contains the expected data.
    ASSERT_EQ(0, memcmp(state.ptr2, state.ptr + state.MAX_LEN, state.MAX_LEN));
  }
}

TEST(STRING_TEST, strncpy) {
  StringTestState<char> state(SMALL);
  for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
    // Choose a random value to fill the string, except \0 (string terminator),
    // or \1 (guarantees it's different from anything in ptr2).
    memset(state.ptr1, 'K', state.MAX_LEN);
    // Choose a random size for our src buffer.
    size_t ptr1_len = random() % state.MAX_LEN;
    state.ptr1[ptr1_len] = '\0';
    // Copy ptr1 into ptr, used to verify that ptr1 does not get modified.
    memcpy(state.ptr, state.ptr1, state.MAX_LEN);
    // Init ptr2 to a set value.
    memset(state.ptr2, '\1', state.MAX_LEN);

    // Choose a random amount of data to copy.
    size_t copy_len = random() % state.MAX_LEN;

    // Set the second half of ptr to the expected pattern in ptr2.
    memset(state.ptr + state.MAX_LEN, '\1', state.MAX_LEN);
    memcpy(state.ptr + state.MAX_LEN, state.ptr1, copy_len);
    size_t expected_end;
    if (copy_len > ptr1_len) {
      memset(state.ptr + state.MAX_LEN + ptr1_len, '\0', copy_len - ptr1_len);
      expected_end = ptr1_len;
    } else {
      expected_end = copy_len;
    }

    ASSERT_EQ(state.ptr2 + expected_end, stpncpy(state.ptr2, state.ptr1, copy_len));

    // Verify ptr1 was not modified.
    ASSERT_EQ(0, memcmp(state.ptr1, state.ptr, state.MAX_LEN));
    // Verify ptr2 contains the expected data.
    ASSERT_EQ(0, memcmp(state.ptr2, state.ptr + state.MAX_LEN, state.MAX_LEN));
  }
}

TEST(STRING_TEST, strrchr) {
  int seek_char = 'M';
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      if (~seek_char > 0) {
        memset(state.ptr1, ~seek_char, state.len[i]);
      } else {
        memset(state.ptr1, '\1', state.len[i]);
      }
      state.ptr1[state.len[i] - 1] = '\0';

      size_t pos = random() % state.MAX_LEN;
      char* expected;
      if (pos >= state.len[i] - 1) {
        if (seek_char == 0) {
          expected = state.ptr1 + state.len[i] - 1;
        } else {
          expected = nullptr;
        }
      } else {
        state.ptr1[pos] = seek_char;
        expected = state.ptr1 + pos;
      }

      ASSERT_TRUE(strrchr(state.ptr1, seek_char) == expected);
    }
  }
}

TEST(STRING_TEST, memchr) {
  int seek_char = 'N';
  StringTestState<char> state(SMALL);
  for (size_t i = 0; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr1, ~seek_char, state.len[i]);

      size_t pos = random() % state.MAX_LEN;
      char* expected;
      if (pos >= state.len[i]) {
        expected = nullptr;
      } else {
        state.ptr1[pos] = seek_char;
        expected = state.ptr1 + pos;
      }

      ASSERT_TRUE(memchr(state.ptr1, seek_char, state.len[i]) == expected);
    }
  }
}

TEST(STRING_TEST, memchr_zero) {
  uint8_t* buffer;
  ASSERT_EQ(0, posix_memalign(reinterpret_cast<void**>(&buffer), 64, 64));
  memset(buffer, 10, 64);
  ASSERT_TRUE(nullptr == memchr(buffer, 5, 0));
  ASSERT_TRUE(nullptr == memchr(buffer, 10, 0));
}

TEST(STRING_TEST, memrchr) {
  int seek_char = 'P';
  StringTestState<char> state(SMALL);
  for (size_t i = 0; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr1, ~seek_char, state.len[i]);

      size_t pos = random() % state.MAX_LEN;
      char* expected;
      if (pos >= state.len[i]) {
        expected = nullptr;
      } else {
        state.ptr1[pos] = seek_char;
        expected = state.ptr1 + pos;
      }

      ASSERT_TRUE(memrchr(state.ptr1, seek_char, state.len[i]) == expected);
    }
  }
}

TEST(STRING_TEST, memcmp) {
  StringTestState<char> state(SMALL);
  for (size_t i = 0; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      int c1 = 'A';
      int c2 = 'N';
      memset(state.ptr1, c1, state.MAX_LEN);
      memset(state.ptr2, c1, state.MAX_LEN);

      int pos = (state.len[i] == 0) ? 0 : (random() % state.len[i]);
      state.ptr2[pos] = c2;

      int expected = (static_cast<int>(c1) - static_cast<int>(c2));
      int actual = memcmp(state.ptr1, state.ptr2, state.MAX_LEN);

      ASSERT_EQ(signum(expected), signum(actual));
    }
  }
}

TEST(STRING_TEST, wmemcmp) {
  StringTestState<wchar_t> state(SMALL);

  for (size_t i = 0; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      long long mask = ((long long) 1 << 8 * sizeof(wchar_t)) - 1;
      int c1 = rand() & mask;
      int c2 = rand() & mask;
      wmemset(state.ptr1, c1, state.MAX_LEN);
      wmemset(state.ptr2, c1, state.MAX_LEN);

      int pos = (state.len[i] == 0) ? 0 : (random() % state.len[i]);
      state.ptr2[pos] = c2;

      int expected = (static_cast<int>(c1) - static_cast<int>(c2));
      int actual = wmemcmp(state.ptr1, state.ptr2, (size_t) state.MAX_LEN);

      ASSERT_EQ(signum(expected), signum(actual));
    }
  }
}

TEST(STRING_TEST, memcpy) {
  StringTestState<char> state(LARGE);
  int rand = 4;
  for (size_t i = 0; i < state.n - 1; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      size_t pos = random() % (state.MAX_LEN - state.len[i]);

      memset(state.ptr1, rand, state.len[i]);
      memset(state.ptr1 + state.len[i], ~rand, state.MAX_LEN - state.len[i]);

      memset(state.ptr2, rand, state.len[i]);
      memset(state.ptr2 + state.len[i], ~rand, state.MAX_LEN - state.len[i]);
      memset(state.ptr2 + pos, '\0', state.len[i]);

      ASSERT_FALSE(memcpy(state.ptr2 + pos, state.ptr1 + pos, state.len[i]) != state.ptr2 + pos);
      ASSERT_EQ(0, memcmp(state.ptr1, state.ptr2, state.MAX_LEN));
    }
  }
}

TEST(STRING_TEST, memset) {
  StringTestState<char> state(LARGE);
  char ch = 'P';
  for (size_t i = 0; i < state.n - 1; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr1, ~ch, state.MAX_LEN);
      memcpy(state.ptr2, state.ptr1, state.MAX_LEN);

      size_t pos = random () % (state.MAX_LEN - state.len[i]);
      for (size_t k = pos; k < pos + state.len[i]; k++) {
        state.ptr1[k] = ch;
      }

      ASSERT_TRUE(memset(state.ptr2 + pos, ch, state.len[i]) == state.ptr2 + pos);

      ASSERT_EQ(0, memcmp(state.ptr1, state.ptr2, state.MAX_LEN));
    }
  }
}

TEST(STRING_TEST, memmove) {
  StringTestState<char> state(LARGE);
  for (size_t i = 0; i < state.n - 1; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr1, 'Q', 2 * state.MAX_LEN);

      size_t pos = random() % (state.MAX_LEN - state.len[i]);

      memset(state.ptr1, 'R', state.len[i]);
      memcpy(state.ptr2, state.ptr1, 2 * state.MAX_LEN);
      memcpy(state.ptr, state.ptr1, state.len[i]);
      memcpy(state.ptr1 + pos, state.ptr, state.len[i]);

      ASSERT_TRUE(memmove(state.ptr2 + pos, state.ptr2, state.len[i]) == state.ptr2 + pos);
      ASSERT_EQ(0, memcmp(state.ptr2, state.ptr1, 2 * state.MAX_LEN));
    }
  }
}

TEST(STRING_TEST, memmove_cache_size) {
  size_t len = 600000;
  int max_alignment = 31;
  int alignments[] = {0, 5, 11, 29, 30};
  char* ptr = reinterpret_cast<char*>(malloc(sizeof(char) * len));
  char* ptr1 = reinterpret_cast<char*>(malloc(2 * sizeof(char) * len));
  char* glob_ptr2 = reinterpret_cast<char*>(malloc(2 * sizeof(char) * len + max_alignment));
  size_t pos = 64;

  ASSERT_TRUE(ptr != nullptr);
  ASSERT_TRUE(ptr1 != nullptr);
  ASSERT_TRUE(glob_ptr2 != nullptr);

  for (int i = 0; i < 5; i++) {
    char* ptr2 = glob_ptr2 + alignments[i];
    memset(ptr1, 'S', 2 * len);
    memset(ptr1, 'T', len);
    memcpy(ptr2, ptr1, 2 * len);
    memcpy(ptr, ptr1, len);
    memcpy(ptr1 + pos, ptr, len);

    ASSERT_TRUE(memmove(ptr2 + pos, ptr, len) == ptr2 + pos);
    ASSERT_EQ(0, memcmp(ptr2, ptr1, 2 * len));
  }
  free(ptr);
  free(ptr1);
  free(glob_ptr2);
}

static void verify_memmove(char* src_copy, char* dst, char* src, size_t size) {
  memset(dst, 0, size);
  memcpy(src, src_copy, size);
  ASSERT_EQ(dst, memmove(dst, src, size));
  ASSERT_EQ(0, memcmp(dst, src_copy, size));
}

#define MEMMOVE_DATA_SIZE (1024*1024*3)

TEST(STRING_TEST, memmove_check) {
  char* buffer = reinterpret_cast<char*>(malloc(MEMMOVE_DATA_SIZE));
  ASSERT_TRUE(buffer != nullptr);

  char* src_data = reinterpret_cast<char*>(malloc(MEMMOVE_DATA_SIZE));
  ASSERT_TRUE(src_data != nullptr);
  // Initialize to a known pattern to copy into src for each test and
  // to compare dst against.
  for (size_t i = 0; i < MEMMOVE_DATA_SIZE; i++) {
    src_data[i] = (i + 1) % 255;
  }

  // Check all different dst offsets between 0 and 127 inclusive.
  char* src = buffer;
  for (size_t i = 0; i < 127; i++) {
    char* dst = buffer + 256 + i;
    // Small copy.
    verify_memmove(src_data, dst, src, 1024);

    // Medium copy.
    verify_memmove(src_data, dst, src, 64 * 1024);

    // Medium copy.
    verify_memmove(src_data, dst, src, 1024 * 1024 + 128 * 1024);
  }

  // Check all leftover size offsets between 1 and 127 inclusive.
  char* dst = buffer + 256;
  src = buffer;
  for (size_t size = 1; size < 127; size++) {
    // Small copy.
    verify_memmove(src_data, dst, src, 1024);

    // Medium copy.
    verify_memmove(src_data, dst, src, 64 * 1024);

    // Large copy.
    verify_memmove(src_data, dst, src, 1024 * 1024 + 128 * 1024);
  }
}

TEST(STRING_TEST, bcopy) {
  StringTestState<char> state(LARGE);
  for (size_t i = 0; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr1, '4', state.MAX_LEN);
      memset(state.ptr1 + state.MAX_LEN, 'a', state.MAX_LEN);
      memcpy(state.ptr2, state.ptr1, 2 * state.MAX_LEN);

      size_t start = random() % (2 * state.MAX_LEN - state.len[i]);
      memcpy(state.ptr2 + start, state.ptr1, state.len[i]);

      bcopy(state.ptr1, state.ptr1 + start, state.len[i]);
      ASSERT_EQ(0, memcmp(state.ptr1, state.ptr2, 2 * state.MAX_LEN));
    }
  }
}

TEST(STRING_TEST, bzero) {
  StringTestState<char> state(LARGE);
  for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
    memset(state.ptr1, 'R', state.MAX_LEN);

    size_t start = random() % state.MAX_LEN;
    size_t end = start + random() % (state.MAX_LEN - start);

    memcpy(state.ptr2, state.ptr1, start);
    memset(state.ptr2 + start, '\0', end - start);
    memcpy(state.ptr2 + end, state.ptr1 + end, state.MAX_LEN - end);

    bzero(state.ptr1 + start, end - start);

    ASSERT_EQ(0, memcmp(state.ptr1, state.ptr2, state.MAX_LEN));
  }
}

static void DoMemcpyTest(uint8_t* src, uint8_t* dst, size_t len) {
  memset(src, (len % 255) + 1, len);
  memset(dst, 0, len);

  ASSERT_EQ(dst, memcpy(dst, src, len));
  ASSERT_TRUE(memcmp(src, dst, len) == 0);
}

TEST(STRING_TEST, memcpy_align) {
  RunSrcDstBufferAlignTest(LARGE, DoMemcpyTest);
}

TEST(STRING_TEST, memcpy_overread) {
  RunSrcDstBufferOverreadTest(DoMemcpyTest);
}

static void DoMemmoveTest(uint8_t* src, uint8_t* dst, size_t len) {
  memset(src, (len % 255) + 1, len);
  memset(dst, 0, len);

  ASSERT_EQ(dst, memmove(dst, src, len));
  ASSERT_TRUE(memcmp(src, dst, len) == 0);
}

TEST(STRING_TEST, memmove_align) {
  RunSrcDstBufferAlignTest(LARGE, DoMemmoveTest);
}

TEST(STRING_TEST, memmove_overread) {
  RunSrcDstBufferOverreadTest(DoMemmoveTest);
}

static void DoMemsetTest(uint8_t* buf, size_t len) {
  for (size_t i = 0; i < len; i++) {
    buf[i] = 0;
  }
  int value = (len % 255) + 1;
  ASSERT_EQ(buf, memset(buf, value, len));
  for (size_t i = 0; i < len; i++) {
    ASSERT_EQ(value, buf[i]);
  }
}

TEST(STRING_TEST, memset_align) {
  RunSingleBufferAlignTest(LARGE, DoMemsetTest);
}

static void DoStrlenTest(uint8_t* buf, size_t len) {
  if (len >= 1) {
    memset(buf, (32 + (len % 96)), len - 1);
    buf[len-1] = '\0';
    ASSERT_EQ(len-1, strlen(reinterpret_cast<char*>(buf)));
  }
}

TEST(STRING_TEST, strlen_align) {
  RunSingleBufferAlignTest(LARGE, DoStrlenTest);
}

TEST(STRING_TEST, strlen_overread) {
  RunSingleBufferOverreadTest(DoStrlenTest);
}

static void DoStrcpyTest(uint8_t* src, uint8_t* dst, size_t len) {
  if (len >= 1) {
    memset(src, (32 + (len % 96)), len - 1);
    src[len-1] = '\0';
    memset(dst, 0, len);
    ASSERT_EQ(dst, reinterpret_cast<uint8_t*>(strcpy(reinterpret_cast<char*>(dst),
                                                     reinterpret_cast<char*>(src))));
    ASSERT_TRUE(memcmp(src, dst, len) == 0);
  }
}

TEST(STRING_TEST, strcpy_align) {
  RunSrcDstBufferAlignTest(LARGE, DoStrcpyTest);
}

TEST(STRING_TEST, strcpy_overread) {
  RunSrcDstBufferOverreadTest(DoStrcpyTest);
}

#if defined(STRLCPY_SUPPORTED)
static void DoStrlcpyTest(uint8_t* src, uint8_t* dst, size_t len) {
  if (len >= 1) {
    memset(src, (32 + (len % 96)), len - 1);
    src[len-1] = '\0';
    memset(dst, 0, len);
    ASSERT_EQ(len-1, strlcpy(reinterpret_cast<char*>(dst),
                             reinterpret_cast<char*>(src), len));
    ASSERT_TRUE(memcmp(src, dst, len) == 0);
  }
}
#endif

TEST(STRING_TEST, strlcpy_align) {
#if defined(STRLCPY_SUPPORTED)
  RunSrcDstBufferAlignTest(LARGE, DoStrlcpyTest);
#else
  GTEST_SKIP() << "strlcpy not available";
#endif
}

TEST(STRING_TEST, strlcpy_overread) {
#if defined(STRLCPY_SUPPORTED)
  RunSrcDstBufferOverreadTest(DoStrlcpyTest);
#else
  GTEST_SKIP() << "strlcpy not available";
#endif
}


static void DoStpcpyTest(uint8_t* src, uint8_t* dst, size_t len) {
  if (len >= 1) {
    memset(src, (32 + (len % 96)), len - 1);
    src[len-1] = '\0';
    memset(dst, 0, len);
    ASSERT_EQ(dst+len-1, reinterpret_cast<uint8_t*>(stpcpy(reinterpret_cast<char*>(dst),
                                                           reinterpret_cast<char*>(src))));
    ASSERT_TRUE(memcmp(src, dst, len) == 0);
  }
}

TEST(STRING_TEST, stpcpy_align) {
  RunSrcDstBufferAlignTest(LARGE, DoStpcpyTest);
}

TEST(STRING_TEST, stpcpy_overread) {
  RunSrcDstBufferOverreadTest(DoStpcpyTest);
}

// Use our own incrementer to cut down on the total number of calls.
static size_t LargeSetIncrement(size_t len) {
  if (len >= 4096) {
    return 4096;
  } else if (len >= 1024) {
    return 1024;
  } else if (len >= 256) {
    return 256;
  }
  return 1;
}

#define STRCAT_DST_LEN  64

static void DoStrcatTest(uint8_t* src, uint8_t* dst, size_t len) {
  if (len >= 1) {
    int value = 32 + (len % 96);
    memset(src, value, len - 1);
    src[len-1] = '\0';

    if (len >= STRCAT_DST_LEN) {
      // Create a small buffer for doing quick compares in each loop.
      uint8_t cmp_buf[STRCAT_DST_LEN];
      // Make sure dst string contains a different value then the src string.
      int value2 = 32 + (value + 2) % 96;
      memset(cmp_buf, value2, sizeof(cmp_buf));

      for (size_t i = 1; i <= STRCAT_DST_LEN;) {
        memset(dst, value2, i-1);
        memset(dst+i-1, 0, len-i);
        src[len-i] = '\0';
        ASSERT_EQ(dst, reinterpret_cast<uint8_t*>(strcat(reinterpret_cast<char*>(dst),
                                                         reinterpret_cast<char*>(src))));
        ASSERT_TRUE(memcmp(dst, cmp_buf, i-1) == 0);
        ASSERT_TRUE(memcmp(src, dst+i-1, len-i+1) == 0);
        // This is an expensive loop, so don't loop through every value,
        // get to a certain size and then start doubling.
        if (i < 16) {
          i++;
        } else {
          i <<= 1;
        }
      }
    } else {
      dst[0] = '\0';
      ASSERT_EQ(dst, reinterpret_cast<uint8_t*>(strcat(reinterpret_cast<char*>(dst),
                                                       reinterpret_cast<char*>(src))));
      ASSERT_TRUE(memcmp(src, dst, len) == 0);
    }
  }
}

TEST(STRING_TEST, strcat_align) {
  RunSrcDstBufferAlignTest(MEDIUM, DoStrcatTest, LargeSetIncrement);
}

TEST(STRING_TEST, strcat_overread) {
  RunSrcDstBufferOverreadTest(DoStrcatTest);
}

#if defined(STRLCAT_SUPPORTED)
static void DoStrlcatTest(uint8_t* src, uint8_t* dst, size_t len) {
  if (len >= 1) {
    int value = 32 + (len % 96);
    memset(src, value, len - 1);
    src[len-1] = '\0';

    if (len >= STRCAT_DST_LEN) {
      // Create a small buffer for doing quick compares in each loop.
      uint8_t cmp_buf[STRCAT_DST_LEN];
      // Make sure dst string contains a different value then the src string.
      int value2 = 32 + (value + 2) % 96;
      memset(cmp_buf, value2, sizeof(cmp_buf));

      for (size_t i = 1; i <= STRCAT_DST_LEN;) {
        memset(dst, value2, i-1);
        memset(dst+i-1, 0, len-i);
        src[len-i] = '\0';
        ASSERT_EQ(len-1, strlcat(reinterpret_cast<char*>(dst),
                                 reinterpret_cast<char*>(src), len));
        ASSERT_TRUE(memcmp(dst, cmp_buf, i-1) == 0);
        ASSERT_TRUE(memcmp(src, dst+i-1, len-i+1) == 0);
        // This is an expensive loop, so don't loop through every value,
        // get to a certain size and then start doubling.
        if (i < 16) {
          i++;
        } else {
          i <<= 1;
        }
      }
    } else {
      dst[0] = '\0';
      ASSERT_EQ(len-1, strlcat(reinterpret_cast<char*>(dst),
                               reinterpret_cast<char*>(src), len));
      ASSERT_TRUE(memcmp(src, dst, len) == 0);
    }
  }
}
#endif

TEST(STRING_TEST, strlcat_align) {
#if defined(STRLCAT_SUPPORTED)
  RunSrcDstBufferAlignTest(MEDIUM, DoStrlcatTest, LargeSetIncrement);
#else
  GTEST_SKIP() << "strlcat not available";
#endif
}

TEST(STRING_TEST, strlcat_overread) {
#if defined(STRLCAT_SUPPORTED)
  RunSrcDstBufferOverreadTest(DoStrlcatTest);
#else
  GTEST_SKIP() << "strlcat not available";
#endif
}

static void DoStrcmpTest(uint8_t* buf1, uint8_t* buf2, size_t len) {
  if (len >= 1) {
    memset(buf1, (32 + (len % 96)), len - 1);
    buf1[len-1] = '\0';
    memset(buf2, (32 + (len % 96)), len - 1);
    buf2[len-1] = '\0';
    ASSERT_EQ(0, strcmp(reinterpret_cast<char*>(buf1),
                        reinterpret_cast<char*>(buf2)));
  }
}

static void DoStrcmpFailTest(uint8_t* buf1, uint8_t* buf2, size_t len1, size_t len2) {
  // Do string length differences.
  int c = (32 + (len1 % 96));
  memset(buf1, c, len1 - 1);
  buf1[len1-1] = '\0';
  memset(buf2, c, len2 - 1);
  buf2[len2-1] = '\0';
  ASSERT_NE(0, strcmp(reinterpret_cast<char*>(buf1),
                      reinterpret_cast<char*>(buf2)));

  // Do single character differences.
  size_t len;
  if (len1 > len2) {
    len = len2;
  } else {
    len = len1;
  }
  // Need at least a two character buffer to do this test.
  if (len > 1) {
    buf1[len-1] = '\0';
    buf2[len-1] = '\0';
    int diff_c = (c + 1) % 96;

    buf1[len-2] = diff_c;
    ASSERT_NE(0, strcmp(reinterpret_cast<char*>(buf1),
                        reinterpret_cast<char*>(buf2)));

    buf1[len-2] = c;
    buf2[len-2] = diff_c;
    ASSERT_NE(0, strcmp(reinterpret_cast<char*>(buf1),
                        reinterpret_cast<char*>(buf2)));
  }
}

TEST(STRING_TEST, strcmp_align) {
  RunCmpBufferAlignTest(MEDIUM, DoStrcmpTest, DoStrcmpFailTest, LargeSetIncrement);
}

TEST(STRING_TEST, strcmp_overread) {
  RunCmpBufferOverreadTest(DoStrcmpTest, DoStrcmpFailTest);
}

static void DoMemcmpTest(uint8_t* buf1, uint8_t* buf2, size_t len) {
  memset(buf1, len+1, len);
  memset(buf2, len+1, len);
  ASSERT_EQ(0, memcmp(buf1, buf2, len));
}

static void DoMemcmpFailTest(uint8_t* buf1, uint8_t* buf2, size_t len1, size_t len2) {
  size_t len;
  if (len1 > len2) {
    len = len2;
  } else {
    len = len1;
  }

  memset(buf1, len2+1, len);
  buf1[len-1] = len2;
  memset(buf2, len2+1, len);
  ASSERT_NE(0, memcmp(buf1, buf2, len));

  buf1[len-1] = len2+1;
  buf2[len-1] = len2;
  ASSERT_NE(0, memcmp(buf1, buf2, len));
}

TEST(STRING_TEST, memcmp_align) {
  RunCmpBufferAlignTest(MEDIUM, DoMemcmpTest, DoMemcmpFailTest, LargeSetIncrement);
}

TEST(STRING_TEST, memcmp_overread) {
  RunCmpBufferOverreadTest(DoMemcmpTest, DoMemcmpFailTest);
}

static void DoMemchrTest(uint8_t* buf, size_t len) {
  if (len >= 1) {
    int value = len % 128;
    int search_value = (len % 128) + 1;
    memset(buf, value, len);
    // The buffer does not contain the search value.
    ASSERT_EQ(nullptr, memchr(buf, search_value, len));
    if (len >= 2) {
      buf[0] = search_value;
      // The search value is the first element in the buffer.
      ASSERT_EQ(&buf[0], memchr(buf, search_value, len));

      buf[0] = value;
      buf[len - 1] = search_value;
      // The search value is the last element in the buffer.
      ASSERT_EQ(&buf[len - 1], memchr(buf, search_value, len));
    }
  }
}

TEST(STRING_TEST, memchr_align) {
  RunSingleBufferAlignTest(MEDIUM, DoMemchrTest);
}

TEST(STRING_TEST, memchr_overread) {
  RunSingleBufferOverreadTest(DoMemchrTest);
}

static void DoStrchrTest(uint8_t* buf, size_t len) {
  if (len >= 1) {
    char value = 32 + (len % 96);
    char search_value = 33 + (len % 96);
    memset(buf, value, len - 1);
    buf[len - 1] = '\0';
    // The buffer does not contain the search value.
    ASSERT_EQ(nullptr, strchr(reinterpret_cast<char*>(buf), search_value));
    // Search for the special '\0' character.
    ASSERT_EQ(reinterpret_cast<char*>(&buf[len - 1]), strchr(reinterpret_cast<char*>(buf), '\0'));
    if (len >= 2) {
      buf[0] = search_value;
      // The search value is the first element in the buffer.
      ASSERT_EQ(reinterpret_cast<char*>(&buf[0]), strchr(reinterpret_cast<char*>(buf),
                                                         search_value));

      buf[0] = value;
      buf[len - 2] = search_value;
      // The search value is the second to last element in the buffer.
      // The last element is the '\0' character.
      ASSERT_EQ(reinterpret_cast<char*>(&buf[len - 2]), strchr(reinterpret_cast<char*>(buf),
                                                               search_value));
    }
  }
}

TEST(STRING_TEST, strchr_align) {
  RunSingleBufferAlignTest(MEDIUM, DoStrchrTest);
}

TEST(STRING_TEST, strchr_overread) {
  RunSingleBufferOverreadTest(DoStrchrTest);
}

static void DoStrrchrTest(uint8_t* buf, size_t len) {
  if (len >= 1) {
    char value = 32 + (len % 96);
    char search_value = 33 + (len % 96);
    memset(buf, value, len - 1);
    buf[len - 1] = '\0';
    // The buffer does not contain the search value.
    ASSERT_EQ(nullptr, strrchr(reinterpret_cast<char*>(buf), search_value));
    // Search for the special '\0' character.
    ASSERT_EQ(reinterpret_cast<char*>(&buf[len - 1]), strrchr(reinterpret_cast<char*>(buf), '\0'));
    if (len >= 2) {
      buf[0] = search_value;
      // The search value is the first element in the buffer.
      ASSERT_EQ(reinterpret_cast<char*>(&buf[0]), strrchr(reinterpret_cast<char*>(buf),
                                                          search_value));

      buf[0] = value;
      buf[len - 2] = search_value;
      // The search value is the second to last element in the buffer.
      // The last element is the '\0' character.
      ASSERT_EQ(reinterpret_cast<char*>(&buf[len - 2]), strrchr(reinterpret_cast<char*>(buf),
                                                                search_value));
    }
  }
}

TEST(STRING_TEST, strrchr_align) {
  RunSingleBufferAlignTest(MEDIUM, DoStrrchrTest);
}

TEST(STRING_TEST, strrchr_overread) {
  RunSingleBufferOverreadTest(DoStrrchrTest);
}

static void TestBasename(const char* in, const char* expected_out) {
  errno = 0;
  const char* out = basename(in);
  ASSERT_STREQ(expected_out, out) << in;
  ASSERT_EQ(0, errno) << in;
}

TEST(STRING_TEST, __gnu_basename) {
  TestBasename("", "");
  TestBasename("/usr/lib", "lib");
  TestBasename("/usr/", "");
  TestBasename("usr", "usr");
  TestBasename("/", "");
  TestBasename(".", ".");
  TestBasename("..", "..");
  TestBasename("///", "");
  TestBasename("//usr//lib//", "");
}

TEST(STRING_TEST, strnlen_147048) {
  // https://code.google.com/p/android/issues/detail?id=147048
  char stack_src[64] = {0};
  EXPECT_EQ(0U, strnlen(stack_src, 1024*1024*1024));
  char* heap_src = new char[1];
  *heap_src = '\0';
  EXPECT_EQ(0U, strnlen(heap_src, 1024*1024*1024));
  delete[] heap_src;
}

TEST(STRING_TEST, strnlen_74741) {
  ASSERT_EQ(4U, strnlen("test", SIZE_MAX));
}

TEST(STRING_TEST, mempcpy) {
  char dst[6];
  ASSERT_EQ(&dst[4], reinterpret_cast<char*>(mempcpy(dst, "hello", 4)));
}

// clang depends on the fact that a memcpy where src and dst is the same
// still operates correctly. This test verifies that this assumption
// holds true.
// See https://llvm.org/bugs/show_bug.cgi?id=11763 for more information.
static std::vector<uint8_t> g_memcpy_same_buffer;

static void DoMemcpySameTest(uint8_t* buffer, size_t len) {
  memcpy(buffer, g_memcpy_same_buffer.data(), len);
  ASSERT_EQ(buffer, memcpy(buffer, buffer, len));
  ASSERT_TRUE(memcmp(buffer, g_memcpy_same_buffer.data(), len) == 0);
}

TEST(STRING_TEST, memcpy_src_dst_same) {
  g_memcpy_same_buffer.resize(MEDIUM);
  for (size_t i = 0; i < MEDIUM; i++) {
    g_memcpy_same_buffer[i] = i;
  }
  RunSingleBufferAlignTest(MEDIUM, DoMemcpySameTest);
}

TEST(STRING_TEST, memmem_strstr_empty_needle) {
  const char* some_haystack = "haystack";
  const char* empty_haystack = "";

  ASSERT_EQ(some_haystack, memmem(some_haystack, 8, "", 0));
  ASSERT_EQ(empty_haystack, memmem(empty_haystack, 0, "", 0));

  ASSERT_EQ(some_haystack, strstr(some_haystack, ""));
  ASSERT_EQ(empty_haystack, strstr(empty_haystack, ""));
}

TEST(STRING_TEST, memmem_smoke) {
  const char haystack[] = "big\0daddy\0giant\0haystacks";
  ASSERT_EQ(haystack, memmem(haystack, sizeof(haystack), "", 0));
  ASSERT_EQ(haystack + 3, memmem(haystack, sizeof(haystack), "", 1));
  ASSERT_EQ(haystack + 0, memmem(haystack, sizeof(haystack), "b", 1));
  ASSERT_EQ(haystack + 1, memmem(haystack, sizeof(haystack), "i", 1));
  ASSERT_EQ(haystack + 4, memmem(haystack, sizeof(haystack), "da", 2));
  ASSERT_EQ(haystack + 8, memmem(haystack, sizeof(haystack), "y\0g", 3));
}

TEST(STRING_TEST, strstr_smoke) {
  const char* haystack = "big daddy/giant haystacks!";

  // The current strstr() implementation has special cases for needles of
  // lengths 0, 1, 2, 3, and 4, plus a long needle case. We test matches at the
  // beginning, middle, and end of the haystack.

  ASSERT_EQ(haystack + 0, strstr(haystack, ""));

  ASSERT_EQ(haystack + 0, strstr(haystack, "b"));
  ASSERT_EQ(haystack + 0, strstr(haystack, "bi"));
  ASSERT_EQ(haystack + 0, strstr(haystack, "big"));
  ASSERT_EQ(haystack + 0, strstr(haystack, "big "));
  ASSERT_EQ(haystack + 0, strstr(haystack, "big d"));

  ASSERT_EQ(haystack + 2, strstr(haystack, "g"));
  ASSERT_EQ(haystack + 10, strstr(haystack, "gi"));
  ASSERT_EQ(haystack + 10, strstr(haystack, "gia"));
  ASSERT_EQ(haystack + 10, strstr(haystack, "gian"));
  ASSERT_EQ(haystack + 10, strstr(haystack, "giant"));

  ASSERT_EQ(haystack + 25, strstr(haystack, "!"));
  ASSERT_EQ(haystack + 24, strstr(haystack, "s!"));
  ASSERT_EQ(haystack + 23, strstr(haystack, "ks!"));
  ASSERT_EQ(haystack + 22, strstr(haystack, "cks!"));
  ASSERT_EQ(haystack + 21, strstr(haystack, "acks!"));
}

TEST(STRING_TEST, strcasestr_smoke) {
  const char* haystack = "bIg dAdDy/gIaNt hAyStAcKs";
  ASSERT_EQ(haystack, strcasestr(haystack, ""));
  ASSERT_EQ(haystack + 0, strcasestr(haystack, "B"));
  ASSERT_EQ(haystack + 1, strcasestr(haystack, "i"));
  ASSERT_EQ(haystack + 4, strcasestr(haystack, "Da"));
}

TEST(STRING_TEST, strcoll_smoke) {
  ASSERT_TRUE(strcoll("aab", "aac") < 0);
  ASSERT_TRUE(strcoll("aab", "aab") == 0);
  ASSERT_TRUE(strcoll("aac", "aab") > 0);
}

TEST(STRING_TEST, strxfrm_smoke) {
  const char* src1 = "aab";
  char dst1[16] = {};
  ASSERT_GT(strxfrm(dst1, src1, sizeof(dst1)), 0U);
  const char* src2 = "aac";
  char dst2[16] = {};
  ASSERT_GT(strxfrm(dst2, src2, sizeof(dst2)), 0U);
  ASSERT_TRUE(strcmp(dst1, dst2) < 0);
}

TEST(STRING_TEST, memccpy_smoke) {
  char dst[32];

  memset(dst, 0, sizeof(dst));
  char* p = static_cast<char*>(memccpy(dst, "hello world", ' ', 32));
  ASSERT_STREQ("hello ", dst);
  ASSERT_EQ(ptrdiff_t(6), p - dst);

  memset(dst, 0, sizeof(dst));
  ASSERT_EQ(nullptr, memccpy(dst, "hello world", ' ', 4));
  ASSERT_STREQ("hell", dst);
}
