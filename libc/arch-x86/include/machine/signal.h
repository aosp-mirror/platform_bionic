/*
 * Copyright (C) 2012 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _ARCH_X86_MACHINE_SIGNAL_H_
#define _ARCH_X86_MACHINE_SIGNAL_H_

#include <stddef.h>
#include <sys/types.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#include <asm/signal.h>
#include <asm/sigcontext.h>

#ifdef _ASM_GENERIC_SIGINFO_H
#error "You cannot include <asm/siginfo.h> before <signal.h>!"
#endif
#define __ARCH_SI_UID_T __kernel_uid32_t
#include <asm/siginfo.h>
#undef __ARCH_SI_UID_T

#include <stdint.h>

/* See comment in arch-arm/include/machine/signal.h.
 * The x86 kernel also uses 64-bit signal masks while defining sigset-t
 * as a 32-bit type.
 */
typedef unsigned long __kernel_sigset_t[2];

/* _NSIG is used by the SIGRTMAX definition under <asm/signal.h>, however
 * its definition is part of a #if __KERNEL__ .. #endif block in the original
 * kernel headers and is thus not part of our cleaned-up versions.
 *
 * Looking at the current kernel sources, it is defined as 64 for x86.
 */
#ifndef _NSIG
#  define _NSIG  64
#endif

__END_DECLS

#endif /* _ARCH_X86_MACHINE_SIGNAL_H_ */
