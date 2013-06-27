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

#include <gtest/gtest.h>

#include <errno.h>
#include <math.h>
#include <string.h>

#define KB 1024
#define SMALL 1*KB
#define LARGE 64*KB

static int signum(int i) {
  if (i < 0) {
    return -1;
  } else if (i > 0) {
    return 1;
  }
  return 0;
}

TEST(string, strerror) {
  // Valid.
  ASSERT_STREQ("Success", strerror(0));
  ASSERT_STREQ("Operation not permitted", strerror(1));

  // Invalid.
  ASSERT_STREQ("Unknown error -1", strerror(-1));
  ASSERT_STREQ("Unknown error 1234", strerror(1234));
}

#if __BIONIC__ // glibc's strerror isn't thread safe, only its strsignal.

static void* ConcurrentStrErrorFn(void*) {
  bool equal = (strcmp("Unknown error 2002", strerror(2002)) == 0);
  return reinterpret_cast<void*>(equal);
}

TEST(string, strerror_concurrent) {
  const char* strerror1001 = strerror(1001);
  ASSERT_STREQ("Unknown error 1001", strerror1001);

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, ConcurrentStrErrorFn, NULL));
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_TRUE(static_cast<bool>(result));

  ASSERT_STREQ("Unknown error 1001", strerror1001);
}

#endif

#if __BIONIC__ // glibc's strerror_r doesn't even have the same signature as the POSIX one.
TEST(string, strerror_r) {
  char buf[256];

  // Valid.
  ASSERT_EQ(0, strerror_r(0, buf, sizeof(buf)));
  ASSERT_STREQ("Success", buf);
  ASSERT_EQ(0, strerror_r(1, buf, sizeof(buf)));
  ASSERT_STREQ("Operation not permitted", buf);

  // Invalid.
  ASSERT_EQ(0, strerror_r(-1, buf, sizeof(buf)));
  ASSERT_STREQ("Unknown error -1", buf);
  ASSERT_EQ(0, strerror_r(1234, buf, sizeof(buf)));
  ASSERT_STREQ("Unknown error 1234", buf);

  // Buffer too small.
  ASSERT_EQ(-1, strerror_r(0, buf, 2));
  ASSERT_EQ(ERANGE, errno);
}
#endif

TEST(string, strsignal) {
  // A regular signal.
  ASSERT_STREQ("Hangup", strsignal(1));

  // A real-time signal.
#ifdef __GLIBC__ // glibc reserves real-time signals for internal use, and doesn't count those.
  ASSERT_STREQ("Real-time signal 14", strsignal(48));
#else
  ASSERT_STREQ("Real-time signal 16", strsignal(48));
#endif

  // Errors.
  ASSERT_STREQ("Unknown signal -1", strsignal(-1)); // Too small.
  ASSERT_STREQ("Unknown signal 0", strsignal(0)); // Still too small.
  ASSERT_STREQ("Unknown signal 1234", strsignal(1234)); // Too large.
}

static void* ConcurrentStrSignalFn(void*) {
  bool equal = (strcmp("Unknown signal 2002", strsignal(2002)) == 0);
  return reinterpret_cast<void*>(equal);
}

TEST(string, strsignal_concurrent) {
  const char* strsignal1001 = strsignal(1001);
  ASSERT_STREQ("Unknown signal 1001", strsignal1001);

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, ConcurrentStrSignalFn, NULL));
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_TRUE(static_cast<bool>(result));

  ASSERT_STREQ("Unknown signal 1001", strsignal1001);
}

// TODO: where did these numbers come from?
#define POS_ITER    10
#define ITER        500

