/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#define BIONIC_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;             \
  void operator=(const TypeName&) = delete

#define BIONIC_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                                  \
  BIONIC_DISALLOW_COPY_AND_ASSIGN(TypeName)

#define BIONIC_ROUND_UP_POWER_OF_2(value) \
  ((sizeof(value) == 8) \
    ? (1UL << (64 - __builtin_clzl(static_cast<unsigned long>(value)))) \
    : (1UL << (32 - __builtin_clz(static_cast<unsigned int>(value)))))

#if defined(__arm__)
#define BIONIC_STOP_UNWIND asm volatile(".cfi_undefined r14")
#elif defined(__aarch64__)
#define BIONIC_STOP_UNWIND asm volatile(".cfi_undefined x30")
#elif defined(__i386__)
#define BIONIC_STOP_UNWIND asm volatile(".cfi_undefined \%eip")
#elif defined(__riscv)
#define BIONIC_STOP_UNWIND asm volatile(".cfi_undefined ra")
#elif defined(__x86_64__)
#define BIONIC_STOP_UNWIND asm volatile(".cfi_undefined \%rip")
#endif

// The arraysize(arr) macro returns the # of elements in an array arr.
// The expression is a compile-time constant, and therefore can be
// used in defining new arrays, for example.  If you use arraysize on
// a pointer by mistake, you will get a compile-time error.
//
// One caveat is that arraysize() doesn't accept any array of an
// anonymous type or a type defined inside a function.
//
// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];  // NOLINT(readability/casting)

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

// Used to inform clang's -Wimplicit-fallthrough that a fallthrough is intended. There's no way to
// silence (or enable, apparently) -Wimplicit-fallthrough in C yet.
#ifdef __cplusplus
#define __BIONIC_FALLTHROUGH [[clang::fallthrough]]
#else
#define __BIONIC_FALLTHROUGH
#endif

static inline uintptr_t untag_address(uintptr_t p) {
#if defined(__aarch64__)
  return p & ((1ULL << 56) - 1);
#else
  return p;
#endif
}

template <typename T>
static inline T* _Nonnull untag_address(T* _Nonnull p) {
  return reinterpret_cast<T*>(untag_address(reinterpret_cast<uintptr_t>(p)));
}

// MTE globals protects internal and external global variables. One of the main
// things that MTE globals does is force all global variable accesses to go
// through the GOT. In the linker though, some global variables are accessed (or
// address-taken) prior to relocations being processed. Because relocations
// haven't run yet, the GOT entry hasn't been populated, and this leads to
// crashes. Thus, any globals used by the linker prior to relocation should be
// annotated with this attribute, which suppresses tagging of this global
// variable, restoring the pc-relative address computation.
//
// A way to find global variables that need this attribute is to build the
// linker/libc with `SANITIZE_TARGET=memtag_globals`, push them onto a device
// (it doesn't have to be MTE capable), and then run an executable using
// LD_LIBRARY_PATH and using the linker in interpreter mode (e.g.
// `LD_LIBRARY_PATH=/data/tmp/ /data/tmp/linker64 /data/tmp/my_binary`). A
// good heuristic is that the global variable is in a file that should be
// compiled with `-ffreestanding` (but there are global variables there that
// don't need this attribute).
#if __has_feature(memtag_globals)
#define BIONIC_USED_BEFORE_LINKER_RELOCATES __attribute__((no_sanitize("memtag")))
#else  // __has_feature(memtag_globals)
#define BIONIC_USED_BEFORE_LINKER_RELOCATES
#endif  // __has_feature(memtag_globals)
