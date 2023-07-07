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
#ifndef __ASM_GENERIC_SIGNAL_DEFS_H
#define __ASM_GENERIC_SIGNAL_DEFS_H
#include <linux/compiler.h>
#ifndef SA_NOCLDSTOP
#define SA_NOCLDSTOP 0x00000001
#endif
#ifndef SA_NOCLDWAIT
#define SA_NOCLDWAIT 0x00000002
#endif
#ifndef SA_SIGINFO
#define SA_SIGINFO 0x00000004
#endif
#define SA_UNSUPPORTED 0x00000400
#define SA_EXPOSE_TAGBITS 0x00000800
#ifndef SA_ONSTACK
#define SA_ONSTACK 0x08000000
#endif
#ifndef SA_RESTART
#define SA_RESTART 0x10000000
#endif
#ifndef SA_NODEFER
#define SA_NODEFER 0x40000000
#endif
#ifndef SA_RESETHAND
#define SA_RESETHAND 0x80000000
#endif
#define SA_NOMASK SA_NODEFER
#define SA_ONESHOT SA_RESETHAND
#ifndef SIG_BLOCK
#define SIG_BLOCK 0
#endif
#ifndef SIG_UNBLOCK
#define SIG_UNBLOCK 1
#endif
#ifndef SIG_SETMASK
#define SIG_SETMASK 2
#endif
#ifndef __ASSEMBLY__
typedef void __signalfn_t(int);
typedef __signalfn_t  * __sighandler_t;
typedef void __restorefn_t(void);
typedef __restorefn_t  * __sigrestore_t;
#define SIG_DFL (( __sighandler_t) 0)
#define SIG_IGN (( __sighandler_t) 1)
#define SIG_ERR (( __sighandler_t) - 1)
#endif
#endif
