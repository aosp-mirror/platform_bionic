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
#ifndef _UAPI_LINUX_CYCLADES_H
#define _UAPI_LINUX_CYCLADES_H
#warning "Support for features provided by this header has been removed"
#warning "Please consider updating your code"
struct cyclades_monitor {
  unsigned long int_count;
  unsigned long char_count;
  unsigned long char_max;
  unsigned long char_last;
};
#define CYGETMON 0x435901
#define CYGETTHRESH 0x435902
#define CYSETTHRESH 0x435903
#define CYGETDEFTHRESH 0x435904
#define CYSETDEFTHRESH 0x435905
#define CYGETTIMEOUT 0x435906
#define CYSETTIMEOUT 0x435907
#define CYGETDEFTIMEOUT 0x435908
#define CYSETDEFTIMEOUT 0x435909
#define CYSETRFLOW 0x43590a
#define CYGETRFLOW 0x43590b
#define CYSETRTSDTR_INV 0x43590c
#define CYGETRTSDTR_INV 0x43590d
#define CYZSETPOLLCYCLE 0x43590e
#define CYZGETPOLLCYCLE 0x43590f
#define CYGETCD1400VER 0x435910
#define CYSETWAIT 0x435912
#define CYGETWAIT 0x435913
#endif
