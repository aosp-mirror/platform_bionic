/*
 * Copyright (C) 2008 The Android Open Source Project
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

#ifndef _STDLIB_H
#define _STDLIB_H

#include <alloca.h>
#include <bits/wait.h>
#include <malloc.h>
#include <stddef.h>
#include <sys/cdefs.h>
#include <xlocale.h>

__BEGIN_DECLS

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

__noreturn void abort(void) __attribute__((__nomerge__));
__noreturn void exit(int __status);
#if __ANDROID_API__ >= 21
__noreturn void _Exit(int __status) __INTRODUCED_IN(21);
#else
__noreturn void _Exit(int) __RENAME(_exit);
#endif

int atexit(void (* _Nonnull __fn)(void));

int at_quick_exit(void (* _Nonnull __fn)(void)) __INTRODUCED_IN(21);
void quick_exit(int __status) __noreturn __INTRODUCED_IN(21);

char* _Nullable getenv(const char* _Nonnull __name);
int putenv(char* _Nonnull __assignment);
int setenv(const char* _Nonnull __name, const char* _Nonnull __value, int __overwrite);
int unsetenv(const char* _Nonnull __name);
int clearenv(void);

char* _Nullable mkdtemp(char* _Nonnull __template);
char* _Nullable mktemp(char* _Nonnull __template) __attribute__((deprecated("mktemp is unsafe, use mkstemp or tmpfile instead")));

int mkostemp64(char* _Nonnull __template, int __flags) __INTRODUCED_IN(23);
int mkostemp(char* _Nonnull __template, int __flags) __INTRODUCED_IN(23);
int mkostemps64(char* _Nonnull __template, int __suffix_length, int __flags) __INTRODUCED_IN(23);
int mkostemps(char* _Nonnull __template, int __suffix_length, int __flags) __INTRODUCED_IN(23);
int mkstemp64(char* _Nonnull __template) __INTRODUCED_IN(21);
int mkstemp(char* _Nonnull __template);
int mkstemps64(char* _Nonnull __template, int __flags) __INTRODUCED_IN(23);
int mkstemps(char* _Nonnull __template, int __flags);

long strtol(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, int __base);
long long strtoll(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, int __base);
unsigned long strtoul(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, int __base);
unsigned long long strtoull(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, int __base);

int posix_memalign(void* _Nullable * _Nullable __memptr, size_t __alignment, size_t __size) __INTRODUCED_IN(17);

void* _Nullable aligned_alloc(size_t __alignment, size_t __size) __INTRODUCED_IN(28);

double strtod(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr);
long double strtold(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr) __RENAME_LDBL(strtod, 3, 21);

unsigned long strtoul_l(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, int __base, locale_t _Nonnull __l) __INTRODUCED_IN(26);

int atoi(const char* _Nonnull __s) __attribute_pure__;
long atol(const char* _Nonnull __s) __attribute_pure__;
long long atoll(const char* _Nonnull __s) __attribute_pure__;

__wur char* _Nullable realpath(const char* _Nonnull __path, char* _Nullable __resolved);
int system(const char* _Nonnull __command);

void* _Nullable bsearch(const void* _Nonnull __key, const void* _Nullable __base, size_t __nmemb, size_t __size, int (* _Nonnull __comparator)(const void* _Nonnull __lhs, const void* _Nonnull __rhs));

void qsort(void* _Nullable __base, size_t __nmemb, size_t __size, int (* _Nonnull __comparator)(const void* _Nonnull __lhs, const void* _Nonnull __rhs));

uint32_t arc4random(void);
uint32_t arc4random_uniform(uint32_t __upper_bound);
void arc4random_buf(void* _Nonnull __buf, size_t __n);

#define RAND_MAX 0x7fffffff

int rand_r(unsigned int* _Nonnull __seed_ptr) __INTRODUCED_IN(21);

double drand48(void);
double erand48(unsigned short __xsubi[_Nonnull 3]);
long jrand48(unsigned short __xsubi[_Nonnull 3]);
void lcong48(unsigned short __param[_Nonnull 7]) __INTRODUCED_IN(23);
long lrand48(void);
long mrand48(void);
long nrand48(unsigned short __xsubi[_Nonnull 3]);
unsigned short* _Nonnull seed48(unsigned short __seed16v[_Nonnull 3]);
void srand48(long __seed);

char* _Nullable initstate(unsigned int __seed, char* _Nonnull __state, size_t __n) __INTRODUCED_IN(21);
char* _Nullable setstate(char* _Nonnull __state) __INTRODUCED_IN(21);

int getpt(void);
int posix_openpt(int __flags) __INTRODUCED_IN(21);
char* _Nullable ptsname(int __fd);
int ptsname_r(int __fd, char* _Nonnull __buf, size_t __n);
int unlockpt(int __fd);

int getsubopt(char* _Nonnull * _Nonnull __option, char* _Nonnull const* _Nonnull __tokens, char* _Nullable * _Nonnull __value_ptr) __INTRODUCED_IN(26);

typedef struct {
  int quot;
  int rem;
} div_t;

div_t div(int __numerator, int __denominator) __attribute_const__;

typedef struct {
  long int quot;
  long int rem;
} ldiv_t;

ldiv_t ldiv(long __numerator, long __denominator) __attribute_const__;

typedef struct {
  long long int quot;
  long long int rem;
} lldiv_t;

lldiv_t lldiv(long long __numerator, long long __denominator) __attribute_const__;

/**
 * [getloadavg(3)](http://man7.org/linux/man-pages/man3/getloadavg.3.html) queries the
 * number of runnable processes averaged over time. The Linux kernel supports averages
 * over the last 1, 5, and 15 minutes.
 *
 * Returns the number of samples written to `__averages` (at most 3), and returns -1 on failure.
 */
