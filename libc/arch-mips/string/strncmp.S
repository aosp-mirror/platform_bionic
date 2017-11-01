/*
 * Copyright (c) 2017 Imagination Technologies.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer
 *        in the documentation and/or other materials provided with
 *        the distribution.
 *      * Neither the name of Imagination Technologies nor the names of its
 *        contributors may be used to endorse or promote products derived
 *        from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef __ANDROID__
# include <private/bionic_asm.h>
#elif _LIBC
# include <sysdep.h>
# include <regdef.h>
# include <sys/asm.h>
#elif _COMPILING_NEWLIB
# include "machine/asm.h"
# include "machine/regdef.h"
#else
# include <regdef.h>
# include <sys/asm.h>
#endif

#if __mips64
# define NSIZE 8
# define LW ld
# define LWR ldr
# define LWL ldl
# define EXT dext
# define SRL dsrl
# define SUBU dsubu
#else
# define NSIZE 4
# define LW lw
# define LWR lwr
# define LWL lwl
# define EXT ext
# define SRL srl
# define SUBU subu
#endif

/* Technically strcmp should not read past the end of the strings being
   compared.  We will read a full word that may contain excess bits beyond
   the NULL string terminator but unless ENABLE_READAHEAD is set, we will not
   read the next word after the end of string.  Setting ENABLE_READAHEAD will
   improve performance but is technically illegal based on the definition of
   strcmp.  */
#ifdef ENABLE_READAHEAD
# define DELAY_READ
#else
# define DELAY_READ nop
#endif

/* Testing on a little endian machine showed using CLZ was a
   performance loss, so we are not turning it on by default.  */
#if defined(ENABLE_CLZ) && (__mips_isa_rev > 1) && (!__mips64)
# define USE_CLZ
#endif

/* Some asm.h files do not have the L macro definition.  */
#ifndef L
# if _MIPS_SIM == _ABIO32
#  define L(label) $L ## label
# else
#  define L(label) .L ## label
# endif
#endif

/* Some asm.h files do not have the PTR_ADDIU macro definition.  */
#ifndef PTR_ADDIU
# if _MIPS_SIM == _ABIO32
#  define PTR_ADDIU       addiu
# else
#  define PTR_ADDIU       daddiu
# endif
#endif

/* It might seem better to do the 'beq' instruction between the two 'lbu'
   instructions so that the nop is not needed but testing showed that this
   code is actually faster (based on glibc strcmp test).  */
#define BYTECMP01(OFFSET) \
    lbu $v0, OFFSET($a0); \
    lbu $v1, OFFSET($a1); \
    beq $v0, $zero, L(bexit01); \
    nop; \
    bne $v0, $v1, L(bexit01)

#define BYTECMP89(OFFSET) \
    lbu $t8, OFFSET($a0); \
    lbu $t9, OFFSET($a1); \
    beq $t8, $zero, L(bexit89); \
    nop;    \
    bne $t8, $t9, L(bexit89)

/* Allow the routine to be named something else if desired.  */
#ifndef STRNCMP_NAME
# define STRNCMP_NAME strncmp
#endif

#ifdef __ANDROID__
LEAF(STRNCMP_NAME, 0)
#else
LEAF(STRNCMP_NAME)
#endif
    .set    nomips16
    .set    noreorder

    srl $t0, $a2, (2 + NSIZE / 4)
    beqz  $t0, L(byteloop) #process by bytes if less than (2 * NSIZE)
    andi $t1, $a1, (NSIZE - 1)
    beqz  $t1, L(exitalign)
    or   $t0, $zero, NSIZE
    SUBU $t1, $t0, $t1 #process (NSIZE - 1) bytes at max
    SUBU $a2, $a2, $t1 #dec count by t1

L(alignloop): #do by bytes until a1 aligned
    BYTECMP01(0)
    SUBU $t1, $t1, 0x1
    PTR_ADDIU $a0, $a0, 0x1
    bne  $t1, $zero, L(alignloop)
    PTR_ADDIU $a1, $a1, 0x1

L(exitalign):

/* string a1 is NSIZE byte aligned at this point. */
#ifndef __mips1
    lui $t8, 0x0101
    ori $t8, 0x0101
    lui $t9, 0x7f7f
    ori $t9, 0x7f7f
