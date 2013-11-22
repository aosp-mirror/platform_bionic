/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include "private/bionic_name_mem.h"

/*
 * Local definitions of custom prctl arguments to set a vma name in some kernels
 */
#define BIONIC_PR_SET_VMA               0x53564d41
#define BIONIC_PR_SET_VMA_ANON_NAME     0

/*
 * Names a region of memory.  The name is expected to show up in /proc/pid/maps
 * and /proc/pid/smaps.  There is no guarantee that it will work, and it if it
 * does work it is likely to only work on memory that was allocated with
 * mmap(MAP_ANONYMOUS), and only on regions that are page aligned.  name should
 * be a pointer to a string that is valid for as long as the memory is mapped,
 * preferably a compile-time constant string.
 *
 * Returns -1 on error and sets errno.  If it returns an error naming page
 * aligned anonymous memory the kernel doesn't support naming, and an alternate
 * method of naming memory should be used (like ashmem).
 */
int __bionic_name_mem(void *addr, size_t len, const char *name)
{
    return prctl(BIONIC_PR_SET_VMA, BIONIC_PR_SET_VMA_ANON_NAME,
                 addr, len, name);
}
