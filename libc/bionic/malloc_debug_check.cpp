/*
 * Copyright (C) 2012 The Android Open Source Project
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
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/system_properties.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <unwind.h>

#include "debug_mapinfo.h"
#include "debug_backtrace.h"
#include "malloc_debug_backtrace.h"
#include "malloc_debug_common.h"
#include "malloc_debug_disable.h"
#include "private/bionic_macros.h"
#include "private/libc_logging.h"
#include "private/ScopedPthreadMutexLocker.h"

#define MAX_BACKTRACE_DEPTH 16
#define ALLOCATION_TAG      0x1ee7d00d
#define BACKLOG_TAG         0xbabecafe
#define FREE_POISON         0xa5
#define FRONT_GUARD         0xaa
#define FRONT_GUARD_LEN     (1<<5)
#define REAR_GUARD          0xbb
#define REAR_GUARD_LEN      (1<<5)

static void log_message(const char* format, ...) {
  va_list args;
  va_start(args, format);
  __libc_format_log_va_list(ANDROID_LOG_ERROR, "libc", format, args);
  va_end(args);
}

struct hdr_t {
    uint32_t tag;
    void* base;  // Always points to the memory allocated using malloc.
                 // For memory allocated in chk_memalign, this value will
                 // not be the same as the location of the start of this
                 // structure.
    hdr_t* prev;
    hdr_t* next;
    uintptr_t bt[MAX_BACKTRACE_DEPTH];
    int bt_depth;
    uintptr_t freed_bt[MAX_BACKTRACE_DEPTH];
    int freed_bt_depth;
    size_t size;
    uint8_t front_guard[FRONT_GUARD_LEN];
} __attribute__((packed, aligned(MALLOC_ALIGNMENT)));

struct ftr_t {
    uint8_t rear_guard[REAR_GUARD_LEN];
} __attribute__((packed));

static inline ftr_t* to_ftr(hdr_t* hdr) {
    return reinterpret_cast<ftr_t*>(reinterpret_cast<char*>(hdr + 1) + hdr->size);
}

static inline void* user(hdr_t* hdr) {
    return hdr + 1;
}

static inline hdr_t* meta(void* user) {
    return reinterpret_cast<hdr_t*>(user) - 1;
}

static inline const hdr_t* const_meta(const void* user) {
    return reinterpret_cast<const hdr_t*>(user) - 1;
}

// TODO: introduce a struct for this global state.
// There are basically two lists here, the regular list and the backlog list.
// We should be able to remove the duplication.
static unsigned g_allocated_block_count;
static hdr_t* tail;
static hdr_t* head;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static unsigned backlog_num;
static hdr_t* backlog_tail;
static hdr_t* backlog_head;
static pthread_mutex_t backlog_lock = PTHREAD_MUTEX_INITIALIZER;

// This variable is set to the value of property libc.debug.malloc.backlog.
// It determines the size of the backlog we use to detect multiple frees.
static unsigned g_malloc_debug_backlog = 100;

// This variable is set to false if the property libc.debug.malloc.nobacktrace
// is set to non-zero.
__LIBC_HIDDEN__ bool g_backtrace_enabled = true;

__LIBC_HIDDEN__ HashTable* g_hash_table;
__LIBC_HIDDEN__ const MallocDebug* g_malloc_dispatch;

static inline void init_front_guard(hdr_t* hdr) {
    memset(hdr->front_guard, FRONT_GUARD, FRONT_GUARD_LEN);
}

static inline bool is_front_guard_valid(hdr_t* hdr) {
    for (size_t i = 0; i < FRONT_GUARD_LEN; i++) {
        if (hdr->front_guard[i] != FRONT_GUARD) {
            return false;
        }
    }
    return true;
}

static inline void init_rear_guard(hdr_t* hdr) {
    ftr_t* ftr = to_ftr(hdr);
    memset(ftr->rear_guard, REAR_GUARD, REAR_GUARD_LEN);
}

static inline bool is_rear_guard_valid(hdr_t* hdr) {
    unsigned i;
    int valid = 1;
    int first_mismatch = -1;
    ftr_t* ftr = to_ftr(hdr);
    for (i = 0; i < REAR_GUARD_LEN; i++) {
        if (ftr->rear_guard[i] != REAR_GUARD) {
            if (first_mismatch < 0)
                first_mismatch = i;
            valid = 0;
        } else if (first_mismatch >= 0) {
            log_message("+++ REAR GUARD MISMATCH [%d, %d)\n", first_mismatch, i);
            first_mismatch = -1;
        }
    }

    if (first_mismatch >= 0)
        log_message("+++ REAR GUARD MISMATCH [%d, %d)\n", first_mismatch, i);
    return valid;
}

static inline void add_locked(hdr_t* hdr, hdr_t** tail, hdr_t** head) {
    hdr->prev = NULL;
    hdr->next = *head;
    if (*head)
        (*head)->prev = hdr;
    else
        *tail = hdr;
    *head = hdr;
}

static inline int del_locked(hdr_t* hdr, hdr_t** tail, hdr_t** head) {
    if (hdr->prev) {
        hdr->prev->next = hdr->next;
    } else {
        *head = hdr->next;
    }
    if (hdr->next) {
        hdr->next->prev = hdr->prev;
    } else {
        *tail = hdr->prev;
    }
    return 0;
}

static inline void add(hdr_t* hdr, size_t size) {
    ScopedPthreadMutexLocker locker(&lock);
    hdr->tag = ALLOCATION_TAG;
    hdr->size = size;
    init_front_guard(hdr);
    init_rear_guard(hdr);
    ++g_allocated_block_count;
    add_locked(hdr, &tail, &head);
}

static inline int del(hdr_t* hdr) {
    if (hdr->tag != ALLOCATION_TAG) {
        return -1;
    }

    ScopedPthreadMutexLocker locker(&lock);
    del_locked(hdr, &tail, &head);
    --g_allocated_block_count;
    return 0;
}

static inline void poison(hdr_t* hdr) {
    memset(user(hdr), FREE_POISON, hdr->size);
}

static bool was_used_after_free(hdr_t* hdr) {
    const uint8_t* data = reinterpret_cast<const uint8_t*>(user(hdr));
    for (size_t i = 0; i < hdr->size; i++) {
        if (data[i] != FREE_POISON) {
            return true;
        }
    }
    return false;
}

/* returns 1 if valid, *safe == 1 if safe to dump stack */
static inline int check_guards(hdr_t* hdr, int* safe) {
    *safe = 1;
    if (!is_front_guard_valid(hdr)) {
        if (hdr->front_guard[0] == FRONT_GUARD) {
            log_message("+++ ALLOCATION %p SIZE %d HAS A CORRUPTED FRONT GUARD\n",
                       user(hdr), hdr->size);
        } else {
            log_message("+++ ALLOCATION %p HAS A CORRUPTED FRONT GUARD "\
                      "(NOT DUMPING STACKTRACE)\n", user(hdr));
            /* Allocation header is probably corrupt, do not print stack trace */
            *safe = 0;
        }
        return 0;
    }

    if (!is_rear_guard_valid(hdr)) {
        log_message("+++ ALLOCATION %p SIZE %d HAS A CORRUPTED REAR GUARD\n",
                   user(hdr), hdr->size);
        return 0;
    }

    return 1;
}

