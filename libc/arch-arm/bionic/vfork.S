/*
 * Copyright (C) 2015 The Android Open Source Project
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

ENTRY_WEAK_FOR_NATIVE_BRIDGE(vfork)
    // r3 = &__get_tls()[TLS_SLOT_THREAD_ID]
    mrc     p15, 0, r3, c13, c0, 3
    ldr     r3, [r3, #(TLS_SLOT_THREAD_ID * 4)]

    // Set cached_pid_ to 0, vforked_ to 1, and stash the previous value.
    mov     r0, #0x80000000
    ldr     r1, [r3, #12]
    str     r0, [r3, #12]

    mov     ip, r7
    ldr     r7, =__NR_vfork
    swi     #0
    mov     r7, ip

    teq     r0, #0
    bxeq    lr

    // rc != 0: reset cached_pid_ and vforked_.
    str     r1, [r3, #12]
    cmn     r0, #(MAX_ERRNO + 1)

    bxls    lr
    neg     r0, r0
    b       __set_errno_internal
END(vfork)
