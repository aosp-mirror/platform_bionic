/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
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
