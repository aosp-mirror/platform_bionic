/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <arpa/inet.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/system_properties.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <unwind.h>

#include "debug_stacktrace.h"
#include "dlmalloc.h"
#include "malloc_debug_common.h"

#include "private/libc_logging.h"
#include "private/ScopedPthreadMutexLocker.h"

// This file should be included into the build only when
// MALLOC_LEAK_CHECK, or MALLOC_QEMU_INSTRUMENT, or both
// macros are defined.
#ifndef MALLOC_LEAK_CHECK
#error MALLOC_LEAK_CHECK is not defined.
#endif  // !MALLOC_LEAK_CHECK

// Global variables defined in malloc_debug_common.c
extern int gMallocLeakZygoteChild;
extern pthread_mutex_t g_allocations_mutex;
extern HashTable g_hash_table;

// =============================================================================
// stack trace functions
// =============================================================================

#define GUARD               0x48151642
#define DEBUG               0

// =============================================================================
// Structures
// =============================================================================

struct AllocationEntry {
    HashEntry* entry;
    uint32_t guard;
} __attribute__((aligned(MALLOC_ALIGNMENT)));

static inline AllocationEntry* to_header(void* mem) {
  return reinterpret_cast<AllocationEntry*>(mem) - 1;
}

static inline const AllocationEntry* const_to_header(const void* mem) {
  return reinterpret_cast<const AllocationEntry*>(mem) - 1;
}

// =============================================================================
// Hash Table functions
// =============================================================================

static uint32_t get_hash(uintptr_t* backtrace, size_t numEntries) {
    if (backtrace == NULL) return 0;

    int hash = 0;
    size_t i;
    for (i = 0 ; i < numEntries ; i++) {
        hash = (hash * 33) + (backtrace[i] >> 2);
    }

    return hash;
}

static HashEntry* find_entry(HashTable* table, int slot,
                             uintptr_t* backtrace, size_t numEntries, size_t size) {
    HashEntry* entry = table->slots[slot];
    while (entry != NULL) {
        //debug_log("backtrace: %p, entry: %p entry->backtrace: %p\n",
        //        backtrace, entry, (entry != NULL) ? entry->backtrace : NULL);
        /*
         * See if the entry matches exactly.  We compare the "size" field,
         * including the flag bits.
         */
        if (entry->size == size && entry->numEntries == numEntries &&
                !memcmp(backtrace, entry->backtrace, numEntries * sizeof(uintptr_t))) {
            return entry;
        }

        entry = entry->next;
    }

    return NULL;
}

static HashEntry* record_backtrace(uintptr_t* backtrace, size_t numEntries, size_t size) {
    size_t hash = get_hash(backtrace, numEntries);
    size_t slot = hash % HASHTABLE_SIZE;

    if (size & SIZE_FLAG_MASK) {
        debug_log("malloc_debug: allocation %zx exceeds bit width\n", size);
        abort();
    }

    if (gMallocLeakZygoteChild) {
        size |= SIZE_FLAG_ZYGOTE_CHILD;
    }

    HashEntry* entry = find_entry(&g_hash_table, slot, backtrace, numEntries, size);

    if (entry != NULL) {
        entry->allocations++;
    } else {
        // create a new entry
        entry = static_cast<HashEntry*>(dlmalloc(sizeof(HashEntry) + numEntries*sizeof(uintptr_t)));
        if (!entry) {
            return NULL;
        }
        entry->allocations = 1;
        entry->slot = slot;
        entry->prev = NULL;
        entry->next = g_hash_table.slots[slot];
        entry->numEntries = numEntries;
        entry->size = size;

        memcpy(entry->backtrace, backtrace, numEntries * sizeof(uintptr_t));

        g_hash_table.slots[slot] = entry;

        if (entry->next != NULL) {
            entry->next->prev = entry;
        }

        // we just added an entry, increase the size of the hashtable
        g_hash_table.count++;
    }

    return entry;
}