/* returns 1 if valid, *safe == 1 if safe to dump stack */
static inline int check_allocation_locked(hdr_t* hdr, int* safe) {
    int valid = 1;
    *safe = 1;

    if (hdr->tag != ALLOCATION_TAG && hdr->tag != BACKLOG_TAG) {
        log_message("+++ ALLOCATION %p HAS INVALID TAG %08x (NOT DUMPING STACKTRACE)\n",
                   user(hdr), hdr->tag);
        // Allocation header is probably corrupt, do not dequeue or dump stack
        // trace.
        *safe = 0;
        return 0;
    }

    if (hdr->tag == BACKLOG_TAG && was_used_after_free(hdr)) {
        log_message("+++ ALLOCATION %p SIZE %d WAS USED AFTER BEING FREED\n",
                   user(hdr), hdr->size);
        valid = 0;
        /* check the guards to see if it's safe to dump a stack trace */
        check_guards(hdr, safe);
    } else {
        valid = check_guards(hdr, safe);
    }

    if (!valid && *safe && g_backtrace_enabled) {
        log_message("+++ ALLOCATION %p SIZE %d ALLOCATED HERE:\n",
                        user(hdr), hdr->size);
        log_backtrace(hdr->bt, hdr->bt_depth);
        if (hdr->tag == BACKLOG_TAG) {
            log_message("+++ ALLOCATION %p SIZE %d FREED HERE:\n",
                       user(hdr), hdr->size);
            log_backtrace(hdr->freed_bt, hdr->freed_bt_depth);
        }
    }

    return valid;
}

static inline int del_and_check_locked(hdr_t* hdr,
                                       hdr_t** tail, hdr_t** head, unsigned* cnt,
                                       int* safe) {
    int valid = check_allocation_locked(hdr, safe);
    if (safe) {
        (*cnt)--;
        del_locked(hdr, tail, head);
    }
    return valid;
}

