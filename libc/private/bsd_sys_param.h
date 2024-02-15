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

#pragma once

#include <inttypes.h>

/* OpenBSD has these in <sys/param.h>, but "ALIGN" isn't something we want to reserve. */
#define ALIGNBYTES (sizeof(uintptr_t) - 1)
#define ALIGN(p) ((__BIONIC_CAST(reinterpret_cast, uintptr_t, p) + ALIGNBYTES) & ~ALIGNBYTES)
