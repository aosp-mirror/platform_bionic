/*
 * Copyright (C) 2015 The Android Open Source Project
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

#pragma once

#if defined(__aarch64__)
#define VDSO_CLOCK_GETTIME_SYMBOL "__kernel_clock_gettime"
#define VDSO_CLOCK_GETRES_SYMBOL "__kernel_clock_getres"
#define VDSO_GETTIMEOFDAY_SYMBOL "__kernel_gettimeofday"
#else
#define VDSO_CLOCK_GETTIME_SYMBOL "__vdso_clock_gettime"
#define VDSO_CLOCK_GETRES_SYMBOL "__vdso_clock_getres"
#define VDSO_GETTIMEOFDAY_SYMBOL "__vdso_gettimeofday"
#endif
#if defined(__riscv)
#define VDSO_RISCV_HWPROBE_SYMBOL "__vdso_riscv_hwprobe"
#endif
#if defined(__i386__) || defined(__x86_64__)
#define VDSO_TIME_SYMBOL "__vdso_time"
#endif

struct vdso_entry {
  const char* name;
  void* fn;
};

enum {
  VDSO_CLOCK_GETTIME = 0,
  VDSO_CLOCK_GETRES,
  VDSO_GETTIMEOFDAY,
#if defined(VDSO_TIME_SYMBOL)
  VDSO_TIME,
#endif
#if defined(VDSO_RISCV_HWPROBE_SYMBOL)
  VDSO_RISCV_HWPROBE,
#endif
  VDSO_END
};
