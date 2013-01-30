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
#ifndef _IP6T_OPTS_H
#define _IP6T_OPTS_H
#define IP6T_OPTS_OPTSNR 16
struct ip6t_opts
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
{
 u_int32_t hdrlen;
 u_int8_t flags;
 u_int8_t invflags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u_int16_t opts[IP6T_OPTS_OPTSNR];
 u_int8_t optsnr;
};
#define IP6T_OPTS_LEN 0x01
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IP6T_OPTS_OPTS 0x02
#define IP6T_OPTS_NSTRICT 0x04
#define IP6T_OPTS_INV_LEN 0x01
#define IP6T_OPTS_INV_MASK 0x01
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
