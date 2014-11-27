/*	$OpenBSD: signal.h,v 1.8 2006/01/09 18:18:37 millert Exp $	*/

/*
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ralph Campbell.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)signal.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _MIPS_SIGNAL_H_
#define _MIPS_SIGNAL_H_

/* On Mips32, jmpbuf begins with optional 4-byte filler so that
 *  all saved FP regs are aligned on 8-byte boundary, despite this whole
 *  struct being mis-declared to users as an array of (4-byte) longs.
 *  All the following offsets are then from the rounded-up base addr
 */

/* Fields of same size on all MIPS abis: */
#define	SC_MAGIC        (0*4)		/* 4 bytes, identify jmpbuf */
#define	SC_MASK		(1*4)		/* 4 bytes, saved signal mask */
#define	SC_FPSR		(2*4)		/* 4 bytes, floating point control/status reg */
/*     	filler2		(3*4)		   4 bytes, pad to 8-byte boundary */

/* Registers that are 4-byte on mips32 o32, and 8-byte on mips64 n64 abi */
#define	SC_REGS_SAVED	12		/* ra,gp,sp,s0-s8 */
#define	SC_REGS		(4*4)		/* SC_REGS_SAVED*REGSZ bytes */

/* Floating pt registers are 8-bytes on all abis,
 * but the number of saved fp regs varies for o32/n32 versus n64 abis:
 */

#ifdef __LP64__
#define	SC_FPREGS_SAVED	8  /* all  fp regs f24,f25,f26,f27,f28,f29,f30,f31 */
#else
#define	SC_FPREGS_SAVED	6  /* even fp regs f20,f22,f24,f26,f28,f30 */
#endif

#define	SC_FPREGS	(SC_REGS + SC_REGS_SAVED*REGSZ)  /* SC_FPREGS_SAVED*REGSZ_FP bytes */

#define	SC_BYTES	(SC_FPREGS + SC_FPREGS_SAVED*REGSZ_FP)
#define	SC_LONGS	(SC_BYTES/REGSZ)

#ifdef __LP64__
/* SC_LONGS is 22, so _JBLEN should be 22 or larger */
#else
/* SC_LONGS is 28, but must also allocate dynamic-roundup filler.
   so _JBLEN should be 29 or larger */
#endif

#endif	/* !_MIPS_SIGNAL_H_ */
