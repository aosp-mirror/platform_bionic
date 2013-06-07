/*
 * Copyright (C) 2009 The Android Open Source Project
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

/*
 * Contains definition of structures, global variables, and implementation of
 * routines that are used by malloc leak detection code and other components in
 * the system. The trick is that some components expect these data and
 * routines to be defined / implemented in libc.so library, regardless
 * whether or not MALLOC_LEAK_CHECK macro is defined. To make things even
 * more tricky, malloc leak detection code, implemented in
 * libc_malloc_debug.so also requires access to these variables and routines
 * (to fill allocation entry hash table, for example). So, all relevant
 * variables and routines are defined / implemented here and exported
 * to all, leak detection code and other components via dynamic (libc.so),
 * or static (libc.a) linking.
 */

#include "malloc_debug_common.h"

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "dlmalloc.h"
#include "ScopedPthreadMutexLocker.h"

/*
 * In a VM process, this is set to 1 after fork()ing out of zygote.
 */
int gMallocLeakZygoteChild = 0;

pthread_mutex_t gAllocationsMutex = PTHREAD_MUTEX_INITIALIZER;
HashTable gHashTable;

// =============================================================================
// output functions
// =============================================================================

static int hash_entry_compare(const void* arg1, const void* arg2) {
    int result;

    const HashEntry* e1 = *static_cast<HashEntry* const*>(arg1);
    const HashEntry* e2 = *static_cast<HashEntry* const*>(arg2);

    // if one or both arg pointers are null, deal gracefully
    if (e1 == NULL) {
        result = (e2 == NULL) ? 0 : 1;
    } else if (e2 == NULL) {
        result = -1;
    } else {
        size_t nbAlloc1 = e1->allocations;
        size_t nbAlloc2 = e2->allocations;
        size_t size1 = e1->size & ~SIZE_FLAG_MASK;
        size_t size2 = e2->size & ~SIZE_FLAG_MASK;
        size_t alloc1 = nbAlloc1 * size1;
        size_t alloc2 = nbAlloc2 * size2;

        // sort in descending order by:
        // 1) total size
        // 2) number of allocations
        //
        // This is used for sorting, not determination of equality, so we don't
        // need to compare the bit flags.
        if (alloc1 > alloc2) {
            result = -1;
        } else if (alloc1 < alloc2) {
            result = 1;
        } else {
            if (nbAlloc1 > nbAlloc2) {
                result = -1;
            } else if (nbAlloc1 < nbAlloc2) {
                result = 1;
            } else {
                result = 0;
            }
        }
    }
    return result;
}

/*
 * Retrieve native heap information.
 *
 * "*info" is set to a buffer we allocate
 * "*overallSize" is set to the size of the "info" buffer
 * "*infoSize" is set to the size of a single entry
 * "*totalMemory" is set to the sum of all allocations we're tracking; does
 *   not include heap overhead
 * "*backtraceSize" is set to the maximum number of entries in the back trace
 */
