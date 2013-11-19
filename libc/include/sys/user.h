/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef _SYS_USER_H_
#define _SYS_USER_H_

#include <sys/cdefs.h>

__BEGIN_DECLS

#if __i386__

struct user_i387_struct {
  long cwd;
  long swd;
  long twd;
  long fip;
  long fcs;
  long foo;
  long fos;
  long st_space[20];
};
struct user_fxsr_struct {
  unsigned short cwd;
  unsigned short swd;
  unsigned short twd;
  unsigned short fop;
  long fip;
  long fcs;
  long foo;
  long fos;
  long mxcsr;
  long reserved;
  long st_space[32];
  long xmm_space[32];
  long padding[56];
};
struct user_regs_struct {
  unsigned long bx;
  unsigned long cx;
  unsigned long dx;
  unsigned long si;
  unsigned long di;
  unsigned long bp;
  unsigned long ax;
  unsigned long ds;
  unsigned long es;
  unsigned long fs;
  unsigned long gs;
  unsigned long orig_ax;
  unsigned long ip;
  unsigned long cs;
  unsigned long flags;
  unsigned long sp;
  unsigned long ss;
};
struct user{
  struct user_regs_struct regs;
  int u_fpvalid;
  struct user_i387_struct i387;
  unsigned long int u_tsize;
  unsigned long int u_dsize;
  unsigned long int u_ssize;
  unsigned long start_code;
  unsigned long start_stack;
  long int signal;
  int reserved;
  unsigned long u_ar0;
  struct user_i387_struct *u_fpstate;
  unsigned long magic;
  char u_comm[32];
  int u_debugreg[8];
};

#elif defined(__x86_64__)

struct user_i387_struct {
  unsigned short cwd;
  unsigned short swd;
  unsigned short twd;
  unsigned short fop;
  __u64 rip;
  __u64 rdp;
  __u32 mxcsr;
  __u32 mxcsr_mask;
  __u32 st_space[32];
  __u32 xmm_space[64];
  __u32 padding[24];
};
struct user_regs_struct {
  unsigned long r15;
  unsigned long r14;
  unsigned long r13;
  unsigned long r12;
  unsigned long bp;
  unsigned long bx;
  unsigned long r11;
  unsigned long r10;
  unsigned long r9;
  unsigned long r8;
  unsigned long ax;
  unsigned long cx;
  unsigned long dx;
  unsigned long si;
  unsigned long di;
  unsigned long orig_ax;
  unsigned long ip;
  unsigned long cs;
  unsigned long flags;
  unsigned long sp;
  unsigned long ss;
  unsigned long fs_base;
  unsigned long gs_base;
  unsigned long ds;
  unsigned long es;
  unsigned long fs;
  unsigned long gs;
};
struct user {
  struct user_regs_struct regs;
  int u_fpvalid;
  int pad0;
  struct user_i387_struct i387;
  unsigned long int u_tsize;
  unsigned long int u_dsize;
  unsigned long int u_ssize;
  unsigned long start_code;
  unsigned long start_stack;
  long int signal;
  int reserved;
  int pad1;
  unsigned long u_ar0;
  struct user_i387_struct *u_fpstate;
  unsigned long magic;
  char u_comm[32];
  unsigned long u_debugreg[8];
  unsigned long error_code;
  unsigned long fault_address;
};

#elif defined(__mips__)

struct user {
  unsigned long regs[180 / sizeof(unsigned long) + 64];
  size_t u_tsize;
  size_t u_dsize;
  size_t u_ssize;
  unsigned long start_code;
  unsigned long start_data;
  unsigned long start_stack;
  long int signal;
  unsigned long u_ar0;
  unsigned long magic;
  char u_comm[32];
};

#else

/* arm and aarch64 have uapi user.h headers. */
#include <asm/user.h>

#endif

__END_DECLS

#endif  /* _SYS_USER_H_ */
