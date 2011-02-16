/*	$OpenBSD: _setjmp.S,v 1.2 2004/02/01 05:40:52 drahn Exp $	*/
/*	$NetBSD: _setjmp.S,v 1.5 2003/04/05 23:08:51 bjh21 Exp $	*/

/*
 * Copyright (c) 1997 Mark Brinicombe
 * Copyright (c) 2010 Android Open Source Project.
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Mark Brinicombe
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 */

#include <machine/asm.h>
#include <machine/setjmp.h>
#include <machine/cpu-features.h>

/*
 * C library -- _setjmp, _longjmp
 *
 *	_longjmp(a,v)
 * will generate a "return(v)" from the last call to
 *	_setjmp(a)
 * by restoring registers from the stack.
 * The previous signal state is NOT restored.
 *
 * Note: r0 is the return value
 *       r1-r3 are scratch registers in functions
 */

ENTRY(_setjmp)
	ldr	r1, .L_setjmp_magic
	str	r1, [r0, #(_JB_MAGIC * 4)]

	/* Store core registers */
	add     r1, r0, #(_JB_CORE_BASE * 4)
	stmia   r1, {r4-r14}

#ifdef __ARM_HAVE_VFP
	/* Store floating-point registers */
	add     r1, r0, #(_JB_FLOAT_BASE * 4)
	vstmia  r1, {d8-d15}
	/* Store floating-point state */
	fmrx    r1, fpscr
	str     r1, [r0, #(_JB_FLOAT_STATE * 4)]
#endif  /* __ARM_HAVE_VFP */

        mov	r0, #0x00000000
        bx      lr
END(_setjmp)

.L_setjmp_magic:
	.word	_JB_MAGIC__SETJMP

ENTRY(_longjmp)
	ldr	r2, .L_setjmp_magic
	ldr	r3, [r0, #(_JB_MAGIC * 4)]
	teq	r2, r3
	bne	botch

#ifdef __ARM_HAVE_VFP
	/* Restore floating-point registers */
	add     r2, r0, #(_JB_FLOAT_BASE * 4)
	vldmia  r2, {d8-d15}
	/* Restore floating-point state */
	ldr     r2, [r0, #(_JB_FLOAT_STATE * 4)]
	fmxr    fpscr, r2
#endif /* __ARM_HAVE_VFP */

	/* Restore core registers */
	add     r2, r0, #(_JB_CORE_BASE * 4)
	ldmia   r2, {r4-r14}

	/* Validate sp and r14 */
	teq	sp, #0
	teqne	r14, #0
	beq	botch

	/* Set return value */
	mov	r0, r1
	teq	r0, #0x00000000
	moveq	r0, #0x00000001
	bx      lr

	/* validation failed, die die die. */
botch:
	bl	PIC_SYM(_C_LABEL(longjmperror), PLT)
	bl	PIC_SYM(_C_LABEL(abort), PLT)
	b	. - 8		/* Cannot get here */
END(_longjmp)
