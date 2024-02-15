/*	$NetBSD: cdefs.h,v 1.58 2004/12/11 05:59:00 christos Exp $	*/

/*
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Berkeley Software Design, Inc.
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
 *	@(#)cdefs.h	8.8 (Berkeley) 1/9/95
 */

#pragma once

/**
 * `__BIONIC__` is always defined if you're building with bionic. See
 * https://android.googlesource.com/platform/bionic/+/main/docs/defines.md.
 */
#define __BIONIC__ 1

#if defined(__cplusplus)
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif

#define __strong_alias(alias, sym) \
    __asm__(".global " #alias "\n" \
            #alias " = " #sym);

#if defined(__cplusplus)
#define __BIONIC_CAST(_k,_t,_v) (_k<_t>(_v))
#else
#define __BIONIC_CAST(_k,_t,_v) ((_t) (_v))
#endif

#define __BIONIC_ALIGN(__value, __alignment) (((__value) + (__alignment)-1) & ~((__alignment)-1))

/*
 * The nullness constraints of this parameter or return value are
 * quite complex. This is used to highlight spots where developers
 * are encouraged to read relevant manuals or code to understand
 * the full picture of nullness for this pointer.
 */
#define __BIONIC_COMPLICATED_NULLNESS _Null_unspecified

/*
 * The __CONCAT macro is used to concatenate parts of symbol names, e.g.
 * with "#define OLD(foo) __CONCAT(old,foo)", OLD(foo) produces oldfoo.
 * The __CONCAT macro is a bit tricky -- make sure you don't put spaces
 * in between its arguments.  __CONCAT can also concatenate double-quoted
 * strings produced by the __STRING macro, but this only works with ANSI C.
 */

#define	__P(protos)	protos		/* full-blown ANSI C */

#define	__CONCAT1(x,y)	x ## y
#define	__CONCAT(x,y)	__CONCAT1(x,y)
#define	___CONCAT(x,y)	__CONCAT(x,y)

#define	__STRING(x)	#x
#define	___STRING(x)	__STRING(x)

#if defined(__cplusplus)
#define	__inline	inline		/* convert to C++ keyword */
#endif /* !__cplusplus */

#define __always_inline __attribute__((__always_inline__))
#define __attribute_const__ __attribute__((__const__))
#define __attribute_pure__ __attribute__((__pure__))
#define __dead __attribute__((__noreturn__))
#define __noreturn __attribute__((__noreturn__))
#define __mallocfunc  __attribute__((__malloc__))
#define __packed __attribute__((__packed__))
#define __returns_twice __attribute__((__returns_twice__))
#define __unused __attribute__((__unused__))
#define __used __attribute__((__used__))

#define __printflike(x, y) __attribute__((__format__(printf, x, y)))
#define __scanflike(x, y) __attribute__((__format__(scanf, x, y)))
#define __strftimelike(x) __attribute__((__format__(strftime, x, 0)))

/*
 * GNU C version 2.96 added explicit branch prediction so that
 * the CPU back-end can hint the processor and also so that
 * code blocks can be reordered such that the predicted path
 * sees a more linear flow, thus improving cache behavior, etc.
 *
 * The following two macros provide us with a way to use this
 * compiler feature.  Use __predict_true() if you expect the expression
 * to evaluate to true, and __predict_false() if you expect the
 * expression to evaluate to false.
 *
 * A few notes about usage:
 *
 *	* Generally, __predict_false() error condition checks (unless
 *	  you have some _strong_ reason to do otherwise, in which case
 *	  document it), and/or __predict_true() `no-error' condition
 *	  checks, assuming you want to optimize for the no-error case.
 *
 *	* Other than that, if you don't know the likelihood of a test
 *	  succeeding from empirical or other `hard' evidence, don't
 *	  make predictions.
 *
 *	* These are meant to be used in places that are run `a lot'.
 *	  It is wasteful to make predictions in code that is run
 *	  seldomly (e.g. at subsystem initialization time) as the
 *	  basic block reordering that this affects can often generate
 *	  larger code.
 */
#define	__predict_true(exp)	__builtin_expect((exp) != 0, 1)
#define	__predict_false(exp)	__builtin_expect((exp) != 0, 0)

#define __wur __attribute__((__warn_unused_result__))

#define __errorattr(msg) __attribute__((unavailable(msg)))
#define __warnattr(msg) __attribute__((deprecated(msg)))
#define __warnattr_real(msg) __attribute__((deprecated(msg)))
#define __enable_if(cond, msg) __attribute__((enable_if(cond, msg)))
#define __clang_error_if(cond, msg) __attribute__((diagnose_if(cond, msg, "error")))
#define __clang_warning_if(cond, msg) __attribute__((diagnose_if(cond, msg, "warning")))

#if defined(ANDROID_STRICT)
/*
 * For things that are sketchy, but not necessarily an error. FIXME: Enable
 * this.
 */
#  define __warnattr_strict(msg) /* __warnattr(msg) */
#else
#  define __warnattr_strict(msg)
#endif

/*
 * Some BSD source needs these macros.
 * Originally they embedded the rcs versions of each source file
 * in the generated binary. We strip strings during build anyway,.
 */
#define __IDSTRING(_prefix,_s) /* nothing */
#define __COPYRIGHT(_s) /* nothing */
#define __FBSDID(_s) /* nothing */
#define __RCSID(_s) /* nothing */
#define __SCCSID(_s) /* nothing */

/*
 * With bionic, you always get all C and POSIX API.
 *
 * If you want BSD and/or GNU extensions, _BSD_SOURCE and/or _GNU_SOURCE are
 * expected to be defined by callers before *any* standard header file is
 * included.
 *
 * In our header files we test against __USE_BSD and __USE_GNU.
 */
#if defined(_GNU_SOURCE)
#  define __USE_BSD 1
#  define __USE_GNU 1
#endif

#if defined(_BSD_SOURCE)
#  define __USE_BSD 1
#endif

/*
 * _FILE_OFFSET_BITS 64 support.
 * See https://android.googlesource.com/platform/bionic/+/main/docs/32-bit-abi.md
 */
#if !defined(__LP64__) && defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64
#  define __USE_FILE_OFFSET64 1
/*
 * Note that __RENAME_IF_FILE_OFFSET64 is only valid if the off_t and off64_t
 * functions were both added at the same API level because if you use this,
 * you only have one declaration to attach __INTRODUCED_IN to.
 */
#  define __RENAME_IF_FILE_OFFSET64(func) __RENAME(func)
#else
#  define __RENAME_IF_FILE_OFFSET64(func)
#endif

/* glibc compatibility. */
#if defined(__LP64__)
#define __WORDSIZE 64
#else
#define __WORDSIZE 32
#endif

/*
 * When _FORTIFY_SOURCE is defined, automatic bounds checking is
 * added to commonly used libc functions. If a buffer overrun is
 * detected, the program is safely aborted.
 *
 * https://android-developers.googleblog.com/2017/04/fortify-in-android.html
 */

#define __BIONIC_FORTIFY_UNKNOWN_SIZE ((size_t) -1)

#if defined(_FORTIFY_SOURCE) && _FORTIFY_SOURCE > 0
/* FORTIFY can interfere with pattern-matching of clang-tidy/the static analyzer.  */
#  if !defined(__clang_analyzer__)
#    define __BIONIC_FORTIFY 1
/* ASAN has interceptors that FORTIFY's _chk functions can break.  */
#    if __has_feature(address_sanitizer)
#      define __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED 0
#    else
#      define __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED 1
#    endif
#  endif
#endif

// As we move some FORTIFY checks to be always on, __bos needs to be
// always available.
#if defined(__BIONIC_FORTIFY)
#  if _FORTIFY_SOURCE == 2
#    define __bos_level 1
#  else
#    define __bos_level 0
#  endif
#else
#  define __bos_level 0
#endif

#define __bosn(s, n) __builtin_object_size((s), (n))
#define __bos(s) __bosn((s), __bos_level)

#if defined(__BIONIC_FORTIFY)
#  define __bos0(s) __bosn((s), 0)
#  define __pass_object_size_n(n) __attribute__((pass_object_size(n)))
/*
 * FORTIFY'ed functions all have either enable_if or pass_object_size, which
 * makes taking their address impossible. Saying (&read)(foo, bar, baz); will
 * therefore call the unFORTIFYed version of read.
 */
#  define __call_bypassing_fortify(fn) (&fn)
/*
 * Because clang-FORTIFY uses overloads, we can't mark functions as `extern inline` without making
 * them available externally. FORTIFY'ed functions try to be as close to possible as 'invisible';
 * having stack protectors detracts from that (b/182948263).
 */
#  define __BIONIC_FORTIFY_INLINE static __inline__ __attribute__((no_stack_protector)) \
      __always_inline __VERSIONER_FORTIFY_INLINE
/*
 * We should use __BIONIC_FORTIFY_VARIADIC instead of __BIONIC_FORTIFY_INLINE
 * for variadic functions because compilers cannot inline them.
 * The __always_inline attribute is useless, misleading, and could trigger
 * clang compiler bug to incorrectly inline variadic functions.
 */
#  define __BIONIC_FORTIFY_VARIADIC static __inline__
/* Error functions don't have bodies, so they can just be static. */
#  define __BIONIC_ERROR_FUNCTION_VISIBILITY static __attribute__((unused))
#else
/* Further increase sharing for some inline functions */
#  define __pass_object_size_n(n)
#endif
#define __pass_object_size __pass_object_size_n(__bos_level)
#define __pass_object_size0 __pass_object_size_n(0)

/* Intended for use in unevaluated contexts, e.g. diagnose_if conditions. */
#define __bos_unevaluated_lt(bos_val, val) \
  ((bos_val) != __BIONIC_FORTIFY_UNKNOWN_SIZE && (bos_val) < (val))

#define __bos_unevaluated_le(bos_val, val) \
  ((bos_val) != __BIONIC_FORTIFY_UNKNOWN_SIZE && (bos_val) <= (val))

/* Intended for use in evaluated contexts. */
#define __bos_dynamic_check_impl_and(bos_val, op, index, cond) \
  ((bos_val) == __BIONIC_FORTIFY_UNKNOWN_SIZE ||                 \
   (__builtin_constant_p(index) && bos_val op index && (cond)))

#define __bos_dynamic_check_impl(bos_val, op, index) \
  __bos_dynamic_check_impl_and(bos_val, op, index, 1)

#define __bos_trivially_ge(bos_val, index) __bos_dynamic_check_impl((bos_val), >=, (index))
#define __bos_trivially_gt(bos_val, index) __bos_dynamic_check_impl((bos_val), >, (index))

#if defined(__BIONIC_FORTIFY) || defined(__BIONIC_DECLARE_FORTIFY_HELPERS)
#  define __BIONIC_INCLUDE_FORTIFY_HEADERS 1
#endif

#define __overloadable __attribute__((overloadable))

#define __diagnose_as_builtin(...) __attribute__((diagnose_as_builtin(__VA_ARGS__)))

/* Used to tag non-static symbols that are private and never exposed by the shared library. */
#define __LIBC_HIDDEN__ __attribute__((visibility("hidden")))

/*
 * Used to tag symbols that should be hidden for 64-bit,
 * but visible to preserve binary compatibility for LP32.
 */
#ifdef __LP64__
#define __LIBC32_LEGACY_PUBLIC__ __attribute__((visibility("hidden")))
#else
#define __LIBC32_LEGACY_PUBLIC__ __attribute__((visibility("default")))
#endif

/* Used to rename functions so that the compiler emits a call to 'x' rather than the function this was applied to. */
#define __RENAME(x) __asm__(#x)

#if __has_builtin(__builtin_umul_overflow) || __GNUC__ >= 5
#if defined(__LP64__)
#define __size_mul_overflow(a, b, result) __builtin_umull_overflow(a, b, result)
#else
#define __size_mul_overflow(a, b, result) __builtin_umul_overflow(a, b, result)
#endif
#else
extern __inline__ __always_inline __attribute__((gnu_inline))
int __size_mul_overflow(__SIZE_TYPE__ a, __SIZE_TYPE__ b, __SIZE_TYPE__ *result) {
    *result = a * b;
    static const __SIZE_TYPE__ mul_no_overflow = 1UL << (sizeof(__SIZE_TYPE__) * 4);
    return (a >= mul_no_overflow || b >= mul_no_overflow) && a > 0 && (__SIZE_TYPE__)-1 / a < b;
}
#endif

/*
 * Used when we need to check for overflow when multiplying x and y. This
 * should only be used where __size_mul_overflow can not work, because it makes
 * assumptions that __size_mul_overflow doesn't (x and y are positive, ...),
 * *and* doesn't make use of compiler intrinsics, so it's probably slower than
 * __size_mul_overflow.
 */
#define __unsafe_check_mul_overflow(x, y) ((__SIZE_TYPE__)-1 / (x) < (y))

#include <android/versioning.h>
#include <android/api-level.h>
#if __has_include(<android/ndk-version.h>)
#include <android/ndk-version.h>
#endif