static int is_valid_entry(HashEntry* entry) {
    if (entry != NULL) {
        int i;
        for (i = 0 ; i < HASHTABLE_SIZE ; i++) {
            HashEntry* e1 = g_hash_table.slots[i];

            while (e1 != NULL) {
                if (e1 == entry) {
                    return 1;
                }

                e1 = e1->next;
            }
        }
    }

    return 0;
}

static void remove_entry(HashEntry* entry) {
    HashEntry* prev = entry->prev;
    HashEntry* next = entry->next;

    if (prev != NULL) entry->prev->next = next;
    if (next != NULL) entry->next->prev = prev;

    if (prev == NULL) {
        // we are the head of the list. set the head to be next
        g_hash_table.slots[entry->slot] = entry->next;
    }

    // we just removed and entry, decrease the size of the hashtable
    g_hash_table.count--;
}

// =============================================================================
// malloc fill functions
// =============================================================================

#define CHK_FILL_FREE           0xef
#define CHK_SENTINEL_VALUE      0xeb

extern "C" void* fill_calloc(size_t n_elements, size_t elem_size) {
    return dlcalloc(n_elements, elem_size);
}

extern "C" void* fill_malloc(size_t bytes) {
    void* buffer = dlmalloc(bytes);
    if (buffer) {
        memset(buffer, CHK_SENTINEL_VALUE, bytes);
    }
    return buffer;
}

extern "C" void fill_free(void* mem) {
    size_t bytes = dlmalloc_usable_size(mem);
    memset(mem, CHK_FILL_FREE, bytes);
    dlfree(mem);
}

extern "C" void* fill_realloc(void* mem, size_t bytes) {
    size_t oldSize = dlmalloc_usable_size(mem);
    void* newMem = dlrealloc(mem, bytes);
    if (newMem) {
        // If this is larger than before, fill the extra with our pattern.
        size_t newSize = dlmalloc_usable_size(newMem);
        if (newSize > oldSize) {
            memset(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(newMem)+oldSize), CHK_FILL_FREE, newSize-oldSize);
        }
    }
    return newMem;
}

extern "C" void* fill_memalign(size_t alignment, size_t bytes) {
    void* buffer = dlmemalign(alignment, bytes);
    if (buffer) {
        memset(buffer, CHK_SENTINEL_VALUE, bytes);
    }
    return buffer;
}

extern "C" size_t fill_malloc_usable_size(const void* mem) {
    // Since we didn't allocate extra bytes before or after, we can
    // report the normal usable size here.
    return dlmalloc_usable_size(mem);
}

// =============================================================================
// malloc leak functions
// =============================================================================

static uint32_t MEMALIGN_GUARD      = 0xA1A41520;

extern "C" void* leak_malloc(size_t bytes) {
    // allocate enough space infront of the allocation to store the pointer for
    // the alloc structure. This will making free'ing the structer really fast!

    // 1. allocate enough memory and include our header
    // 2. set the base pointer to be right after our header

    size_t size = bytes + sizeof(AllocationEntry);
    if (size < bytes) { // Overflow.
        return NULL;
    }

    void* base = dlmalloc(size);
    if (base != NULL) {
        ScopedPthreadMutexLocker locker(&g_allocations_mutex);

        uintptr_t backtrace[BACKTRACE_SIZE];
        size_t numEntries = get_backtrace(backtrace, BACKTRACE_SIZE);

        AllocationEntry* header = reinterpret_cast<AllocationEntry*>(base);
        header->entry = record_backtrace(backtrace, numEntries, bytes);
        header->guard = GUARD;

        // now increment base to point to after our header.
        // this should just work since our header is 8 bytes.
        base = reinterpret_cast<AllocationEntry*>(base) + 1;
    }

    return base;
}

