/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <new>

#include <stdlib.h>

#include <async_safe/log.h>

__attribute__((weak)) const std::nothrow_t std::nothrow = {};

// We can't throw in bionic, so we go straight to the equivalent of
// std::terminate for these two instead.
void* operator new(std::size_t size) {
    void* p = malloc(size);
    if (p == nullptr) {
        async_safe_fatal("new failed to allocate %zu bytes", size);
    }
    return p;
}
void* operator new[](std::size_t size) {
    void* p = malloc(size);
    if (p == nullptr) {
        async_safe_fatal("new[] failed to allocate %zu bytes", size);
    }
    return p;
}

// These two are the "nothrow" variants, so we just return nullptr on failure.
void* operator new(std::size_t size, const std::nothrow_t&) {
    return malloc(size);
}
void* operator new[](std::size_t size, const std::nothrow_t&) {
    return malloc(size);
}

// free() can't throw anyway (except on heap corruption, which is always fatal),
// so there's no difference between the regular and "nothrow" variants here.
void operator delete(void* p) noexcept { free(p); }
void operator delete[](void* p) noexcept { free(p); }
void operator delete(void* p, const std::nothrow_t&) noexcept { free(p); }
void operator delete[](void* p, const std::nothrow_t&) noexcept { free(p); }

// TODO: these can use free_sized() once we have it (http://b/284321795).
void operator delete(void* p, std::size_t) noexcept { free(p); }
void operator delete[](void* p, std::size_t) noexcept { free(p); }
