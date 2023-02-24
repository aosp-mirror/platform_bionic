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
#ifndef __LINUX_KVM_RISCV_H
#define __LINUX_KVM_RISCV_H
#ifndef __ASSEMBLY__
#include <linux/types.h>
#include <asm/ptrace.h>
#define __KVM_HAVE_READONLY_MEM
#define KVM_COALESCED_MMIO_PAGE_OFFSET 1
#define KVM_INTERRUPT_SET - 1U
#define KVM_INTERRUPT_UNSET - 2U
struct kvm_regs {
};
struct kvm_fpu {
};
struct kvm_debug_exit_arch {
};
struct kvm_guest_debug_arch {
};
struct kvm_sync_regs {
};
struct kvm_sregs {
};
struct kvm_riscv_config {
  unsigned long isa;
  unsigned long zicbom_block_size;
  unsigned long mvendorid;
  unsigned long marchid;
  unsigned long mimpid;
};
struct kvm_riscv_core {
  struct user_regs_struct regs;
  unsigned long mode;
};
#define KVM_RISCV_MODE_S 1
#define KVM_RISCV_MODE_U 0
struct kvm_riscv_csr {
  unsigned long sstatus;
  unsigned long sie;
  unsigned long stvec;
  unsigned long sscratch;
  unsigned long sepc;
  unsigned long scause;
  unsigned long stval;
  unsigned long sip;
  unsigned long satp;
  unsigned long scounteren;
};
struct kvm_riscv_timer {
  __u64 frequency;
  __u64 time;
  __u64 compare;
  __u64 state;
};
enum KVM_RISCV_ISA_EXT_ID {
  KVM_RISCV_ISA_EXT_A = 0,
  KVM_RISCV_ISA_EXT_C,
  KVM_RISCV_ISA_EXT_D,
  KVM_RISCV_ISA_EXT_F,
  KVM_RISCV_ISA_EXT_H,
  KVM_RISCV_ISA_EXT_I,
  KVM_RISCV_ISA_EXT_M,
  KVM_RISCV_ISA_EXT_SVPBMT,
  KVM_RISCV_ISA_EXT_SSTC,
  KVM_RISCV_ISA_EXT_SVINVAL,
  KVM_RISCV_ISA_EXT_ZIHINTPAUSE,
  KVM_RISCV_ISA_EXT_ZICBOM,
  KVM_RISCV_ISA_EXT_MAX,
};
#define KVM_RISCV_TIMER_STATE_OFF 0
#define KVM_RISCV_TIMER_STATE_ON 1
#define KVM_REG_SIZE(id) (1U << (((id) & KVM_REG_SIZE_MASK) >> KVM_REG_SIZE_SHIFT))
#define KVM_REG_RISCV_TYPE_MASK 0x00000000FF000000
#define KVM_REG_RISCV_TYPE_SHIFT 24
#define KVM_REG_RISCV_CONFIG (0x01 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_CONFIG_REG(name) (offsetof(struct kvm_riscv_config, name) / sizeof(unsigned long))
#define KVM_REG_RISCV_CORE (0x02 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_CORE_REG(name) (offsetof(struct kvm_riscv_core, name) / sizeof(unsigned long))
#define KVM_REG_RISCV_CSR (0x03 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_CSR_REG(name) (offsetof(struct kvm_riscv_csr, name) / sizeof(unsigned long))
#define KVM_REG_RISCV_TIMER (0x04 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_TIMER_REG(name) (offsetof(struct kvm_riscv_timer, name) / sizeof(__u64))
#define KVM_REG_RISCV_FP_F (0x05 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_FP_F_REG(name) (offsetof(struct __riscv_f_ext_state, name) / sizeof(__u32))
#define KVM_REG_RISCV_FP_D (0x06 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_FP_D_REG(name) (offsetof(struct __riscv_d_ext_state, name) / sizeof(__u64))
#define KVM_REG_RISCV_ISA_EXT (0x07 << KVM_REG_RISCV_TYPE_SHIFT)
#endif
#endif
