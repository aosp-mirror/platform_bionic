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
#ifndef _LINUX_SMB_H
#define _LINUX_SMB_H
#include <linux/types.h>
enum smb_protocol {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 SMB_PROTOCOL_NONE,
 SMB_PROTOCOL_CORE,
 SMB_PROTOCOL_COREPLUS,
 SMB_PROTOCOL_LANMAN1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 SMB_PROTOCOL_LANMAN2,
 SMB_PROTOCOL_NT1
};
enum smb_case_hndl {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 SMB_CASE_DEFAULT,
 SMB_CASE_LOWER,
 SMB_CASE_UPPER
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct smb_dskattr {
 __u16 total;
 __u16 allocblocks;
 __u16 blocksize;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 free;
};
struct smb_conn_opt {
 unsigned int fd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 enum smb_protocol protocol;
 enum smb_case_hndl case_handling;
 __u32 max_xmit;
 __u16 server_uid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 tid;
 __u16 secmode;
 __u16 maxmux;
 __u16 maxvcs;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 rawmode;
 __u32 sesskey;
 __u32 maxraw;
 __u32 capabilities;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __s16 serverzone;
};
#endif
