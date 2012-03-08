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
#ifndef _XOR_H
#define _XOR_H
#include <linux/raid/md.h>
#define MAX_XOR_BLOCKS 5
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct xor_block_template {
 struct xor_block_template *next;
 const char *name;
 int speed;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*do_2)(unsigned long, unsigned long *, unsigned long *);
 void (*do_3)(unsigned long, unsigned long *, unsigned long *,
 unsigned long *);
 void (*do_4)(unsigned long, unsigned long *, unsigned long *,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long *, unsigned long *);
 void (*do_5)(unsigned long, unsigned long *, unsigned long *,
 unsigned long *, unsigned long *, unsigned long *);
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
