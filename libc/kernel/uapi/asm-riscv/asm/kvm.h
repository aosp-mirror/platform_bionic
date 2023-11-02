/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __LINUX_KVM_RISCV_H
#define __LINUX_KVM_RISCV_H
#ifndef __ASSEMBLY__
#include <linux/types.h>
#include <asm/bitsperlong.h>
#include <asm/ptrace.h>
#define __KVM_HAVE_IRQ_LINE
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
  unsigned long zicboz_block_size;
  unsigned long satp_mode;
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
struct kvm_riscv_aia_csr {
  unsigned long siselect;
  unsigned long iprio1;
  unsigned long iprio2;
  unsigned long sieh;
  unsigned long siph;
  unsigned long iprio1h;
  unsigned long iprio2h;
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
  KVM_RISCV_ISA_EXT_ZICBOZ,
  KVM_RISCV_ISA_EXT_ZBB,
  KVM_RISCV_ISA_EXT_SSAIA,
  KVM_RISCV_ISA_EXT_V,
  KVM_RISCV_ISA_EXT_SVNAPOT,
  KVM_RISCV_ISA_EXT_ZBA,
  KVM_RISCV_ISA_EXT_ZBS,
  KVM_RISCV_ISA_EXT_ZICNTR,
  KVM_RISCV_ISA_EXT_ZICSR,
  KVM_RISCV_ISA_EXT_ZIFENCEI,
  KVM_RISCV_ISA_EXT_ZIHPM,
  KVM_RISCV_ISA_EXT_MAX,
};
enum KVM_RISCV_SBI_EXT_ID {
  KVM_RISCV_SBI_EXT_V01 = 0,
  KVM_RISCV_SBI_EXT_TIME,
  KVM_RISCV_SBI_EXT_IPI,
  KVM_RISCV_SBI_EXT_RFENCE,
  KVM_RISCV_SBI_EXT_SRST,
  KVM_RISCV_SBI_EXT_HSM,
  KVM_RISCV_SBI_EXT_PMU,
  KVM_RISCV_SBI_EXT_EXPERIMENTAL,
  KVM_RISCV_SBI_EXT_VENDOR,
  KVM_RISCV_SBI_EXT_MAX,
};
#define KVM_RISCV_TIMER_STATE_OFF 0
#define KVM_RISCV_TIMER_STATE_ON 1
#define KVM_REG_SIZE(id) (1U << (((id) & KVM_REG_SIZE_MASK) >> KVM_REG_SIZE_SHIFT))
#define KVM_REG_RISCV_TYPE_MASK 0x00000000FF000000
#define KVM_REG_RISCV_TYPE_SHIFT 24
#define KVM_REG_RISCV_SUBTYPE_MASK 0x0000000000FF0000
#define KVM_REG_RISCV_SUBTYPE_SHIFT 16
#define KVM_REG_RISCV_CONFIG (0x01 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_CONFIG_REG(name) (offsetof(struct kvm_riscv_config, name) / sizeof(unsigned long))
#define KVM_REG_RISCV_CORE (0x02 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_CORE_REG(name) (offsetof(struct kvm_riscv_core, name) / sizeof(unsigned long))
#define KVM_REG_RISCV_CSR (0x03 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_CSR_GENERAL (0x0 << KVM_REG_RISCV_SUBTYPE_SHIFT)
#define KVM_REG_RISCV_CSR_AIA (0x1 << KVM_REG_RISCV_SUBTYPE_SHIFT)
#define KVM_REG_RISCV_CSR_REG(name) (offsetof(struct kvm_riscv_csr, name) / sizeof(unsigned long))
#define KVM_REG_RISCV_CSR_AIA_REG(name) (offsetof(struct kvm_riscv_aia_csr, name) / sizeof(unsigned long))
#define KVM_REG_RISCV_TIMER (0x04 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_TIMER_REG(name) (offsetof(struct kvm_riscv_timer, name) / sizeof(__u64))
#define KVM_REG_RISCV_FP_F (0x05 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_FP_F_REG(name) (offsetof(struct __riscv_f_ext_state, name) / sizeof(__u32))
#define KVM_REG_RISCV_FP_D (0x06 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_FP_D_REG(name) (offsetof(struct __riscv_d_ext_state, name) / sizeof(__u64))
#define KVM_REG_RISCV_ISA_EXT (0x07 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_ISA_SINGLE (0x0 << KVM_REG_RISCV_SUBTYPE_SHIFT)
#define KVM_REG_RISCV_ISA_MULTI_EN (0x1 << KVM_REG_RISCV_SUBTYPE_SHIFT)
#define KVM_REG_RISCV_ISA_MULTI_DIS (0x2 << KVM_REG_RISCV_SUBTYPE_SHIFT)
#define KVM_REG_RISCV_ISA_MULTI_REG(__ext_id) ((__ext_id) / __BITS_PER_LONG)
#define KVM_REG_RISCV_ISA_MULTI_MASK(__ext_id) (1UL << ((__ext_id) % __BITS_PER_LONG))
#define KVM_REG_RISCV_ISA_MULTI_REG_LAST KVM_REG_RISCV_ISA_MULTI_REG(KVM_RISCV_ISA_EXT_MAX - 1)
#define KVM_REG_RISCV_SBI_EXT (0x08 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_SBI_SINGLE (0x0 << KVM_REG_RISCV_SUBTYPE_SHIFT)
#define KVM_REG_RISCV_SBI_MULTI_EN (0x1 << KVM_REG_RISCV_SUBTYPE_SHIFT)
#define KVM_REG_RISCV_SBI_MULTI_DIS (0x2 << KVM_REG_RISCV_SUBTYPE_SHIFT)
#define KVM_REG_RISCV_SBI_MULTI_REG(__ext_id) ((__ext_id) / __BITS_PER_LONG)
#define KVM_REG_RISCV_SBI_MULTI_MASK(__ext_id) (1UL << ((__ext_id) % __BITS_PER_LONG))
#define KVM_REG_RISCV_SBI_MULTI_REG_LAST KVM_REG_RISCV_SBI_MULTI_REG(KVM_RISCV_SBI_EXT_MAX - 1)
#define KVM_REG_RISCV_VECTOR (0x09 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_VECTOR_CSR_REG(name) (offsetof(struct __riscv_v_ext_state, name) / sizeof(unsigned long))
#define KVM_REG_RISCV_VECTOR_REG(n) ((n) + sizeof(struct __riscv_v_ext_state) / sizeof(unsigned long))
#define KVM_DEV_RISCV_APLIC_ALIGN 0x1000
#define KVM_DEV_RISCV_APLIC_SIZE 0x4000
#define KVM_DEV_RISCV_APLIC_MAX_HARTS 0x4000
#define KVM_DEV_RISCV_IMSIC_ALIGN 0x1000
#define KVM_DEV_RISCV_IMSIC_SIZE 0x1000
#define KVM_DEV_RISCV_AIA_GRP_CONFIG 0
#define KVM_DEV_RISCV_AIA_CONFIG_MODE 0
#define KVM_DEV_RISCV_AIA_CONFIG_IDS 1
#define KVM_DEV_RISCV_AIA_CONFIG_SRCS 2
#define KVM_DEV_RISCV_AIA_CONFIG_GROUP_BITS 3
#define KVM_DEV_RISCV_AIA_CONFIG_GROUP_SHIFT 4
#define KVM_DEV_RISCV_AIA_CONFIG_HART_BITS 5
#define KVM_DEV_RISCV_AIA_CONFIG_GUEST_BITS 6
#define KVM_DEV_RISCV_AIA_MODE_EMUL 0
#define KVM_DEV_RISCV_AIA_MODE_HWACCEL 1
#define KVM_DEV_RISCV_AIA_MODE_AUTO 2
#define KVM_DEV_RISCV_AIA_IDS_MIN 63
#define KVM_DEV_RISCV_AIA_IDS_MAX 2048
#define KVM_DEV_RISCV_AIA_SRCS_MAX 1024
#define KVM_DEV_RISCV_AIA_GROUP_BITS_MAX 8
#define KVM_DEV_RISCV_AIA_GROUP_SHIFT_MIN 24
#define KVM_DEV_RISCV_AIA_GROUP_SHIFT_MAX 56
#define KVM_DEV_RISCV_AIA_HART_BITS_MAX 16
#define KVM_DEV_RISCV_AIA_GUEST_BITS_MAX 8
#define KVM_DEV_RISCV_AIA_GRP_ADDR 1
#define KVM_DEV_RISCV_AIA_ADDR_APLIC 0
#define KVM_DEV_RISCV_AIA_ADDR_IMSIC(__vcpu) (1 + (__vcpu))
#define KVM_DEV_RISCV_AIA_ADDR_MAX (1 + KVM_DEV_RISCV_APLIC_MAX_HARTS)
#define KVM_DEV_RISCV_AIA_GRP_CTRL 2
#define KVM_DEV_RISCV_AIA_CTRL_INIT 0
#define KVM_DEV_RISCV_AIA_GRP_APLIC 3
#define KVM_DEV_RISCV_AIA_GRP_IMSIC 4
#define KVM_DEV_RISCV_AIA_IMSIC_ISEL_BITS 12
#define KVM_DEV_RISCV_AIA_IMSIC_ISEL_MASK ((1U << KVM_DEV_RISCV_AIA_IMSIC_ISEL_BITS) - 1)
#define KVM_DEV_RISCV_AIA_IMSIC_MKATTR(__vcpu,__isel) (((__vcpu) << KVM_DEV_RISCV_AIA_IMSIC_ISEL_BITS) | ((__isel) & KVM_DEV_RISCV_AIA_IMSIC_ISEL_MASK))
#define KVM_DEV_RISCV_AIA_IMSIC_GET_ISEL(__attr) ((__attr) & KVM_DEV_RISCV_AIA_IMSIC_ISEL_MASK)
#define KVM_DEV_RISCV_AIA_IMSIC_GET_VCPU(__attr) ((__attr) >> KVM_DEV_RISCV_AIA_IMSIC_ISEL_BITS)
#define KVM_NR_IRQCHIPS 1
#endif
#endif
