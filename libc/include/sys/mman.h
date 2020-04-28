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

#ifndef _SYS_MMAN_H_
#define _SYS_MMAN_H_

#include <sys/cdefs.h>
#include <sys/types.h>
#include <asm/mman.h>

__BEGIN_DECLS

#ifndef MAP_ANON
#define MAP_ANON  MAP_ANONYMOUS
#endif

#define MAP_FAILED __BIONIC_CAST(reinterpret_cast, void*, -1)

#define MREMAP_MAYMOVE  1
#define MREMAP_FIXED    2

/*
 * See https://android.googlesource.com/platform/bionic/+/master/docs/32-bit-abi.md
 *
 * mmap64 wasn't really around until L, but we added an inline for it since it
 * allows a lot more code to compile with _FILE_OFFSET_BITS=64.
 *
 * GCC removes the static inline unless it is explicitly used. We can get around
 * this with __attribute__((used)), but that needlessly adds a definition of
 * mmap64 to every translation unit that includes this header. Instead, just
 * preserve the old behavior for GCC and emit a useful diagnostic.
 */
#if defined(__USE_FILE_OFFSET64)
void* mmap(void* __addr, size_t __size, int __prot, int __flags, int __fd, off_t __offset) __RENAME(mmap64);
#else
void* mmap(void* __addr, size_t __size, int __prot, int __flags, int __fd, off_t __offset);
#endif

#if __ANDROID_API__ >= __ANDROID_API_L__
void* mmap64(void* __addr, size_t __size, int __prot, int __flags, int __fd, off64_t __offset) __INTRODUCED_IN(21);
#endif

int munmap(void* __addr, size_t __size);
int msync(void* __addr, size_t __size, int __flags);
int mprotect(void* __addr, size_t __size, int __prot);
void* mremap(void* __old_addr, size_t __old_size, size_t __new_size, int __flags, ...);

int mlockall(int __flags) __INTRODUCED_IN(17);
int munlockall(void) __INTRODUCED_IN(17);

int mlock(const void* __addr, size_t __size);
int munlock(const void* __addr, size_t __size);

int mincore(void* __addr, size_t __size, unsigned char* __vector);

int madvise(void* __addr, size_t __size, int __advice);

#if __ANDROID_API__ >= __ANDROID_API_M__
/*
 * Some third-party code uses the existence of POSIX_MADV_NORMAL to detect the
 * availability of posix_madvise. This is not correct, since having up-to-date
 * UAPI headers says nothing about the C library, but for the time being we
 * don't want to harm adoption of the unified headers.
 *
 * https://github.com/android-ndk/ndk/issues/395
 */
#define POSIX_MADV_NORMAL     MADV_NORMAL
#define POSIX_MADV_RANDOM     MADV_RANDOM
#define POSIX_MADV_SEQUENTIAL MADV_SEQUENTIAL
#define POSIX_MADV_WILLNEED   MADV_WILLNEED
#define POSIX_MADV_DONTNEED   MADV_DONTNEED
#endif
int posix_madvise(void* __addr, size_t __size, int __advice) __INTRODUCED_IN(23);

__END_DECLS

#include <android/legacy_sys_mman_inlines.h>

#endif
