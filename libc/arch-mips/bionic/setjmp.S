/*
 * Copyright (C) 2014 The Android Open Source Project
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
 * Copyright (c) 2001-2002 Opsycon AB  (www.opsycon.se / www.opsycon.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Opsycon AB nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
/*-
 * Copyright (c) 1991, 1993, 1995,
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Havard Eidnes.
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
 */
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

#include <private/bionic_asm.h>
#include <machine/setjmp.h>

/* jmpbuf is declared to users as an array of longs, which is only
 * 4-byte aligned in 32-bit builds.  The Mips jmpbuf begins with a
 * dynamically-sized 0- or 4-byte unused filler so that double-prec FP regs
 * are saved to 8-byte-aligned mem cells.
 * All the following jmpbuf offsets are from the rounded-DOWN base addr.
 */

/* Fields of same size on all MIPS abis: */
/*     	field:		byte offset:	size:						*/
/*     	dynam filler	(0*4)		   0-4 bytes of rounddown filler, DON'T TOUCH!!
						often overlays user storage!!		*/
#define	SC_MAGIC_OFFSET	(1*4)		/* 4 bytes, identify jmpbuf, first actual field */
#define	SC_FLAG_OFFSET	(2*4)		/* 4 bytes, savesigs flag */
#define	SC_FPSR_OFFSET	(3*4)		/* 4 bytes, floating point control/status reg */
/* following fields are 8-byte aligned */
#define	SC_MASK_OFFSET	(4*4)		/* 16 bytes, mips32/mips64 version of sigset_t */
#define	SC_SPARE_OFFSET	(8*4)		/* 8 bytes, reserved for future uses */

/* Registers that are 4-byte on mips32 o32, and 8-byte on mips64 n64 abi */
#define	SC_REGS_OFFSET	(10*4)		/* SC_REGS_BYTES */
#define	SC_REGS_SAVED	12 /*regs*/	/* ra,s0-s8,gp,sp */
#define	SC_REGS_BYTES   (SC_REGS_SAVED*REGSZ)
#define	SC_REGS		SC_REGS_OFFSET

/* Double floating pt registers are 8-bytes on all abis,
 * but the number of saved fp regs varies for o32/n32 versus n64 abis:
 */

#ifdef __LP64__
#define	SC_FPREGS_SAVED	8  /* all  fp regs f24,f25,f26,f27,f28,f29,f30,f31 */
#else
#define	SC_FPREGS_SAVED	6  /* even fp regs f20,f22,f24,f26,f28,f30 */
#endif

#define	SC_FPREGS_OFFSET (SC_REGS_OFFSET + SC_REGS_BYTES)  /* SC_FPREGS_BYTES */
#define	SC_FPREGS_BYTES (SC_FPREGS_SAVED*REGSZ_FP)
#define	SC_FPREGS	SC_FPREGS_OFFSET

#define	SC_TOTAL_BYTES	(SC_FPREGS_OFFSET + SC_FPREGS_BYTES)
#define	SC_TOTAL_LONGS	(SC_TOTAL_BYTES/REGSZ)

#if SC_TOTAL_LONGS > _JBLEN
#error _JBLEN is too small
#endif

/*
 *
 *  GPOFF and FRAMESIZE must be the same for all setjmp/longjmp routines
 *
 */

FRAMESZ= MKFSIZ(2,6)
A1OFF= FRAMESZ-4*REGSZ
A0OFF= FRAMESZ-3*REGSZ
GPOFF= FRAMESZ-2*REGSZ
RAOFF= FRAMESZ-1*REGSZ

NON_LEAF(sigsetjmp, FRAMESZ, ra)
	.mask	0x80000000, RAOFF
	PTR_SUBU sp, FRAMESZ			# allocate stack frame
	SETUP_GP64(GPOFF, sigsetjmp)
	SAVE_GP(GPOFF)
	.set	reorder

setjmp_common:
#ifndef __LP64__
	li	t0, ~7
	and	a0, t0				# round jmpbuf addr DOWN to 8-byte boundary