// For every length we want to test, vary and change alignment
// of allocated memory, fill it with some values, calculate
// expected result and then run function and compare what we got.
// These tests contributed by Intel Corporation.
// TODO: make these tests more intention-revealing and less random.
template<class Character>
struct StringTestState {
  StringTestState(size_t MAX_LEN) : MAX_LEN(MAX_LEN) {
    int max_alignment = 64;

    // TODO: fix the tests to not sometimes use twice their specified "MAX_LEN".
    glob_ptr = reinterpret_cast<Character*>(valloc(2 * sizeof(Character) * MAX_LEN + max_alignment));
    glob_ptr1 = reinterpret_cast<Character*>(valloc(2 * sizeof(Character) * MAX_LEN + max_alignment));
    glob_ptr2 = reinterpret_cast<Character*>(valloc(2 * sizeof(Character) * MAX_LEN + max_alignment));

    InitLenArray();

    srandom(1234);
  }

  ~StringTestState() {
    free(glob_ptr);
    free(glob_ptr1);
    free(glob_ptr2);
  }

  void NewIteration() {
    int alignments[] = { 24, 32, 16, 48, 1, 2, 3, 0, 5, 11 };
    int usable_alignments = 10;
    int align1 = alignments[random() % (usable_alignments - 1)];
    int align2 = alignments[random() % (usable_alignments - 1)];

    ptr = glob_ptr + align1;
    ptr1 = glob_ptr1 + align1;
    ptr2 = glob_ptr2 + align2;
  }

  const size_t MAX_LEN;
  Character *ptr, *ptr1, *ptr2;
  size_t n;
  int len[ITER + 1];

 private:
  Character *glob_ptr, *glob_ptr1, *glob_ptr2;

  // Calculate input lengths and fill state.len with them.
  // Test small lengths with more density than big ones. Manually push
  // smallest (0) and biggest (MAX_LEN) lengths. Avoid repeats.
  // Return number of lengths to test.
  void InitLenArray() {
    n = 0;
    len[n++] = 0;
    for (size_t i = 1; i < ITER; ++i) {
      int l = (int) exp(log((double) MAX_LEN) * i / ITER);
      if (l != len[n - 1]) {
        len[n++] = l;
      }
    }
    len[n++] = MAX_LEN;
  }
};

TEST(string, strcat) {
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      memset(state.ptr2, '\2', state.MAX_LEN);
      state.ptr2[state.MAX_LEN - 1] = '\0';
      memcpy(state.ptr, state.ptr2, 2 * state.MAX_LEN);

      memset(state.ptr1, random() & 255, state.len[i]);
      state.ptr1[random() % state.len[i]] = '\0';
      state.ptr1[state.len[i] - 1] = '\0';

      strcpy(state.ptr + state.MAX_LEN - 1, state.ptr1);

      EXPECT_TRUE(strcat(state.ptr2, state.ptr1) == state.ptr2);
      EXPECT_TRUE(memcmp(state.ptr, state.ptr2, 2 * state.MAX_LEN) == 0);
    }
  }
}

// one byte target with "\0" source
TEST(string, strcpy2) {
  char buf[1];
  char* orig = strdup("");
  strcpy(buf, orig);
  ASSERT_EQ('\0', buf[0]);
  free(orig);
}

// multibyte target where we under fill target
TEST(string, strcpy3) {
  char buf[10];
  char* orig = strdup("12345");
  memset(buf, 'A', sizeof(buf));
  strcpy(buf, orig);
  ASSERT_EQ('1',  buf[0]);
  ASSERT_EQ('2',  buf[1]);
  ASSERT_EQ('3',  buf[2]);
  ASSERT_EQ('4',  buf[3]);
  ASSERT_EQ('5',  buf[4]);
  ASSERT_EQ('\0', buf[5]);
  ASSERT_EQ('A',  buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
  free(orig);
}

// multibyte target where we fill target exactly
TEST(string, strcpy4) {
  char buf[10];
  char* orig = strdup("123456789");
  memset(buf, 'A', sizeof(buf));
  strcpy(buf, orig);
  ASSERT_EQ('1',  buf[0]);
  ASSERT_EQ('2',  buf[1]);
  ASSERT_EQ('3',  buf[2]);
  ASSERT_EQ('4',  buf[3]);
  ASSERT_EQ('5',  buf[4]);
  ASSERT_EQ('6',  buf[5]);
  ASSERT_EQ('7',  buf[6]);
  ASSERT_EQ('8',  buf[7]);
  ASSERT_EQ('9',  buf[8]);
  ASSERT_EQ('\0', buf[9]);
  free(orig);
}

TEST(string, strcat2) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strcat(buf, "01234");
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('\0', buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
}

TEST(string, strcat3) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strcat(buf, "01234567");
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('5', buf[6]);
  ASSERT_EQ('6',  buf[7]);
  ASSERT_EQ('7',  buf[8]);
  ASSERT_EQ('\0',  buf[9]);
}

