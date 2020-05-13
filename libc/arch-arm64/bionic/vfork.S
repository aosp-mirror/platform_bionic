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
#include <asm/signal.h>

// Must match the defines in linux/sched.h
#define CLONE_VM 0x00000100
#define CLONE_VFORK 0x00004000

ENTRY(vfork)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(vfork)
    // x9 = __get_tls()[TLS_SLOT_THREAD_ID]
    mrs     x9, tpidr_el0
    ldr     x9, [x9, #(TLS_SLOT_THREAD_ID * 8)]

    // Set cached_pid_ to 0, vforked_ to 1, and stash the previous value.
    mov     w0, #0x80000000
    ldr     w10, [x9, #20]
    str     w0, [x9, #20]

    mov     x0, #(CLONE_VM | CLONE_VFORK | SIGCHLD)
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

#if __has_feature(hwaddress_sanitizer)
    cbz x0, .L_exit

    // Clean up stack shadow in the parent process.
    // https://github.com/google/sanitizers/issues/925
    stp x0, x30, [sp, #-16]!
    .cfi_adjust_cfa_offset 16
    .cfi_rel_offset x0, 0
    .cfi_rel_offset x30, 8

    add x0, sp, #16
    bl __hwasan_handle_vfork

    ldp x0, x30, [sp], #16
    .cfi_adjust_cfa_offset -16
    .cfi_restore x0
    .cfi_restore x30

#endif

.L_exit:
    ret
END(vfork)
