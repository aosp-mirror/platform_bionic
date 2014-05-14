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

#include <private/NetdClient.h>
#include <private/libc_logging.h>
#include <pthread.h>

#include <dlfcn.h>

template <typename FunctionType>
static void netdClientInitFunction(void* handle, const char* symbol, FunctionType* function) {
    typedef void (*InitFunctionType)(FunctionType*);
    InitFunctionType initFunction = reinterpret_cast<InitFunctionType>(dlsym(handle, symbol));
    if (initFunction != NULL) {
        initFunction(function);
    }
}

static void netdClientInitImpl() {
    void* netdClientHandle = dlopen("libnetd_client.so", RTLD_LAZY);
    if (netdClientHandle == NULL) {
        // If the library is not available, it's not an error. We'll just use
        // default implementations of functions that it would've overridden.
        return;
    }
    netdClientInitFunction(netdClientHandle, "netdClientInitAccept", &__netdClientDispatch.accept);
    netdClientInitFunction(netdClientHandle, "netdClientInitConnect",
                           &__netdClientDispatch.connect);
}

static pthread_once_t netdClientInitOnce = PTHREAD_ONCE_INIT;

extern "C" __LIBC_HIDDEN__ void netdClientInit() {
    if (pthread_once(&netdClientInitOnce, netdClientInitImpl)) {
        __libc_format_log(ANDROID_LOG_ERROR, "netdClient",
                          "Unable to initialize netd_client component.");
    }
}
