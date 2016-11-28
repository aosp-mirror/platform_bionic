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

    .syntax unified

    .thumb
    .thumb_func

// Get the length of the source string first, then do a memcpy of the data
// instead of a strcpy.
ENTRY(__strcpy_chk)
    pld     [r0, #0]
    push    {r0, lr}
    .cfi_def_cfa_offset 8
    .cfi_rel_offset r0, 0
    .cfi_rel_offset lr, 4

    mov     lr, r2
    mov     r0, r1

    ands    r3, r1, #7
    beq     .L_mainloop

    // Align to a double word (64 bits).
    rsb     r3, r3, #8
    lsls    ip, r3, #31
    beq     .L_align_to_32

    ldrb    r2, [r0], #1
    cbz     r2, .L_update_count_and_finish

.L_align_to_32:
    bcc     .L_align_to_64
    ands    ip, r3, #2
    beq     .L_align_to_64

    ldrb    r2, [r0], #1
    cbz     r2, .L_update_count_and_finish
    ldrb    r2, [r0], #1
    cbz     r2, .L_update_count_and_finish

.L_align_to_64:
    tst     r3, #4
    beq     .L_mainloop
    ldr     r3, [r0], #4

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_zero_in_second_register

    .p2align 2
.L_mainloop:
    ldrd    r2, r3, [r0], #8

    pld     [r0, #64]

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     .L_zero_in_first_register

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_zero_in_second_register
    b       .L_mainloop

.L_update_count_and_finish:
    sub     r3, r0, r1
    sub     r3, r3, #1
    b       .L_check_size

.L_zero_in_first_register:
    sub     r3, r0, r1
    lsls    r2, ip, #17
    bne     .L_sub8_and_finish
    bcs     .L_sub7_and_finish
    lsls    ip, ip, #1
    bne     .L_sub6_and_finish

    sub     r3, r3, #5
    b       .L_check_size

.L_sub8_and_finish:
    sub     r3, r3, #8
    b       .L_check_size

.L_sub7_and_finish:
    sub     r3, r3, #7
    b       .L_check_size

.L_sub6_and_finish:
    sub     r3, r3, #6
    b       .L_check_size

.L_zero_in_second_register:
    sub     r3, r0, r1
    lsls    r2, ip, #17
    bne     .L_sub4_and_finish
    bcs     .L_sub3_and_finish
    lsls    ip, ip, #1
    bne     .L_sub2_and_finish

    sub     r3, r3, #1
    b       .L_check_size

.L_sub4_and_finish:
    sub     r3, r3, #4
    b       .L_check_size

.L_sub3_and_finish:
    sub     r3, r3, #3
    b       .L_check_size

.L_sub2_and_finish:
    sub     r3, r3, #2

.L_check_size:
    pld     [r1, #0]
    pld     [r1, #64]
    ldr     r0, [sp]

    // Add 1 for copy length to get the string terminator.
    add     r2, r3, #1

    cmp     r2, lr
    itt     hi
    movhi   r0, r2
    bhi     __strcpy_chk_fail

#include "memcpy_base.S"

END(__strcpy_chk)