int getloadavg(double __averages[_Nonnull], int __n) __INTRODUCED_IN(29);

/* BSD compatibility. */
const char* _Nullable getprogname(void) __INTRODUCED_IN(21);
void setprogname(const char* _Nonnull __name) __INTRODUCED_IN(21);

int mblen(const char* _Nullable __s, size_t __n) __INTRODUCED_IN_NO_GUARD_FOR_NDK(26);
size_t mbstowcs(wchar_t* _Nullable __dst, const char* _Nullable __src, size_t __n) __INTRODUCED_IN_NO_GUARD_FOR_NDK(21);
int mbtowc(wchar_t* _Nullable __wc_ptr, const char*  _Nullable __s, size_t __n) __INTRODUCED_IN_NO_GUARD_FOR_NDK(21);
int wctomb(char* _Nullable __dst, wchar_t __wc) __INTRODUCED_IN_NO_GUARD_FOR_NDK(21);

size_t wcstombs(char* _Nullable __dst, const wchar_t* _Nullable __src, size_t __n) __INTRODUCED_IN_NO_GUARD_FOR_NDK(21);

#if __ANDROID_API__ >= 21
size_t __ctype_get_mb_cur_max(void) __INTRODUCED_IN(21);
#define MB_CUR_MAX __ctype_get_mb_cur_max()
#else
/*
 * Pre-L we didn't have any locale support and so we were always the POSIX
 * locale. POSIX specifies that MB_CUR_MAX for the POSIX locale is 1:
 * http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdlib.h.html
 */
#define MB_CUR_MAX 1
#endif

#if defined(__BIONIC_INCLUDE_FORTIFY_HEADERS)
#include <bits/fortify/stdlib.h>
#endif

#if __ANDROID_API__ >= 19
int abs(int __x) __attribute_const__ __INTRODUCED_IN(19);
long labs(long __x) __attribute_const__ __INTRODUCED_IN(19);
long long llabs(long long __x) __attribute_const__ __INTRODUCED_IN(19);
#else
// Implemented as static inlines before 19.
#endif

#if __ANDROID_API__ >= 21
float strtof(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr) __INTRODUCED_IN(21);
double atof(const char* _Nonnull __s) __attribute_pure__ __INTRODUCED_IN(21);
int rand(void) __INTRODUCED_IN(21);
void srand(unsigned int __seed) __INTRODUCED_IN(21);
long random(void) __INTRODUCED_IN(21);
void srandom(unsigned int __seed) __INTRODUCED_IN(21);
int grantpt(int __fd) __INTRODUCED_IN(21);

long long strtoll_l(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, int __base, locale_t _Nonnull __l) __INTRODUCED_IN(21);
unsigned long long strtoull_l(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, int __base, locale_t _Nonnull __l) __INTRODUCED_IN(21);
long double strtold_l(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, locale_t _Nonnull __l) __INTRODUCED_IN(21);
#else
// Implemented as static inlines before 21.
#endif

#if __ANDROID_API__ >= 26
double strtod_l(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, locale_t _Nonnull __l) __INTRODUCED_IN(26);
float strtof_l(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, locale_t _Nonnull __l) __INTRODUCED_IN(26);
long strtol_l(const char* _Nonnull __s, char* _Nullable * _Nullable __end_ptr, int, locale_t _Nonnull __l) __INTRODUCED_IN(26);
#else
// Implemented as static inlines before 26.
#endif

__END_DECLS

#include <android/legacy_stdlib_inlines.h>

#endif /* _STDLIB_H */
