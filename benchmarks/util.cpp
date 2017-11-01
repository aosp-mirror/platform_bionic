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

#include "util.h"

#include <err.h>
#include <math.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include <cstdlib>

// This function returns a pointer less than 2 * alignment + or_mask bytes into the array.
char* GetAlignedMemory(char* orig_ptr, size_t alignment, size_t or_mask) {
  if ((alignment & (alignment - 1)) != 0) {
    errx(1, "warning: alignment passed into GetAlignedMemory is not a power of two.");
  }
  if (or_mask > alignment) {
    errx(1, "warning: or_mask passed into GetAlignedMemory is too high.");
  }
  uintptr_t ptr = reinterpret_cast<uintptr_t>(orig_ptr);
  if (alignment > 0) {
    // When setting the alignment, set it to exactly the alignment chosen.
    // The pointer returned will be guaranteed not to be aligned to anything
    // more than that.
    ptr += alignment - (ptr & (alignment - 1));
    ptr |= alignment | or_mask;
  }

  return reinterpret_cast<char*>(ptr);
}

char* GetAlignedPtr(std::vector<char>* buf, size_t alignment, size_t nbytes) {
  buf->resize(nbytes + 3 * alignment);
  return GetAlignedMemory(buf->data(), alignment, 0);
}

wchar_t* GetAlignedPtr(std::vector<wchar_t>* buf, size_t alignment, size_t nchars) {
  buf->resize(nchars + ceil((3 * alignment) / sizeof(wchar_t)));
  return reinterpret_cast<wchar_t*>(GetAlignedMemory(reinterpret_cast<char*>(buf->data()),
                                                     alignment, 0));
}

char* GetAlignedPtrFilled(std::vector<char>* buf, size_t alignment, size_t nbytes, char fill_byte) {
  char* buf_aligned = GetAlignedPtr(buf, alignment, nbytes);
  memset(buf_aligned, fill_byte, nbytes);
  return buf_aligned;
}

#if defined(__APPLE__)

// Darwin doesn't support this, so do nothing.
bool LockToCPU(int) {
  return false;
}

#else

bool LockToCPU(long cpu_to_lock) {
  cpu_set_t cpuset;

  CPU_ZERO(&cpuset);
  if (sched_getaffinity(0, sizeof(cpuset), &cpuset) != 0) {
    perror("sched_getaffinity failed");
    return false;
  }

  if (cpu_to_lock < 0) {
    // Lock to the last active core we find.
    for (int i = 0; i < CPU_SETSIZE; i++) {
      if (CPU_ISSET(i, &cpuset)) {
        cpu_to_lock = i;
      }
    }
  } else if (!CPU_ISSET(cpu_to_lock, &cpuset)) {
    printf("Cpu %ld does not exist.\n", cpu_to_lock);
    return false;
  }

  if (cpu_to_lock < 0) {
    printf("Cannot find any valid cpu to lock.\n");
    return false;
  }

  CPU_ZERO(&cpuset);
  CPU_SET(cpu_to_lock, &cpuset);
  if (sched_setaffinity(0, sizeof(cpuset), &cpuset) != 0) {
    perror("sched_setaffinity failed");
    return false;
  }

  return true;
}

#endif
