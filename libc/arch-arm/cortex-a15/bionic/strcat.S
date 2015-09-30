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

    .macro m_pop
    pop     {r0, r4, r5, pc}
    .endm // m_pop

    .macro m_scan_byte
    ldrb    r3, [r0]
    cbz     r3, .L_strcat_r0_scan_done
    add     r0, #1
    .endm // m_scan_byte

    .macro m_copy_byte reg, cmd, label
    ldrb    \reg, [r1], #1
    strb    \reg, [r0], #1
    \cmd    \reg, \label
    .endm // m_copy_byte

ENTRY(strcat)
    // Quick check to see if src is empty.
    ldrb    r2, [r1]
    pld     [r1, #0]
    cbnz    r2, .L_strcat_continue
    bx      lr

.L_strcat_continue:
    // To speed up really small dst strings, unroll checking the first 4 bytes.
    m_push
    m_scan_byte
    m_scan_byte
    m_scan_byte
    m_scan_byte

    ands    r3, r0, #7
    beq     .L_strcat_mainloop

    // Align to a double word (64 bits).
    rsb     r3, r3, #8
    lsls    ip, r3, #31
    beq     .L_strcat_align_to_32

    ldrb    r5, [r0]
    cbz     r5, .L_strcat_r0_scan_done
    add     r0, r0, #1

.L_strcat_align_to_32:
    bcc     .L_strcat_align_to_64

    ldrb    r2, [r0]
    cbz     r2, .L_strcat_r0_scan_done
    add     r0, r0, #1
    ldrb    r4, [r0]
    cbz     r4, .L_strcat_r0_scan_done
    add     r0, r0, #1

.L_strcat_align_to_64:
    tst     r3, #4
    beq     .L_strcat_mainloop
    ldr     r3, [r0], #4

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_strcat_zero_in_second_register
    b       .L_strcat_mainloop

.L_strcat_r0_scan_done:
    // For short copies, hard-code checking the first 8 bytes since this
    // new code doesn't win until after about 8 bytes.
    m_copy_byte reg=r2, cmd=cbz, label=.L_strcpy_finish
    m_copy_byte reg=r3, cmd=cbz, label=.L_strcpy_finish
    m_copy_byte reg=r4, cmd=cbz, label=.L_strcpy_finish
    m_copy_byte reg=r5, cmd=cbz, label=.L_strcpy_finish
    m_copy_byte reg=r2, cmd=cbz, label=.L_strcpy_finish
    m_copy_byte reg=r3, cmd=cbz, label=.L_strcpy_finish
    m_copy_byte reg=r4, cmd=cbz, label=.L_strcpy_finish
    m_copy_byte reg=r5, cmd=cbnz, label=.L_strcpy_continue

.L_strcpy_finish:
    m_pop

.L_strcpy_continue:
    ands    r3, r0, #7
    beq     .L_strcpy_check_src_align

    // Align to a double word (64 bits).
    rsb     r3, r3, #8
    lsls    ip, r3, #31
    beq     .L_strcpy_align_to_32

    ldrb    r2, [r1], #1
    strb    r2, [r0], #1
    cbz     r2, .L_strcpy_complete

.L_strcpy_align_to_32:
    bcc     .L_strcpy_align_to_64

    ldrb    r2, [r1], #1
    strb    r2, [r0], #1
    cbz     r2, .L_strcpy_complete
    ldrb    r2, [r1], #1
    strb    r2, [r0], #1
    cbz     r2, .L_strcpy_complete

.L_strcpy_align_to_64:
    tst     r3, #4
    beq     .L_strcpy_check_src_align
    // Read one byte at a time since we don't know the src alignment
    // and we don't want to read into a different page.
    ldrb    r2, [r1], #1
    strb    r2, [r0], #1
    cbz     r2, .L_strcpy_complete
    ldrb    r2, [r1], #1
    strb    r2, [r0], #1
    cbz     r2, .L_strcpy_complete
    ldrb    r2, [r1], #1
    strb    r2, [r0], #1
    cbz     r2, .L_strcpy_complete
    ldrb    r2, [r1], #1
    strb    r2, [r0], #1
    cbz     r2, .L_strcpy_complete

.L_strcpy_check_src_align:
    // At this point dst is aligned to a double word, check if src
    // is also aligned to a double word.
    ands    r3, r1, #7
    bne     .L_strcpy_unaligned_copy

    .p2align 2
.L_strcpy_mainloop:
    ldrd    r2, r3, [r1], #8

    pld     [r1, #64]

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_first_register

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_second_register

    strd    r2, r3, [r0], #8
    b       .L_strcpy_mainloop

.L_strcpy_complete:
    m_pop

.L_strcpy_zero_in_first_register:
    lsls    lr, ip, #17
    bne     .L_strcpy_copy1byte
    bcs     .L_strcpy_copy2bytes
    lsls    ip, ip, #1
    bne     .L_strcpy_copy3bytes

.L_strcpy_copy4bytes:
    // Copy 4 bytes to the destiniation.
    str     r2, [r0]
    m_pop

.L_strcpy_copy1byte:
    strb    r2, [r0]
    m_pop

.L_strcpy_copy2bytes:
    strh    r2, [r0]
    m_pop

.L_strcpy_copy3bytes:
    strh    r2, [r0], #2
    lsr     r2, #16
    strb    r2, [r0]
    m_pop

.L_strcpy_zero_in_second_register:
    lsls    lr, ip, #17
    bne     .L_strcpy_copy5bytes
    bcs     .L_strcpy_copy6bytes
    lsls    ip, ip, #1
    bne     .L_strcpy_copy7bytes

    // Copy 8 bytes to the destination.
    strd    r2, r3, [r0]
    m_pop

.L_strcpy_copy5bytes:
    str     r2, [r0], #4
    strb    r3, [r0]
    m_pop

.L_strcpy_copy6bytes:
    str     r2, [r0], #4
    strh    r3, [r0]
    m_pop

.L_strcpy_copy7bytes:
    str     r2, [r0], #4
    strh    r3, [r0], #2
    lsr     r3, #16
    strb    r3, [r0]
    m_pop

.L_strcpy_unaligned_copy:
    // Dst is aligned to a double word, while src is at an unknown alignment.
    // There are 7 different versions of the unaligned copy code
    // to prevent overreading the src. The mainloop of every single version
    // will store 64 bits per loop. The difference is how much of src can
    // be read without potentially crossing a page boundary.
    tbb     [pc, r3]
.L_strcpy_unaligned_branchtable:
    .byte 0
    .byte ((.L_strcpy_unalign7 - .L_strcpy_unaligned_branchtable)/2)
    .byte ((.L_strcpy_unalign6 - .L_strcpy_unaligned_branchtable)/2)
    .byte ((.L_strcpy_unalign5 - .L_strcpy_unaligned_branchtable)/2)
    .byte ((.L_strcpy_unalign4 - .L_strcpy_unaligned_branchtable)/2)
    .byte ((.L_strcpy_unalign3 - .L_strcpy_unaligned_branchtable)/2)
    .byte ((.L_strcpy_unalign2 - .L_strcpy_unaligned_branchtable)/2)
    .byte ((.L_strcpy_unalign1 - .L_strcpy_unaligned_branchtable)/2)

    .p2align 2
    // Can read 7 bytes before possibly crossing a page.
.L_strcpy_unalign7:
    ldr     r2, [r1], #4

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_first_register

    ldrb    r3, [r1]
    cbz     r3, .L_strcpy_unalign7_copy5bytes
    ldrb    r4, [r1, #1]
    cbz     r4, .L_strcpy_unalign7_copy6bytes
    ldrb    r5, [r1, #2]
    cbz     r5, .L_strcpy_unalign7_copy7bytes

    ldr     r3, [r1], #4
    pld     [r1, #64]

    lsrs    ip, r3, #24
    strd    r2, r3, [r0], #8
    beq     .L_strcpy_unalign_return
    b       .L_strcpy_unalign7

.L_strcpy_unalign7_copy5bytes:
    str     r2, [r0], #4
    strb    r3, [r0]
.L_strcpy_unalign_return:
    m_pop

.L_strcpy_unalign7_copy6bytes:
    str     r2, [r0], #4
    strb    r3, [r0], #1
    strb    r4, [r0], #1
    m_pop

.L_strcpy_unalign7_copy7bytes:
    str     r2, [r0], #4
    strb    r3, [r0], #1
    strb    r4, [r0], #1
    strb    r5, [r0], #1
    m_pop

    .p2align 2
    // Can read 6 bytes before possibly crossing a page.
.L_strcpy_unalign6:
    ldr     r2, [r1], #4

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_first_register

    ldrb    r4, [r1]
    cbz     r4, .L_strcpy_unalign_copy5bytes
    ldrb    r5, [r1, #1]
    cbz     r5, .L_strcpy_unalign_copy6bytes

    ldr     r3, [r1], #4
    pld     [r1, #64]

    tst     r3, #0xff0000
    beq     .L_strcpy_copy7bytes
    lsrs    ip, r3, #24
    strd    r2, r3, [r0], #8
    beq     .L_strcpy_unalign_return
    b       .L_strcpy_unalign6

    .p2align 2
    // Can read 5 bytes before possibly crossing a page.
.L_strcpy_unalign5:
    ldr     r2, [r1], #4

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_first_register

    ldrb    r4, [r1]
    cbz     r4, .L_strcpy_unalign_copy5bytes

    ldr     r3, [r1], #4

    pld     [r1, #64]

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_second_register

    strd    r2, r3, [r0], #8
    b       .L_strcpy_unalign5

.L_strcpy_unalign_copy5bytes:
    str     r2, [r0], #4
    strb    r4, [r0]
    m_pop

.L_strcpy_unalign_copy6bytes:
    str     r2, [r0], #4
    strb    r4, [r0], #1
    strb    r5, [r0]
    m_pop

    .p2align 2
    // Can read 4 bytes before possibly crossing a page.
.L_strcpy_unalign4:
    ldr     r2, [r1], #4

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_first_register

    ldr     r3, [r1], #4
    pld     [r1, #64]

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_second_register

    strd    r2, r3, [r0], #8
    b       .L_strcpy_unalign4

    .p2align 2
    // Can read 3 bytes before possibly crossing a page.
.L_strcpy_unalign3:
    ldrb    r2, [r1]
    cbz     r2, .L_strcpy_unalign3_copy1byte
    ldrb    r3, [r1, #1]
    cbz     r3, .L_strcpy_unalign3_copy2bytes
    ldrb    r4, [r1, #2]
    cbz     r4, .L_strcpy_unalign3_copy3bytes

    ldr     r2, [r1], #4
    ldr     r3, [r1], #4

    pld     [r1, #64]

    lsrs    lr, r2, #24
    beq     .L_strcpy_copy4bytes

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_second_register

    strd    r2, r3, [r0], #8
    b       .L_strcpy_unalign3

.L_strcpy_unalign3_copy1byte:
    strb    r2, [r0]
    m_pop

.L_strcpy_unalign3_copy2bytes:
    strb    r2, [r0], #1
    strb    r3, [r0]
    m_pop

.L_strcpy_unalign3_copy3bytes:
    strb    r2, [r0], #1
    strb    r3, [r0], #1
    strb    r4, [r0]
    m_pop

    .p2align 2
    // Can read 2 bytes before possibly crossing a page.
.L_strcpy_unalign2:
    ldrb    r2, [r1]
    cbz     r2, .L_strcpy_unalign_copy1byte
    ldrb    r4, [r1, #1]
    cbz     r4, .L_strcpy_unalign_copy2bytes

    ldr     r2, [r1], #4
    ldr     r3, [r1], #4
    pld     [r1, #64]

    tst     r2, #0xff0000
    beq     .L_strcpy_copy3bytes
    lsrs    ip, r2, #24
    beq     .L_strcpy_copy4bytes

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_second_register

    strd    r2, r3, [r0], #8
    b       .L_strcpy_unalign2

    .p2align 2
    // Can read 1 byte before possibly crossing a page.
.L_strcpy_unalign1:
    ldrb    r2, [r1]
    cbz     r2, .L_strcpy_unalign_copy1byte

    ldr     r2, [r1], #4
    ldr     r3, [r1], #4

    pld     [r1, #64]

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_first_register

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_strcpy_zero_in_second_register

    strd    r2, r3, [r0], #8
    b       .L_strcpy_unalign1

.L_strcpy_unalign_copy1byte:
    strb    r2, [r0]
    m_pop

.L_strcpy_unalign_copy2bytes:
    strb    r2, [r0], #1
    strb    r4, [r0]
    m_pop

    .p2align 2
.L_strcat_mainloop:
    ldrd    r2, r3, [r0], #8

    pld     [r0, #64]

    sub     ip, r2, #0x01010101
    bic     ip, ip, r2
    ands    ip, ip, #0x80808080
    bne     .L_strcat_zero_in_first_register

    sub     ip, r3, #0x01010101
    bic     ip, ip, r3
    ands    ip, ip, #0x80808080
    bne     .L_strcat_zero_in_second_register
    b       .L_strcat_mainloop

.L_strcat_zero_in_first_register:
    // Prefetch the src now, it's going to be used soon.
    pld     [r1, #0]
    lsls    lr, ip, #17
    bne     .L_strcat_sub8
    bcs     .L_strcat_sub7
    lsls    ip, ip, #1
    bne     .L_strcat_sub6

    sub     r0, r0, #5
    b       .L_strcat_r0_scan_done

.L_strcat_sub8:
    sub     r0, r0, #8
    b       .L_strcat_r0_scan_done

.L_strcat_sub7:
    sub     r0, r0, #7
    b       .L_strcat_r0_scan_done

.L_strcat_sub6:
    sub     r0, r0, #6
    b       .L_strcat_r0_scan_done

.L_strcat_zero_in_second_register:
    // Prefetch the src now, it's going to be used soon.
    pld     [r1, #0]
    lsls    lr, ip, #17
    bne     .L_strcat_sub4
    bcs     .L_strcat_sub3
    lsls    ip, ip, #1
    bne     .L_strcat_sub2

    sub     r0, r0, #1
    b       .L_strcat_r0_scan_done

.L_strcat_sub4:
    sub     r0, r0, #4
    b       .L_strcat_r0_scan_done

.L_strcat_sub3:
    sub     r0, r0, #3
    b       .L_strcat_r0_scan_done

.L_strcat_sub2:
    sub     r0, r0, #2
    b       .L_strcat_r0_scan_done
END(strcat)