#endif
	sw	a1, SC_FLAG_OFFSET(a0)		# save savesigs flag
	beqz	a1, 1f				# do saving of signal mask?

	REG_S	ra, RAOFF(sp)			# spill state
	REG_S	a0, A0OFF(sp)
	# call sigprocmask(int how ignored, sigset_t* null, sigset_t* SC_MASK(a0)):
	LA	a2, SC_MASK_OFFSET(a0)		# gets current signal mask
	li	a0, 0				# how; ignored when new mask is null
	li	a1, 0				# null new mask
	jal	sigprocmask			# get current signal mask
	REG_L	a0, A0OFF(sp)
	REG_L	ra, RAOFF(sp)
1:
	li	v0, 0xACEDBADE			# sigcontext magic number
	sw	v0, SC_MAGIC_OFFSET(a0)
	# callee-saved long-sized regs:
	REG_S	ra, SC_REGS+0*REGSZ(a0)
	REG_S	s0, SC_REGS+1*REGSZ(a0)
	REG_S	s1, SC_REGS+2*REGSZ(a0)
	REG_S	s2, SC_REGS+3*REGSZ(a0)
	REG_S	s3, SC_REGS+4*REGSZ(a0)
	REG_S	s4, SC_REGS+5*REGSZ(a0)
	REG_S	s5, SC_REGS+6*REGSZ(a0)
	REG_S	s6, SC_REGS+7*REGSZ(a0)
	REG_S	s7, SC_REGS+8*REGSZ(a0)
	REG_S	s8, SC_REGS+9*REGSZ(a0)
	REG_L	v0, GPOFF(sp)
	REG_S	v0, SC_REGS+10*REGSZ(a0)	# save gp
	PTR_ADDU v0, sp, FRAMESZ
	REG_S	v0, SC_REGS+11*REGSZ(a0)	# save orig sp

	cfc1	v0, $31

#ifdef __LP64__
	# callee-saved fp regs on mips n64 ABI are $f24..$f31
	s.d	$f24, SC_FPREGS+0*REGSZ_FP(a0)
	s.d	$f25, SC_FPREGS+1*REGSZ_FP(a0)
	s.d	$f26, SC_FPREGS+2*REGSZ_FP(a0)
	s.d	$f27, SC_FPREGS+3*REGSZ_FP(a0)
	s.d	$f28, SC_FPREGS+4*REGSZ_FP(a0)
	s.d	$f29, SC_FPREGS+5*REGSZ_FP(a0)
	s.d	$f30, SC_FPREGS+6*REGSZ_FP(a0)
	s.d	$f31, SC_FPREGS+7*REGSZ_FP(a0)
#else
	# callee-saved fp regs on mips o32 ABI are
	#   the even-numbered double fp regs $f20,$f22,...$f30
	s.d	$f20, SC_FPREGS+0*REGSZ_FP(a0)
	s.d	$f22, SC_FPREGS+1*REGSZ_FP(a0)
	s.d	$f24, SC_FPREGS+2*REGSZ_FP(a0)
	s.d	$f26, SC_FPREGS+3*REGSZ_FP(a0)
	s.d	$f28, SC_FPREGS+4*REGSZ_FP(a0)
	s.d	$f30, SC_FPREGS+5*REGSZ_FP(a0)
#endif
	sw	v0, SC_FPSR_OFFSET(a0)
	move	v0, zero
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
	j	ra
END(sigsetjmp)


# Alternate entry points:

NON_LEAF(setjmp, FRAMESZ, ra)
	.mask	0x80000000, RAOFF
	PTR_SUBU sp, FRAMESZ
	SETUP_GP64(GPOFF, setjmp)		# can't share sigsetjmp's gp code
	SAVE_GP(GPOFF)
	.set	reorder

	li	a1, 1				# save/restore signals state
	b	setjmp_common			# tail call
END(setjmp)


NON_LEAF(_setjmp, FRAMESZ, ra)
	.mask	0x80000000, RAOFF
	PTR_SUBU sp, FRAMESZ
	SETUP_GP64(GPOFF, _setjmp)		# can't share sigsetjmp's gp code
	SAVE_GP(GPOFF)
	.set	reorder

	li	a1, 0				# don't save/restore signals
	b	setjmp_common			# tail call
