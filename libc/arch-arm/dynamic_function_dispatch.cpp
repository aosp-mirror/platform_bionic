/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <fcntl.h>
#include <sys/syscall.h>

extern "C" {

enum CpuVariant {
    kUnknown = 0,
    kGeneric,
    kCortexA7,
    kCortexA9,
    kCortexA53,
    kCortexA55,
    kKrait,
    kKryo,
};

static constexpr int MAX_CPU_NAME_LEN = 12;
struct CpuVariantNames {
    alignas(alignof(int)) char name[MAX_CPU_NAME_LEN];
    CpuVariant variant;
};

static constexpr CpuVariantNames cpu_variant_names[] = {
    {"cortex-a76", kCortexA55},
    {"kryo385", kCortexA55},
    {"cortex-a75", kCortexA55},
    {"kryo", kKryo},
    {"cortex-a73", kCortexA55},
    {"cortex-a55", kCortexA55},
    {"cortex-a53", kCortexA53},
    {"krait", kKrait},
    {"cortex-a9", kCortexA9},
    {"cortex-a7", kCortexA7},
    // kUnknown indicates the end of this array.
    {"", kUnknown},
};

static long ifunc_open(const char* pathname) {
    register long r0 __asm__("r0") = AT_FDCWD;
    register long r1 __asm__("r1") = reinterpret_cast<long>(pathname);
    register long r2 __asm__("r2") = O_RDONLY;
    register long r3 __asm__("r3") = 0;
    register long r7 __asm__("r7") = __NR_openat;
    __asm__ volatile("swi #0" : "=r"(r0) : "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r7));
    return r0;
}

static ssize_t ifunc_read(int fd, void* buf, size_t count) {
    register long r0 __asm__("r0") = fd;
    register long r1 __asm__("r1") = reinterpret_cast<long>(buf);
    register long r2 __asm__("r2") = count;
    register long r7 __asm__("r7") = __NR_read;
    __asm__ volatile("swi #0" : "=r"(r0) : "r"(r0), "r"(r1), "r"(r2), "r"(r7) : "memory");
    return r0;
}

static int ifunc_close(int fd) {
    register long r0 __asm__("r0") = fd;
    register long r7 __asm__("r7") = __NR_close;
    __asm__ volatile("swi #0" : "=r"(r0) : "r"(r0), "r"(r7));
    return r0;
}

#define DEFINE_IFUNC(name) \
    name##_func name __attribute__((ifunc(#name "_resolver"))); \
    __attribute__((visibility("hidden"))) \
    name##_func* name##_resolver()

#define DECLARE_FUNC(type, name) \
    __attribute__((visibility("hidden"))) \
    type name

#define RETURN_FUNC(type, name) { \
        DECLARE_FUNC(type, name); \
        return name; \
    }

static bool is_same_name(const char* a, const char* b) {
    static_assert(MAX_CPU_NAME_LEN % sizeof(int) == 0, "");
    const int* ia = reinterpret_cast<const int*>(a);
    const int* ib = reinterpret_cast<const int*>(b);
    for (size_t i = 0; i < MAX_CPU_NAME_LEN / sizeof(int); ++i) {
        if (ia[i] != ib[i]) {
            return false;
        }
    }
    return true;
}

static CpuVariant init_cpu_variant() {
    int fd = ifunc_open("/dev/cpu_variant:arm");
    if (fd < 0) return kGeneric;

    alignas(alignof(int)) char name[MAX_CPU_NAME_LEN] = {};

    int bytes_read, total_read = 0;
    while (total_read < MAX_CPU_NAME_LEN - 1 &&
           (bytes_read = ifunc_read(fd, name + total_read,
                                    MAX_CPU_NAME_LEN - 1 - total_read)) > 0) {
        total_read += bytes_read;
    }
    ifunc_close(fd);

    if (bytes_read != 0) {
        // The file is too big. We haven't reach the end. Or maybe there is an
        // error when reading.
        return kGeneric;
    }
    name[total_read] = 0;

    const CpuVariantNames* cpu_variant = cpu_variant_names;
    while (cpu_variant->variant != kUnknown) {
        if (is_same_name(cpu_variant->name, name)) {
            return cpu_variant->variant;
        }
        cpu_variant++;
    }
    return kGeneric;
}

static CpuVariant get_cpu_variant() {
    static CpuVariant cpu_variant = kUnknown;
    if (cpu_variant == kUnknown) {
        cpu_variant = init_cpu_variant();
    }
    return cpu_variant;
}

typedef void* memmove_func(void* __dst, const void* __src, size_t __n);
DEFINE_IFUNC(memmove) {
    RETURN_FUNC(memmove_func, memmove_a15);
}

typedef void* memcpy_func(void*, const void*, size_t);
DEFINE_IFUNC(memcpy) {
    return memmove_resolver();
}

typedef void* __memcpy_func(void*, const void*, size_t);
DEFINE_IFUNC(__memcpy) {
    switch(get_cpu_variant()) {
        case kCortexA7:
            RETURN_FUNC(__memcpy_func, __memcpy_a7);
        case kCortexA9:
            RETURN_FUNC(__memcpy_func, __memcpy_a9);
        case kKrait:
            RETURN_FUNC(__memcpy_func, __memcpy_krait);
        case kCortexA53:
            RETURN_FUNC(__memcpy_func, __memcpy_a53);
        case kCortexA55:
            RETURN_FUNC(__memcpy_func, __memcpy_a55);
        case kKryo:
            RETURN_FUNC(__memcpy_func, __memcpy_kryo);
        default:
            RETURN_FUNC(__memcpy_func, __memcpy_a15);
    }
}

typedef void* __memset_chk_func(void* s, int c, size_t n, size_t n2);
DEFINE_IFUNC(__memset_chk) {
    switch(get_cpu_variant()) {
        case kCortexA7:
        case kCortexA53:
        case kCortexA55:
        case kKryo:
            RETURN_FUNC(__memset_chk_func, __memset_chk_a7);
        case kCortexA9:
            RETURN_FUNC(__memset_chk_func, __memset_chk_a9);
        case kKrait:
            RETURN_FUNC(__memset_chk_func, __memset_chk_krait);
        default:
            RETURN_FUNC(__memset_chk_func, __memset_chk_a15);
    }
}

typedef void* memset_func(void* __dst, int __ch, size_t __n);
DEFINE_IFUNC(memset) {
    switch(get_cpu_variant()) {
        case kCortexA7:
        case kCortexA53:
        case kCortexA55:
        case kKryo:
             RETURN_FUNC(memset_func, memset_a7);
        case kCortexA9:
             RETURN_FUNC(memset_func, memset_a9);
        case kKrait:
             RETURN_FUNC(memset_func, memset_krait);
        default:
             RETURN_FUNC(memset_func, memset_a15);
    }
}

typedef char* strcpy_func(char* __dst, const char* __src);
DEFINE_IFUNC(strcpy) {
    switch(get_cpu_variant()) {
        case kCortexA9:
            RETURN_FUNC(strcpy_func, strcpy_a9);
        default:
            RETURN_FUNC(strcpy_func, strcpy_a15);
    }
}

typedef char* __strcpy_chk_func(char* dst, const char* src, size_t dst_len);
DEFINE_IFUNC(__strcpy_chk) {
    switch(get_cpu_variant()) {
        case kCortexA7:
            RETURN_FUNC(__strcpy_chk_func, __strcpy_chk_a7);
        case kCortexA9:
            RETURN_FUNC(__strcpy_chk_func, __strcpy_chk_a9);
        case kKrait:
        case kKryo:
            RETURN_FUNC(__strcpy_chk_func, __strcpy_chk_krait);
        case kCortexA53:
            RETURN_FUNC(__strcpy_chk_func, __strcpy_chk_a53);
        case kCortexA55:
            RETURN_FUNC(__strcpy_chk_func, __strcpy_chk_a55);
        default:
            RETURN_FUNC(__strcpy_chk_func, __strcpy_chk_a15);
    }
}

typedef char* stpcpy_func(char* __dst, const char* __src);
DEFINE_IFUNC(stpcpy) {
    switch(get_cpu_variant()) {
        case kCortexA9:
            RETURN_FUNC(stpcpy_func, stpcpy_a9);
        default:
            RETURN_FUNC(stpcpy_func, stpcpy_a15);
    }
}

typedef char* strcat_func(char* __dst, const char* __src);
DEFINE_IFUNC(strcat) {
    switch(get_cpu_variant()) {
        case kCortexA9:
            RETURN_FUNC(strcat_func, strcat_a9);
        default:
            RETURN_FUNC(strcat_func, strcat_a15);
    }
}

typedef char* __strcat_chk_func(char* dst, const char* src, size_t dst_buf_size);
DEFINE_IFUNC(__strcat_chk) {
    switch(get_cpu_variant()) {
        case kCortexA7:
            RETURN_FUNC(__strcat_chk_func, __strcat_chk_a7);
        case kCortexA9:
            RETURN_FUNC(__strcat_chk_func, __strcat_chk_a9);
        case kKrait:
        case kKryo:
            RETURN_FUNC(__strcat_chk_func, __strcat_chk_krait);
        case kCortexA53:
            RETURN_FUNC(__strcat_chk_func, __strcat_chk_a53);
        case kCortexA55:
            RETURN_FUNC(__strcat_chk_func, __strcat_chk_a55);
        default:
            RETURN_FUNC(__strcat_chk_func, __strcat_chk_a15);
    }
}

typedef int strcmp_func(const char* __lhs, const char* __rhs);
DEFINE_IFUNC(strcmp) {
    RETURN_FUNC(strcmp_func, strcmp_a15);
}

typedef size_t strlen_func(const char* __s);
DEFINE_IFUNC(strlen) {
    switch(get_cpu_variant()) {
        case kCortexA9:
            RETURN_FUNC(strlen_func, strlen_a9);
        default:
            RETURN_FUNC(strlen_func, strlen_a15);
    }
}

}  // extern "C"
