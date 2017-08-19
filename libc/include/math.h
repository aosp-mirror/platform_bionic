/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * from: @(#)fdlibm.h 5.1 93/09/24
 * $FreeBSD$
 */

#ifndef _MATH_H_
#define _MATH_H_

#include <sys/cdefs.h>
#include <limits.h>

__BEGIN_DECLS

#define HUGE_VAL	__builtin_huge_val()

#define FP_ILOGB0	(-INT_MAX)
#define FP_ILOGBNAN	INT_MAX

#define HUGE_VALF	__builtin_huge_valf()
#define HUGE_VALL	__builtin_huge_vall()
#define INFINITY	__builtin_inff()
#define NAN		__builtin_nanf("")

#define MATH_ERRNO	1
#define MATH_ERREXCEPT	2
#define math_errhandling	MATH_ERREXCEPT

#if defined(__FP_FAST_FMA)
#define FP_FAST_FMA 1
#endif
#if defined(__FP_FAST_FMAF)
#define FP_FAST_FMAF 1
#endif
#if defined(__FP_FAST_FMAL)
#define FP_FAST_FMAL 1
#endif

/* Symbolic constants to classify floating point numbers. */
#define FP_INFINITE	0x01
#define FP_NAN		0x02
#define FP_NORMAL	0x04
#define FP_SUBNORMAL	0x08
#define FP_ZERO		0x10
#define fpclassify(x) \
    __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)

#define isfinite(x) __builtin_isfinite(x)
#define isinf(x) __builtin_isinf(x)
#define isnan(x) __builtin_isnan(x)
#define isnormal(x) __builtin_isnormal(x)

#define isgreater(x, y) __builtin_isgreater((x), (y))
#define isgreaterequal(x, y) __builtin_isgreaterequal((x), (y))
#define isless(x, y) __builtin_isless((x), (y))
#define islessequal(x, y) __builtin_islessequal((x), (y))
#define islessgreater(x, y) __builtin_islessgreater((x), (y))
#define isunordered(x, y) __builtin_isunordered((x), (y))

#define signbit(x) \
    ((sizeof(x) == sizeof(float)) ? __builtin_signbitf(x) \
    : (sizeof(x) == sizeof(double)) ? __builtin_signbit(x) \
    : __builtin_signbitl(x))

typedef double __double_t;
typedef __double_t double_t;
typedef float __float_t;
typedef __float_t float_t;

#if defined(__USE_BSD)
#define HUGE MAXFLOAT
#endif

extern int signgam;

/*
 * Most of these functions depend on the rounding mode and have the side
 * effect of raising floating-point exceptions, so they are not declared
 * as __attribute_const__. In C99, FENV_ACCESS affects the purity of these functions.
 */

int __fpclassifyd(double __x) __attribute_const__;
int __fpclassifyf(float __x) __attribute_const__;
int __fpclassifyl(long double __x) __attribute_const__;
int __isfinitef(float __x) __attribute_const__;
int __isfinite(double __x) __attribute_const__;
int __isfinitel(long double __x) __attribute_const__;
int __isinff(float __x) __attribute_const__;
int __isinfl(long double __x) __attribute_const__;
int __isnanf(float __x) __attribute_const__ __INTRODUCED_IN(21);
int __isnanl(long double __x) __attribute_const__;
int __isnormalf(float __x) __attribute_const__;
int __isnormal(double __x) __attribute_const__;
int __isnormall(long double __x) __attribute_const__;
int __signbit(double __x) __attribute_const__;
int __signbitf(float __x) __attribute_const__;
int __signbitl(long double __x) __attribute_const__;

double acos(double __x);
double asin(double __x);
double atan(double __x);
double atan2(double __y, double __x);
double cos(double __x);
double sin(double __x);
double tan(double __x);

double cosh(double __x);
double sinh(double __x);
double tanh(double __x);

double exp(double __x);
double frexp(double __x, int* __exponent); /* fundamentally !__attribute_const__ */
double ldexp(double __x, int __exponent);
double log(double __x);
double log10(double __x);
double modf(double __x, double* __integral_part); /* fundamentally !__attribute_const__ */