extern "C" void get_malloc_leak_info(uint8_t** info, size_t* overallSize,
        size_t* infoSize, size_t* totalMemory, size_t* backtraceSize) {
    // don't do anything if we have invalid arguments
    if (info == NULL || overallSize == NULL || infoSize == NULL ||
            totalMemory == NULL || backtraceSize == NULL) {
        return;
    }
    *totalMemory = 0;

    ScopedPthreadMutexLocker locker(&gAllocationsMutex);

    if (gHashTable.count == 0) {
        *info = NULL;
        *overallSize = 0;
        *infoSize = 0;
        *backtraceSize = 0;
        return;
    }

    HashEntry** list = static_cast<HashEntry**>(dlmalloc(sizeof(void*) * gHashTable.count));

    // get the entries into an array to be sorted
    int index = 0;
    for (size_t i = 0 ; i < HASHTABLE_SIZE ; ++i) {
        HashEntry* entry = gHashTable.slots[i];
        while (entry != NULL) {
            list[index] = entry;
            *totalMemory = *totalMemory +
                ((entry->size & ~SIZE_FLAG_MASK) * entry->allocations);
            index++;
            entry = entry->next;
        }
    }

    // XXX: the protocol doesn't allow variable size for the stack trace (yet)
    *infoSize = (sizeof(size_t) * 2) + (sizeof(uintptr_t) * BACKTRACE_SIZE);
    *overallSize = *infoSize * gHashTable.count;
    *backtraceSize = BACKTRACE_SIZE;

    // now get a byte array big enough for this
    *info = static_cast<uint8_t*>(dlmalloc(*overallSize));

    if (*info == NULL) {
        *overallSize = 0;
        dlfree(list);
        return;
    }

    qsort(list, gHashTable.count, sizeof(void*), hash_entry_compare);

    uint8_t* head = *info;
    const int count = gHashTable.count;
    for (int i = 0 ; i < count ; ++i) {
        HashEntry* entry = list[i];
        size_t entrySize = (sizeof(size_t) * 2) + (sizeof(uintptr_t) * entry->numEntries);
        if (entrySize < *infoSize) {
            /* we're writing less than a full entry, clear out the rest */
            memset(head + entrySize, 0, *infoSize - entrySize);
        } else {
            /* make sure the amount we're copying doesn't exceed the limit */
            entrySize = *infoSize;
        }
        memcpy(head, &(entry->size), entrySize);
        head += *infoSize;
    }

    dlfree(list);
}

extern "C" void free_malloc_leak_info(uint8_t* info) {
    dlfree(info);
}

extern "C" struct mallinfo mallinfo() {
    return dlmallinfo();
}

extern "C" void* valloc(size_t bytes) {
    return dlvalloc(bytes);
}

extern "C" void* pvalloc(size_t bytes) {
    return dlpvalloc(bytes);
}

extern "C" int posix_memalign(void** memptr, size_t alignment, size_t size) {
    return dlposix_memalign(memptr, alignment, size);
}

/* Support for malloc debugging.
 * Note that if USE_DL_PREFIX is not defined, it's assumed that memory
 * allocation routines are implemented somewhere else, so all our custom
 * malloc routines should not be compiled at all.
 */
#ifdef USE_DL_PREFIX

/* Table for dispatching malloc calls, initialized with default dispatchers. */
extern const MallocDebug __libc_malloc_default_dispatch;
const MallocDebug __libc_malloc_default_dispatch __attribute__((aligned(32))) =
{
    dlmalloc, dlfree, dlcalloc, dlrealloc, dlmemalign, dlmalloc_usable_size,
};

/* Selector of dispatch table to use for dispatching malloc calls. */
const MallocDebug* __libc_malloc_dispatch = &__libc_malloc_default_dispatch;

extern "C" void* malloc(size_t bytes) {
    return __libc_malloc_dispatch->malloc(bytes);
}

extern "C" void free(void* mem) {
    __libc_malloc_dispatch->free(mem);
}

extern "C" void* calloc(size_t n_elements, size_t elem_size) {
    return __libc_malloc_dispatch->calloc(n_elements, elem_size);
}

extern "C" void* realloc(void* oldMem, size_t bytes) {
    return __libc_malloc_dispatch->realloc(oldMem, bytes);
}

extern "C" void* memalign(size_t alignment, size_t bytes) {
    return __libc_malloc_dispatch->memalign(alignment, bytes);
}

extern "C" size_t malloc_usable_size(const void* mem) {
    return __libc_malloc_dispatch->malloc_usable_size(mem);
}

/* We implement malloc debugging only in libc.so, so code below
 * must be excluded if we compile this file for static libc.a
 */
#ifndef LIBC_STATIC
#include <sys/system_properties.h>
#include <dlfcn.h>
#include <stdio.h>
#include "libc_logging.h"

