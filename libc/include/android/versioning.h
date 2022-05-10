/*
 * Copyright (C) 2016 The Android Open Source Project
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

// The `annotate` attribute always pulls the annotated (inline) function into the object files, thus
// we should only annotate headers when we are running versioner.
#if defined(__BIONIC_VERSIONER)

#define __INTRODUCED_IN(api_level) __attribute__((annotate("introduced_in=" #api_level)))
#define __INTRODUCED_IN_NO_GUARD_FOR_NDK(api_level) __attribute__((annotate("introduced_in=" #api_level))) __VERSIONER_NO_GUARD
#define __DEPRECATED_IN(api_level) __attribute__((annotate("deprecated_in=" #api_level)))
#define __REMOVED_IN(api_level) __attribute__((annotate("obsoleted_in=" #api_level)))
#define __INTRODUCED_IN_32(api_level) __attribute__((annotate("introduced_in_32=" #api_level)))
#define __INTRODUCED_IN_64(api_level) __attribute__((annotate("introduced_in_64=" #api_level)))
#define __INTRODUCED_IN_ARM(api_level) __attribute__((annotate("introduced_in_arm=" #api_level)))
#define __INTRODUCED_IN_X86(api_level) __attribute__((annotate("introduced_in_x86=" #api_level)))
#define __INTRODUCED_IN_X86_NO_GUARD_FOR_NDK(api_level) __attribute__((annotate("introduced_in_x86=" #api_level))) __VERSIONER_NO_GUARD

#define __VERSIONER_NO_GUARD __attribute__((annotate("versioner_no_guard")))
#define __VERSIONER_FORTIFY_INLINE __attribute__((annotate("versioner_fortify_inline")))

#else

// When headers are not processed by the versioner (i.e. compiled into object files),
// the availability attributed is emitted instead. The clang compiler will make the symbol weak
// when targeting old api_level and enforce the reference to the symbol to be guarded with
// __builtin_available(android api_level, *).

// The 'strict' flag is required for NDK clients where the use of "-Wunguarded-availability" cannot
// be enforced. In the case, the absence of 'strict' makes it possible to call an unavailable API
// without the __builtin_available check, which will cause a link error at runtime.
// Android platform build system defines this macro along with -Wunguarded-availability
//
// The _NO_GUARD_FOR_NDK variants keep the __VERSIONER_NO_GUARD behavior working for the NDK. This
// allows libc++ to refer to these functions in inlines without needing to guard them, needed since
// libc++ doesn't currently guard these calls. There's no risk to the apps though because using
// those APIs will still cause a link error.
#if defined(__ANDROID_UNAVAILABLE_SYMBOLS_ARE_WEAK__)
#define __BIONIC_AVAILABILITY(__what) __attribute__((__availability__(android,__what)))
#define __INTRODUCED_IN_NO_GUARD_FOR_NDK(api_level) __INTRODUCED_IN(api_level)
#define __INTRODUCED_IN_X86_NO_GUARD_FOR_NDK(api_level) __INTRODUCED_IN_X86(api_level)
#else
#define __BIONIC_AVAILABILITY(__what) __attribute__((__availability__(android,strict,__what)))
#define __INTRODUCED_IN_NO_GUARD_FOR_NDK(api_level)
#define __INTRODUCED_IN_X86_NO_GUARD_FOR_NDK(api_level)
#endif

#define __INTRODUCED_IN(api_level) __BIONIC_AVAILABILITY(introduced=api_level)
#define __DEPRECATED_IN(api_level) __BIONIC_AVAILABILITY(deprecated=api_level)
#define __REMOVED_IN(api_level) __BIONIC_AVAILABILITY(obsoleted=api_level)

// The same availability attribute can't be annotated multiple times. Therefore, the macros are
// defined for the configuration that it is valid for so that declarations like the below doesn't
// cause inconsistent availability values which is an error with -Wavailability:
//
// void foo() __INTRODUCED_IN_32(30) __INTRODUCED_IN_64(31);
//
// This also takes the advantage of the fact that we never use bitness-specific macro with
// arch-specific macro. In other words,
//
// void foo() __INTRODUCED_IN_ARM(30) __INTRODUCED_IN_64(31);
//
// hasn't been supported and won't be.
#if !defined(__LP64__)
#define __INTRODUCED_IN_32(api_level) __BIONIC_AVAILABILITY(introduced=api_level)
#define __INTRODUCED_IN_64(api_level)
#else
#define __INTRODUCED_IN_32(api_level)
#define __INTRODUCED_IN_64(api_level) __BIONIC_AVAILABILITY(introduced=api_level)
#endif

#if defined(__arm__) || defined(__aarch64__)
#define __INTRODUCED_IN_ARM(api_level) __BIONIC_AVAILABILITY(introduced=api_level)
#define __INTRODUCED_IN_X86(api_level)
#elif defined(__i386__) || defined(__x86_64__)
#define __INTRODUCED_IN_ARM(api_level)
#define __INTRODUCED_IN_X86(api_level) __BIONIC_AVAILABILITY(introduced=api_level)
#else
#define __INTRODUCED_IN_ARM(api_level)
#define __INTRODUCED_IN_X86(api_level)
#endif

#define __VERSIONER_NO_GUARD
#define __VERSIONER_FORTIFY_INLINE

#endif  // defined(__BIONIC_VERSIONER)
