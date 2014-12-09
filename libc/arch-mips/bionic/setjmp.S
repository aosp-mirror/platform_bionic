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

/*
 * _setjmp, _longjmp (restoring signal state)
 *
 *  GPOFF and FRAMESIZE must be the same for both _setjmp and _longjmp!
 *
 */

FRAMESZ= MKFSIZ(2,6)
A1OFF= FRAMESZ-4*REGSZ
A0OFF= FRAMESZ-3*REGSZ
GPOFF= FRAMESZ-2*REGSZ
RAOFF= FRAMESZ-1*REGSZ

NON_LEAF(setjmp, FRAMESZ, ra)
	.mask	0x80000000, RAOFF
	PTR_SUBU sp, FRAMESZ			# allocate stack frame
	SETUP_GP64(GPOFF, setjmp)
	SAVE_GP(GPOFF)
	.set	reorder

#ifndef __LP64__
	addiu   a0, 7				# roundup jmpbuf addr to 8-byte boundary
	li      t0, ~7
	and     a0, t0
#endif

	REG_S	ra, RAOFF(sp)			# save state
	REG_S	a0, A0OFF(sp)
	move	a0, zero			# get current signal mask
	jal	sigblock
	REG_L	a0, A0OFF(sp)
	REG_L	ra, RAOFF(sp)

	REG_S	v0, SC_MASK(a0)			# save sc_mask = sigblock(0)

	li	v0, 0xACEDBADE			# sigcontext magic number
	sw	v0, SC_MAGIC(a0)
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
	REG_S	v0, SC_REGS+10*REGSZ(a0)
	PTR_ADDU v0, sp, FRAMESZ
	REG_S	v0, SC_REGS+11*REGSZ(a0)

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
	#   the even-numbered fp regs $f20,$f22,...$f30
	s.d	$f20, SC_FPREGS+0*REGSZ_FP(a0)
	s.d	$f22, SC_FPREGS+1*REGSZ_FP(a0)
	s.d	$f24, SC_FPREGS+2*REGSZ_FP(a0)
	s.d	$f26, SC_FPREGS+3*REGSZ_FP(a0)
	s.d	$f28, SC_FPREGS+4*REGSZ_FP(a0)
	s.d	$f30, SC_FPREGS+5*REGSZ_FP(a0)
#endif
	sw	v0, SC_FPSR(a0)
	move	v0, zero
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
	j	ra
END(setjmp)

NON_LEAF(longjmp, FRAMESZ, ra)
	.mask	0x80000000, RAOFF
	PTR_SUBU sp, FRAMESZ
	SETUP_GP64(GPOFF, longjmp)
	SAVE_GP(GPOFF)
	.set	reorder

#ifndef __LP64__
	addiu	a0, 7				# roundup jmpbuf addr to 8-byte boundary
	li      t0, ~7
	and	a0, t0
#endif

	REG_S	a1, A1OFF(sp)
	REG_S	a0, A0OFF(sp)
	lw	a0, SC_MASK(a0)
	jal	sigsetmask
	REG_L	a0, A0OFF(sp)
	REG_L	a1, A1OFF(sp)

	lw	v0, SC_MAGIC(a0)
	li	t0, 0xACEDBADE
	bne	v0, t0, longjmp_botch			# jump if error

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

	lw	v0, SC_FPSR(a0)
	ctc1	v0, $31
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
	#   the even-numbered fp regs $f20,$f22,...$f30
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
	j	ra

longjmp_botch:
	jal	longjmperror
	jal	abort
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
END(longjmp)


/*
 * _setjmp, _longjmp (not restoring signal state)
 *
 *  GPOFF and FRAMESIZE must be the same for both _setjmp and _longjmp!
 *
 */

FRAMESZ= MKFSIZ(0,4)
GPOFF= FRAMESZ-2*REGSZ

LEAF(_setjmp, FRAMESZ)
	PTR_SUBU sp, FRAMESZ
	SETUP_GP64(GPOFF, _setjmp)
	SAVE_GP(GPOFF)
	.set	reorder

#ifndef __LP64__
	addiu   a0, 7				# roundup jmpbuf addr to 8-byte boundary
	li      t0, ~7
	and     a0, t0
#endif

	# SC_MASK is unused here

	li	v0, 0xACEDBADE			# sigcontext magic number
	sw	v0, SC_MAGIC(a0)
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
	REG_S	v0, SC_REGS+10*REGSZ(a0)
	PTR_ADDU v0, sp, FRAMESZ
	REG_S	v0, SC_REGS+11*REGSZ(a0)

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
	#   the even-numbered fp regs $f20,$f22,...$f30
	s.d	$f20, SC_FPREGS+0*REGSZ_FP(a0)
	s.d	$f22, SC_FPREGS+1*REGSZ_FP(a0)
	s.d	$f24, SC_FPREGS+2*REGSZ_FP(a0)
	s.d	$f26, SC_FPREGS+3*REGSZ_FP(a0)
	s.d	$f28, SC_FPREGS+4*REGSZ_FP(a0)
	s.d	$f30, SC_FPREGS+5*REGSZ_FP(a0)
#endif
	sw	v0, SC_FPSR(a0)
	move	v0, zero
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
	j	ra
END(_setjmp)


LEAF(_longjmp, FRAMESZ)
	PTR_SUBU sp, FRAMESZ
	SETUP_GP64(GPOFF, _longjmp)
	SAVE_GP(GPOFF)
	.set	reorder

#ifndef __LP64__
	addiu	a0, 7				# roundup jmpbuf addr to 8-byte boundary
	li      t0, ~7
	and	a0, t0
#endif

	# SC_MASK is unused here

	lw	v0, SC_MAGIC(a0)
	li	t0, 0xACEDBADE
	bne	v0, t0, _longjmp_botch			# jump if error

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

	lw	v0, SC_FPSR(a0)
	ctc1	v0, $31
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
	#   the even-numbered fp regs $f20,$f22,...$f30
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
	j	ra

_longjmp_botch:
	jal	longjmperror
	jal	abort
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
END(_longjmp)

/*
 * trampolines for sigsetjmp and  siglongjmp save and restore mask.
 *
 */
FRAMESZ= MKFSIZ(1,1)
GPOFF= FRAMESZ-2*REGSZ

LEAF(sigsetjmp, FRAMESZ)
	PTR_SUBU sp, FRAMESZ
	SETUP_GP64(GPOFF, sigsetjmp)
	.set	reorder
	sw	a1, _JBLEN*REGSZ(a0)		# save "savemask"
	bne	a1, 0x0, 1f			# do saving of signal mask?
	LA	t9, _setjmp
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
	jr t9

1:	LA	t9, setjmp
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
	jr t9
END(sigsetjmp)

LEAF(siglongjmp, FRAMESZ)
	PTR_SUBU sp, FRAMESZ
	SETUP_GP64(GPOFF, siglongjmp)
	.set	reorder
	lw	t0, _JBLEN*REGSZ(a0)		# get "savemask"
	bne	t0, 0x0, 1f			# restore signal mask?
	LA	t9, _longjmp
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
	jr	t9
1:
	LA	t9, longjmp
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
	jr	t9
END(siglongjmp)
