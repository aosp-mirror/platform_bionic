/*	$OpenBSD: endian.h,v 1.3 2005/12/13 00:35:23 millert Exp $	*/

/*
 * Copyright (C) 2010 The Android Open Source Project
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

#ifndef _ARM_ENDIAN_H_
#define _ARM_ENDIAN_H_

#ifdef __GNUC__

/* NOTE: header <machine/cpu-features.h> could not be included directly
 * since it defines extra macros, such as PLD.
 */
#if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||	\
    defined(__ARM_ARCH_7__) ||					\
    defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || 	\
    defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || 	\
    defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__)

/* According to RealView Assembler User's Guide, REV and REV16 are available
 * in Thumb code and 16-bit instructions when used in Thumb-2 code.
 *
 * REV Rd, Rm
 *   Rd and Rm must both be Lo registers.
 *
 * REV16 Rd, Rm
 *   Rd and Rm must both be Lo registers.
 */
#ifdef __thumb__
#define REV_LO_REG	asm("r4")
#else
#define REV_LO_REG
#endif

#define __swap16md(x) ({						\
	register u_int16_t _x REV_LO_REG = (x);				\
	__asm volatile ("rev16 %0, %0" : "+r" (_x));			\
	_x;								\
})

#define __swap32md(x) ({						\
	register u_int32_t _x REV_LO_REG = (x);				\
	__asm volatile ("rev %0, %0" : "+r" (_x));			\
	_x;								\
})

#define __swap64md(x) ({						\
	u_int64_t _x = (x);						\
	(u_int64_t) __swap32md(_x >> 32) |				\
	(u_int64_t) __swap32md(_x & 0xffffffff) << 32;			\
})

/* Tell sys/endian.h we have MD variants of the swap macros.  */
#define MD_SWAP

#endif	/* __ARM_ARCH__ */
#endif	/* __GNUC__ */

#ifdef __ARMEB__
#define _BYTE_ORDER _BIG_ENDIAN
#else
#define _BYTE_ORDER _LITTLE_ENDIAN
#endif
#define	__STRICT_ALIGNMENT
#include <sys/types.h>
#include <sys/endian.h>

#endif	/* !_ARM_ENDIAN_H_ */
