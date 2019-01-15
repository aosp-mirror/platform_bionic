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
/*
 * Copyright (c) 2013 ARM Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the company may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARM LTD ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ARM LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <private/bionic_asm.h>

    .syntax unified

    // To avoid warning about deprecated instructions, add an explicit
    // arch. The code generated is exactly the same.
    .arch armv7-a

    .thumb
    .thumb_func

ENTRY(strlen_a9)
    pld     [r0, #0]
    mov     r1, r0

    ands    r3, r0, #7
    bne     align_src

    .p2align 2
mainloop:
    ldmia   r1!, {r2, r3}

    pld     [r1, #64]

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     zero_in_first_register

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     zero_in_second_register
    b       mainloop

zero_in_first_register:
    sub     r0, r1, r0
    // Check for zero in byte 0.
    lsls    r2, ip, #17
    beq     check_byte1_reg1

    sub     r0, r0, #8
    bx      lr

check_byte1_reg1:
    bcc     check_byte2_reg1

    sub     r0, r0, #7
    bx      lr

check_byte2_reg1:
    // Check for zero in byte 2.
    tst     ip, #0x800000
    itt     ne
    subne   r0, r0, #6
    bxne    lr
    sub     r0, r0, #5
    bx      lr

zero_in_second_register:
    sub     r0, r1, r0
    // Check for zero in byte 0.
    lsls    r2, ip, #17
    beq     check_byte1_reg2

    sub     r0, r0, #4
    bx      lr

check_byte1_reg2:
    bcc     check_byte2_reg2

    sub     r0, r0, #3
    bx      lr

check_byte2_reg2:
    // Check for zero in byte 2.
    tst     ip, #0x800000
    itt     ne
    subne   r0, r0, #2
    bxne    lr
    sub     r0, r0, #1
    bx      lr

align_src:
    // Align to a double word (64 bits).
    rsb     r3, r3, #8
    lsls    ip, r3, #31
    beq     align_to_32

    ldrb    r2, [r1], #1
    cbz     r2, done

align_to_32:
    bcc     align_to_64

    ldrb    r2, [r1], #1
    cbz     r2, done
    ldrb    r2, [r1], #1
    cbz     r2, done

align_to_64:
    tst     r3, #4
    beq     mainloop
    ldr     r2, [r1], #4

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     zero_in_second_register
    b       mainloop

done:
    sub     r0, r1, r0
    sub     r0, r0, #1
    bx      lr
END(strlen_a9)
