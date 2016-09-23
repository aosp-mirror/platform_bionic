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

#ifndef ANDROID_LEGACY_TIME_INLINES_H
#define ANDROID_LEGACY_TIME_INLINES_H

#include <sys/cdefs.h>
#include <time.h>

__BEGIN_DECLS

#if __ANDROID_API__ < 21

static __inline int strftime_l(char* s, size_t max, const char* format, const struct tm* tm,
                               locale_t l __unused) {
  return strftime(s, max, format, tm);
}

#endif /* __ANDROID_API__ < 21 */

__END_DECLS

#endif /* ANDROID_LEGACY_TIME_INLINES_H */
