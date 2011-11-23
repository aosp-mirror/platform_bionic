/*
 * Copyright (C) 2011 The Android Open Source Project
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


/* The purpose of this file is to export a small set of atomic-related
 * functions from the C library, to ensure binary ABI compatibility for
 * the NDK.
 *
 * These functions were initially exposed by the NDK through <sys/atomics.h>,
 * which was unfortunate because their implementation didn't provide any
 * memory barriers at all.
 *
 * This wasn't a problem for the platform code that used them, because it
 * used explicit barrier instructions around them. On the other hand, it means
 * that any NDK-generated machine code that linked against them would not
 * perform correctly when running on multi-core devices.
 *
 * To fix this, the platform code was first modified to not use any of these
 * functions (everything is now inlined through assembly statements, see
 * libc/private/bionic_arm_inline.h and the headers it includes.
 *
 * The functions here are thus only for the benefit of NDK applications,
 * and now includes full memory barriers to prevent any random memory ordering
 * issue from cropping.
 *
 * Note that we also provide an updated <sys/atomics.h> header that defines
 * always_inlined versions of the functions that use the GCC builtin
 * intrinsics to perform the same thing.
 *
 * NOTE: There is no need for a similar file for non-ARM platforms.
 */

/* DO NOT INCLUDE <sys/atomics.h> HERE ! */

int
__atomic_cmpxchg(int old, int _new, volatile int *ptr)
{
    /* We must return 0 on success */
    return __sync_val_compare_and_swap(ptr, old, _new) != old;
}

int
__atomic_swap(int _new, volatile int *ptr)
{
    int prev;
    do {
        prev = *ptr;
    } while (__sync_val_compare_and_swap(ptr, prev, _new) != prev);
    return prev;
}

int
__atomic_dec(volatile int *ptr)
{
  return __sync_fetch_and_sub (ptr, 1);
}

int
__atomic_inc(volatile int *ptr)
{
  return __sync_fetch_and_add (ptr, 1);
}
