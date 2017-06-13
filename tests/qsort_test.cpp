/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <stdlib.h>
#include <sys/types.h>

#include <gtest/gtest.h>

#include "gtest_globals.h"

#define BUFFER_SIZE 1024

static int cmp_long(const void *l, const void *r)
{

  return (*(long *)l - *(long *)r);
}

static int cmp_int(const void *l, const void *r)
{

  return (*(int *)l - *(int *)r);
}

#ifndef arc4random_uniform
static bool seeded;

u_int32_t arc4random_uniform(uint32_t upper_bound)
{
  if (!seeded) {
    srandom((int)time(NULL));
    seeded = true;
  }

  return (random() % upper_bound);
}
#endif

TEST(qsort_test, long_test) {
  long buf[BUFFER_SIZE];
  long i;

  /* Initialize buffer with known numbers */
  for (i=0; i<BUFFER_SIZE; i++)
    buf[i] = i;

  /* Stir 1/4 pairs in the buffer */
  for (i=0; i<BUFFER_SIZE/4; i++) {
    u_int32_t pos1, pos2;
    long t;

    pos1 = arc4random_uniform(BUFFER_SIZE);
    pos2 = arc4random_uniform(BUFFER_SIZE);

    t = buf[pos1];
    buf[pos1] = buf[pos2];
    buf[pos2] = t;
  }

  /* Sort */
  qsort(buf, BUFFER_SIZE, sizeof(buf[0]), &cmp_long);

  for (i=0; i<BUFFER_SIZE; i++)
    EXPECT_EQ(i, buf[i]);
}

TEST(qsort_test, int_test) {
  int buf[BUFFER_SIZE];
  int i;

  /* Initialize buffer with known numbers */
  for (i=0; i<BUFFER_SIZE; i++)
    buf[i] = i;

  /* Stir 1/4 pairs in the buffer */
  for (i=0; i<BUFFER_SIZE/4; i++) {
    u_int32_t pos1, pos2;
    int t;

    pos1 = arc4random_uniform(BUFFER_SIZE);
    pos2 = arc4random_uniform(BUFFER_SIZE);

    t = buf[pos1];
    buf[pos1] = buf[pos2];
    buf[pos2] = t;
  }

  /* Sort */
  qsort(buf, BUFFER_SIZE, sizeof(buf[0]), &cmp_int);

  for (i=0; i<BUFFER_SIZE; i++)
    EXPECT_EQ(i, buf[i]);
}
