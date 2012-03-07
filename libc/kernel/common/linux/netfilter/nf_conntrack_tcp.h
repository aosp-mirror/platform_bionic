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
#ifndef _NF_CONNTRACK_TCP_H
#define _NF_CONNTRACK_TCP_H
enum tcp_conntrack {
 TCP_CONNTRACK_NONE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 TCP_CONNTRACK_SYN_SENT,
 TCP_CONNTRACK_SYN_RECV,
 TCP_CONNTRACK_ESTABLISHED,
 TCP_CONNTRACK_FIN_WAIT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 TCP_CONNTRACK_CLOSE_WAIT,
 TCP_CONNTRACK_LAST_ACK,
 TCP_CONNTRACK_TIME_WAIT,
 TCP_CONNTRACK_CLOSE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 TCP_CONNTRACK_LISTEN,
 TCP_CONNTRACK_MAX,
 TCP_CONNTRACK_IGNORE
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IP_CT_TCP_FLAG_WINDOW_SCALE 0x01
#define IP_CT_TCP_FLAG_SACK_PERM 0x02
#define IP_CT_TCP_FLAG_CLOSE_INIT 0x03
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
