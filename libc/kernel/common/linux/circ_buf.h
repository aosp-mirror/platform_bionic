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
#ifndef _LINUX_CIRC_BUF_H
#define _LINUX_CIRC_BUF_H 1
struct circ_buf {
 char *buf;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int head;
 int tail;
};
#define CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define CIRC_SPACE(head,tail,size) CIRC_CNT((tail),((head)+1),(size))
#define CIRC_CNT_TO_END(head,tail,size)   ({int end = (size) - (tail);   int n = ((head) + end) & ((size)-1);   n < end ? n : end;})
#define CIRC_SPACE_TO_END(head,tail,size)   ({int end = (size) - 1 - (head);   int n = (end + (tail)) & ((size)-1);   n <= end ? n : end+1;})
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
