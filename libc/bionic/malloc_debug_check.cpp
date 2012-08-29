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

#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unwind.h>
#include <dlfcn.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/system_properties.h>

#include "dlmalloc.h"
#include "logd.h"

#include "malloc_debug_common.h"
#include "malloc_debug_check_mapinfo.h"

static mapinfo *milist;

/* libc.debug.malloc.backlog */
extern unsigned int malloc_double_free_backlog;

#define MAX_BACKTRACE_DEPTH 15
#define ALLOCATION_TAG      0x1ee7d00d
#define BACKLOG_TAG         0xbabecafe
#define FREE_POISON         0xa5
#define BACKLOG_DEFAULT_LEN 100
#define FRONT_GUARD         0xaa
#define FRONT_GUARD_LEN     (1<<5)
#define REAR_GUARD          0xbb
#define REAR_GUARD_LEN      (1<<5)

static void log_message(const char* format, ...) {
    extern const MallocDebug __libc_malloc_default_dispatch;
    extern const MallocDebug* __libc_malloc_dispatch;
    extern pthread_mutex_t gAllocationsMutex;

    va_list args;
    {
        ScopedPthreadMutexLocker locker(&gAllocationsMutex);
        const MallocDebug* current_dispatch = __libc_malloc_dispatch;
        __libc_malloc_dispatch = &__libc_malloc_default_dispatch;
        va_start(args, format);
        __libc_android_log_vprint(ANDROID_LOG_ERROR, "libc", format, args);
        va_end(args);
        __libc_malloc_dispatch = current_dispatch;
    }
}

struct hdr_t {
    uint32_t tag;
    hdr_t* prev;
    hdr_t* next;
    intptr_t bt[MAX_BACKTRACE_DEPTH];
    int bt_depth;
    intptr_t freed_bt[MAX_BACKTRACE_DEPTH];
    int freed_bt_depth;
    size_t size;
    char front_guard[FRONT_GUARD_LEN];
} __attribute__((packed));

