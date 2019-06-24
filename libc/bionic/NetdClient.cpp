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

#ifdef LIBC_STATIC
#error NetdClient.cpp should NOT be included in static libc builds.
#endif

#include <async_safe/log.h>

#include "private/NetdClientDispatch.h"

#include <dlfcn.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

template <typename FunctionType>
static void netdClientInitFunction(void* handle, const char* symbol, FunctionType* function) {
    typedef void (*InitFunctionType)(FunctionType*);
    InitFunctionType initFunction = reinterpret_cast<InitFunctionType>(dlsym(handle, symbol));
    if (initFunction != nullptr) {
        initFunction(function);
    }
}

static void netdClientInitImpl() {
    // Prevent netd from looping back fwmarkd connections to itself. It would work, but it's
    // a deadlock hazard and unnecessary overhead for the resolver.
    if (getuid() == 0 && strcmp(basename(getprogname()), "netd") == 0) {
        async_safe_format_log(ANDROID_LOG_INFO, "netdClient",
                              "Skipping libnetd_client init since *we* are netd");
        return;
    }

    void* handle = dlopen("libnetd_client.so", RTLD_NOW);
    if (handle == nullptr) {
        // If the library is not available, it's not an error. We'll just use
        // default implementations of functions that it would've overridden.
        return;
    }

    netdClientInitFunction(handle, "netdClientInitAccept4", &__netdClientDispatch.accept4);
    netdClientInitFunction(handle, "netdClientInitConnect", &__netdClientDispatch.connect);
    netdClientInitFunction(handle, "netdClientInitSendmmsg", &__netdClientDispatch.sendmmsg);
    netdClientInitFunction(handle, "netdClientInitSendmsg", &__netdClientDispatch.sendmsg);
    netdClientInitFunction(handle, "netdClientInitSendto", &__netdClientDispatch.sendto);
    netdClientInitFunction(handle, "netdClientInitSocket", &__netdClientDispatch.socket);

    netdClientInitFunction(handle, "netdClientInitNetIdForResolv",
                           &__netdClientDispatch.netIdForResolv);
    netdClientInitFunction(handle, "netdClientInitDnsOpenProxy",
                           &__netdClientDispatch.dnsOpenProxy);
}

static pthread_once_t netdClientInitOnce = PTHREAD_ONCE_INIT;

extern "C" __LIBC_HIDDEN__ void netdClientInit() {
    if (pthread_once(&netdClientInitOnce, netdClientInitImpl)) {
        async_safe_format_log(ANDROID_LOG_ERROR, "netdClient", "Failed to initialize libnetd_client");
    }
}
