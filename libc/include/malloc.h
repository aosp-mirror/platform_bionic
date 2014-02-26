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

#ifndef LIBC_INCLUDE_MALLOC_H_
#define LIBC_INCLUDE_MALLOC_H_

/*
 * Declaration of malloc routines. Bionic uses dlmalloc (see
 * upstream-dlmalloc) but doesn't directly include it here to keep the
 * defined malloc.h interface small.
 */
#include <sys/cdefs.h>
#include <stddef.h>

__BEGIN_DECLS

extern void* malloc(size_t byte_count) __mallocfunc __wur __attribute__((alloc_size(1)));
extern void* calloc(size_t item_count, size_t item_size) __mallocfunc __wur __attribute__((alloc_size(1,2)));
extern void* realloc(void* p, size_t byte_count) __wur __attribute__((alloc_size(2)));
extern void free(void* p);

extern void* memalign(size_t alignment, size_t byte_count) __mallocfunc __wur __attribute__((alloc_size(2)));
extern size_t malloc_usable_size(const void* p);

extern void* valloc(size_t byte_count) __mallocfunc __wur __attribute__((alloc_size(1)));
extern void* pvalloc(size_t byte_count) __mallocfunc __wur __attribute__((alloc_size(1)));

#ifndef STRUCT_MALLINFO_DECLARED
#define STRUCT_MALLINFO_DECLARED 1
struct mallinfo {
  size_t arena;    /* Total number of non-mmapped bytes currently allocated from OS. */
  size_t ordblks;  /* Number of free chunks. */
  size_t smblks;   /* (Unused.) */
  size_t hblks;    /* (Unused.) */
  size_t hblkhd;   /* Total number of bytes in mmapped regions. */
  size_t usmblks;  /* Maximum total allocated space; greater than total if trimming has occurred. */
  size_t fsmblks;  /* (Unused.) */
  size_t uordblks; /* Total allocated space (normal or mmapped.) */
  size_t fordblks; /* Total free space. */
  size_t keepcost; /* Upper bound on number of bytes releasable by malloc_trim. */
};
#endif  /* STRUCT_MALLINFO_DECLARED */

extern struct mallinfo mallinfo(void);

__END_DECLS

#endif  /* LIBC_INCLUDE_MALLOC_H_ */