static inline void del_from_backlog_locked(hdr_t* hdr) {
    int safe;
    del_and_check_locked(hdr,
                         &backlog_tail, &backlog_head, &backlog_num,
                         &safe);
    hdr->tag = 0; /* clear the tag */
}

static inline void del_from_backlog(hdr_t* hdr) {
    ScopedPthreadMutexLocker locker(&backlog_lock);
    del_from_backlog_locked(hdr);
}

static inline int del_leak(hdr_t* hdr, int* safe) {
    ScopedPthreadMutexLocker locker(&lock);
    return del_and_check_locked(hdr, &tail, &head, &g_allocated_block_count, safe);
}

static inline void add_to_backlog(hdr_t* hdr) {
    ScopedPthreadMutexLocker locker(&backlog_lock);
    hdr->tag = BACKLOG_TAG;
    backlog_num++;
    add_locked(hdr, &backlog_tail, &backlog_head);
    poison(hdr);
    /* If we've exceeded the maximum backlog, clear it up */
    while (backlog_num > g_malloc_debug_backlog) {
        hdr_t* gone = backlog_tail;
        del_from_backlog_locked(gone);
        g_malloc_dispatch->free(gone->base);
    }
}

extern "C" void* chk_malloc(size_t bytes) {
//  log_message("%s: %s\n", __FILE__, __FUNCTION__);
    if (DebugCallsDisabled()) {
        return g_malloc_dispatch->malloc(bytes);
    }

    size_t size = sizeof(hdr_t) + bytes + sizeof(ftr_t);
    if (size < bytes) { // Overflow
        errno = ENOMEM;
        return NULL;
    }
    hdr_t* hdr = static_cast<hdr_t*>(g_malloc_dispatch->malloc(size));
    if (hdr) {
        hdr->base = hdr;
        hdr->bt_depth = GET_BACKTRACE(hdr->bt, MAX_BACKTRACE_DEPTH);
        add(hdr, bytes);
        return user(hdr);
    }
    return NULL;
}

extern "C" void* chk_memalign(size_t alignment, size_t bytes) {
    if (DebugCallsDisabled()) {
        return g_malloc_dispatch->memalign(alignment, bytes);
    }

    if (alignment <= MALLOC_ALIGNMENT) {
        return chk_malloc(bytes);
    }

    // Make the alignment a power of two.
    if (!powerof2(alignment)) {
        alignment = BIONIC_ROUND_UP_POWER_OF_2(alignment);
    }

    // here, alignment is at least MALLOC_ALIGNMENT<<1 bytes
    // we will align by at least MALLOC_ALIGNMENT bytes
    // and at most alignment-MALLOC_ALIGNMENT bytes
    size_t size = (alignment-MALLOC_ALIGNMENT) + bytes;
    if (size < bytes) { // Overflow.
        return NULL;
    }

    void* base = g_malloc_dispatch->malloc(sizeof(hdr_t) + size + sizeof(ftr_t));
    if (base != NULL) {
        // Check that the actual pointer that will be returned is aligned
        // properly.
        uintptr_t ptr = reinterpret_cast<uintptr_t>(user(reinterpret_cast<hdr_t*>(base)));
        if ((ptr % alignment) != 0) {
            // Align the pointer.
            ptr += ((-ptr) % alignment);
        }

        hdr_t* hdr = meta(reinterpret_cast<void*>(ptr));
        hdr->base = base;
        hdr->bt_depth = GET_BACKTRACE(hdr->bt, MAX_BACKTRACE_DEPTH);
        add(hdr, bytes);
        return user(hdr);
    }
    return base;
}

extern "C" void chk_free(void* ptr) {
//  log_message("%s: %s\n", __FILE__, __FUNCTION__);
    if (DebugCallsDisabled()) {
        return g_malloc_dispatch->free(ptr);
    }

    if (!ptr) /* ignore free(NULL) */
        return;

    hdr_t* hdr = meta(ptr);

    if (del(hdr) < 0) {
        uintptr_t bt[MAX_BACKTRACE_DEPTH];
        int depth = GET_BACKTRACE(bt, MAX_BACKTRACE_DEPTH);
        if (hdr->tag == BACKLOG_TAG) {
            log_message("+++ ALLOCATION %p SIZE %d BYTES MULTIPLY FREED!\n",
                       user(hdr), hdr->size);
            if (g_backtrace_enabled) {
                log_message("+++ ALLOCATION %p SIZE %d ALLOCATED HERE:\n",
                          user(hdr), hdr->size);
                log_backtrace(hdr->bt, hdr->bt_depth);
                /* hdr->freed_bt_depth should be nonzero here */
                log_message("+++ ALLOCATION %p SIZE %d FIRST FREED HERE:\n",
                          user(hdr), hdr->size);
                log_backtrace(hdr->freed_bt, hdr->freed_bt_depth);
                log_message("+++ ALLOCATION %p SIZE %d NOW BEING FREED HERE:\n",
                          user(hdr), hdr->size);
                log_backtrace(bt, depth);
            }
        } else {
            log_message("+++ ALLOCATION %p IS CORRUPTED OR NOT ALLOCATED VIA TRACKER!\n",
                       user(hdr));
            if (g_backtrace_enabled) {
                log_backtrace(bt, depth);
            }
        }
    } else {
        hdr->freed_bt_depth = GET_BACKTRACE(hdr->freed_bt, MAX_BACKTRACE_DEPTH);
        add_to_backlog(hdr);
    }
}

