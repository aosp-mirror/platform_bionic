/*	$OpenBSD: _setjmp.S,v 1.4 2005/08/07 16:40:15 espie Exp $ */

/*
 * Copyright (c) 2002 Opsycon AB  (www.opsycon.se / www.opsycon.com)
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

#include <private/bionic_asm.h>
#include <machine/signal.h>

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
	bne	v0, t0, botch			# jump if error

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

botch:
	jal	longjmperror
	jal	abort
	RESTORE_GP64
	PTR_ADDU sp, FRAMESZ
END(_longjmp)
