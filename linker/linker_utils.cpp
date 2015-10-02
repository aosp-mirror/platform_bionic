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

#include "linker_utils.h"
#include "linker_debug.h"

bool normalize_path(const char* path, std::string* normalized_path) {
  // Input should be an absolute path
  if (path[0] != '/') {
    PRINT("canonize_path - invalid input: '%s', the input path should be absolute", path);
    return false;
  }

  const size_t len = strlen(path) + 1;
  char buf[len];

  const char* in_ptr = path;
  char* out_ptr = buf;

  while (*in_ptr != 0) {
    if (*in_ptr == '/') {
      char c1 = in_ptr[1];
      if (c1 == '.') {
        char c2 = in_ptr[2];
        if (c2 == '/') {
          in_ptr += 2;
          continue;
        } else if (c2 == '.' && (in_ptr[3] == '/' || in_ptr[3] == 0)) {
          in_ptr += 3;
          while (out_ptr > buf && *--out_ptr != '/') {
          }
          if (in_ptr[0] == 0) {
            // retain '/'
            out_ptr++;
          }
          continue;
        }
      } else if (c1 == '/') {
        ++in_ptr;
        continue;
      }
    }
    *out_ptr++ = *in_ptr++;
  }

  *out_ptr = 0;
  *normalized_path = buf;
  return true;
}