TEST(string, strncat2) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strncat(buf, "01234", sizeof(buf) - strlen(buf) - 1);
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('\0', buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
}

TEST(string, strncat3) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strncat(buf, "0123456789", 5);
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('\0', buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
}

TEST(string, strncat4) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strncat(buf, "01234567", 8);
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('5', buf[6]);
  ASSERT_EQ('6',  buf[7]);
  ASSERT_EQ('7',  buf[8]);
  ASSERT_EQ('\0',  buf[9]);
}

TEST(string, strncat5) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = strncat(buf, "01234567", 9);
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('5', buf[6]);
  ASSERT_EQ('6',  buf[7]);
  ASSERT_EQ('7',  buf[8]);
  ASSERT_EQ('\0',  buf[9]);
}

TEST(string, strchr_with_0) {
  char buf[10];
  const char* s = "01234";
  memcpy(buf, s, strlen(s) + 1);
  EXPECT_TRUE(strchr(buf, '\0') == (buf + strlen(s)));
}

TEST(string, strchr) {
  int seek_char = random() & 255;

  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      if (~seek_char > 0) {
        memset(state.ptr1, ~seek_char, state.len[i]);
      } else {
        memset(state.ptr1, '\1', state.len[i]);
      }
      state.ptr1[state.len[i] - 1] = '\0';

      int pos = random() % state.MAX_LEN;
      char* expected;
      if (pos >= state.len[i] - 1) {
        if (seek_char == 0) {
          expected = state.ptr1 + state.len[i] - 1;
        } else {
          expected = NULL;
        }
      } else {
        state.ptr1[pos] = seek_char;
        expected = state.ptr1 + pos;
      }

      ASSERT_TRUE(strchr(state.ptr1, seek_char) == expected);
    }
  }
}

