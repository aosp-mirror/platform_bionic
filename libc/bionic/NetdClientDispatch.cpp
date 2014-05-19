/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include "private/NetdClientDispatch.h"

#ifdef __i386__
#define __socketcall __attribute__((__cdecl__))
#else
#define __socketcall
#endif

extern "C" __socketcall int __accept4(int, sockaddr*, socklen_t*, int);
extern "C" __socketcall int __connect(int, const sockaddr*, socklen_t);
extern "C" __socketcall int __socket(int, int, int);

static unsigned fallBackNetIdForResolv(unsigned netId) {
    return netId;
}

// This structure is modified only at startup (when libc.so is loaded) and never
// afterwards, so it's okay that it's read later at runtime without a lock.
__LIBC_HIDDEN__ NetdClientDispatch __netdClientDispatch __attribute__((aligned(32))) = {
    __accept4,
    __connect,
    __socket,
    fallBackNetIdForResolv,
};
