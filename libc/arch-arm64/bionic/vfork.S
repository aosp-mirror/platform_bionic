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

#include <private/bionic_asm.h>
#include <private/bionic_asm_tls.h>
#include <asm/signal.h>
#include <linux/sched.h>

ENTRY(vfork)
__BIONIC_WEAK_ASM_FOR_NATIVE_BRIDGE(vfork)
    // __get_tls()[TLS_SLOT_THREAD_ID]->cached_pid_ = 0
    mrs     x0, tpidr_el0
    ldr     x0, [x0, #(TLS_SLOT_THREAD_ID * 8)]
    str     wzr, [x0, #20]

    mov     x0, #(CLONE_VM | CLONE_VFORK | SIGCHLD)
    mov     x1, xzr
    mov     x2, xzr
    mov     x3, xzr
    mov     x4, xzr

    mov     x8, __NR_clone
    svc     #0

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