TEST(string, strcmp) {
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      memset(state.ptr1, 'v', state.MAX_LEN);
      memset(state.ptr2, 'n', state.MAX_LEN);
      state.ptr1[state.len[i] - 1] = '\0';
      state.ptr2[state.len[i] - 1] = '\0';

      int pos = 1 + (random() % (state.MAX_LEN - 1));
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

TEST(string, strcpy) {
  StringTestState<char> state(SMALL);
  for (size_t j = 0; j < POS_ITER; j++) {
    state.NewIteration();

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


#if __BIONIC__
TEST(string, strlcat) {
  StringTestState<char> state(SMALL);
  for (size_t i = 0; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      memset(state.ptr2, '\2', state.MAX_LEN + state.len[i]);
      state.ptr2[state.MAX_LEN - 1] = '\0';
      memcpy(state.ptr, state.ptr2, state.MAX_LEN + state.len[i]);

      int pos = random() % state.MAX_LEN;
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
}
#endif

#if __BIONIC__
TEST(string, strlcpy) {
  StringTestState<char> state(SMALL);
  for (size_t j = 0; j < POS_ITER; j++) {
    state.NewIteration();

    int rand = random() & 255;
    if (rand < 1) {
      rand = 1;
    }
    memset(state.ptr1, rand, state.MAX_LEN);

    size_t pos = random() % state.MAX_LEN;
    if (pos < state.MAX_LEN) {
      state.ptr1[pos] = '\0';
    }
    memcpy(state.ptr, state.ptr1, state.MAX_LEN);

    memset(state.ptr2, random() & 255, state.MAX_LEN);
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
}
#endif

TEST(string, strncat) {
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      memset(state.ptr2, '\2', state.MAX_LEN);
      state.ptr2[state.MAX_LEN - 1] = '\0';
      memcpy(state.ptr, state.ptr2, 2 * state.MAX_LEN);

      memset(state.ptr1, random() & 255, state.len[i]);
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

TEST(string, strncmp) {
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      memset(state.ptr1, 'v', state.MAX_LEN);
      memset(state.ptr2, 'n', state.MAX_LEN);
      state.ptr1[state.len[i] - 1] = '\0';
      state.ptr2[state.len[i] - 1] = '\0';

      int pos = 1 + (random() % (state.MAX_LEN - 1));
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

TEST(string, strncpy) {
  StringTestState<char> state(SMALL);
  for (size_t j = 0; j < ITER; j++) {
    state.NewIteration();

    memset(state.ptr1, random() & 255, state.MAX_LEN);
    state.ptr1[random () % state.MAX_LEN] = '\0';
    memcpy(state.ptr, state.ptr1, state.MAX_LEN);

    memset(state.ptr2, '\1', state.MAX_LEN);

    size_t pos;
    if (memchr(state.ptr1, 0, state.MAX_LEN)) {
      pos = strlen(state.ptr1);
    } else {
      pos = state.MAX_LEN - 1;
    }

    memset(state.ptr + state.MAX_LEN, '\0', state.MAX_LEN);
    memcpy(state.ptr + state.MAX_LEN, state.ptr1, pos + 1);

    ASSERT_TRUE(strncpy(state.ptr2, state.ptr1, state.MAX_LEN) == state.ptr2);
    ASSERT_FALSE(memcmp(state.ptr1, state.ptr, state.MAX_LEN) != 0 ||
                 memcmp(state.ptr2, state.ptr + state.MAX_LEN, state.MAX_LEN) != 0);
  }
}

TEST(string, strrchr) {
  int seek_char = random() & 255;
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      if (~seek_char > 0) {
        memset(state.ptr1, ~seek_char, state.len[i]);
      } else {
        memset(state.ptr1, '\1', state.len[i]);
      }
      state.ptr1[state.len[i] - 1] = '\0';

      int pos = random() % state.MAX_LEN;
      char* expected;
      if (pos >= state.len[i] - 1) {
        if (seek_char == 0) {
          expected = state.ptr1 + state.len[i] - 1;
        } else {
          expected = NULL;
        }
      } else {
        state.ptr1[pos] = seek_char;
        expected = state.ptr1 + pos;
      }

      ASSERT_TRUE(strrchr(state.ptr1, seek_char) == expected);
    }
  }
}

TEST(string, memchr) {
  int seek_char = random() & 255;
  StringTestState<char> state(SMALL);
  for (size_t i = 0; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      memset(state.ptr1, ~seek_char, state.len[i]);

      int pos = random() % state.MAX_LEN;
      char* expected;
      if (pos >= state.len[i]) {
        expected = NULL;
      } else {
        state.ptr1[pos] = seek_char;
        expected = state.ptr1 + pos;
      }

      ASSERT_TRUE(memchr(state.ptr1, seek_char, state.len[i]) == expected);
    }
  }
}

TEST(string, memrchr) {
  int seek_char = random() & 255;
  StringTestState<char> state(SMALL);
  for (size_t i = 0; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      memset(state.ptr1, ~seek_char, state.len[i]);

      int pos = random() % state.MAX_LEN;
      char* expected;
      if (pos >= state.len[i]) {
        expected = NULL;
      } else {
        state.ptr1[pos] = seek_char;
        expected = state.ptr1 + pos;
      }

      ASSERT_TRUE(memrchr(state.ptr1, seek_char, state.len[i]) == expected);
    }
  }
}

TEST(string, memcmp) {
  StringTestState<char> state(SMALL);
  for (size_t i = 0; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      int c1 = random() & 0xff;
      int c2 = random() & 0xff;
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

#if defined(__BIONIC__)
extern "C" int __memcmp16(const unsigned short *ptr1, const unsigned short *ptr2, size_t n);

TEST(string, __memcmp16) {
  StringTestState<unsigned short> state(SMALL);

  for (size_t i = 0; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      unsigned short mask = 0xffff;
      unsigned short c1 = rand() & mask;
      unsigned short c2 = rand() & mask;

      std::fill(state.ptr1, state.ptr1 + state.MAX_LEN, c1);
      std::fill(state.ptr2, state.ptr2 + state.MAX_LEN, c1);

      int pos = (state.len[i] == 0) ? 0 : (random() % state.len[i]);
      state.ptr2[pos] = c2;

      int expected = (static_cast<unsigned short>(c1) - static_cast<unsigned short>(c2));
      int actual = __memcmp16(state.ptr1, state.ptr2, (size_t) state.MAX_LEN);

      ASSERT_EQ(expected, actual);
    }
  }
}
#endif

TEST(string, wmemcmp) {
  StringTestState<wchar_t> state(SMALL);

  for (size_t i = 0; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

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

TEST(string, memcpy) {
  StringTestState<char> state(LARGE);
  int rand = random() & 255;
  for (size_t i = 0; i < state.n - 1; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

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

TEST(string, memset) {
  StringTestState<char> state(LARGE);
  char ch = random () & 255;
  for (size_t i = 0; i < state.n - 1; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

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

TEST(string, memmove) {
  StringTestState<char> state(LARGE);
  for (size_t i = 0; i < state.n - 1; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      memset(state.ptr1, random() & 255, 2 * state.MAX_LEN);

      size_t pos = random() % (state.MAX_LEN - state.len[i]);

      memset(state.ptr1, random() & 255, state.len[i]);
      memcpy(state.ptr2, state.ptr1, 2 * state.MAX_LEN);
      memcpy(state.ptr, state.ptr1, state.len[i]);
      memcpy(state.ptr1 + pos, state.ptr, state.len[i]);

      ASSERT_TRUE(memmove(state.ptr2 + pos, state.ptr2, state.len[i]) == state.ptr2 + pos);
      ASSERT_EQ(0, memcmp(state.ptr2, state.ptr1, 2 * state.MAX_LEN));
    }
  }
}

TEST(string, bcopy) {
  StringTestState<char> state(LARGE);
  for (size_t i = 0; i < state.n; i++) {
    for (size_t j = 0; j < POS_ITER; j++) {
      state.NewIteration();

      memset(state.ptr1, random() & 255, state.MAX_LEN);
      memset(state.ptr1 + state.MAX_LEN, random() & 255, state.MAX_LEN);
      memcpy(state.ptr2, state.ptr1, 2 * state.MAX_LEN);

      size_t start = random() % (2 * state.MAX_LEN - state.len[i]);
      memcpy(state.ptr2 + start, state.ptr1, state.len[i]);

      bcopy(state.ptr1, state.ptr1 + start, state.len[i]);
      ASSERT_EQ(0, memcmp(state.ptr1, state.ptr2, 2 * state.MAX_LEN));
    }
  }
}

TEST(string, bzero) {
  StringTestState<char> state(LARGE);
  for (size_t j = 0; j < ITER; j++) {
    state.NewIteration();

    memset(state.ptr1, random() & 255, state.MAX_LEN);

    size_t start = random() % state.MAX_LEN;
    size_t end = start + random() % (state.MAX_LEN - start);

    memcpy(state.ptr2, state.ptr1, start);
    memset(state.ptr2 + start, '\0', end - start);
    memcpy(state.ptr2 + end, state.ptr1 + end, state.MAX_LEN - end);

    bzero(state.ptr1 + start, end - start);

    ASSERT_EQ(0, memcmp(state.ptr1, state.ptr2, state.MAX_LEN));
  }
}