double pow(double __x, double __y);
double sqrt(double __x);

double ceil(double __x);
double fabs(double __x) __attribute_const__;
double floor(double __x);
double fmod(double __x, double __y);

double acosh(double __x);
double asinh(double __x);
double atanh(double __x);
double cbrt(double __x);
double erf(double __x);
double erfc(double __x);
double exp2(double __x);
double expm1(double __x);
double fma(double __x, double __y, double __z);
double hypot(double __x, double __y);
int ilogb(double __x) __attribute_const__;
double lgamma(double __x);
long long llrint(double __x);
long long llround(double __x);
double log1p(double __x);
double log2(double __x) __INTRODUCED_IN(18);
double logb(double __x);
long lrint(double __x);
long lround(double __x);

/*
 * https://code.google.com/p/android/issues/detail?id=271629
 * To be fully compliant with C++, we need to not define these (C doesn't
 * specify them either). Exposing these means that isinf and isnan will have a
 * return type of int in C++ rather than bool like they're supposed to be.
 *
 * GNU libstdc++ 4.9 isn't able to handle a standard compliant C library. Its
 * <cmath> will `#undef isnan` from math.h and only adds the function overloads
 * to the std namespace, making it impossible to use both <cmath> (which gets
 * included by a lot of other standard headers) and ::isnan.
 */
int (isinf)(double __x) __attribute_const__ __INTRODUCED_IN(21);
int (isnan)(double __x) __attribute_const__;

double nan(const char* __kind) __attribute_const__ __INTRODUCED_IN_ARM(13) __INTRODUCED_IN_MIPS(13)
    __INTRODUCED_IN_X86(9);

double nextafter(double __x, double __y);
double remainder(double __x, double __y);
double remquo(double __x, double __y, int* __quotient_bits);
double rint(double __x);

double copysign(double __value, double __sign) __attribute_const__;
double fdim(double __x, double __y);
double fmax(double __x, double __y) __attribute_const__;
double fmin(double __x, double __y) __attribute_const__;
double nearbyint(double __x);
double round(double __x);
double scalbln(double __x, long __exponent) __INTRODUCED_IN_X86(18) __VERSIONER_NO_GUARD;
double scalbn(double __x, int __exponent);
double tgamma(double __x);
double trunc(double __x);

float acosf(float __x);
float asinf(float __x);
float atanf(float __x);
float atan2f(float __y, float __x);
float cosf(float __x);
float sinf(float __x);
float tanf(float __x);

float coshf(float __x);
float sinhf(float __x);
float tanhf(float __x);

float exp2f(float __x);
float expf(float __x);
float expm1f(float __x);
float frexpf(float __x, int* __exponent); /* fundamentally !__attribute_const__ */
int ilogbf(float __x) __attribute_const__;
float ldexpf(float __x, int __exponent);
float log10f(float __x);
float log1pf(float __x);
float log2f(float __x) __INTRODUCED_IN(18);
float logf(float __x);
float modff(float __x, float* __integral_part); /* fundamentally !__attribute_const__ */

float powf(float __x, float __y);
float sqrtf(float __x);

float ceilf(float __x);
float fabsf(float __x) __attribute_const__;
float floorf(float __x);
float fmodf(float __x, float __y);
float roundf(float __x);

float erff(float __x);
float erfcf(float __x);
float hypotf(float __x, float __y);
float lgammaf(float __x);
float tgammaf(float __x) __INTRODUCED_IN_ARM(13) __INTRODUCED_IN_MIPS(13) __INTRODUCED_IN_X86(9);

float acoshf(float __x);
float asinhf(float __x);
float atanhf(float __x);
float cbrtf(float __x);
float logbf(float __x);
float copysignf(float __value, float __sign) __attribute_const__;
long long llrintf(float __x);
long long llroundf(float __x);
long lrintf(float __x);
long lroundf(float __x);
float nanf(const char* __kind) __attribute_const__ __INTRODUCED_IN_ARM(13) __INTRODUCED_IN_MIPS(13)
    __INTRODUCED_IN_X86(9);
