/*-
 * Copyright (c) 2004 David Schultz <das@FreeBSD.ORG>
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
 * $FreeBSD: src/lib/msun/mips/fenv.c,v 1.1 2008/04/26 12:20:29 imp Exp $
 */

#include <fenv.h>

/*
 * Hopefully the system ID byte is immutable, so it's valid to use
 * this as a default environment.
 */
const fenv_t __fe_dfl_env = 0;

int fegetenv(fenv_t* __envp) {
  fenv_t _fcsr = 0;
#ifdef  __mips_hard_float
  __asm__ __volatile__("cfc1 %0,$31" : "=r" (_fcsr));
#endif
  *__envp = _fcsr;
  return 0;
}

int fesetenv(const fenv_t* __envp) {
  fenv_t _fcsr = *__envp;
#ifdef  __mips_hard_float
  __asm__ __volatile__("ctc1 %0,$31" : : "r" (_fcsr));
#endif
  return 0;
}

int feclearexcept(int __excepts) {
  fexcept_t __fcsr;
  fegetenv(&__fcsr);
  __excepts &= FE_ALL_EXCEPT;
  __fcsr &= ~(__excepts | (__excepts << _FCSR_CAUSE_SHIFT));
  fesetenv(&__fcsr);
  return 0;
}

int fegetexceptflag(fexcept_t* __flagp, int __excepts) {
  fexcept_t __fcsr;
  fegetenv(&__fcsr);
  *__flagp = __fcsr & __excepts & FE_ALL_EXCEPT;
  return 0;
}

int fesetexceptflag(const fexcept_t* __flagp, int __excepts) {
  fexcept_t __fcsr;
  fegetenv(&__fcsr);
  /* Ensure that flags are all legal */
  __excepts &= FE_ALL_EXCEPT;
  __fcsr &= ~__excepts;
  __fcsr |= *__flagp & __excepts;
  fesetenv(&__fcsr);
  return 0;
}

int feraiseexcept(int __excepts) {
  fexcept_t __fcsr;
  fegetenv(&__fcsr);
  /* Ensure that flags are all legal */
  __excepts &= FE_ALL_EXCEPT;
  /* Cause bit needs to be set as well for generating the exception*/
  __fcsr |= __excepts | (__excepts << _FCSR_CAUSE_SHIFT);
  fesetenv(&__fcsr);
  return 0;
}

int fetestexcept(int __excepts) {
  fexcept_t __FCSR;
  fegetenv(&__FCSR);
  return (__FCSR & __excepts & FE_ALL_EXCEPT);
}

int fegetround(void) {
  fenv_t _fcsr;
  fegetenv(&_fcsr);
  return (_fcsr & _FCSR_RMASK);
}

int fesetround(int __round) {
  fenv_t _fcsr;
  fegetenv(&_fcsr);
  _fcsr &= ~_FCSR_RMASK;
  _fcsr |= (__round & _FCSR_RMASK ) ;
  fesetenv(&_fcsr);
  return 0;
}

int feholdexcept(fenv_t* __envp) {
  fenv_t __env;
  fegetenv(&__env);
  *__envp = __env;
  __env &= ~(FE_ALL_EXCEPT | _FCSR_ENABLE_MASK);
  fesetenv(&__env);
  return 0;
}

int feupdateenv(const fenv_t* __envp) {
  fexcept_t __fcsr;
  fegetenv(&__fcsr);
  fesetenv(__envp);
  feraiseexcept(__fcsr & FE_ALL_EXCEPT);
  return 0;
}

int feenableexcept(int __mask) {
  fenv_t __old_fcsr, __new_fcsr;
  fegetenv(&__old_fcsr);
  __new_fcsr = __old_fcsr | (__mask & FE_ALL_EXCEPT) << _ENABLE_SHIFT;
  fesetenv(&__new_fcsr);
  return ((__old_fcsr >> _ENABLE_SHIFT) & FE_ALL_EXCEPT);
}

int fedisableexcept(int __mask) {
  fenv_t __old_fcsr, __new_fcsr;
  fegetenv(&__old_fcsr);
  __new_fcsr = __old_fcsr & ~((__mask & FE_ALL_EXCEPT) << _ENABLE_SHIFT);
  fesetenv(&__new_fcsr);
  return ((__old_fcsr >> _ENABLE_SHIFT) & FE_ALL_EXCEPT);
}

int fegetexcept(void) {
  fenv_t __fcsr;
  fegetenv(&__fcsr);
  return ((__fcsr & _FCSR_ENABLE_MASK) >> _ENABLE_SHIFT);
}
