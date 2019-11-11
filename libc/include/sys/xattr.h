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

#pragma once

/**
 * @file sys/xattr.h
 * @brief Extended attribute functions.
 */

#include <linux/xattr.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/**
 * [fsetxattr(2)](http://man7.org/linux/man-pages/man2/fsetxattr.2.html)
 * sets an extended attribute on the file referred to by the given file
 * descriptor.
 *
 * Valid flags are `XATTR_CREATE` and `XATTR_REPLACE`.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int fsetxattr(int __fd, const char* __name, const void* __value, size_t __size, int __flags);

/**
 * [setxattr(2)](http://man7.org/linux/man-pages/man2/setxattr.2.html)
 * sets an extended attribute on the file referred to by the given path.
 *
 * Valid flags are `XATTR_CREATE` and `XATTR_REPLACE`.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int setxattr(const char* __path, const char* __name, const void* __value, size_t __size, int __flags);

/**
 * [lsetxattr(2)](http://man7.org/linux/man-pages/man2/lsetxattr.2.html)
 * sets an extended attribute on the file referred to by the given path, which
 * is the link itself rather than its target in the case of a symbolic link.
 *
 * Valid flags are `XATTR_CREATE` and `XATTR_REPLACE`.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int lsetxattr(const char* __path, const char* __name, const void* __value, size_t __size, int __flags);

/**
 * [fgetxattr(2)](http://man7.org/linux/man-pages/man2/fgetxattr.2.html)
 * gets an extended attribute on the file referred to by the given file
 * descriptor.
 *
 * Returns the non-negative length of the value on success, or
 * returns -1 and sets `errno` on failure.
 */
ssize_t fgetxattr(int __fd, const char* __name, void* __value, size_t __size);

/**
 * [getxattr(2)](http://man7.org/linux/man-pages/man2/getxattr.2.html)
 * gets an extended attribute on the file referred to by the given path.
 *
 * Returns the non-negative length of the value on success, or
 * returns -1 and sets `errno` on failure.
 */
ssize_t getxattr(const char* __path, const char* __name, void* __value, size_t __size);

/**
 * [lgetxattr(2)](http://man7.org/linux/man-pages/man2/lgetxattr.2.html)
 * gets an extended attribute on the file referred to by the given path, which
 * is the link itself rather than its target in the case of a symbolic link.
 *
 * Returns the non-negative length of the value on success, or
 * returns -1 and sets `errno` on failure.
 */
ssize_t lgetxattr(const char* __path, const char* __name, void* __value, size_t __size);

/**
 * [flistxattr(2)](http://man7.org/linux/man-pages/man2/flistxattr.2.html)
 * lists the extended attributes on the file referred to by the given file
 * descriptor.
 *
 * Returns the non-negative length of the list on success, or
 * returns -1 and sets `errno` on failure.
 */
ssize_t flistxattr(int __fd, char* __list, size_t __size);

/**
 * [listxattr(2)](http://man7.org/linux/man-pages/man2/listxattr.2.html)
 * lists the extended attributes on the file referred to by the given path.
 *
 * Returns the non-negative length of the list on success, or
 * returns -1 and sets `errno` on failure.
 */
ssize_t listxattr(const char* __path, char* __list, size_t __size);

/**
 * [llistxattr(2)](http://man7.org/linux/man-pages/man2/llistxattr.2.html)
 * lists the extended attributes on the file referred to by the given path, which
 * is the link itself rather than its target in the case of a symbolic link.
 *
 * Returns the non-negative length of the list on success, or
 * returns -1 and sets `errno` on failure.
 */
ssize_t llistxattr(const char* __path, char* __list, size_t __size);

/**
 * [fremovexattr(2)](http://man7.org/linux/man-pages/man2/fremovexattr.2.html)
 * removes an extended attribute on the file referred to by the given file
 * descriptor.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int fremovexattr(int __fd, const char* __name);

/**
 * [lremovexattr(2)](http://man7.org/linux/man-pages/man2/lremovexattr.2.html)
 * removes an extended attribute on the file referred to by the given path, which
 * is the link itself rather than its target in the case of a symbolic link.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int lremovexattr(const char* __path, const char* __name);

/**
 * [removexattr(2)](http://man7.org/linux/man-pages/man2/removexattr.2.html)
 * removes an extended attribute on the file referred to by the given path.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int removexattr(const char* __path, const char* __name);

__END_DECLS
