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
#ifndef __ASMARM_ELF_H
#define __ASMARM_ELF_H
#include <asm/ptrace.h>
#include <asm/user.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef __KERNEL
#include <asm/procinfo.h>
#endif
typedef unsigned long elf_greg_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef unsigned long elf_freg_t[3];
#define EM_ARM 40
#define EF_ARM_APCS26 0x08
#define EF_ARM_SOFT_FLOAT 0x200
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define EF_ARM_EABI_MASK 0xFF000000
#define R_ARM_NONE 0
#define R_ARM_PC24 1
#define R_ARM_ABS32 2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define R_ARM_CALL 28
#define R_ARM_JUMP24 29
#define ELF_NGREG (sizeof (struct pt_regs) / sizeof(elf_greg_t))
typedef elf_greg_t elf_gregset_t[ELF_NGREG];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct user_fp elf_fpregset_t;
#define elf_check_arch(x) ( ((x)->e_machine == EM_ARM) && (ELF_PROC_OK((x))) )
#define ELF_CLASS ELFCLASS32
#ifdef __ARMEB__
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ELF_DATA ELFDATA2MSB
#else
#define ELF_DATA ELFDATA2LSB
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ELF_ARCH EM_ARM
#define USE_ELF_CORE_DUMP
#define ELF_EXEC_PAGESIZE 4096
#define ELF_ET_DYN_BASE (2 * TASK_SIZE / 3)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ELF_PLAT_INIT(_r, load_addr) (_r)->ARM_r0 = 0
#define ELF_HWCAP (elf_hwcap)
#define ELF_PLATFORM_SIZE 8
#define ELF_PLATFORM (elf_platform)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
