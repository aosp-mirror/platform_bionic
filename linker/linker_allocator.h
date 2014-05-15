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

#ifndef __LINKER_ALLOCATOR_H
#define __LINKER_ALLOCATOR_H

#include <stdlib.h>
#include <limits.h>
#include "private/bionic_macros.h"

struct LinkerAllocatorPage;

/*
 * This class is a non-template version of the LinkerAllocator
 * It keeps code inside .cpp file by keeping the interface
 * template-free.
 *
 * Please use LinkerAllocator<type> where possible (everywhere).
 */
class LinkerBlockAllocator {
 public:
  LinkerBlockAllocator();

  void init(size_t block_size);
  void* alloc();
  void free(void* block);
  void protect_all(int prot);

 private:
  void create_new_page();
  LinkerAllocatorPage* find_page(void* block);

  size_t block_size_;
  LinkerAllocatorPage* page_list_;
  void* free_block_list_;

  DISALLOW_COPY_AND_ASSIGN(LinkerBlockAllocator);
};

/*
 * We can't use malloc(3) in the dynamic linker.
 *
 * A simple allocator for the dynamic linker. An allocator allocates instances
 * of a single fixed-size type. Allocations are backed by page-sized private
 * anonymous mmaps.
 */
template<typename T>
class LinkerAllocator {
 public:
  LinkerAllocator() : block_allocator_() {}
  void init() { block_allocator_.init(sizeof(T)); }
  T* alloc() { return reinterpret_cast<T*>(block_allocator_.alloc()); }
  void free(T* t) { block_allocator_.free(t); }
  void protect_all(int prot) { block_allocator_.protect_all(prot); }
 private:
  LinkerBlockAllocator block_allocator_;
  DISALLOW_COPY_AND_ASSIGN(LinkerAllocator);
};
#endif // __LINKER_ALLOCATOR_H
