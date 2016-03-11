/*-
 * Copyright (c) 2001-2011 The FreeBSD Project.
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
 * $FreeBSD$
 */

#ifndef _COMPLEX_H
#define	_COMPLEX_H

#include <sys/cdefs.h>

#ifdef __GNUC__
#define	_Complex_I	((float _Complex)1.0i)
#endif

#ifdef __generic
_Static_assert(__generic(_Complex_I, float _Complex, 1, 0),
    "_Complex_I must be of type float _Complex");
#endif

#define	complex		_Complex
#define	I		_Complex_I

#if __ISO_C_VISIBLE >= 2011
#ifdef __clang__
#define	CMPLX(x, y)	((double complex){ x, y })
#define	CMPLXF(x, y)	((float complex){ x, y })
#define	CMPLXL(x, y)	((long double complex){ x, y })
#else
#define	CMPLX(x, y)	__builtin_complex((double)(x), (double)(y))
#define	CMPLXF(x, y)	__builtin_complex((float)(x), (float)(y))
#define	CMPLXL(x, y)	__builtin_complex((long double)(x), (long double)(y))
#endif
#endif /* __ISO_C_VISIBLE >= 2011 */

__BEGIN_DECLS
#pragma GCC visibility push(default)

/* 7.3.5 Trigonometric functions */
/* 7.3.5.1 The cacos functions */
double complex cacos(double complex);
float complex cacosf(float complex);
long double complex cacosl(long double complex);
/* 7.3.5.2 The casin functions */
double complex casin(double complex);
float complex casinf(float complex);
long double complex casinl(long double complex);
/* 7.3.5.1 The catan functions */
double complex catan(double complex);
float complex catanf(float complex);
long double complex catanl(long double complex);
/* 7.3.5.1 The ccos functions */
double complex ccos(double complex);
float complex ccosf(float complex);
long double complex ccosl(long double complex);
/* 7.3.5.1 The csin functions */
double complex csin(double complex);
float complex csinf(float complex);
long double complex csinl(long double complex);
/* 7.3.5.1 The ctan functions */
double complex ctan(double complex);
float complex ctanf(float complex);
long double complex ctanl(long double complex);

/* 7.3.6 Hyperbolic functions */
/* 7.3.6.1 The cacosh functions */
double complex cacosh(double complex);
float complex cacoshf(float complex);
long double complex cacoshl(long double complex);
/* 7.3.6.2 The casinh functions */
double complex casinh(double complex);
float complex casinhf(float complex);
long double complex casinhl(long double complex);
/* 7.3.6.3 The catanh functions */
double complex catanh(double complex);
float complex catanhf(float complex);
long double complex catanhl(long double complex);
/* 7.3.6.4 The ccosh functions */
double complex ccosh(double complex);
float complex ccoshf(float complex);
long double complex ccoshl(long double complex);
/* 7.3.6.5 The csinh functions */
double complex csinh(double complex);
float complex csinhf(float complex);
long double complex csinhl(long double complex);
/* 7.3.6.6 The ctanh functions */
double complex ctanh(double complex);
float complex ctanhf(float complex);
long double complex ctanhl(long double complex);

/* 7.3.7 Exponential and logarithmic functions */
/* 7.3.7.1 The cexp functions */
double complex cexp(double complex);
float complex cexpf(float complex);
long double complex cexpl(long double complex);
/* 7.3.7.2 The clog functions */
double complex clog(double complex);
float complex clogf(float complex);
long double complex clogl(long double complex);

/* 7.3.8 Power and absolute-value functions */
/* 7.3.8.1 The cabs functions */
double cabs(double complex);
float cabsf(float complex);
long double cabsl(long double complex);
/* 7.3.8.2 The cpow functions */
double complex cpow(double complex, double complex);
float complex cpowf(float complex, float complex);
long double complex cpowl(long double complex, long double complex);
/* 7.3.8.3 The csqrt functions */
double complex csqrt(double complex);
float complex csqrtf(float complex);
long double complex csqrtl(long double complex);

/* 7.3.9 Manipulation functions */
/* 7.3.9.1 The carg functions */
double carg(double complex);
float cargf(float complex);
long double cargl(long double complex);
/* 7.3.9.2 The cimag functions */
double cimag(double complex);
float cimagf(float complex);
long double cimagl(long double complex);
/* 7.3.9.3 The conj functions */
double complex conj(double complex);
float complex conjf(float complex);
long double complex conjl(long double complex);
/* 7.3.9.4 The cproj functions */
double complex cproj(double complex);
float complex cprojf(float complex);
long double complex cprojl(long double complex);
/* 7.3.9.5 The creal functions */
double creal(double complex);
float crealf(float complex);
long double creall(long double complex);

#pragma GCC visibility pop
__END_DECLS

#endif /* _COMPLEX_H */