#if __mips64
    dsll $t0, $t8, 32
    or  $t8, $t0
    dsll $t1, $t9, 32
    or  $t9, $t1
#endif
#endif

/* hardware or software alignment not supported for mips1
   rev6 archs have h/w unaligned support
   remainings archs need to implemented with unaligned instructions */

#if __mips1
    andi $t0, $a0, (NSIZE - 1)
    bne  $t0, $zero, L(byteloop)
#elif __mips_isa_rev < 6
    andi $t0, $a0, (NSIZE - 1)
    bne  $t0, $zero, L(uwordloop)
#endif

#define STRCMPW(OFFSET) \
    LW   $v0, (OFFSET)($a0); \
    LW   $v1, (OFFSET)($a1); \
    SUBU $t0, $v0, $t8; \
    bne  $v0, $v1, L(worddiff); \
    nor  $t1, $v0, $t9; \
    and  $t0, $t0, $t1; \
    bne  $t0, $zero, L(returnzero);\

L(wordloop):
    SUBU $t1, $a2, (8 * NSIZE)
    bltz $t1, L(onewords)
    STRCMPW(0 * NSIZE)
    DELAY_READ
    STRCMPW(1 * NSIZE)
    DELAY_READ
    STRCMPW(2 * NSIZE)
    DELAY_READ
    STRCMPW(3 * NSIZE)
    DELAY_READ
    STRCMPW(4 * NSIZE)
    DELAY_READ
    STRCMPW(5 * NSIZE)
    DELAY_READ
    STRCMPW(6 * NSIZE)
    DELAY_READ
    STRCMPW(7 * NSIZE)
    SUBU $a2, $a2, (8 * NSIZE)
    PTR_ADDIU $a0, $a0, (8 * NSIZE)
    b   L(wordloop)
    PTR_ADDIU $a1, $a1, (8 * NSIZE)

L(onewords):
    SUBU $t1, $a2, NSIZE
    bltz $t1, L(byteloop)
    STRCMPW(0)
    SUBU $a2, $a2, NSIZE
    PTR_ADDIU $a0, $a0, NSIZE
    b   L(onewords)
    PTR_ADDIU $a1, $a1, NSIZE

#if __mips_isa_rev < 6 && !__mips1
#define USTRCMPW(OFFSET) \
    LWR $v0, (OFFSET)($a0); \
    LWL $v0, (OFFSET + NSIZE - 1)($a0); \
    LW  $v1, (OFFSET)($a1); \
    SUBU    $t0, $v0, $t8; \
    bne $v0, $v1, L(worddiff); \
    nor $t1, $v0, $t9; \
    and $t0, $t0, $t1; \
    bne $t0, $zero, L(returnzero);\

L(uwordloop):
    SUBU $t1, $a2, (8 * NSIZE)
    bltz $t1, L(uonewords)
    USTRCMPW(0 * NSIZE)
    DELAY_READ
    USTRCMPW(1 * NSIZE)
    DELAY_READ
    USTRCMPW(2 * NSIZE)
    DELAY_READ
    USTRCMPW(3 * NSIZE)
    DELAY_READ
    USTRCMPW(4 * NSIZE)
    DELAY_READ
    USTRCMPW(5 * NSIZE)
    DELAY_READ
    USTRCMPW(6 * NSIZE)
    DELAY_READ
    USTRCMPW(7 * NSIZE)
    SUBU $a2, $a2, (8 * NSIZE)
    PTR_ADDIU $a0, $a0, (8 * NSIZE)
    b   L(uwordloop)
    PTR_ADDIU $a1, $a1, (8 * NSIZE)

L(uonewords):
    SUBU $t1, $a2, NSIZE
    bltz $t1, L(byteloop)
    USTRCMPW(0)
    SUBU $a2, $a2, NSIZE
    PTR_ADDIU $a0, $a0, NSIZE
    b   L(uonewords)
    PTR_ADDIU $a1, $a1, NSIZE

#endif

L(returnzero):
    j   $ra
    move    $v0, $zero

#if __mips_isa_rev > 1
#define EXT_COMPARE01(POS) \
    EXT $t0, $v0, POS, 8; \
    beq $t0, $zero, L(wexit01); \
    EXT $t1, $v1, POS, 8; \
    bne $t0, $t1, L(wexit01)
