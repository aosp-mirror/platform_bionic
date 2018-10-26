/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include "malloc_info.h"

#include <errno.h>
#include "private/bionic_macros.h"

class __LIBC_HIDDEN__ Elem {
public:
  // name must be valid throughout lifetime of the object.
  explicit Elem(FILE* fp, const char* name,
                const char* attr_fmt = nullptr, ...) {
    this->fp = fp;
    this->name = name;

    fprintf(fp, "<%s", name);
    if (attr_fmt != nullptr) {
      va_list args;
      va_start(args, attr_fmt);
      fputc(' ', fp);
      vfprintf(fp, attr_fmt, args);
      va_end(args);
    }
    fputc('>', fp);
  }

  ~Elem() noexcept {
    fprintf(fp, "</%s>", name);
  }

  void contents(const char* fmt, ...) {
      va_list args;
      va_start(args, fmt);
      vfprintf(fp, fmt, args);
      va_end(args);
  }

private:
  FILE* fp;
  const char* name;

  BIONIC_DISALLOW_IMPLICIT_CONSTRUCTORS(Elem);
};

int malloc_info(int options, FILE* fp) {
  if (options != 0) {
    errno = EINVAL;
    return -1;
  }

  Elem root(fp, "malloc", "version=\"jemalloc-1\"");

  // Dump all of the large allocations in the arenas.
  for (size_t i = 0; i < __mallinfo_narenas(); i++) {
    struct mallinfo mi = __mallinfo_arena_info(i);
    if (mi.hblkhd != 0) {
      Elem arena_elem(fp, "heap", "nr=\"%d\"", i);
      {
        Elem(fp, "allocated-large").contents("%zu", mi.ordblks);
        Elem(fp, "allocated-huge").contents("%zu", mi.uordblks);
        Elem(fp, "allocated-bins").contents("%zu", mi.fsmblks);

        size_t total = 0;
        for (size_t j = 0; j < __mallinfo_nbins(); j++) {
          struct mallinfo mi = __mallinfo_bin_info(i, j);
          if (mi.ordblks != 0) {
            Elem bin_elem(fp, "bin", "nr=\"%d\"", j);
            Elem(fp, "allocated").contents("%zu", mi.ordblks);
            Elem(fp, "nmalloc").contents("%zu", mi.uordblks);
            Elem(fp, "ndalloc").contents("%zu", mi.fordblks);
            total += mi.ordblks;
          }
        }
        Elem(fp, "bins-total").contents("%zu", total);
      }
    }
  }

  return 0;
}
