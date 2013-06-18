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
#ifndef _FCNTL_H
#define _FCNTL_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <linux/fcntl.h>
#include <unistd.h>  /* this is not required, but makes client code much happier */

__BEGIN_DECLS

#ifndef O_ASYNC
#define O_ASYNC  FASYNC
#endif

#ifndef O_CLOEXEC
#define O_CLOEXEC  02000000
#endif

extern int  open(const char*  path, int  mode, ...);
extern int  openat(int fd, const char*  path, int  mode, ...);
extern int  unlinkat(int dirfd, const char *pathname, int flags);
extern int  fcntl(int   fd, int   command, ...);
extern int  creat(const char*  path, mode_t  mode);

#if defined(__BIONIC_FORTIFY) && !defined(__clang__)
__errordecl(__creat_missing_mode, "called with O_CREAT, but missing mode");
__errordecl(__creat_too_many_args, "too many arguments");
extern int __open_real(const char *pathname, int flags, ...)
    __asm__(__USER_LABEL_PREFIX__ "open");
extern int __open_2(const char *, int);

__BIONIC_FORTIFY_INLINE
int open(const char *pathname, int flags, ...) {
    if (__builtin_constant_p(flags)) {
        if ((flags & O_CREAT) && __builtin_va_arg_pack_len() == 0) {
            __creat_missing_mode();  // compile time error
        }
    }

    if (__builtin_va_arg_pack_len() > 1) {
        __creat_too_many_args();  // compile time error
    }

    if ((__builtin_va_arg_pack_len() == 0) && !__builtin_constant_p(flags)) {
        return __open_2(pathname, flags);
    }

    return __open_real(pathname, flags, __builtin_va_arg_pack());
}

extern int __openat_2(int, const char *, int);
extern int __openat_real(int dirfd, const char *pathname, int flags, ...)
    __asm__(__USER_LABEL_PREFIX__ "openat");

__BIONIC_FORTIFY_INLINE
int openat(int dirfd, const char *pathname, int flags, ...) {
    if (__builtin_constant_p(flags)) {
        if ((flags & O_CREAT) && __builtin_va_arg_pack_len() == 0) {
            __creat_missing_mode();  // compile time error
        }
    }

    if (__builtin_va_arg_pack_len() > 1) {
        __creat_too_many_args();  // compile time error
    }

    if ((__builtin_va_arg_pack_len() == 0) && !__builtin_constant_p(flags)) {
        return __openat_2(dirfd, pathname, flags);
    }

    return __openat_real(dirfd, pathname, flags, __builtin_va_arg_pack());
}

#endif /* defined(__BIONIC_FORTIFY) && !defined(__clang__) */

__END_DECLS

#endif /* _FCNTL_H */
