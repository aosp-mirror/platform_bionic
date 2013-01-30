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
#ifndef __ASM_PROCINFO_H
#define __ASM_PROCINFO_H
#ifndef __ASSEMBLY__
struct cpu_tlb_fns;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct cpu_user_fns;
struct cpu_cache_fns;
struct processor;
struct proc_info_list {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int cpu_val;
 unsigned int cpu_mask;
 unsigned long __cpu_mm_mmu_flags;
 unsigned long __cpu_io_mmu_flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long __cpu_flush;
 const char *arch_name;
 const char *elf_name;
 unsigned int elf_hwcap;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const char *cpu_name;
 struct processor *proc;
 struct cpu_tlb_fns *tlb;
 struct cpu_user_fns *user;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct cpu_cache_fns *cache;
};
#endif
#define HWCAP_SWP 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define HWCAP_HALF 2
#define HWCAP_THUMB 4
#define HWCAP_26BIT 8
#define HWCAP_FAST_MULT 16
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define HWCAP_FPA 32
#define HWCAP_VFP 64
#define HWCAP_EDSP 128
#define HWCAP_JAVA 256
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define HWCAP_IWMMXT 512
#endif
