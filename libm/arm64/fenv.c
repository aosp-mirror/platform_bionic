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
 * $FreeBSD: libm/aarch64/fenv.c $
 */

#include <stdint.h>
#include <fenv.h>

#define FPCR_EXCEPT_SHIFT 8
#define FPCR_EXCEPT_MASK  (FE_ALL_EXCEPT << FPCR_EXCEPT_SHIFT)

#define FPCR_RMODE_SHIFT 22

const fenv_t __fe_dfl_env = { 0 /* control */, 0 /* status */};

typedef __uint32_t fpu_control_t;   // FPCR, Floating-point Control Register.
typedef __uint32_t fpu_status_t;    // FPSR, Floating-point Status Register.

#define __get(REGISTER, __value) { \
  uint64_t __value64; \
  __asm__ __volatile__("mrs %0," REGISTER : "=r" (__value64)); \
  __value = (__uint32_t) __value64; \
}
#define __get_fpcr(__fpcr) __get("fpcr", __fpcr)
#define __get_fpsr(__fpsr) __get("fpsr", __fpsr)

#define __set(REGISTER, __value) { \
  uint64_t __value64 = __value; \
  __asm__ __volatile__("msr " REGISTER ",%0" : : "ri" (__value64)); \
}
#define __set_fpcr(__fpcr) __set("fpcr", __fpcr)
#define __set_fpsr(__fpsr) __set("fpsr", __fpsr)

int fegetenv(fenv_t* envp) {
  __get_fpcr(envp->__control);
  __get_fpsr(envp->__status);
  return 0;
}

int fesetenv(const fenv_t* envp) {
  fpu_control_t fpcr;

  __get_fpcr(fpcr);
  if (envp->__control != fpcr) {
    __set_fpcr(envp->__control);
  }
  __set_fpsr(envp->__status);
  return 0;
}

int feclearexcept(int excepts) {
  fpu_status_t fpsr;

  excepts &= FE_ALL_EXCEPT;
  __get_fpsr(fpsr);
  fpsr &= ~excepts;
  __set_fpsr(fpsr);
  return 0;
}

int fegetexceptflag(fexcept_t* flagp, int excepts) {
  fpu_status_t fpsr;

  excepts &= FE_ALL_EXCEPT;
  __get_fpsr(fpsr);
  *flagp = fpsr & excepts;
  return 0;
}

int fesetexceptflag(const fexcept_t* flagp, int excepts) {
  fpu_status_t fpsr;

  excepts &= FE_ALL_EXCEPT;
  __get_fpsr(fpsr);
  fpsr &= ~excepts;
  fpsr |= *flagp & excepts;
  __set_fpsr(fpsr);
  return 0;
}

int feraiseexcept(int excepts) {
  fexcept_t ex = excepts;

  fesetexceptflag(&ex, excepts);
  return 0;
}

int fetestexcept(int excepts) {
  fpu_status_t fpsr;

  excepts &= FE_ALL_EXCEPT;
  __get_fpsr(fpsr);
  return (fpsr & excepts);
}

int fegetround(void) {
  fpu_control_t fpcr;

  __get_fpcr(fpcr);
  return ((fpcr >> FPCR_RMODE_SHIFT) & FE_TOWARDZERO);
}

int fesetround(int round) {
  fpu_control_t fpcr, new_fpcr;

  round &= FE_TOWARDZERO;
  __get_fpcr(fpcr);
  new_fpcr = fpcr & ~(FE_TOWARDZERO << FPCR_RMODE_SHIFT);
  new_fpcr |= (round << FPCR_RMODE_SHIFT);
  if (new_fpcr != fpcr) {
    __set_fpcr(new_fpcr);
  }
  return 0;
}

int feholdexcept(fenv_t* envp) {
  fenv_t env;
  fpu_status_t fpsr;
  fpu_control_t fpcr, new_fpcr;

  __get_fpsr(fpsr);
  __get_fpcr(fpcr);
  env.__status = fpsr;
  env.__control = fpcr;
  *envp = env;

  // Set exceptions to untrapped.
  new_fpcr = fpcr & ~(FE_ALL_EXCEPT << FPCR_EXCEPT_SHIFT);
  if (new_fpcr != fpcr) {
    __set_fpcr(new_fpcr);
  }

  // Clear all exceptions.
  fpsr &= ~FE_ALL_EXCEPT;
  __set_fpsr(fpsr);
  return 0;
}

int feupdateenv(const fenv_t* envp) {
  fpu_status_t fpsr;
  fpu_control_t fpcr;

  // Set FPU Control register.
  __get_fpcr(fpcr);
  if (envp->__control != fpcr) {
    __set_fpcr(envp->__control);
  }

  // Set FPU Status register to status | currently raised exceptions.
  __get_fpsr(fpsr);
  fpsr = envp->__status | (fpsr & FE_ALL_EXCEPT);
  __set_fpsr(fpsr);
  return 0;
}

int feenableexcept(int mask) {
  fpu_control_t old_fpcr, new_fpcr;

  __get_fpcr(old_fpcr);
  new_fpcr = old_fpcr | ((mask & FE_ALL_EXCEPT) << FPCR_EXCEPT_SHIFT);
  if (new_fpcr != old_fpcr) {
    __set_fpcr(new_fpcr);
  }
  return ((old_fpcr >> FPCR_EXCEPT_SHIFT) & FE_ALL_EXCEPT);
}

int fedisableexcept(int mask) {
  fpu_control_t old_fpcr, new_fpcr;

  __get_fpcr(old_fpcr);
  new_fpcr = old_fpcr & ~((mask & FE_ALL_EXCEPT) << FPCR_EXCEPT_SHIFT);
  if (new_fpcr != old_fpcr) {
    __set_fpcr(new_fpcr);
  }
  return ((old_fpcr >> FPCR_EXCEPT_SHIFT) & FE_ALL_EXCEPT);
}

int fegetexcept(void) {
  fpu_control_t fpcr;

  __get_fpcr(fpcr);
  return ((fpcr & FPCR_EXCEPT_MASK) >> FPCR_EXCEPT_SHIFT);
}
