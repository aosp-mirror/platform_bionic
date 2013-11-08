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
#ifndef _UAPI_ASM_SIGINFO_H
#define _UAPI_ASM_SIGINFO_H
#define __ARCH_SIGEV_PREAMBLE_SIZE (sizeof(long) + 2*sizeof(int))
#undef __ARCH_SI_TRAPNO
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define HAVE_ARCH_SIGINFO_T
#define HAVE_ARCH_COPY_SIGINFO
struct siginfo;
#if _MIPS_SZLONG == 32
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __ARCH_SI_PREAMBLE_SIZE (3 * sizeof(int))
#elif _MIPS_SZLONG == 64
#define __ARCH_SI_PREAMBLE_SIZE (4 * sizeof(int))
#else
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#error _MIPS_SZLONG neither 32 nor 64
#endif
#include <asm-generic/siginfo.h>
typedef struct siginfo {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int si_signo;
 int si_code;
 int si_errno;
 int __pad0[SI_MAX_SIZE / sizeof(int) - SI_PAD_SIZE - 3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 union {
 int _pad[SI_PAD_SIZE];
 struct {
 pid_t _pid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __ARCH_SI_UID_T _uid;
 } _kill;
 struct {
 timer_t _tid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int _overrun;
 char _pad[sizeof( __ARCH_SI_UID_T) - sizeof(int)];
 sigval_t _sigval;
 int _sys_private;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 } _timer;
 struct {
 pid_t _pid;
 __ARCH_SI_UID_T _uid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sigval_t _sigval;
 } _rt;
 struct {
 pid_t _pid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __ARCH_SI_UID_T _uid;
 int _status;
 clock_t _utime;
 clock_t _stime;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 } _sigchld;
 struct {
 pid_t _pid;
 clock_t _utime;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int _status;
 clock_t _stime;
 } _irix_sigchld;
 struct {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void __user *_addr;
#ifdef __ARCH_SI_TRAPNO
 int _trapno;
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 short _addr_lsb;
 } _sigfault;
 struct {
 __ARCH_SI_BAND_T _band;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int _fd;
 } _sigpoll;
 } _sifields;
} siginfo_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#undef SI_ASYNCIO
#undef SI_TIMER
#undef SI_MESGQ
#define SI_ASYNCIO -2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SI_TIMER __SI_CODE(__SI_TIMER, -3)
#define SI_MESGQ __SI_CODE(__SI_MESGQ, -4)
#endif
