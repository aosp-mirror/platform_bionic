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

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <string>

#include "utils.h"

int Round(int n) {
  int base = 1;
  while (base*10 < n) {
    base *= 10;
  }
  if (n < 2*base) {
    return 2*base;
  }
  if (n < 5*base) {
    return 5*base;
  }
  return 10*base;
}

// Similar to the code in art, but supporting both binary and decimal prefixes.
std::string PrettyInt(long value, size_t base) {
  if (base != 2 && base != 10) abort();

  uint64_t count = static_cast<uint64_t>(value);
  bool negative_number = false;
  if (value < 0) {
    negative_number = true;
    count = static_cast<uint64_t>(-value);
  }

  // The byte thresholds at which we display amounts. A count is displayed
  // in unit U when kUnitThresholds[U] <= bytes < kUnitThresholds[U+1].
  static const uint64_t kUnitThresholds2[] = {
    1024*1024*1024 /* Gi */, 2*1024*1024 /* Mi */, 3*1024 /* Ki */, 0,
  };
  static const uint64_t kUnitThresholds10[] = {
    1000*1000*1000 /* G */, 2*1000*1000 /* M */, 3*1000 /* k */, 0,
  };
  static const uint64_t kAmountPerUnit2[] = { 1024*1024*1024, 1024*1024, 1024, 1 };
  static const uint64_t kAmountPerUnit10[] = { 1000*1000*1000, 1000*1000, 1000, 1 };
  static const char* const kUnitStrings2[] = { "Gi", "Mi", "Ki", "" };
  static const char* const kUnitStrings10[] = { "G", "M", "k", "" };

  // Which set are we using?
  const uint64_t* kUnitThresholds = ((base == 2) ? kUnitThresholds2 : kUnitThresholds10);
  const uint64_t* kAmountPerUnit = ((base == 2) ? kAmountPerUnit2 : kAmountPerUnit10);
  const char* const* kUnitStrings = ((base == 2) ? kUnitStrings2 : kUnitStrings10);

  size_t i = 0;
  for (; kUnitThresholds[i] != 0; ++i) {
    if (count >= kUnitThresholds[i]) {
      break;
    }
  }
  char* s = NULL;
  asprintf(&s, "%s%" PRId64 "%s", (negative_number ? "-" : ""),
           count / kAmountPerUnit[i], kUnitStrings[i]);
  std::string result(s);
  free(s);
  return result;
}
