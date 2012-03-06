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
#ifndef _LINUX_PRCTL_H
#define _LINUX_PRCTL_H
#define PR_SET_PDEATHSIG 1  
#define PR_GET_PDEATHSIG 2  
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PR_GET_DUMPABLE 3
#define PR_SET_DUMPABLE 4
#define PR_GET_UNALIGN 5
#define PR_SET_UNALIGN 6
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PR_UNALIGN_NOPRINT 1  
#define PR_UNALIGN_SIGBUS 2  
#define PR_GET_KEEPCAPS 7
#define PR_SET_KEEPCAPS 8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PR_GET_FPEMU 9
#define PR_SET_FPEMU 10
#define PR_FPEMU_NOPRINT 1  
#define PR_FPEMU_SIGFPE 2  
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PR_GET_FPEXC 11
#define PR_SET_FPEXC 12
#define PR_FP_EXC_SW_ENABLE 0x80  
#define PR_FP_EXC_DIV 0x010000  
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PR_FP_EXC_OVF 0x020000  
#define PR_FP_EXC_UND 0x040000  
#define PR_FP_EXC_RES 0x080000  
#define PR_FP_EXC_INV 0x100000  
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PR_FP_EXC_DISABLED 0  
#define PR_FP_EXC_NONRECOV 1  
#define PR_FP_EXC_ASYNC 2  
#define PR_FP_EXC_PRECISE 3  
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PR_GET_TIMING 13
#define PR_SET_TIMING 14
#define PR_TIMING_STATISTICAL 0  
#define PR_TIMING_TIMESTAMP 1  
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PR_SET_NAME 15  
#define PR_GET_NAME 16  
#define PR_GET_ENDIAN 19
#define PR_SET_ENDIAN 20
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PR_ENDIAN_BIG 0
#define PR_ENDIAN_LITTLE 1  
#define PR_ENDIAN_PPC_LITTLE 2  
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
