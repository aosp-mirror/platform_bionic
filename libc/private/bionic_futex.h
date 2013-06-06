/*
 * Copyright (C) 2008 The Android Open Source Project
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
#ifndef _BIONIC_FUTEX_H
#define _BIONIC_FUTEX_H

#include <linux/futex.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

extern int __futex_wait(volatile void *ftx, int val, const struct timespec *timeout);
extern int __futex_wake(volatile void *ftx, int count);

extern int __futex_syscall3(volatile void *ftx, int op, int val);
extern int __futex_syscall4(volatile void *ftx, int op, int val, const struct timespec *timeout);

#ifndef FUTEX_PRIVATE_FLAG
#define FUTEX_PRIVATE_FLAG  128
#endif

#ifndef FUTEX_WAIT_PRIVATE
#define FUTEX_WAIT_PRIVATE  (FUTEX_WAIT|FUTEX_PRIVATE_FLAG)
#endif

#ifndef FUTEX_WAKE_PRIVATE
#define FUTEX_WAKE_PRIVATE  (FUTEX_WAKE|FUTEX_PRIVATE_FLAG)
#endif

/* Like __futex_wait/wake, but take an additionnal 'pshared' argument.
 * when non-0, this will use normal futexes. Otherwise, private futexes.
 */
extern int  __futex_wake_ex(volatile void *ftx, int pshared, int val);
extern int  __futex_wait_ex(volatile void *ftx, int pshared, int val, const struct timespec *timeout);

__END_DECLS

#endif /* _BIONIC_FUTEX_H */
