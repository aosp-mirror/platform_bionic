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
#define __ARCH_SIGEV_PREAMBLE_SIZE (sizeof(long) + 2 * sizeof(int))
#undef __ARCH_SI_TRAPNO
#if _MIPS_SZLONG == 32
#define __ARCH_SI_PREAMBLE_SIZE (3 * sizeof(int))
#elif _MIPS_SZLONG==64
#define __ARCH_SI_PREAMBLE_SIZE (4 * sizeof(int))
#else
#error _MIPS_SZLONG neither 32 nor 64
#endif
#define __ARCH_HAS_SWAPPED_SIGINFO
#include <asm-generic/siginfo.h>
#undef SI_ASYNCIO
#undef SI_TIMER
#undef SI_MESGQ
#define SI_ASYNCIO - 2
#define SI_TIMER - 3
#define SI_MESGQ - 4
#endif
