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
#ifndef _LINUX_MISCDEVICE_H
#define _LINUX_MISCDEVICE_H
#include <linux/module.h>
#include <linux/major.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PSMOUSE_MINOR 1
#define MS_BUSMOUSE_MINOR 2
#define ATIXL_BUSMOUSE_MINOR 3
#define ATARIMOUSE_MINOR 5
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SUN_MOUSE_MINOR 6
#define APOLLO_MOUSE_MINOR 7
#define PC110PAD_MINOR 9
#define WATCHDOG_MINOR 130
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TEMP_MINOR 131
#define RTC_MINOR 135
#define EFI_RTC_MINOR 136
#define SUN_OPENPROM_MINOR 139
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DMAPI_MINOR 140
#define NVRAM_MINOR 144
#define SGI_MMTIMER 153
#define STORE_QUEUE_MINOR 155
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I2O_MINOR 166
#define MICROCODE_MINOR 184
#define MWAVE_MINOR 219
#define MPT_MINOR 220
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MISC_DYNAMIC_MINOR 255
#define TUN_MINOR 200
#define HPET_MINOR 228
struct device;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct class_device;
struct miscdevice {
 int minor;
 const char *name;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const struct file_operations *fops;
 struct list_head list;
 struct device *dev;
 struct class_device *class;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define MODULE_ALIAS_MISCDEV(minor)   MODULE_ALIAS("char-major-" __stringify(MISC_MAJOR)   "-" __stringify(minor))
#endif