/* Table for dispatching malloc calls, depending on environment. */
static MallocDebug gMallocUse __attribute__((aligned(32))) = {
    dlmalloc, dlfree, dlcalloc, dlrealloc, dlmemalign, dlmalloc_usable_size
};

extern const char* __progname;

/* Handle to shared library where actual memory allocation is implemented.
 * This library is loaded and memory allocation calls are redirected there
 * when libc.debug.malloc environment variable contains value other than
 * zero:
 * 1  - For memory leak detections.
 * 5  - For filling allocated / freed memory with patterns defined by
 *      CHK_SENTINEL_VALUE, and CHK_FILL_FREE macros.
 * 10 - For adding pre-, and post- allocation stubs in order to detect
 *      buffer overruns.
 * Note that emulator's memory allocation instrumentation is not controlled by
 * libc.debug.malloc value, but rather by emulator, started with -memcheck
 * option. Note also, that if emulator has started with -memcheck option,
 * emulator's instrumented memory allocation will take over value saved in
 * libc.debug.malloc. In other words, if emulator has started with -memcheck
 * option, libc.debug.malloc value is ignored.
 * Actual functionality for debug levels 1-10 is implemented in
 * libc_malloc_debug_leak.so, while functionality for emultor's instrumented
 * allocations is implemented in libc_malloc_debug_qemu.so and can be run inside
 * the emulator only.
 */
static void* libc_malloc_impl_handle = NULL;

/* This variable is set to the value of property libc.debug.malloc.backlog,
 * when the value of libc.debug.malloc = 10.  It determines the size of the
 * backlog we use to detect multiple frees.  If the property is not set, the
 * backlog length defaults to BACKLOG_DEFAULT_LEN.
 */
unsigned int gMallocDebugBacklog;
#define BACKLOG_DEFAULT_LEN 100

/* The value of libc.debug.malloc. */
int gMallocDebugLevel;

template<typename FunctionType>
void InitMallocFunction(void* malloc_impl_handler, FunctionType* func, const char* prefix, const char* suffix) {
    char symbol[128];
    snprintf(symbol, sizeof(symbol), "%s_%s", prefix, suffix);
    *func = reinterpret_cast<FunctionType>(dlsym(malloc_impl_handler, symbol));
    if (*func == NULL) {
        error_log("%s: dlsym(\"%s\") failed", __progname, symbol);
    }
}

static void InitMalloc(void* malloc_impl_handler, MallocDebug* table, const char* prefix) {
    __libc_format_log(ANDROID_LOG_INFO, "libc", "%s: using libc.debug.malloc %d (%s)\n",
                      __progname, gMallocDebugLevel, prefix);

    InitMallocFunction<MallocDebugMalloc>(malloc_impl_handler, &table->malloc, prefix, "malloc");
    InitMallocFunction<MallocDebugFree>(malloc_impl_handler, &table->free, prefix, "free");
    InitMallocFunction<MallocDebugCalloc>(malloc_impl_handler, &table->calloc, prefix, "calloc");
    InitMallocFunction<MallocDebugRealloc>(malloc_impl_handler, &table->realloc, prefix, "realloc");
    InitMallocFunction<MallocDebugMemalign>(malloc_impl_handler, &table->memalign, prefix, "memalign");
    InitMallocFunction<MallocDebugMallocUsableSize>(malloc_impl_handler, &table->malloc_usable_size, prefix, "malloc_usable_size");
}

