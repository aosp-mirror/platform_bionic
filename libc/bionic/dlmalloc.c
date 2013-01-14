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

#include "dlmalloc.h"

/* Bionic error handling declarations */
#define PROCEED_ON_ERROR 0
static void __bionic_heap_error(const char* msg, const char* function, void* p);
#define CORRUPTION_ERROR_ACTION(m) \
    __bionic_heap_error("HEAP MEMORY CORRUPTION", __FUNCTION__, NULL)
#define USAGE_ERROR_ACTION(m,p) \
    __bionic_heap_error("ARGUMENT IS INVALID HEAP ADDRESS", __FUNCTION__, p)

/* Bionic named anonymous memory declarations */
static void* named_anonymous_mmap(size_t length);
#define MMAP(s) named_anonymous_mmap(s)

/*
 * Ugly inclusion of C file so that bionic specific #defines configure
 * dlmalloc.
 */
#include "../upstream-dlmalloc/malloc.c"


/* Bionic error handling definitions */
/* Convert a pointer into hex string */
static void __bionic_itox(char* hex, void* ptr)
{
    intptr_t val = (intptr_t) ptr;
    /* Terminate with NULL */
    hex[8] = 0;
    int i;

    for (i = 7; i >= 0; i--) {
        int digit = val & 15;
        hex[i] = (digit <= 9) ? digit + '0' : digit - 10 + 'a';
        val >>= 4;
    }
}

#include <private/logd.h>
static void __bionic_heap_error(const char* msg, const char* function, void* p)
{
    /* We format the buffer explicitely, i.e. without using snprintf()
     * which may use malloc() internally. Not something we can trust
     * if we just detected a corrupted heap.
     */
    char buffer[256];
    strlcpy(buffer, "@@@ ABORTING: LIBC: ", sizeof(buffer));
    strlcat(buffer, msg, sizeof(buffer));
    if (function != NULL) {
        strlcat(buffer, " IN ", sizeof(buffer));
        strlcat(buffer, function, sizeof(buffer));
    }

    if (p != NULL) {
        char hexbuffer[9];
        __bionic_itox(hexbuffer, p);
        strlcat(buffer, " addr=0x", sizeof(buffer));
        strlcat(buffer, hexbuffer, sizeof(buffer));
    }

    __libc_android_log_write(ANDROID_LOG_FATAL, "libc", buffer);

    /* So that we can get a memory dump around p */
    *((int **) 0xdeadbaad) = (int *) p;
}

/* Bionic named anonymous memory definitions */
#include <linux/ashmem.h>
static int __ashmem_create_region(const char* name, size_t size)
{
    int fd, ret;
    fd = open("/dev/ashmem", O_RDWR);
    if (fd < 0)
        return fd;
    if (name != NULL) {
        char buf[ASHMEM_NAME_LEN];

        strlcpy(buf, name, sizeof(buf));
        ret = ioctl(fd, ASHMEM_SET_NAME, buf);
        if (ret < 0) {  /* error */
            close(fd);
            return ret;
        }
    }
    ret = ioctl(fd, ASHMEM_SET_SIZE, size);
    if (ret < 0) {  /* error */
        close(fd);
        return ret;
    }
    return fd;
}

static void* named_anonymous_mmap(size_t length)
{
    void* ret;
    int fd = __ashmem_create_region("libc malloc", length);
    if (fd < 0)
        return MAP_FAILED;
    ret = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    close (fd);
    return ret;
}
