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

#include "benchmark.h"

#include <time.h>

#if defined(__BIONIC__)

// Used by the horrible android.text.format.Time class, which is used by Calendar. http://b/8270865.
extern "C" void localtime_tz(const time_t* const timep, struct tm* tmp, const char* tz);

static void BM_time_localtime_tz(int iters) {
  StartBenchmarkTiming();

  time_t now(time(NULL));
  tm broken_down_time;
  for (int i = 0; i < iters; ++i) {
    localtime_tz(&now, &broken_down_time, "Europe/Berlin");
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_time_localtime_tz);
#endif
