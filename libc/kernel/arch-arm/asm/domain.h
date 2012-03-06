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
#ifndef __ASM_PROC_DOMAIN_H
#define __ASM_PROC_DOMAIN_H
#define DOMAIN_KERNEL 0
#define DOMAIN_TABLE 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DOMAIN_USER 1
#define DOMAIN_IO 2
#define DOMAIN_NOACCESS 0
#define DOMAIN_CLIENT 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DOMAIN_MANAGER 3
#define domain_val(dom,type) ((type) << (2*(dom)))
#ifndef __ASSEMBLY__
#define set_domain(x) do { } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define modify_domain(dom,type) do { } while (0)
#endif
#endif
