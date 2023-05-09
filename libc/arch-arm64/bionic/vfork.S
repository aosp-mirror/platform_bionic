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

#include <platform/bionic/tls_defines.h>
#include <private/bionic_asm.h>
#include <private/bionic_asm_offsets.h>
#include <asm/signal.h>
#include <linux/sched.h>

ENTRY(vfork)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(vfork)
    // x9 = __get_tls()[TLS_SLOT_THREAD_ID]
    mrs     x9, tpidr_el0
    ldr     x9, [x9, #(TLS_SLOT_THREAD_ID * 8)]

    // Set cached_pid_ to 0, vforked_ to 1, and stash the previous value.
    mov     w0, #0x80000000
    ldr     w10, [x9, #20]
    str     w0, [x9, #20]

    mov     x0, #SIGCHLD

    // If either HWASan or stack MTE is enabled, set up the clone() flags to
    // make vfork() act like fork(). We don't call the atfork handlers, so we
    // may deadlock if the child allocates, but we have seen badly written
    // atfork handlers themselves cause deadlocks [1]. ndk_translation already
    // implements vfork() as fork() without calling handlers, so we have some
    // evidence that it isn't necessary to call them.
    //
    // POSIX.1 defines vfork() to have the same effect as fork() except that
    // most behavior, including heap allocation, becomes undefined in the child,
    // so we aren't violating POSIX by doing this.
    //
    // [1] https://cs.android.com/android/platform/superproject/+/master:system/extras/simpleperf/app_api/cpp/simpleperf.cpp;drc=788fa4183441f4977ddbd5a055e42a7fe7691d21;l=308
#if !__has_feature(hwaddress_sanitizer)
    // if (!__libc_globals->memtag_stack) x0 |= CLONE_VM | CLONE_VFORK;
    adrp    x1, __libc_globals + OFFSETOF_libc_globals_memtag_stack
    ldrb    w1, [x1, :lo12:__libc_globals + OFFSETOF_libc_globals_memtag_stack]
    cbnz    w1, 1f
    orr     x0, x0, #CLONE_VM
    orr     x0, x0, #CLONE_VFORK
1:
#endif
    mov     x1, xzr
    mov     x2, xzr
    mov     x3, xzr
    mov     x4, xzr

    mov     x8, __NR_clone
    svc     #0

    cbz     x0, .L_exit

    // rc != 0: reset cached_pid_ and vforked_.
    str     w10, [x9, #20]
    cmn     x0, #(MAX_ERRNO + 1)
    cneg    x0, x0, hi
    b.hi    __set_errno_internal

.L_exit:
    ret
END(vfork)

NOTE_GNU_PROPERTY()