float nearbyintf(float __x);
float nextafterf(float __x, float __y);
float remainderf(float __x, float __y);
float remquof(float __x, float __y, int* __quotient_bits);
float rintf(float __x);
float scalblnf(float __x, long __exponent) __INTRODUCED_IN_X86(18) __VERSIONER_NO_GUARD;
float scalbnf(float __x, int __exponent);
float truncf(float __x);

float fdimf(float __x, float __y);
float fmaf(float __x, float __y, float __z);
float fmaxf(float __x, float __y) __attribute_const__;
float fminf(float __x, float __y) __attribute_const__;

long double acoshl(long double __x) __INTRODUCED_IN(21);
long double acosl(long double __x) __INTRODUCED_IN(21);
long double asinhl(long double __x) __INTRODUCED_IN(21);
long double asinl(long double __x) __INTRODUCED_IN(21);
long double atan2l(long double __y, long double __x) __INTRODUCED_IN(21);
long double atanhl(long double __x) __INTRODUCED_IN(21);
long double atanl(long double __x) __INTRODUCED_IN(21);
long double cbrtl(long double __x) __INTRODUCED_IN(21);
long double ceill(long double __x);
long double copysignl(long double __value, long double __sign) __attribute_const__;
long double coshl(long double __x) __INTRODUCED_IN(21);
long double cosl(long double __x) __INTRODUCED_IN(21);
long double erfcl(long double __x) __INTRODUCED_IN(21);
long double erfl(long double __x) __INTRODUCED_IN(21);
long double exp2l(long double __x) __INTRODUCED_IN(21);
long double expl(long double __x) __INTRODUCED_IN(21);
long double expm1l(long double __x) __INTRODUCED_IN(21);
long double fabsl(long double __x) __attribute_const__;
long double fdiml(long double __x, long double __y);
long double floorl(long double __x);
long double fmal(long double __x, long double __y, long double __z) __INTRODUCED_IN(21) __VERSIONER_NO_GUARD;
long double fmaxl(long double __x, long double __y) __attribute_const__;
long double fminl(long double __x, long double __y) __attribute_const__;
long double fmodl(long double __x, long double __y) __INTRODUCED_IN(21);
long double frexpl(long double __x, int* __exponent)
    __INTRODUCED_IN(21) __VERSIONER_NO_GUARD; /* fundamentally !__attribute_const__ */
long double hypotl(long double __x, long double __y) __INTRODUCED_IN(21);
int ilogbl(long double __x) __attribute_const__;
long double ldexpl(long double __x, int __exponent);
long double lgammal(long double __x) __INTRODUCED_IN(21);
long long llrintl(long double __x) __INTRODUCED_IN(21);
long long llroundl(long double __x);
long double log10l(long double __x) __INTRODUCED_IN(21);
long double log1pl(long double __x) __INTRODUCED_IN(21);
long double log2l(long double __x) __INTRODUCED_IN(18);
long double logbl(long double __x) __INTRODUCED_IN(18);
long double logl(long double __x) __INTRODUCED_IN(21);
long lrintl(long double __x) __INTRODUCED_IN(21);
long lroundl(long double __x);
long double modfl(long double __x, long double* __integral_part) __INTRODUCED_IN(21); /* fundamentally !__attribute_const__ */
long double nanl(const char* __kind) __attribute_const__ __INTRODUCED_IN(13);
long double nearbyintl(long double __x) __INTRODUCED_IN(21);
long double nextafterl(long double __x, long double __y) __INTRODUCED_IN(21) __VERSIONER_NO_GUARD;
double nexttoward(double __x, long double __y) __INTRODUCED_IN(18) __VERSIONER_NO_GUARD;
float nexttowardf(float __x, long double __y);
long double nexttowardl(long double __x, long double __y) __INTRODUCED_IN(18) __VERSIONER_NO_GUARD;
long double powl(long double __x, long double __y) __INTRODUCED_IN(21);
long double remainderl(long double __x, long double __y) __INTRODUCED_IN(21);
long double remquol(long double __x, long double __y, int* __quotient_bits) __INTRODUCED_IN(21);
long double rintl(long double __x) __INTRODUCED_IN(21);
long double roundl(long double __x);
long double scalblnl(long double __x, long __exponent) __INTRODUCED_IN_X86(18) __VERSIONER_NO_GUARD;
long double scalbnl(long double __x, int __exponent);
long double sinhl(long double __x) __INTRODUCED_IN(21);
long double sinl(long double __x) __INTRODUCED_IN(21);
long double sqrtl(long double __x) __INTRODUCED_IN(21);
long double tanhl(long double __x) __INTRODUCED_IN(21);
long double tanl(long double __x) __INTRODUCED_IN(21);
long double tgammal(long double __x) __INTRODUCED_IN(21);
long double truncl(long double __x);

