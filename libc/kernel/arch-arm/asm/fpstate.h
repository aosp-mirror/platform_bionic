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
#ifndef __ASM_ARM_FPSTATE_H
#define __ASM_ARM_FPSTATE_H
#ifndef __ASSEMBLY__
struct vfp_hard_struct {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 fpregs[16];
#if __LINUX_ARM_ARCH__ < 6
 __u32 fpmx_state;
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 fpexc;
 __u32 fpscr;
 __u32 fpinst;
 __u32 fpinst2;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
union vfp_state {
 struct vfp_hard_struct hard;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FP_HARD_SIZE 35
struct fp_hard_struct {
 unsigned int save[FP_HARD_SIZE];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FP_SOFT_SIZE 35
struct fp_soft_struct {
 unsigned int save[FP_SOFT_SIZE];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IWMMXT_SIZE 0x98
struct iwmmxt_struct {
 unsigned int save[IWMMXT_SIZE / sizeof(unsigned int)];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
union fp_state {
 struct fp_hard_struct hard;
 struct fp_soft_struct soft;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FP_SIZE (sizeof(union fp_state) / sizeof(int))
struct crunch_state {
 unsigned int mvdx[16][2];
 unsigned int mvax[4][3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int dspsc[2];
};
#define CRUNCH_SIZE sizeof(struct crunch_state)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
