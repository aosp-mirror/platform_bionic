/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef _UAPI_ASM_RISCV_SIGCONTEXT_H
#define _UAPI_ASM_RISCV_SIGCONTEXT_H
#include <asm/ptrace.h>
#define RISCV_V_MAGIC 0x53465457
#define END_MAGIC 0x0
#define END_HDR_SIZE 0x0
#ifndef __ASSEMBLY__
struct __sc_riscv_v_state {
  struct __riscv_v_ext_state v_state;
} __attribute__((aligned(16)));
struct sigcontext {
  struct user_regs_struct sc_regs;
  union {
    union __riscv_fp_state sc_fpregs;
    struct __riscv_extra_ext_header sc_extdesc;
  };
};
#endif
#endif
