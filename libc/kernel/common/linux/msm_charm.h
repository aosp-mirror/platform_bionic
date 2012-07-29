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
#ifndef _ARCH_ARM_MACH_MSM_MDM_IOCTLS_H
#define _ARXH_ARM_MACH_MSM_MDM_IOCTLS_H
#define CHARM_CODE 0xCC
#define WAKE_CHARM _IO(CHARM_CODE, 1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RESET_CHARM _IO(CHARM_CODE, 2)
#define CHECK_FOR_BOOT _IOR(CHARM_CODE, 3, int)
#define WAIT_FOR_BOOT _IO(CHARM_CODE, 4)
#define NORMAL_BOOT_DONE _IOW(CHARM_CODE, 5, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RAM_DUMP_DONE _IOW(CHARM_CODE, 6, int)
#define WAIT_FOR_RESTART _IOR(CHARM_CODE, 7, int)
#define GET_DLOAD_STATUS _IOR(CHARM_CODE, 8, int)
enum charm_boot_type {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 CHARM_NORMAL_BOOT = 0,
 CHARM_RAM_DUMPS,
};
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */

