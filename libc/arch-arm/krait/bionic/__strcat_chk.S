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
#include <private/libc_events.h>

    .syntax unified

    .thumb
    .thumb_func

// Get the length of src string, then get the source of the dst string.
// Check that the two lengths together don't exceed the threshold, then
// do a memcpy of the data.
ENTRY(__strcat_chk)
    pld     [r0, #0]
    push    {r0, lr}
    .save   {r0, lr}
    .cfi_def_cfa_offset 8
    .cfi_rel_offset r0, 0
    .cfi_rel_offset lr, 4
    push    {r4, r5}
    .save   {r4, r5}
    .cfi_adjust_cfa_offset 8
    .cfi_rel_offset r4, 0
    .cfi_rel_offset r5, 4

    mov     lr, r2

    // Save the dst register to r5
    mov     r5, r0

    // Zero out r4
    eor     r4, r4, r4

    // r1 contains the address of the string to count.
.L_strlen_start:
    mov     r0, r1
    ands    r3, r1, #7
    beq     .L_mainloop

    // Align to a double word (64 bits).
    rsb     r3, r3, #8
    lsls    ip, r3, #31
    beq     .L_align_to_32

    ldrb    r2, [r1], #1
    cbz     r2, .L_update_count_and_finish

.L_align_to_32:
    bcc     .L_align_to_64
    ands    ip, r3, #2
    beq     .L_align_to_64

    ldrb    r2, [r1], #1
    cbz     r2, .L_update_count_and_finish
    ldrb    r2, [r1], #1
    cbz     r2, .L_update_count_and_finish

.L_align_to_64:
    tst     r3, #4
    beq     .L_mainloop
    ldr     r3, [r1], #4

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_zero_in_second_register

    .p2align 2
.L_mainloop:
    ldrd    r2, r3, [r1], #8

    pld     [r1, #64]

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
    sub     r3, r1, r0
    sub     r3, r3, #1
    b       .L_finish

.L_zero_in_first_register:
    sub     r3, r1, r0
    lsls    r2, ip, #17
    bne     .L_sub8_and_finish
    bcs     .L_sub7_and_finish
    lsls    ip, ip, #1
    bne     .L_sub6_and_finish

    sub     r3, r3, #5
    b       .L_finish

.L_sub8_and_finish:
    sub     r3, r3, #8
    b       .L_finish

.L_sub7_and_finish:
    sub     r3, r3, #7
    b       .L_finish

.L_sub6_and_finish:
    sub     r3, r3, #6
    b       .L_finish

.L_zero_in_second_register:
    sub     r3, r1, r0
    lsls    r2, ip, #17
    bne     .L_sub4_and_finish
    bcs     .L_sub3_and_finish
    lsls    ip, ip, #1
    bne     .L_sub2_and_finish

    sub     r3, r3, #1
    b       .L_finish

.L_sub4_and_finish:
    sub     r3, r3, #4
    b       .L_finish

.L_sub3_and_finish:
    sub     r3, r3, #3
    b       .L_finish

.L_sub2_and_finish:
    sub     r3, r3, #2

.L_finish:
    cmp     r4, #0
    bne     .L_strlen_done

    // Time to get the dst string length.
    mov     r1, r5

    // Save the original source address to r5.
    mov     r5, r0

    // Save the current length (adding 1 for the terminator).
    add     r4, r3, #1
    b       .L_strlen_start

    // r0 holds the pointer to the dst string.
    // r3 holds the dst string length.
    // r4 holds the src string length + 1.
.L_strlen_done:
    add     r2, r3, r4
    cmp     r2, lr
    bhi     __strcat_chk_failed

    // Set up the registers for the memcpy code.
    mov     r1, r5
    pld     [r1, #64]
    mov     r2, r4
    add     r0, r0, r3
    pop     {r4, r5}
END(__strcat_chk)

#define MEMCPY_BASE         __strcat_chk_memcpy_base
#define MEMCPY_BASE_ALIGNED __strcat_chk_memcpy_base_aligned
#include "memcpy_base.S"

ENTRY_PRIVATE(__strcat_chk_failed)
    .save   {r0, lr}
    .save   {r4, r5}
    .cfi_def_cfa_offset 8
    .cfi_rel_offset r0, 0
    .cfi_rel_offset lr, 4
    .cfi_adjust_cfa_offset 8
    .cfi_rel_offset r4, 0
    .cfi_rel_offset r5, 4

    ldr     r0, error_message
    ldr     r1, error_code
1:
    add     r0, pc
    bl      __fortify_chk_fail
error_code:
    .word   BIONIC_EVENT_STRCAT_BUFFER_OVERFLOW
error_message:
    .word   error_string-(1b+4)
END(__strcat_chk_failed)

    .data
error_string:
    .string "strcat: prevented write past end of buffer"
