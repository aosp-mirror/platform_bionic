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
#ifndef __TPS6594_PFSM_H
#define __TPS6594_PFSM_H
#include <linux/const.h>
#include <linux/ioctl.h>
#include <linux/types.h>
struct pmic_state_opt {
  __u8 gpio_retention;
  __u8 ddr_retention;
  __u8 mcu_only_startup_dest;
};
#define PMIC_BASE 'P'
#define PMIC_GOTO_STANDBY _IO(PMIC_BASE, 0)
#define PMIC_GOTO_LP_STANDBY _IO(PMIC_BASE, 1)
#define PMIC_UPDATE_PGM _IO(PMIC_BASE, 2)
#define PMIC_SET_ACTIVE_STATE _IO(PMIC_BASE, 3)
#define PMIC_SET_MCU_ONLY_STATE _IOW(PMIC_BASE, 4, struct pmic_state_opt)
#define PMIC_SET_RETENTION_STATE _IOW(PMIC_BASE, 5, struct pmic_state_opt)
#endif