/* Initializes memory allocation framework once per process. */
static void malloc_init_impl() {
    const char* so_name = NULL;
    MallocDebugInit malloc_debug_initialize = NULL;
    unsigned int qemu_running = 0;
    unsigned int memcheck_enabled = 0;
    char env[PROP_VALUE_MAX];
    char memcheck_tracing[PROP_VALUE_MAX];
    char debug_program[PROP_VALUE_MAX];

    /* Get custom malloc debug level. Note that emulator started with
     * memory checking option will have priority over debug level set in
     * libc.debug.malloc system property. */
    if (__system_property_get("ro.kernel.qemu", env) && atoi(env)) {
        qemu_running = 1;
        if (__system_property_get("ro.kernel.memcheck", memcheck_tracing)) {
            if (memcheck_tracing[0] != '0') {
                // Emulator has started with memory tracing enabled. Enforce it.
                gMallocDebugLevel = 20;
                memcheck_enabled = 1;
            }
        }
    }

    /* If debug level has not been set by memcheck option in the emulator,
     * lets grab it from libc.debug.malloc system property. */
    if (gMallocDebugLevel == 0 && __system_property_get("libc.debug.malloc", env)) {
        gMallocDebugLevel = atoi(env);
    }

    /* Debug level 0 means that we should use dlxxx allocation
     * routines (default). */
    if (gMallocDebugLevel == 0) {
        return;
    }

    /* If libc.debug.malloc.program is set and is not a substring of progname,
     * then exit.
     */
    if (__system_property_get("libc.debug.malloc.program", debug_program)) {
        if (!strstr(__progname, debug_program)) {
            return;
        }
    }

    // mksh is way too leaky. http://b/7291287.
    if (gMallocDebugLevel >= 10) {
        if (strcmp(__progname, "sh") == 0 || strcmp(__progname, "/system/bin/sh") == 0) {
            return;
        }
    }

    // Choose the appropriate .so for the requested debug level.
    switch (gMallocDebugLevel) {
        case 1:
        case 5:
        case 10: {
            char debug_backlog[PROP_VALUE_MAX];
            if (__system_property_get("libc.debug.malloc.backlog", debug_backlog)) {
                gMallocDebugBacklog = atoi(debug_backlog);
                info_log("%s: setting backlog length to %d\n", __progname, gMallocDebugBacklog);
            }
            if (gMallocDebugBacklog == 0) {
                gMallocDebugBacklog = BACKLOG_DEFAULT_LEN;
            }
            so_name = "/system/lib/libc_malloc_debug_leak.so";
            break;
        }
        case 20:
            // Quick check: debug level 20 can only be handled in emulator.
            if (!qemu_running) {
                error_log("%s: Debug level %d can only be set in emulator\n",
                          __progname, gMallocDebugLevel);
                return;
            }
            // Make sure that memory checking has been enabled in emulator.
            if (!memcheck_enabled) {
                error_log("%s: Memory checking is not enabled in the emulator\n",
                          __progname);
                return;
            }
            so_name = "/system/lib/libc_malloc_debug_qemu.so";
            break;
        default:
            error_log("%s: Debug level %d is unknown\n", __progname, gMallocDebugLevel);
            return;
    }

    // Load .so that implements the required malloc debugging functionality.
    void* malloc_impl_handle = dlopen(so_name, RTLD_LAZY);
    if (malloc_impl_handle == NULL) {
        error_log("%s: Missing module %s required for malloc debug level %d: %s",
                  __progname, so_name, gMallocDebugLevel, dlerror());
        return;
    }

    // Initialize malloc debugging in the loaded module.
    malloc_debug_initialize = reinterpret_cast<MallocDebugInit>(dlsym(malloc_impl_handle,
                                                                      "malloc_debug_initialize"));
    if (malloc_debug_initialize == NULL) {
        error_log("%s: Initialization routine is not found in %s\n",
                  __progname, so_name);
        dlclose(malloc_impl_handle);
        return;
    }
    if (malloc_debug_initialize() == -1) {
        dlclose(malloc_impl_handle);
        return;
    }

    if (gMallocDebugLevel == 20) {
        // For memory checker we need to do extra initialization.
        typedef int (*MemCheckInit)(int, const char*);
        MemCheckInit memcheck_initialize =
            reinterpret_cast<MemCheckInit>(dlsym(malloc_impl_handle,
                                                 "memcheck_initialize"));
        if (memcheck_initialize == NULL) {
            error_log("%s: memcheck_initialize routine is not found in %s\n",
                      __progname, so_name);
            dlclose(malloc_impl_handle);
            return;
        }

        if (memcheck_initialize(MALLOC_ALIGNMENT, memcheck_tracing)) {
            dlclose(malloc_impl_handle);
            return;
        }
    }


    // Initialize malloc dispatch table with appropriate routines.
    switch (gMallocDebugLevel) {
        case 1:
            InitMalloc(malloc_impl_handle, &gMallocUse, "leak");
            break;
        case 5:
            InitMalloc(malloc_impl_handle, &gMallocUse, "fill");
            break;
        case 10:
            InitMalloc(malloc_impl_handle, &gMallocUse, "chk");
            break;
        case 20:
            InitMalloc(malloc_impl_handle, &gMallocUse, "qemu_instrumented");
            break;
        default:
            break;
    }

    // Make sure dispatch table is initialized
    if ((gMallocUse.malloc == NULL) ||
        (gMallocUse.free == NULL) ||
        (gMallocUse.calloc == NULL) ||
        (gMallocUse.realloc == NULL) ||
        (gMallocUse.memalign == NULL) ||
        (gMallocUse.malloc_usable_size == NULL)) {
        error_log("%s: some symbols for libc.debug.malloc level %d were not found (see above)",
                  __progname, gMallocDebugLevel);
        dlclose(malloc_impl_handle);
    } else {
        __libc_malloc_dispatch = &gMallocUse;
        libc_malloc_impl_handle = malloc_impl_handle;
    }
}

