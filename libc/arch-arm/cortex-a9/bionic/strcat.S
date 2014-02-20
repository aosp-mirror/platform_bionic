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

    .thumb
    .thumb_func

    .macro m_push
    push    {r0, r4, r5, lr}
    .endm // m_push

    .macro m_ret inst
    \inst   {r0, r4, r5, pc}
    .endm // m_ret

    .macro m_scan_byte
    ldrb    r3, [r0]
    cbz     r3, strcat_r0_scan_done
    add     r0, #1
    .endm // m_scan_byte

    .macro m_copy_byte reg, cmd, label
    ldrb    \reg, [r1], #1
    strb    \reg, [r0], #1
    \cmd    \reg, \label
    .endm // m_copy_byte

ENTRY(strcat)
    // Quick check to see if src is empty.
    ldrb        r2, [r1]
    pld         [r1, #0]
    cbnz        r2, strcat_continue
    bx          lr

strcat_continue:
    // To speed up really small dst strings, unroll checking the first 4 bytes.
    m_push
    m_scan_byte
    m_scan_byte
    m_scan_byte
    m_scan_byte

    ands    r3, r0, #7
    bne     strcat_align_src

    .p2align 2
strcat_mainloop:
    ldmia   r0!, {r2, r3}

    pld     [r0, #64]

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     strcat_zero_in_first_register

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     strcat_zero_in_second_register
    b       strcat_mainloop

strcat_zero_in_first_register:
    sub     r0, r0, #4

strcat_zero_in_second_register:
    // Check for zero in byte 0.
    tst     ip, #0x80
    it      ne
    subne   r0, r0, #4
    bne     strcat_r0_scan_done
    // Check for zero in byte 1.
    tst     ip, #0x8000
    it      ne
    subne   r0, r0, #3
    bne     strcat_r0_scan_done
    // Check for zero in byte 2.
    tst     ip, #0x800000
    it      ne
    subne   r0, r0, #2
    it      eq
    // Zero is in byte 3.
    subeq   r0, r0, #1

strcat_r0_scan_done:
    // Unroll the first 8 bytes that will be copied.
    m_copy_byte reg=r2, cmd=cbz, label=strcpy_finish
    m_copy_byte reg=r3, cmd=cbz, label=strcpy_finish
    m_copy_byte reg=r4, cmd=cbz, label=strcpy_finish
    m_copy_byte reg=r5, cmd=cbz, label=strcpy_finish
    m_copy_byte reg=r2, cmd=cbz, label=strcpy_finish
    m_copy_byte reg=r3, cmd=cbz, label=strcpy_finish
    m_copy_byte reg=r4, cmd=cbz, label=strcpy_finish
    m_copy_byte reg=r5, cmd=cbnz, label=strcpy_continue

strcpy_finish:
    m_ret   inst=pop

strcpy_continue:
    pld     [r1, #0]
    ands    r3, r0, #7
    bne     strcpy_align_dst

strcpy_check_src_align:
    // At this point dst is aligned to a double word, check if src
    // is also aligned to a double word.
    ands    r3, r1, #7
    bne     strcpy_unaligned_copy

    .p2align 2
strcpy_mainloop:
    ldmia   r1!, {r2, r3}

    pld     [r1, #64]

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_first_register

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_second_register

    stmia   r0!, {r2, r3}
    b       strcpy_mainloop

strcpy_zero_in_first_register:
    lsls    lr, ip, #17
    itt     ne
    strbne  r2, [r0]
    m_ret   inst=popne
    itt     cs
    strhcs  r2, [r0]
    m_ret   inst=popcs
    lsls    ip, ip, #1
    itt     eq
    streq   r2, [r0]
    m_ret   inst=popeq
    strh    r2, [r0], #2
    lsr     r3, r2, #16
    strb    r3, [r0]
    m_ret   inst=pop

strcpy_zero_in_second_register:
    lsls    lr, ip, #17
    ittt    ne
    stmiane r0!, {r2}
    strbne  r3, [r0]
    m_ret   inst=popne
    ittt    cs
    strcs   r2, [r0], #4
    strhcs  r3, [r0]
    m_ret   inst=popcs
    lsls    ip, ip, #1
    itt     eq
    stmiaeq r0, {r2, r3}
    m_ret   inst=popeq
    stmia   r0!, {r2}
    strh    r3, [r0], #2
    lsr     r4, r3, #16
    strb    r4, [r0]
    m_ret   inst=pop

strcpy_align_dst:
    // Align to a double word (64 bits).
    rsb     r3, r3, #8
    lsls    ip, r3, #31
    beq     strcpy_align_to_32

    ldrb    r2, [r1], #1
    strb    r2, [r0], #1
    cbz     r2, strcpy_complete

strcpy_align_to_32:
    bcc     strcpy_align_to_64

    ldrb    r4, [r1], #1
    strb    r4, [r0], #1
    cmp     r4, #0
    it      eq
    m_ret   inst=popeq
    ldrb    r5, [r1], #1
    strb    r5, [r0], #1
    cmp     r5, #0
    it      eq
    m_ret   inst=popeq

strcpy_align_to_64:
    tst     r3, #4
    beq     strcpy_check_src_align
    ldr     r2, [r1], #4

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_first_register
    stmia   r0!, {r2}
    b       strcpy_check_src_align

strcpy_complete:
    m_ret   inst=pop

strcpy_unaligned_copy:
    // Dst is aligned to a double word, while src is at an unknown alignment.
    // There are 7 different versions of the unaligned copy code
    // to prevent overreading the src. The mainloop of every single version
    // will store 64 bits per loop. The difference is how much of src can
    // be read without potentially crossing a page boundary.
    tbb     [pc, r3]
strcpy_unaligned_branchtable:
    .byte 0
    .byte ((strcpy_unalign7 - strcpy_unaligned_branchtable)/2)
    .byte ((strcpy_unalign6 - strcpy_unaligned_branchtable)/2)
    .byte ((strcpy_unalign5 - strcpy_unaligned_branchtable)/2)
    .byte ((strcpy_unalign4 - strcpy_unaligned_branchtable)/2)
    .byte ((strcpy_unalign3 - strcpy_unaligned_branchtable)/2)
    .byte ((strcpy_unalign2 - strcpy_unaligned_branchtable)/2)
    .byte ((strcpy_unalign1 - strcpy_unaligned_branchtable)/2)

    .p2align 2
    // Can read 7 bytes before possibly crossing a page.
strcpy_unalign7:
    ldr     r2, [r1], #4

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_first_register

    ldrb    r3, [r1]
    cbz     r3, strcpy_unalign7_copy5bytes
    ldrb    r4, [r1, #1]
    cbz     r4, strcpy_unalign7_copy6bytes
    ldrb    r5, [r1, #2]
    cbz     r5, strcpy_unalign7_copy7bytes

    ldr     r3, [r1], #4
    pld     [r1, #64]

    lsrs    ip, r3, #24
    stmia   r0!, {r2, r3}
    beq     strcpy_unalign_return
    b       strcpy_unalign7

strcpy_unalign7_copy5bytes:
    stmia   r0!, {r2}
    strb    r3, [r0]
strcpy_unalign_return:
    m_ret   inst=pop

strcpy_unalign7_copy6bytes:
    stmia   r0!, {r2}
    strb    r3, [r0], #1
    strb    r4, [r0], #1
    m_ret   inst=pop

strcpy_unalign7_copy7bytes:
    stmia   r0!, {r2}
    strb    r3, [r0], #1
    strb    r4, [r0], #1
    strb    r5, [r0], #1
    m_ret   inst=pop

    .p2align 2
    // Can read 6 bytes before possibly crossing a page.
strcpy_unalign6:
    ldr     r2, [r1], #4

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_first_register

    ldrb    r4, [r1]
    cbz     r4, strcpy_unalign_copy5bytes
    ldrb    r5, [r1, #1]
    cbz     r5, strcpy_unalign_copy6bytes

    ldr     r3, [r1], #4
    pld     [r1, #64]

    tst     r3, #0xff0000
    beq     strcpy_unalign6_copy7bytes
    lsrs    ip, r3, #24
    stmia   r0!, {r2, r3}
    beq     strcpy_unalign_return
    b       strcpy_unalign6

strcpy_unalign6_copy7bytes:
    stmia   r0!, {r2}
    strh    r3, [r0], #2
    lsr     r3, #16
    strb    r3, [r0]
    m_ret   inst=pop

    .p2align 2
    // Can read 5 bytes before possibly crossing a page.
strcpy_unalign5:
    ldr     r2, [r1], #4

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_first_register

    ldrb    r4, [r1]
    cbz     r4, strcpy_unalign_copy5bytes

    ldr     r3, [r1], #4

    pld     [r1, #64]

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_second_register

    stmia   r0!, {r2, r3}
    b       strcpy_unalign5

strcpy_unalign_copy5bytes:
    stmia   r0!, {r2}
    strb    r4, [r0]
    m_ret   inst=pop

strcpy_unalign_copy6bytes:
    stmia   r0!, {r2}
    strb    r4, [r0], #1
    strb    r5, [r0]
    m_ret   inst=pop

    .p2align 2
    // Can read 4 bytes before possibly crossing a page.
strcpy_unalign4:
    ldmia   r1!, {r2}

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_first_register

    ldmia   r1!, {r3}
    pld     [r1, #64]

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_second_register

    stmia   r0!, {r2, r3}
    b       strcpy_unalign4

    .p2align 2
    // Can read 3 bytes before possibly crossing a page.
strcpy_unalign3:
    ldrb    r2, [r1]
    cbz     r2, strcpy_unalign3_copy1byte
    ldrb    r3, [r1, #1]
    cbz     r3, strcpy_unalign3_copy2bytes
    ldrb    r4, [r1, #2]
    cbz     r4, strcpy_unalign3_copy3bytes

    ldr     r2, [r1], #4
    ldr     r3, [r1], #4

    pld     [r1, #64]

    lsrs    lr, r2, #24
    beq     strcpy_unalign_copy4bytes

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_second_register

    stmia   r0!, {r2, r3}
    b       strcpy_unalign3

strcpy_unalign3_copy1byte:
    strb    r2, [r0]
    m_ret   inst=pop

strcpy_unalign3_copy2bytes:
    strb    r2, [r0], #1
    strb    r3, [r0]
    m_ret   inst=pop

strcpy_unalign3_copy3bytes:
    strb    r2, [r0], #1
    strb    r3, [r0], #1
    strb    r4, [r0]
    m_ret   inst=pop

    .p2align 2
    // Can read 2 bytes before possibly crossing a page.
strcpy_unalign2:
    ldrb    r2, [r1]
    cbz     r2, strcpy_unalign_copy1byte
    ldrb    r3, [r1, #1]
    cbz     r3, strcpy_unalign_copy2bytes

    ldr     r2, [r1], #4
    ldr     r3, [r1], #4
    pld     [r1, #64]

    tst     r2, #0xff0000
    beq     strcpy_unalign_copy3bytes
    lsrs    ip, r2, #24
    beq     strcpy_unalign_copy4bytes

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_second_register

    stmia   r0!, {r2, r3}
    b       strcpy_unalign2

    .p2align 2
    // Can read 1 byte before possibly crossing a page.
strcpy_unalign1:
    ldrb    r2, [r1]
    cbz     r2, strcpy_unalign_copy1byte

    ldr     r2, [r1], #4
    ldr     r3, [r1], #4

    pld     [r1, #64]

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_first_register

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     strcpy_zero_in_second_register

    stmia   r0!, {r2, r3}
    b       strcpy_unalign1

strcpy_unalign_copy1byte:
    strb    r2, [r0]
    m_ret   inst=pop

strcpy_unalign_copy2bytes:
    strb    r2, [r0], #1
    strb    r3, [r0]
    m_ret   inst=pop

strcpy_unalign_copy3bytes:
    strh    r2, [r0], #2
    lsr     r2, #16
    strb    r2, [r0]
    m_ret   inst=pop

strcpy_unalign_copy4bytes:
    stmia   r0, {r2}
    m_ret   inst=pop

strcat_align_src:
    // Align to a double word (64 bits).
    rsb     r3, r3, #8
    lsls    ip, r3, #31
    beq     strcat_align_to_32
    ldrb    r2, [r0], #1
    cbz     r2, strcat_r0_update

strcat_align_to_32:
    bcc     strcat_align_to_64
    ldrb    r2, [r0], #1
    cbz     r2, strcat_r0_update
    ldrb    r2, [r0], #1
    cbz     r2, strcat_r0_update

strcat_align_to_64:
    tst     r3, #4
    beq     strcat_mainloop
    ldr     r3, [r0], #4

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     strcat_zero_in_second_register
    b       strcat_mainloop

strcat_r0_update:
    sub     r0, r0, #1
    b strcat_r0_scan_done
END(strcat)
