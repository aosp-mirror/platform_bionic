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

#include <sys/statvfs.h>

// libc++ uses statvfs (for Darwin compatibility), but on Linux statvfs is
// just another name for statfs, so it didn't arrive until API level 19. We
// make the implementation available as inlines to support std::filesystem
// for NDK users (see https://github.com/android-ndk/ndk/issues/609).

#define __BIONIC_SYS_STATVFS_INLINE /* Out of line. */
#define __BIONIC_NEED_STATVFS_INLINES
#undef __BIONIC_NEED_STATVFS64_INLINES
#include <bits/sys_statvfs_inlines.h>

// Historically we provided actual symbols for statvfs64 and fstatvfs64.
// They're not particularly useful, but we can't take them away.
__strong_alias(statvfs64, statvfs);
__strong_alias(fstatvfs64, fstatvfs);