END(_setjmp)


NON_LEAF(siglongjmp, FRAMESZ, ra)
	.mask	0x80000000, RAOFF
	PTR_SUBU sp, FRAMESZ
	SETUP_GP64(GPOFF, siglongjmp)
	SAVE_GP(GPOFF)
	.set	reorder

#ifndef __LP64__
	li	t0, ~7
	and	a0, t0				# round jmpbuf addr DOWN to 8-byte boundary
#endif
	lw	v0, SC_MAGIC_OFFSET(a0)
	li	t0, 0xACEDBADE
	bne	v0, t0, longjmp_botch		# jump if error

	lw	t0, SC_FLAG_OFFSET(a0)		# get savesigs flag
	beqz	t0, 1f				# restore signal mask?

	REG_S	a1, A1OFF(sp)			# temp spill
	REG_S	a0, A0OFF(sp)
        # call sigprocmask(int how SIG_SETMASK, sigset_t* SC_MASK(a0), sigset_t* null):
	LA	a1, SC_MASK_OFFSET(a0)		# signals being restored
	li	a0, 3				# mips SIG_SETMASK
	li	a2, 0				# null
	jal	sigprocmask			# restore signal mask
	REG_L	a0, A0OFF(sp)
	REG_L	a1, A1OFF(sp)
1:
	# callee-saved long-sized regs:
	REG_L	ra, SC_REGS+0*REGSZ(a0)
	REG_L	s0, SC_REGS+1*REGSZ(a0)
	REG_L	s1, SC_REGS+2*REGSZ(a0)
	REG_L	s2, SC_REGS+3*REGSZ(a0)
	REG_L	s3, SC_REGS+4*REGSZ(a0)
	REG_L	s4, SC_REGS+5*REGSZ(a0)
	REG_L	s5, SC_REGS+6*REGSZ(a0)
	REG_L	s6, SC_REGS+7*REGSZ(a0)
	REG_L	s7, SC_REGS+8*REGSZ(a0)
	REG_L	s8, SC_REGS+9*REGSZ(a0)
	REG_L	gp, SC_REGS+10*REGSZ(a0)
	REG_L	sp, SC_REGS+11*REGSZ(a0)

	lw	v0, SC_FPSR_OFFSET(a0)
	ctc1	v0, $31			# restore old fr mode before fp values
#ifdef __LP64__
	# callee-saved fp regs on mips n64 ABI are $f24..$f31
	l.d	$f24, SC_FPREGS+0*REGSZ_FP(a0)
	l.d	$f25, SC_FPREGS+1*REGSZ_FP(a0)
	l.d	$f26, SC_FPREGS+2*REGSZ_FP(a0)
	l.d	$f27, SC_FPREGS+3*REGSZ_FP(a0)
	l.d	$f28, SC_FPREGS+4*REGSZ_FP(a0)
	l.d	$f29, SC_FPREGS+5*REGSZ_FP(a0)
	l.d	$f30, SC_FPREGS+6*REGSZ_FP(a0)
	l.d	$f31, SC_FPREGS+7*REGSZ_FP(a0)
#else
	# callee-saved fp regs on mips o32 ABI are
	#   the even-numbered double fp regs $f20,$f22,...$f30
	l.d	$f20, SC_FPREGS+0*REGSZ_FP(a0)
	l.d	$f22, SC_FPREGS+1*REGSZ_FP(a0)
	l.d	$f24, SC_FPREGS+2*REGSZ_FP(a0)
	l.d	$f26, SC_FPREGS+3*REGSZ_FP(a0)
	l.d	$f28, SC_FPREGS+4*REGSZ_FP(a0)
	l.d	$f30, SC_FPREGS+5*REGSZ_FP(a0)
#endif
	bne	a1, zero, 1f
	li	a1, 1			# never return 0!
1:
	move	v0, a1
	j	ra			# return to setjmp call site

longjmp_botch:
	jal	longjmperror
	jal	abort
END(siglongjmp)

ALIAS_SYMBOL(longjmp, siglongjmp)
ALIAS_SYMBOL(_longjmp, siglongjmp)
