/*
 * Copyright (C) 2022 The Android Open Source Project
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

#include <fenv.h>
#include <stdint.h>

const fenv_t __fe_dfl_env = 0;

int fegetenv(fenv_t* envp) {
  __asm__ __volatile__("frcsr %0" : "=r"(*envp));
  return 0;
}

int fesetenv(const fenv_t* envp) {
  fenv_t env;
  fegetenv(&env);
  if (*envp != env) {
    __asm__ __volatile__("fscsr %z0" : : "r"(*envp));
  }
  return 0;
}

int feclearexcept(int excepts) {
  __asm__ __volatile__("csrc fflags, %0" : : "r"(excepts & FE_ALL_EXCEPT));
  return 0;
}

int fegetexceptflag(fexcept_t* flagp, int excepts) {
  *flagp = fetestexcept(excepts & FE_ALL_EXCEPT);
  return 0;
}

int fesetexceptflag(const fexcept_t* flagp, int excepts) {
  feclearexcept((~*flagp) & excepts);
  feraiseexcept(*flagp & excepts);
  return 0;
}

int feraiseexcept(int excepts) {
  __asm__ __volatile__("csrs fflags, %0" : : "r"(excepts));
  return 0;
}

int fetestexcept(int excepts) {
  int flags;
  __asm__ __volatile__("frflags %0" : "=r"(flags));
  return flags & excepts;
}

int fegetround(void) {
  int rm;
  __asm__ __volatile__("frrm %0" : "=r"(rm));
  return rm;
}

int fesetround(int round) {
  if (round < FE_TONEAREST || round > FE_UPWARD) return -1;
  __asm__ __volatile__("fsrm %z0" : : "r"(round));
  return 0;
}

int feholdexcept(fenv_t* envp) {
  fegetenv(envp);
  feclearexcept(FE_ALL_EXCEPT);
  return 0;
}

int feupdateenv(const fenv_t* envp) {
  int excepts = fetestexcept(FE_ALL_EXCEPT);
  fesetenv(envp);
  feraiseexcept(excepts);
  return 0;
}

int feenableexcept(int mask __unused) {
  return -1;
}

int fedisableexcept(int mask __unused) {
  return 0;
}

int fegetexcept(void) {
  return 0;
}
