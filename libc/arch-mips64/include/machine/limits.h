/*	$OpenBSD: limits.h,v 1.5 2007/05/07 20:51:07 kettenis Exp $	*/

/*
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	@(#)limits.h	8.3 (Berkeley) 1/4/94
 */

#ifndef _MIPS_LIMITS_H_
#define _MIPS_LIMITS_H_

#include <sys/cdefs.h>

#define	MB_LEN_MAX	6		/* Allow 31 bit UTF2 */

#ifndef	SIZE_MAX
#define	SIZE_MAX	ULONG_MAX	/* max value for a size_t */
#endif
#define	SSIZE_MAX	LONG_MAX	/* max value for a ssize_t */

#if __BSD_VISIBLE
#define	SIZE_T_MAX	ULONG_MAX	/* max value for a size_t (historic) */

/* Quads and longs are the same on mips64 */
#define	UQUAD_MAX	(ULONG_MAX)	/* max value for a uquad_t */
#define	QUAD_MAX	(LONG_MAX)	/* max value for a quad_t */
#define	QUAD_MIN	(LONG_MIN)	/* min value for a quad_t */

#endif /* __BSD_VISIBLE */


#define LONGLONG_BIT    64
#define LONGLONG_MIN    (-9223372036854775807LL-1)
#define LONGLONG_MAX    9223372036854775807LL
#define ULONGLONG_MAX   18446744073709551615ULL

#endif /* !_MIPS_LIMITS_H_ */
