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

#ifndef ANDROID_LEGACY_STRING_INLINES_H
#define ANDROID_LEGACY_STRING_INLINES_H

#include <string.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#if __ANDROID_API__ < 21

static __inline __attribute_pure__ int strcoll_l(const char* _Nonnull s1, const char* _Nonnull s2,
                                                 locale_t l __unused)  {
  return strcoll(s1, s2);
}

size_t strxfrm_l(char* __restrict dest, const char* _Nonnull __restrict src, size_t n,
                 locale_t l __unused) {
  return strxfrm(dest, src, n);
}

#endif /* __ANDROID_API__ < 21 */

__END_DECLS

#endif /* ANDROID_LEGACY_STRING_INLINES_H */
