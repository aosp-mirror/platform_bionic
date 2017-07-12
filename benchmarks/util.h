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

#ifndef _BIONIC_BENCHMARKS_UTIL_H_
#define _BIONIC_BENCHMARKS_UTIL_H_

#include <vector>

// This function returns a pointer less than 2 * alignment + or_mask bytes into the array.
char *GetAlignedMemory(char *orig_ptr, size_t alignment, size_t or_mask);

char *GetAlignedPtr(std::vector<char>* buf, size_t alignment, size_t nbytes);

char *GetAlignedPtrFilled(std::vector<char>* buf, size_t alignment, size_t nbytes, char fill_byte);

bool LockToCPU(int cpu_to_lock);

#endif // _BIONIC_BENCHMARKS_UTIL_H
