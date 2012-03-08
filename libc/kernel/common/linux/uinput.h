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
#ifndef __UINPUT_H_
#define __UINPUT_H_
#include <linux/input.h>
#define UINPUT_VERSION 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct uinput_ff_upload {
 int request_id;
 int retval;
 struct ff_effect effect;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct ff_effect old;
};
struct uinput_ff_erase {
 int request_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int retval;
 int effect_id;
};
#define UINPUT_IOCTL_BASE 'U'
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define UI_DEV_CREATE _IO(UINPUT_IOCTL_BASE, 1)
#define UI_DEV_DESTROY _IO(UINPUT_IOCTL_BASE, 2)
#define UI_SET_EVBIT _IOW(UINPUT_IOCTL_BASE, 100, int)
#define UI_SET_KEYBIT _IOW(UINPUT_IOCTL_BASE, 101, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define UI_SET_RELBIT _IOW(UINPUT_IOCTL_BASE, 102, int)
#define UI_SET_ABSBIT _IOW(UINPUT_IOCTL_BASE, 103, int)
#define UI_SET_MSCBIT _IOW(UINPUT_IOCTL_BASE, 104, int)
#define UI_SET_LEDBIT _IOW(UINPUT_IOCTL_BASE, 105, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define UI_SET_SNDBIT _IOW(UINPUT_IOCTL_BASE, 106, int)
#define UI_SET_FFBIT _IOW(UINPUT_IOCTL_BASE, 107, int)
#define UI_SET_PHYS _IOW(UINPUT_IOCTL_BASE, 108, char*)
#define UI_SET_SWBIT _IOW(UINPUT_IOCTL_BASE, 109, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define UI_BEGIN_FF_UPLOAD _IOWR(UINPUT_IOCTL_BASE, 200, struct uinput_ff_upload)
#define UI_END_FF_UPLOAD _IOW(UINPUT_IOCTL_BASE, 201, struct uinput_ff_upload)
#define UI_BEGIN_FF_ERASE _IOWR(UINPUT_IOCTL_BASE, 202, struct uinput_ff_erase)
#define UI_END_FF_ERASE _IOW(UINPUT_IOCTL_BASE, 203, struct uinput_ff_erase)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define EV_UINPUT 0x0101
#define UI_FF_UPLOAD 1
#define UI_FF_ERASE 2
#define UINPUT_MAX_NAME_SIZE 80
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct uinput_user_dev {
 char name[UINPUT_MAX_NAME_SIZE];
 struct input_id id;
 int ff_effects_max;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int absmax[ABS_MAX + 1];
 int absmin[ABS_MAX + 1];
 int absfuzz[ABS_MAX + 1];
 int absflat[ABS_MAX + 1];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#endif
