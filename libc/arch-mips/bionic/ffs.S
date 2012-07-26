/*	$NetBSD: ffs.S,v 1.5 2003/04/05 23:08:52 bjh21 Exp $	*/
/*
 * Copyright (c) 2001 Christopher Gilbert
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
 * 3. The name of the company nor the name of the author may be used to
 *    endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * ffs - find first set bit, this algorithm isolates the first set
 * bit, then multiplies the number by 0x0450fbaf which leaves the top
 * 6 bits as an index into the table.  This algorithm should be a win
 * over the checking each bit in turn as per the C compiled version.
 *
 * This is the ffs algorithm devised by d.seal and posted to comp.sys.arm on
 * 16 Feb 1994.
 */

	.text

	.ent	ffs
	.type	ffs, @function
	.global	ffs
	.align	4
	.set noreorder
ffs:
#if (__mips==32)
	subu	$t0,$0,$a0
	and	$a0,$t0
	clz	$t0,$a0
	li	$v0,32
	j	$ra	
	 subu	$v0,$t0
#else
/* Size of the ffs routine in bytes
 * This is used to index .L_ffs_table from $t9
 * FIXME: there must be a better way to do this
 */
#define FFSSIZE	12*4
	subu	$t0,$0,$a0
	and	$a0,$t0
	/*
	 * now a0 has at most one set bit, call this X
	 * if X = 0, all further instructions are skipped
	 */
	sll	$t0,$a0,4	/* t0 = X * 0x00000010 */
	or	$a0,$t0		/* a0 = X * 0x00000011 */ 
	sll	$t0,$a0,6	/* t0 = X * 0x00000440 */
	or	$a0,$t0		/* a0 = X * 0x00000451 */
	sll	$t0,$a0,16	/* t0 = X * 0x04510000 */
	subu	$a0,$t0,$a0	/* a0 = X * 0x0450fbaf */
	
	/* now lookup in table indexed on top 6 bits of a0 */
	srl	$a0,25
	addu	$t9,$a0
	j	$ra
	 lbu	$v0,FFSSIZE($t9)
.L_ffs_table:
/*               0   1   2   3   4   5   6   7           */
	.byte	 0,  1,  2, 13,  3,  7,  0, 14  /*  0- 7 */
	.byte	 4,  0,  8,  0,  0,  0,  0, 15  /*  8-15 */
	.byte	11,  5,  0,  0,  9,  0,  0, 26  /* 16-23 */
	.byte	 0,  0,  0,  0,  0, 22, 28, 16  /* 24-31 */
	.byte	32, 12,  6,  0,  0,  0,  0,  0	/* 32-39 */
	.byte	10,  0,  0, 25,  0,  0, 21, 27  /* 40-47 */
	.byte	31,  0,  0,  0,  0, 24,  0, 20  /* 48-55 */
	.byte   30,  0, 23, 19, 29, 18, 17,  0  /* 56-63 */
	
#endif
	.end	ffs