extern "C" void* chk_realloc(void* ptr, size_t bytes) {
//  log_message("%s: %s\n", __FILE__, __FUNCTION__);
    if (DebugCallsDisabled()) {
        return g_malloc_dispatch->realloc(ptr, bytes);
    }

    if (!ptr) {
        return chk_malloc(bytes);
    }

#ifdef REALLOC_ZERO_BYTES_FREE
    if (!bytes) {
        chk_free(ptr);
        return NULL;
    }
#endif

    hdr_t* hdr = meta(ptr);

    if (del(hdr) < 0) {
        uintptr_t bt[MAX_BACKTRACE_DEPTH];
        int depth = GET_BACKTRACE(bt, MAX_BACKTRACE_DEPTH);
        if (hdr->tag == BACKLOG_TAG) {
            log_message("+++ REALLOCATION %p SIZE %d OF FREED MEMORY!\n",
                       user(hdr), bytes, hdr->size);
            if (g_backtrace_enabled) {
                log_message("+++ ALLOCATION %p SIZE %d ALLOCATED HERE:\n",
                          user(hdr), hdr->size);
                log_backtrace(hdr->bt, hdr->bt_depth);
                /* hdr->freed_bt_depth should be nonzero here */
                log_message("+++ ALLOCATION %p SIZE %d FIRST FREED HERE:\n",
                          user(hdr), hdr->size);
                log_backtrace(hdr->freed_bt, hdr->freed_bt_depth);
                log_message("+++ ALLOCATION %p SIZE %d NOW BEING REALLOCATED HERE:\n",
                          user(hdr), hdr->size);
                log_backtrace(bt, depth);
            }

            /* We take the memory out of the backlog and fall through so the
             * reallocation below succeeds.  Since we didn't really free it, we
             * can default to this behavior.
             */
            del_from_backlog(hdr);
        } else {
            log_message("+++ REALLOCATION %p SIZE %d IS CORRUPTED OR NOT ALLOCATED VIA TRACKER!\n",
                       user(hdr), bytes);
            if (g_backtrace_enabled) {
                log_backtrace(bt, depth);
            }
            // just get a whole new allocation and leak the old one
            return g_malloc_dispatch->realloc(0, bytes);
            // return realloc(user(hdr), bytes); // assuming it was allocated externally
        }
    }

    size_t size = sizeof(hdr_t) + bytes + sizeof(ftr_t);
    if (size < bytes) { // Overflow
        errno = ENOMEM;
        return NULL;
    }
    if (hdr->base != hdr) {
        // An allocation from memalign, so create another allocation and
        // copy the data out.
        void* newMem = g_malloc_dispatch->malloc(size);
        if (newMem == NULL) {
            return NULL;
        }
        memcpy(newMem, hdr, sizeof(hdr_t) + hdr->size);
        g_malloc_dispatch->free(hdr->base);
        hdr = static_cast<hdr_t*>(newMem);
    } else {
        hdr = static_cast<hdr_t*>(g_malloc_dispatch->realloc(hdr, size));
    }
    if (hdr) {
        hdr->base = hdr;
        hdr->bt_depth = GET_BACKTRACE(hdr->bt, MAX_BACKTRACE_DEPTH);
        add(hdr, bytes);
        return user(hdr);
    }
    return NULL;
}