#define EXT_COMPARE89(POS) \
    EXT $t8, $v0, POS, 8; \
    beq $t8, $zero, L(wexit89); \
    EXT $t9, $v1, POS, 8; \
    bne $t8, $t9, L(wexit89)
#else
#define EXT_COMPARE01(POS) \
    SRL  $t0, $v0, POS; \
    SRL  $t1, $v1, POS; \
    andi $t0, $t0, 0xff; \
    beq  $t0, $zero, L(wexit01); \
    andi $t1, $t1, 0xff; \
    bne  $t0, $t1, L(wexit01)
#define EXT_COMPARE89(POS) \
    SRL  $t8, $v0, POS; \
    SRL  $t9, $v1, POS; \
    andi $t8, $t8, 0xff; \
    beq  $t8, $zero, L(wexit89); \
    andi $t9, $t9, 0xff; \
    bne  $t8, $t9, L(wexit89)
#endif

L(worddiff):
#ifdef USE_CLZ
    SUBU    $t0, $v0, $t8
    nor $t1, $v0, $t9
    and $t1, $t0, $t1
    xor $t0, $v0, $v1
    or  $t0, $t0, $t1
# if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    wsbh    $t0, $t0
    rotr    $t0, $t0, 16
# endif
    clz $t1, $t0
    and $t1, 0xf8
# if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    neg $t1
    addu    $t1, 24
# endif
    rotrv   $v0, $v0, $t1
    rotrv   $v1, $v1, $t1
    and $v0, $v0, 0xff
    and $v1, $v1, 0xff
    j   $ra
    SUBU    $v0, $v0, $v1
#else /* USE_CLZ */
# if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    andi    $t0, $v0, 0xff
    beq $t0, $zero, L(wexit01)
    andi    $t1, $v1, 0xff
    bne $t0, $t1, L(wexit01)
    EXT_COMPARE89(8)
    EXT_COMPARE01(16)
#ifndef __mips64
    SRL $t8, $v0, 24
    SRL $t9, $v1, 24
#else
    EXT_COMPARE89(24)
    EXT_COMPARE01(32)
    EXT_COMPARE89(40)
    EXT_COMPARE01(48)
    SRL $t8, $v0, 56
    SRL $t9, $v1, 56
#endif

# else /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */
#ifdef __mips64
    SRL $t0, $v0, 56
    beq $t0, $zero, L(wexit01)
    SRL $t1, $v1, 56
    bne $t0, $t1, L(wexit01)
    EXT_COMPARE89(48)
    EXT_COMPARE01(40)
    EXT_COMPARE89(32)
    EXT_COMPARE01(24)
#else
    SRL $t0, $v0, 24
    beq $t0, $zero, L(wexit01)
    SRL $t1, $v1, 24
    bne $t0, $t1, L(wexit01)
#endif
    EXT_COMPARE89(16)
    EXT_COMPARE01(8)

    andi    $t8, $v0, 0xff
    andi    $t9, $v1, 0xff
# endif /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */

L(wexit89):
    j   $ra
    SUBU    $v0, $t8, $t9
L(wexit01):
    j   $ra
    SUBU    $v0, $t0, $t1
#endif /* USE_CLZ */

L(byteloop):
    beq $a2, $zero, L(returnzero)
    SUBU $a2, $a2, 1
    BYTECMP01(0)
    nop
    beq $a2, $zero, L(returnzero)
    SUBU $a2, $a2, 1
    BYTECMP89(1)
    nop
    beq $a2, $zero, L(returnzero)
    SUBU $a2, $a2, 1
    BYTECMP01(2)
    nop
    beq $a2, $zero, L(returnzero)
    SUBU $a2, $a2, 1
    BYTECMP89(3)
    PTR_ADDIU $a0, $a0, 4
    b   L(byteloop)
    PTR_ADDIU $a1, $a1, 4

L(bexit01):
    j   $ra
    SUBU    $v0, $v0, $v1
L(bexit89):
    j   $ra
    SUBU    $v0, $t8, $t9

    .set    at
    .set    reorder

END(STRNCMP_NAME)
#ifndef __ANDROID__
# ifdef _LIBC
libc_hidden_builtin_def (STRNCMP_NAME)
# endif
#endif