static void malloc_fini_impl() {
    // Our BSD stdio implementation doesn't close the standard streams, it only flushes them.
    // And it doesn't do that until its atexit handler (_cleanup) is run, and we run first!
    // It's great that other unclosed FILE*s show up as malloc leaks, but we need to manually
    // clean up the standard streams ourselves.
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    if (libc_malloc_impl_handle != NULL) {
        MallocDebugFini malloc_debug_finalize =
            reinterpret_cast<MallocDebugFini>(dlsym(libc_malloc_impl_handle,
                                                    "malloc_debug_finalize"));
        if (malloc_debug_finalize != NULL) {
            malloc_debug_finalize();
        }
    }
}

static pthread_once_t  malloc_init_once_ctl = PTHREAD_ONCE_INIT;
static pthread_once_t  malloc_fini_once_ctl = PTHREAD_ONCE_INIT;

#endif  // !LIBC_STATIC
#endif  // USE_DL_PREFIX

/* Initializes memory allocation framework.
 * This routine is called from __libc_init routines implemented
 * in libc_init_static.c and libc_init_dynamic.c files.
 */
extern "C" __LIBC_HIDDEN__ void malloc_debug_init() {
    /* We need to initialize malloc iff we implement here custom
     * malloc routines (i.e. USE_DL_PREFIX is defined) for libc.so */
#if defined(USE_DL_PREFIX) && !defined(LIBC_STATIC)
    if (pthread_once(&malloc_init_once_ctl, malloc_init_impl)) {
        error_log("Unable to initialize malloc_debug component.");
    }
#endif  // USE_DL_PREFIX && !LIBC_STATIC
}

extern "C" __LIBC_HIDDEN__ void malloc_debug_fini() {
    /* We need to finalize malloc iff we implement here custom
     * malloc routines (i.e. USE_DL_PREFIX is defined) for libc.so */
#if defined(USE_DL_PREFIX) && !defined(LIBC_STATIC)
    if (pthread_once(&malloc_fini_once_ctl, malloc_fini_impl)) {
        error_log("Unable to finalize malloc_debug component.");
    }
#endif  // USE_DL_PREFIX && !LIBC_STATIC
}
