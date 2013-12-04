/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef _BIONIC_TESTS_BUFFER_TESTS_H
#define _BIONIC_TESTS_BUFFER_TESTS_H

#include <stdint.h>
#include <sys/types.h>

void RunSingleBufferAlignTest(
    size_t max_test_size, void (*test_func)(uint8_t*, size_t),
    size_t (*set_incr)(size_t) = NULL);

void RunSrcDstBufferAlignTest(
    size_t max_test_size, void (*test_func)(uint8_t*, uint8_t*, size_t),
    size_t (*set_incr)(size_t) = NULL);

void RunCmpBufferAlignTest(
    size_t max_test_size, void (*test_cmp_func)(uint8_t*, uint8_t*, size_t),
    void (*test_miscmp_func)(uint8_t*, uint8_t*, size_t, size_t),
    size_t (*set_incr)(size_t) = NULL);

void RunSingleBufferOverreadTest(void (*test_func)(uint8_t*, size_t));

void RunSrcDstBufferOverreadTest(void (*test_func)(uint8_t*, uint8_t*, size_t));

void RunCmpBufferOverreadTest(
    void (*test_cmp_func)(uint8_t*, uint8_t*, size_t),
    void (*test_miscmp_func)(uint8_t*, uint8_t*, size_t, size_t));

#endif // _BIONIC_TESTS_BUFFER_TESTS_H
