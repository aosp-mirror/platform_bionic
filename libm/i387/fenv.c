/*-
 * Copyright (c) 2004-2005 David Schultz <das@FreeBSD.ORG>
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
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/lib/msun/i387/fenv.c,v 1.2 2005/03/17 22:21:46 das Exp $
 */

#include <sys/cdefs.h>
#include <sys/types.h>
#include "npx.h"
#include "fenv.h"

/*
 * As compared to the x87 control word, the SSE unit's control word
 * has the rounding control bits offset by 3 and the exception mask
 * bits offset by 7.
 */
#define	_SSE_ROUND_SHIFT	3
#define	_SSE_EMASK_SHIFT	7

const fenv_t __fe_dfl_env = {
	__INITIAL_NPXCW__, /*__control*/
	0x0000,            /*__mxcsr_hi*/
	0x0000,            /*__status*/
	0x1f80,            /*__mxcsr_lo*/
	0xffffffff,        /*__tag*/
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff } /*__other*/
};

#define __fldcw(__cw)           __asm __volatile("fldcw %0" : : "m" (__cw))
#define __fldenv(__env)         __asm __volatile("fldenv %0" : : "m" (__env))
#define	__fldenvx(__env)	__asm __volatile("fldenv %0" : : "m" (__env)  \
				: "st", "st(1)", "st(2)", "st(3)", "st(4)",   \
				"st(5)", "st(6)", "st(7)")
#define __fnclex()              __asm __volatile("fnclex")
#define __fnstenv(__env)        __asm __volatile("fnstenv %0" : "=m" (*(__env)))
#define __fnstcw(__cw)          __asm __volatile("fnstcw %0" : "=m" (*(__cw)))
#define __fnstsw(__sw)          __asm __volatile("fnstsw %0" : "=am" (*(__sw)))
#define __fwait()               __asm __volatile("fwait")
#define __ldmxcsr(__csr)        __asm __volatile("ldmxcsr %0" : : "m" (__csr))
#define __stmxcsr(__csr)        __asm __volatile("stmxcsr %0" : "=m" (*(__csr)))

/* After testing for SSE support once, we cache the result in __has_sse. */
enum __sse_support { __SSE_YES, __SSE_NO, __SSE_UNK };
#ifdef __SSE__
#define __HAS_SSE()     1
#else
#define __HAS_SSE()     (__has_sse == __SSE_YES ||                      \
                         (__has_sse == __SSE_UNK && __test_sse()))
#endif

enum __sse_support __has_sse =
#ifdef __SSE__
	__SSE_YES;
#else
	__SSE_UNK;
#endif

#ifndef __SSE__
#define	getfl(x)	__asm __volatile("pushfl\n\tpopl %0" : "=mr" (*(x)))
#define	setfl(x)	__asm __volatile("pushl %0\n\tpopfl" : : "g" (x))
#define	cpuid_dx(x)	__asm __volatile("pushl %%ebx\n\tmovl $1, %%eax\n\t"  \
					 "cpuid\n\tpopl %%ebx"		      \
					: "=d" (*(x)) : : "eax", "ecx")

/*
 * Test for SSE support on this processor.  We need to do this because
 * we need to use ldmxcsr/stmxcsr to get correct results if any part
 * of the program was compiled to use SSE floating-point, but we can't
 * use SSE on older processors.
 */
int
__test_sse(void)
{
	int flag, nflag;
	int dx_features;

	/* Am I a 486? */
	getfl(&flag);
	nflag = flag ^ 0x200000;
	setfl(nflag);
	getfl(&nflag);
	if (flag != nflag) {
		/* Not a 486, so CPUID should work. */
		cpuid_dx(&dx_features);
		if (dx_features & 0x2000000) {
			__has_sse = __SSE_YES;
			return (1);
		}
	}
	__has_sse = __SSE_NO;
	return (0);
}
#endif /* __SSE__ */

int
fesetexceptflag(const fexcept_t *flagp, int excepts)
{
	fenv_t env;
	__uint32_t mxcsr;

	excepts &= FE_ALL_EXCEPT;
	if (excepts) { /* Do nothing if excepts is 0 */
		__fnstenv(&env);
		env.__status &= ~excepts;
		env.__status |= *flagp & excepts;
		__fnclex();
		__fldenv(env);
		if (__HAS_SSE()) {
			__stmxcsr(&mxcsr);
			mxcsr &= ~excepts;
			mxcsr |= *flagp & excepts;
			__ldmxcsr(mxcsr);
		}
	}

	return (0);
}

int
feraiseexcept(int excepts)
{
	fexcept_t ex = excepts;

	fesetexceptflag(&ex, excepts);
	__fwait();
	return (0);
}

int
fegetenv(fenv_t *envp)
{
	__uint32_t mxcsr;

	__fnstenv(envp);
	/*
	 * fnstenv masks all exceptions, so we need to restore
	 * the old control word to avoid this side effect.
	 */
	__fldcw(envp->__control);
	if (__HAS_SSE()) {
		__stmxcsr(&mxcsr);
		envp->__mxcsr_hi = mxcsr >> 16;
		envp->__mxcsr_lo = mxcsr & 0xffff;
	}
	return (0);
}

int
feholdexcept(fenv_t *envp)
{
	__uint32_t mxcsr;
	fenv_t env;

	__fnstenv(&env);
	*envp = env;
	env.__status &= ~FE_ALL_EXCEPT;
	env.__control |= FE_ALL_EXCEPT;
	__fnclex();
	__fldenv(env);
	if (__HAS_SSE()) {
		__stmxcsr(&mxcsr);
		envp->__mxcsr_hi = mxcsr >> 16;
		envp->__mxcsr_lo = mxcsr & 0xffff;
		mxcsr &= ~FE_ALL_EXCEPT;
		mxcsr |= FE_ALL_EXCEPT << _SSE_EMASK_SHIFT;
		__ldmxcsr(mxcsr);
	}
	return (0);
}

