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

#if __STDC_VERSION__ >= 201112L
#define	CMPLX(x, y)	((double complex){ x, y })
#define	CMPLXF(x, y)	((float complex){ x, y })
#define	CMPLXL(x, y)	((long double complex){ x, y })
#endif

__BEGIN_DECLS

/* 7.3.5 Trigonometric functions */
/* 7.3.5.1 The cacos functions */
double complex cacos(double complex __z) __INTRODUCED_IN(23);
float complex cacosf(float complex __z) __INTRODUCED_IN(23);
long double complex cacosl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.5.2 The casin functions */
double complex casin(double complex __z) __INTRODUCED_IN(23);
float complex casinf(float complex __z) __INTRODUCED_IN(23);
long double complex casinl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.5.1 The catan functions */
double complex catan(double complex __z) __INTRODUCED_IN(23);
float complex catanf(float complex __z) __INTRODUCED_IN(23);
long double complex catanl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.5.1 The ccos functions */
double complex ccos(double complex __z) __INTRODUCED_IN(23);
float complex ccosf(float complex __z) __INTRODUCED_IN(23);
long double complex ccosl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.5.1 The csin functions */
double complex csin(double complex __z) __INTRODUCED_IN(23);
float complex csinf(float complex __z) __INTRODUCED_IN(23);
long double complex csinl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.5.1 The ctan functions */
double complex ctan(double complex __z) __INTRODUCED_IN(23);
float complex ctanf(float complex __z) __INTRODUCED_IN(23);
long double complex ctanl(long double complex __z) __INTRODUCED_IN(26);

/* 7.3.6 Hyperbolic functions */
/* 7.3.6.1 The cacosh functions */
double complex cacosh(double complex __z) __INTRODUCED_IN(23);
float complex cacoshf(float complex __z) __INTRODUCED_IN(23);
long double complex cacoshl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.6.2 The casinh functions */
double complex casinh(double complex __z) __INTRODUCED_IN(23);
float complex casinhf(float complex __z) __INTRODUCED_IN(23);
long double complex casinhl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.6.3 The catanh functions */
double complex catanh(double complex __z) __INTRODUCED_IN(23);
float complex catanhf(float complex __z) __INTRODUCED_IN(23);
long double complex catanhl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.6.4 The ccosh functions */
double complex ccosh(double complex __z) __INTRODUCED_IN(23);
float complex ccoshf(float complex __z) __INTRODUCED_IN(23);
long double complex ccoshl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.6.5 The csinh functions */
double complex csinh(double complex __z) __INTRODUCED_IN(23);
float complex csinhf(float complex __z) __INTRODUCED_IN(23);
long double complex csinhl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.6.6 The ctanh functions */
double complex ctanh(double complex __z) __INTRODUCED_IN(23);
float complex ctanhf(float complex __z) __INTRODUCED_IN(23);
long double complex ctanhl(long double complex __z) __INTRODUCED_IN(26);

/* 7.3.7 Exponential and logarithmic functions */
/* 7.3.7.1 The cexp functions */
double complex cexp(double complex __z) __INTRODUCED_IN(23);
float complex cexpf(float complex __z) __INTRODUCED_IN(23);
long double complex cexpl(long double complex __z) __INTRODUCED_IN(26);
/* 7.3.7.2 The clog functions */
double complex clog(double complex __z) __INTRODUCED_IN(26);
float complex clogf(float complex __z) __INTRODUCED_IN(26);
long double complex clogl(long double complex __z) __INTRODUCED_IN(26);

/* 7.3.8 Power and absolute-value functions */
/* 7.3.8.1 The cabs functions */
double cabs(double complex __z) __INTRODUCED_IN(23);
float cabsf(float complex __z) __INTRODUCED_IN(23);
long double cabsl(long double complex __z) __INTRODUCED_IN(23);
/* 7.3.8.2 The cpow functions */
double complex cpow(double complex __x, double complex __z) __INTRODUCED_IN(26);
float complex cpowf(float complex __x, float complex __z) __INTRODUCED_IN(26);
long double complex cpowl(long double complex __x, long double complex __z) __INTRODUCED_IN(26);
/* 7.3.8.3 The csqrt functions */
double complex csqrt(double complex __z) __INTRODUCED_IN(23);
float complex csqrtf(float complex __z) __INTRODUCED_IN(23);
long double complex csqrtl(long double complex __z) __INTRODUCED_IN(23);

/* 7.3.9 Manipulation functions */
/* 7.3.9.1 The carg functions */
double carg(double complex __z) __INTRODUCED_IN(23);
float cargf(float complex __z) __INTRODUCED_IN(23);
long double cargl(long double complex __z) __INTRODUCED_IN(23);
/* 7.3.9.2 The cimag functions */
double cimag(double complex __z) __INTRODUCED_IN(23);
float cimagf(float complex __z) __INTRODUCED_IN(23);
long double cimagl(long double complex __z) __INTRODUCED_IN(23);
/* 7.3.9.3 The conj functions */
double complex conj(double complex __z) __INTRODUCED_IN(23);
float complex conjf(float complex __z) __INTRODUCED_IN(23);
long double complex conjl(long double complex __z) __INTRODUCED_IN(23);
/* 7.3.9.4 The cproj functions */
double complex cproj(double complex __z) __INTRODUCED_IN(23);
float complex cprojf(float complex __z) __INTRODUCED_IN(23);
long double complex cprojl(long double complex __z) __INTRODUCED_IN(23);
/* 7.3.9.5 The creal functions */
double creal(double complex __z) __INTRODUCED_IN(23);
float crealf(float complex __z) __INTRODUCED_IN(23);
long double creall(long double complex __z) __INTRODUCED_IN(23);

__END_DECLS

#endif