double j0(double __x);
double j1(double __x);
double jn(int __n, double __x);
double y0(double __x);
double y1(double __x);
double yn(int __n, double __x);

#define M_E		2.7182818284590452354	/* e */
#define M_LOG2E		1.4426950408889634074	/* log 2e */
#define M_LOG10E	0.43429448190325182765	/* log 10e */
#define M_LN2		0.69314718055994530942	/* log e2 */
#define M_LN10		2.30258509299404568402	/* log e10 */
#define M_PI		3.14159265358979323846	/* pi */
#define M_PI_2		1.57079632679489661923	/* pi/2 */
#define M_PI_4		0.78539816339744830962	/* pi/4 */
#define M_1_PI		0.31830988618379067154	/* 1/pi */
#define M_2_PI		0.63661977236758134308	/* 2/pi */
#define M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
#define M_SQRT2		1.41421356237309504880	/* sqrt(2) */
#define M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */

#define MAXFLOAT	((float)3.40282346638528860e+38)

#if defined(__USE_BSD) || defined(__USE_GNU)
double gamma(double __x);
double scalb(double __x, double __exponent);
double drem(double __x, double __y);
int finite(double __x) __attribute_const__;
int isnanf(float __x) __attribute_const__;
double gamma_r(double __x, int* __sign);
double lgamma_r(double __x, int* __sign);
double significand(double __x);
long double lgammal_r(long double __x, int* __sign) __INTRODUCED_IN(23);
long double significandl(long double __x) __INTRODUCED_IN(21);
float dremf(float __x, float __y);
int finitef(float __x) __attribute_const__;
float gammaf(float __x);
float j0f(float __x);
float j1f(float __x);
float jnf(int __n, float __x);
float scalbf(float __x, float __exponent);
float y0f(float __x);
float y1f(float __x);
float ynf(int __n, float __x);
float gammaf_r(float __x, int* __sign);
float lgammaf_r(float __x, int* __sign);
float significandf(float __x);
#endif

#if defined(__USE_GNU)
#define M_El            2.718281828459045235360287471352662498L /* e */
#define M_LOG2El        1.442695040888963407359924681001892137L /* log 2e */
#define M_LOG10El       0.434294481903251827651128918916605082L /* log 10e */
#define M_LN2l          0.693147180559945309417232121458176568L /* log e2 */
#define M_LN10l         2.302585092994045684017991454684364208L /* log e10 */
#define M_PIl           3.141592653589793238462643383279502884L /* pi */
#define M_PI_2l         1.570796326794896619231321691639751442L /* pi/2 */
#define M_PI_4l         0.785398163397448309615660845819875721L /* pi/4 */
#define M_1_PIl         0.318309886183790671537767526745028724L /* 1/pi */
#define M_2_PIl         0.636619772367581343075535053490057448L /* 2/pi */
#define M_2_SQRTPIl     1.128379167095512573896158903121545172L /* 2/sqrt(pi) */
#define M_SQRT2l        1.414213562373095048801688724209698079L /* sqrt(2) */
#define M_SQRT1_2l      0.707106781186547524400844362104849039L /* 1/sqrt(2) */
void sincos(double __x, double* __sin, double* __cos);
void sincosf(float __x, float* __sin, float* __cos);
void sincosl(long double __x, long double* __sin, long double* __cos);
#endif

__END_DECLS

#endif