extern "C" void* chk_calloc(size_t nmemb, size_t bytes) {
//  log_message("%s: %s\n", __FILE__, __FUNCTION__);
    if (DebugCallsDisabled()) {
        return g_malloc_dispatch->calloc(nmemb, bytes);
    }

    size_t total_bytes = nmemb * bytes;
    size_t size = sizeof(hdr_t) + total_bytes + sizeof(ftr_t);
    if (size < total_bytes || (nmemb && SIZE_MAX / nmemb < bytes)) { // Overflow
        errno = ENOMEM;
        return NULL;
    }
    hdr_t* hdr = static_cast<hdr_t*>(g_malloc_dispatch->calloc(1, size));
    if (hdr) {
        hdr->base = hdr;
        hdr->bt_depth = GET_BACKTRACE(hdr->bt, MAX_BACKTRACE_DEPTH);
        add(hdr, total_bytes);
        return user(hdr);
    }
    return NULL;
}

extern "C" size_t chk_malloc_usable_size(const void* ptr) {
    if (DebugCallsDisabled()) {
        return g_malloc_dispatch->malloc_usable_size(ptr);
    }

    // malloc_usable_size returns 0 for NULL and unknown blocks.
    if (ptr == NULL)
        return 0;

    const hdr_t* hdr = const_meta(ptr);

    // The sentinel tail is written just after the request block bytes
    // so there is no extra room we can report here.
    return hdr->size;
}

extern "C" struct mallinfo chk_mallinfo() {
  return g_malloc_dispatch->mallinfo();
}

extern "C" int chk_posix_memalign(void** memptr, size_t alignment, size_t size) {
  if (DebugCallsDisabled()) {
    return g_malloc_dispatch->posix_memalign(memptr, alignment, size);
  }

  if (!powerof2(alignment)) {
    return EINVAL;
  }
  int saved_errno = errno;
  *memptr = chk_memalign(alignment, size);
  errno = saved_errno;
  return (*memptr != NULL) ? 0 : ENOMEM;
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* chk_pvalloc(size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_malloc_dispatch->pvalloc(bytes);
  }

  size_t pagesize = getpagesize();
  size_t size = BIONIC_ALIGN(bytes, pagesize);
  if (size < bytes) { // Overflow
    return NULL;
  }
  return chk_memalign(pagesize, size);
}

extern "C" void* chk_valloc(size_t size) {
  if (DebugCallsDisabled()) {
    return g_malloc_dispatch->valloc(size);
  }
  return chk_memalign(getpagesize(), size);
}
#endif

static void ReportMemoryLeaks() {
  ScopedDisableDebugCalls disable;

  // Use /proc/self/exe link to obtain the program name for logging
  // purposes. If it's not available, we set it to "<unknown>".
  char exe[PATH_MAX];
  int count;
  if ((count = readlink("/proc/self/exe", exe, sizeof(exe) - 1)) == -1) {
    strlcpy(exe, "<unknown>", sizeof(exe));
  } else {
    exe[count] = '\0';
  }

  if (g_allocated_block_count == 0) {
    log_message("+++ %s did not leak", exe);
    return;
  }

  size_t index = 1;
  const size_t total = g_allocated_block_count;
  while (head != NULL) {
    int safe;
    hdr_t* block = head;
    log_message("+++ %s leaked block of size %d at %p (leak %d of %d)",
                exe, block->size, user(block), index++, total);
    if (del_leak(block, &safe) && g_backtrace_enabled) {
      /* safe == 1, because the allocation is valid */
      log_backtrace(block->bt, block->bt_depth);
    }
  }

  while (backlog_head != NULL) {
    del_from_backlog(backlog_tail);
  }
}

pthread_key_t g_debug_calls_disabled;

extern "C" bool malloc_debug_initialize(HashTable* hash_table, const MallocDebug* malloc_dispatch) {
  g_hash_table = hash_table;
  g_malloc_dispatch = malloc_dispatch;

  pthread_key_create(&g_debug_calls_disabled, NULL);

  char debug_backlog[PROP_VALUE_MAX];
  if (__system_property_get("libc.debug.malloc.backlog", debug_backlog)) {
    g_malloc_debug_backlog = atoi(debug_backlog);
    info_log("%s: setting backlog length to %d\n", getprogname(), g_malloc_debug_backlog);
  }

  // Check if backtracing should be disabled.
  char env[PROP_VALUE_MAX];
  if (__system_property_get("libc.debug.malloc.nobacktrace", env) && atoi(env) != 0) {
    g_backtrace_enabled = false;
    __libc_format_log(ANDROID_LOG_INFO, "libc", "not gathering backtrace information\n");
  }

  if (g_backtrace_enabled) {
    backtrace_startup();
  }

  return true;
}

extern "C" void malloc_debug_finalize(int malloc_debug_level) {
  // We only track leaks at level 10.
  if (malloc_debug_level == 10) {
    ReportMemoryLeaks();
  }
  if (g_backtrace_enabled) {
    backtrace_shutdown();
  }

  pthread_setspecific(g_debug_calls_disabled, NULL);
}