struct ftr_t {
    char rear_guard[REAR_GUARD_LEN];
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

static unsigned num;
static hdr_t *tail;
static hdr_t *head;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static unsigned backlog_num;
static hdr_t *backlog_tail;
static hdr_t *backlog_head;
static pthread_mutex_t backlog_lock = PTHREAD_MUTEX_INITIALIZER;

extern __LIBC_HIDDEN__ int get_backtrace(intptr_t* addrs, size_t max_entries);

static void print_backtrace(const intptr_t *bt, unsigned int depth) {
    const mapinfo *mi;
    unsigned int cnt;
    unsigned int rel_pc;
    intptr_t self_bt[MAX_BACKTRACE_DEPTH];

    if (!bt) {
        depth = get_backtrace(self_bt, MAX_BACKTRACE_DEPTH);
        bt = self_bt;
    }

    log_message("*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***\n");
    for (cnt = 0; cnt < depth && cnt < MAX_BACKTRACE_DEPTH; cnt++) {
        mi = pc_to_mapinfo(milist, bt[cnt], &rel_pc);
        log_message("\t#%02d  pc %08x  %s\n", cnt,
                   mi ? (intptr_t)rel_pc : bt[cnt],
                   mi ? mi->name : "(unknown)");
    }
}

static inline void init_front_guard(hdr_t *hdr) {
    memset(hdr->front_guard, FRONT_GUARD, FRONT_GUARD_LEN);
}

static inline bool is_front_guard_valid(hdr_t *hdr) {
    for (size_t i = 0; i < FRONT_GUARD_LEN; i++) {
        if (hdr->front_guard[i] != FRONT_GUARD) {
            return 0;
        }
    }
    return 1;
}

static inline void init_rear_guard(hdr_t *hdr) {
    ftr_t* ftr = to_ftr(hdr);
    memset(ftr->rear_guard, REAR_GUARD, REAR_GUARD_LEN);
}

static inline bool is_rear_guard_valid(hdr_t *hdr) {
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

static inline void add_locked(hdr_t *hdr, hdr_t **tail, hdr_t **head) {
    hdr->prev = NULL;
    hdr->next = *head;
    if (*head)
        (*head)->prev = hdr;
    else
        *tail = hdr;
    *head = hdr;
}

static inline int del_locked(hdr_t *hdr, hdr_t **tail, hdr_t **head) {
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

static inline void add(hdr_t *hdr, size_t size) {
    ScopedPthreadMutexLocker locker(&lock);
    hdr->tag = ALLOCATION_TAG;
    hdr->size = size;
    init_front_guard(hdr);
    init_rear_guard(hdr);
    num++;
    add_locked(hdr, &tail, &head);
}

static inline int del(hdr_t *hdr) {
    if (hdr->tag != ALLOCATION_TAG) {
        return -1;
    }

    ScopedPthreadMutexLocker locker(&lock);
    del_locked(hdr, &tail, &head);
    num--;
    return 0;
}

static inline void poison(hdr_t *hdr) {
    memset(user(hdr), FREE_POISON, hdr->size);
}

static int was_used_after_free(hdr_t *hdr) {
    unsigned i;
    const char *data = (const char *)user(hdr);
    for (i = 0; i < hdr->size; i++)
        if (data[i] != FREE_POISON)
            return 1;
    return 0;
}

/* returns 1 if valid, *safe == 1 if safe to dump stack */
static inline int check_guards(hdr_t *hdr, int *safe) {
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
static inline int check_allocation_locked(hdr_t *hdr, int *safe) {
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

    if (!valid && *safe) {
        log_message("+++ ALLOCATION %p SIZE %d ALLOCATED HERE:\n",
                        user(hdr), hdr->size);
        print_backtrace(hdr->bt, hdr->bt_depth);
        if (hdr->tag == BACKLOG_TAG) {
            log_message("+++ ALLOCATION %p SIZE %d FREED HERE:\n",
                       user(hdr), hdr->size);
            print_backtrace(hdr->freed_bt, hdr->freed_bt_depth);
        }
    }

    return valid;
}

static inline int del_and_check_locked(hdr_t *hdr,
                                       hdr_t **tail, hdr_t **head, unsigned *cnt,
                                       int *safe) {
    int valid = check_allocation_locked(hdr, safe);
    if (safe) {
        (*cnt)--;
        del_locked(hdr, tail, head);
    }
    return valid;
}

static inline void del_from_backlog_locked(hdr_t *hdr) {
    int safe;
    del_and_check_locked(hdr,
                         &backlog_tail, &backlog_head, &backlog_num,
                         &safe);
    hdr->tag = 0; /* clear the tag */
}

static inline void del_from_backlog(hdr_t *hdr) {
    ScopedPthreadMutexLocker locker(&backlog_lock);
    del_from_backlog_locked(hdr);
}

static inline int del_leak(hdr_t *hdr, int *safe) {
    ScopedPthreadMutexLocker locker(&lock);
    return del_and_check_locked(hdr, &tail, &head, &num, safe);
}

static inline void add_to_backlog(hdr_t *hdr) {
    ScopedPthreadMutexLocker locker(&backlog_lock);
    hdr->tag = BACKLOG_TAG;
    backlog_num++;
    add_locked(hdr, &backlog_tail, &backlog_head);
    poison(hdr);
    /* If we've exceeded the maximum backlog, clear it up */
    while (backlog_num > malloc_double_free_backlog) {
        hdr_t *gone = backlog_tail;
        del_from_backlog_locked(gone);
        dlfree(gone);
    }
}

extern "C" void* chk_malloc(size_t size) {
//  log_message("%s: %s\n", __FILE__, __FUNCTION__);

    hdr_t* hdr = static_cast<hdr_t*>(dlmalloc(sizeof(hdr_t) + size + sizeof(ftr_t)));
    if (hdr) {
        hdr->bt_depth = get_backtrace(hdr->bt, MAX_BACKTRACE_DEPTH);
        add(hdr, size);
        return user(hdr);
    }
    return NULL;
}

extern "C" void* chk_memalign(size_t, size_t bytes) {
//  log_message("%s: %s\n", __FILE__, __FUNCTION__);
    // XXX: it's better to use malloc, than being wrong
    return chk_malloc(bytes);
}

extern "C" void chk_free(void *ptr) {
//  log_message("%s: %s\n", __FILE__, __FUNCTION__);

    if (!ptr) /* ignore free(NULL) */
        return;

    hdr_t* hdr = meta(ptr);

    if (del(hdr) < 0) {
        intptr_t bt[MAX_BACKTRACE_DEPTH];
        int depth;
        depth = get_backtrace(bt, MAX_BACKTRACE_DEPTH);
        if (hdr->tag == BACKLOG_TAG) {
            log_message("+++ ALLOCATION %p SIZE %d BYTES MULTIPLY FREED!\n",
                       user(hdr), hdr->size);
            log_message("+++ ALLOCATION %p SIZE %d ALLOCATED HERE:\n",
                       user(hdr), hdr->size);
            print_backtrace(hdr->bt, hdr->bt_depth);
            /* hdr->freed_bt_depth should be nonzero here */
            log_message("+++ ALLOCATION %p SIZE %d FIRST FREED HERE:\n",
                       user(hdr), hdr->size);
            print_backtrace(hdr->freed_bt, hdr->freed_bt_depth);
            log_message("+++ ALLOCATION %p SIZE %d NOW BEING FREED HERE:\n",
                       user(hdr), hdr->size);
            print_backtrace(bt, depth);
        } else {
            log_message("+++ ALLOCATION %p IS CORRUPTED OR NOT ALLOCATED VIA TRACKER!\n",
                       user(hdr));
            print_backtrace(bt, depth);
            /* Leak here so that we do not crash */
            //dlfree(user(hdr));
        }
    } else {
        hdr->freed_bt_depth = get_backtrace(hdr->freed_bt,
                                      MAX_BACKTRACE_DEPTH);
        add_to_backlog(hdr);
    }
}

extern "C" void *chk_realloc(void *ptr, size_t size) {
//  log_message("%s: %s\n", __FILE__, __FUNCTION__);

    if (!size) {
        chk_free(ptr);
        return NULL;
    }

    if (!ptr) {
        return chk_malloc(size);
    }

    hdr_t* hdr = meta(ptr);

    if (del(hdr) < 0) {
        intptr_t bt[MAX_BACKTRACE_DEPTH];
        int depth;
        depth = get_backtrace(bt, MAX_BACKTRACE_DEPTH);
        if (hdr->tag == BACKLOG_TAG) {
            log_message("+++ REALLOCATION %p SIZE %d OF FREED MEMORY!\n",
                       user(hdr), size, hdr->size);
            log_message("+++ ALLOCATION %p SIZE %d ALLOCATED HERE:\n",
                       user(hdr), hdr->size);
            print_backtrace(hdr->bt, hdr->bt_depth);
            /* hdr->freed_bt_depth should be nonzero here */
            log_message("+++ ALLOCATION %p SIZE %d FIRST FREED HERE:\n",
                       user(hdr), hdr->size);
            print_backtrace(hdr->freed_bt, hdr->freed_bt_depth);
            log_message("+++ ALLOCATION %p SIZE %d NOW BEING REALLOCATED HERE:\n",
                       user(hdr), hdr->size);
            print_backtrace(bt, depth);

             /* We take the memory out of the backlog and fall through so the
             * reallocation below succeeds.  Since we didn't really free it, we
             * can default to this behavior.
             */
            del_from_backlog(hdr);
        } else {
            log_message("+++ REALLOCATION %p SIZE %d IS CORRUPTED OR NOT ALLOCATED VIA TRACKER!\n",
                       user(hdr), size);
            print_backtrace(bt, depth);
            // just get a whole new allocation and leak the old one
            return dlrealloc(0, size);
            // return dlrealloc(user(hdr), size); // assuming it was allocated externally
        }
    }

    hdr = static_cast<hdr_t*>(dlrealloc(hdr, sizeof(hdr_t) + size + sizeof(ftr_t)));
    if (hdr) {
        hdr->bt_depth = get_backtrace(hdr->bt, MAX_BACKTRACE_DEPTH);
        add(hdr, size);
        return user(hdr);
    }

    return NULL;
}

extern "C" void *chk_calloc(int nmemb, size_t size) {
//  log_message("%s: %s\n", __FILE__, __FUNCTION__);
    size_t total_size = nmemb * size;
    hdr_t* hdr = static_cast<hdr_t*>(dlcalloc(1, sizeof(hdr_t) + total_size + sizeof(ftr_t)));
    if (hdr) {
        hdr->bt_depth = get_backtrace(
                            hdr->bt, MAX_BACKTRACE_DEPTH);
        add(hdr, total_size);
        return user(hdr);
    }
    return NULL;
}

static void heaptracker_free_leaked_memory() {
    if (num) {
        log_message("+++ THERE ARE %d LEAKED ALLOCATIONS\n", num);
    }

    hdr_t *del = NULL;
    while (head) {
        int safe;
        del = head;
        log_message("+++ DELETING %d BYTES OF LEAKED MEMORY AT %p (%d REMAINING)\n",
                del->size, user(del), num);
        if (del_leak(del, &safe)) {
            /* safe == 1, because the allocation is valid */
            log_message("+++ ALLOCATION %p SIZE %d ALLOCATED HERE:\n",
                        user(del), del->size);
            print_backtrace(del->bt, del->bt_depth);
        }
        dlfree(del);
    }

//  log_message("+++ DELETING %d BACKLOGGED ALLOCATIONS\n", backlog_num);
    while (backlog_head) {
        del = backlog_tail;
        del_from_backlog(del);
        dlfree(del);
    }
}

/* Initializes malloc debugging framework.
 * See comments on MallocDebugInit in malloc_debug_common.h
 */
extern "C" int malloc_debug_initialize() {
    if (!malloc_double_free_backlog)
        malloc_double_free_backlog = BACKLOG_DEFAULT_LEN;
    milist = init_mapinfo(getpid());
    return 0;
}

extern "C" void malloc_debug_finalize() {
    heaptracker_free_leaked_memory();
    deinit_mapinfo(milist);
}