int
feupdateenv(const fenv_t *envp)
{
	__uint32_t mxcsr;
	__uint16_t status;

	__fnstsw(&status);
	if (__HAS_SSE()) {
		__stmxcsr(&mxcsr);
	} else {
		mxcsr = 0;
	}
	fesetenv(envp);
	feraiseexcept((mxcsr | status) & FE_ALL_EXCEPT);
	return (0);
}

int
feenableexcept(int mask)
{
	__uint32_t mxcsr;
	__uint16_t control, omask;

	mask &= FE_ALL_EXCEPT;
	__fnstcw(&control);
	if (__HAS_SSE()) {
		__stmxcsr(&mxcsr);
	} else {
		mxcsr = 0;
	}
	omask = ~(control | mxcsr >> _SSE_EMASK_SHIFT) & FE_ALL_EXCEPT;
	if (mask) {
		control &= ~mask;
		__fldcw(control);
		if (__HAS_SSE()) {
			mxcsr &= ~(mask << _SSE_EMASK_SHIFT);
			__ldmxcsr(mxcsr);
		}
	}
	return (omask);
}

int
fedisableexcept(int mask)
{
	__uint32_t mxcsr;
	__uint16_t control, omask;

	mask &= FE_ALL_EXCEPT;
	__fnstcw(&control);
	if (__HAS_SSE()) {
		__stmxcsr(&mxcsr);
	} else {
		mxcsr = 0;
	}
	omask = ~(control | mxcsr >> _SSE_EMASK_SHIFT) & FE_ALL_EXCEPT;
	if (mask) {
		control |= mask;
		__fldcw(control);
		if (__HAS_SSE()) {
			mxcsr |= mask << _SSE_EMASK_SHIFT;
			__ldmxcsr(mxcsr);
		}
	}
	return (omask);
}

int
feclearexcept(int excepts)
{
	fenv_t env;
	__uint32_t mxcsr;

	excepts &= FE_ALL_EXCEPT;
	if (excepts) { /* Do nothing if excepts is 0 */
		__fnstenv(&env);
		env.__status &= ~excepts;
		__fnclex();
		__fldenv(env);
		if (__HAS_SSE()) {
			__stmxcsr(&mxcsr);
			mxcsr &= ~excepts;
			__ldmxcsr(mxcsr);
		}
	}
	return (0);
}

int
fegetexceptflag(fexcept_t *flagp, int excepts)
{
	__uint32_t mxcsr;
	__uint16_t status;

	excepts &= FE_ALL_EXCEPT;
	__fnstsw(&status);
	if (__HAS_SSE()) {
		__stmxcsr(&mxcsr);
	} else {
		mxcsr = 0;
	}
	*flagp = (status | mxcsr) & excepts;
	return (0);
}

int
fetestexcept(int excepts)
{
	__uint32_t mxcsr;
	__uint16_t status;

	excepts &= FE_ALL_EXCEPT;
	if (excepts) { /* Do nothing if excepts is 0 */
		__fnstsw(&status);
		if (__HAS_SSE()) {
			__stmxcsr(&mxcsr);
		} else {
			mxcsr = 0;
		}
		return ((status | mxcsr) & excepts);
	}
	return (0);
}

int
fegetround(void)
{
	__uint16_t control;

	/*
	 * We assume that the x87 and the SSE unit agree on the
	 * rounding mode.  Reading the control word on the x87 turns
	 * out to be about 5 times faster than reading it on the SSE
	 * unit on an Opteron 244.
	 */
	__fnstcw(&control);
	return (control & _ROUND_MASK);
}

int
fesetround(int round)
{
	__uint32_t mxcsr;
	__uint16_t control;

	if (round & ~_ROUND_MASK) {
		return (-1);
	} else {
		__fnstcw(&control);
		control &= ~_ROUND_MASK;
		control |= round;
		__fldcw(control);
		if (__HAS_SSE()) {
			__stmxcsr(&mxcsr);
			mxcsr &= ~(_ROUND_MASK << _SSE_ROUND_SHIFT);
			mxcsr |= round << _SSE_ROUND_SHIFT;
			__ldmxcsr(mxcsr);
		}
		return (0);
	}
}

int
fesetenv(const fenv_t *envp)
{
	fenv_t env = *envp;
	__uint32_t mxcsr;

	mxcsr = (env.__mxcsr_hi << 16) | (env.__mxcsr_lo);
	env.__mxcsr_hi = 0xffff;
	env.__mxcsr_lo = 0xffff;
	/*
	 * XXX Using fldenvx() instead of fldenv() tells the compiler that this
	 * instruction clobbers the i387 register stack.  This happens because
	 * we restore the tag word from the saved environment.  Normally, this
	 * would happen anyway and we wouldn't care, because the ABI allows
	 * function calls to clobber the i387 regs.  However, fesetenv() is
	 * inlined, so we need to be more careful.
	 */
	__fldenvx(env);
	if (__HAS_SSE()) {
		__ldmxcsr(mxcsr);
	}
	return (0);
}

int
fegetexcept(void)
{
	__uint16_t control;

	/*
	 * We assume that the masks for the x87 and the SSE unit are
	 * the same.
	 */
	__fnstcw(&control);
	return (~control & FE_ALL_EXCEPT);
}