extern "C" void leak_free(void* mem) {
    if (mem != NULL) {
        ScopedPthreadMutexLocker locker(&g_allocations_mutex);

        // check the guard to make sure it is valid
        AllocationEntry* header = to_header(mem);

        if (header->guard != GUARD) {
            // could be a memaligned block
            if (header->guard == MEMALIGN_GUARD) {
                // For memaligned blocks, header->entry points to the memory
                // allocated through leak_malloc.
                header = to_header(header->entry);
            }
        }

        if (header->guard == GUARD || is_valid_entry(header->entry)) {
            // decrement the allocations
            HashEntry* entry = header->entry;
            entry->allocations--;
            if (entry->allocations <= 0) {
                remove_entry(entry);
                dlfree(entry);
            }

            // now free the memory!
            dlfree(header);
        } else {
            debug_log("WARNING bad header guard: '0x%x'! and invalid entry: %p\n",
                    header->guard, header->entry);
        }
    }
}

extern "C" void* leak_calloc(size_t n_elements, size_t elem_size) {
    /* Fail on overflow - just to be safe even though this code runs only
     * within the debugging C library, not the production one */
    if (n_elements && MAX_SIZE_T / n_elements < elem_size) {
        return NULL;
    }
    size_t size = n_elements * elem_size;
    void* ptr  = leak_malloc(size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
    }
    return ptr;
}

extern "C" void* leak_realloc(void* oldMem, size_t bytes) {
    if (oldMem == NULL) {
        return leak_malloc(bytes);
    }

    void* newMem = NULL;
    AllocationEntry* header = to_header(oldMem);
    if (header->guard == MEMALIGN_GUARD) {
        // Get the real header.
        header = to_header(header->entry);
    } else if (header->guard != GUARD) {
        debug_log("WARNING bad header guard: '0x%x'! and invalid entry: %p\n",
                   header->guard, header->entry);
        return NULL;
    }

    newMem = leak_malloc(bytes);
    if (newMem != NULL) {
        size_t oldSize = header->entry->size & ~SIZE_FLAG_MASK;
        size_t copySize = (oldSize <= bytes) ? oldSize : bytes;
        memcpy(newMem, oldMem, copySize);
    }
    leak_free(oldMem);

    return newMem;
}

extern "C" void* leak_memalign(size_t alignment, size_t bytes) {
    // we can just use malloc
    if (alignment <= MALLOC_ALIGNMENT) {
        return leak_malloc(bytes);
    }

    // need to make sure it's a power of two
    if (alignment & (alignment-1)) {
        alignment = 1L << (31 - __builtin_clz(alignment));
    }

    // here, alignment is at least MALLOC_ALIGNMENT<<1 bytes
    // we will align by at least MALLOC_ALIGNMENT bytes
    // and at most alignment-MALLOC_ALIGNMENT bytes
    size_t size = (alignment-MALLOC_ALIGNMENT) + bytes;
    if (size < bytes) { // Overflow.
        return NULL;
    }

    void* base = leak_malloc(size);
    if (base != NULL) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(base);
        if ((ptr % alignment) == 0) {
            return base;
        }

        // align the pointer
        ptr += ((-ptr) % alignment);

        // Already allocated enough space for the header. This assumes
        // that the malloc alignment is at least 8, otherwise, this is
        // not guaranteed to have the space for the header.
        AllocationEntry* header = to_header(reinterpret_cast<void*>(ptr));
        header->guard = MEMALIGN_GUARD;
        header->entry = reinterpret_cast<HashEntry*>(base);

        return reinterpret_cast<void*>(ptr);
    }
    return base;
}

extern "C" size_t leak_malloc_usable_size(const void* mem) {
    if (mem != NULL) {
        // Check the guard to make sure it is valid.
        const AllocationEntry* header = const_to_header((void*)mem);

        if (header->guard == MEMALIGN_GUARD) {
            // If this is a memalign'd pointer, then grab the header from
            // entry.
            header = const_to_header(header->entry);
        } else if (header->guard != GUARD) {
            debug_log("WARNING bad header guard: '0x%x'! and invalid entry: %p\n",
                      header->guard, header->entry);
            return 0;
        }

        size_t ret = dlmalloc_usable_size(header);
        if (ret != 0) {
            // The usable area starts at 'mem' and stops at 'header+ret'.
            return reinterpret_cast<uintptr_t>(header) + ret - reinterpret_cast<uintptr_t>(mem);
        }
    }
    return 0;
}
