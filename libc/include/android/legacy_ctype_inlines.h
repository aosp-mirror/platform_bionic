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

#ifndef ANDROID_LEGACY_CTYPE_INLINES_H
#define ANDROID_LEGACY_CTYPE_INLINES_H

#include <ctype.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#if __ANDROID_API__ < 21

static __inline int isalnum_l(int c, locale_t l __unused) {
  return isalnum(c);
}

static __inline int isalpha_l(int c, locale_t l __unused) {
  return isalpha(c);
}

static __inline int isblank_l(int c, locale_t l __unused) {
  return isblank(c);
}

static __inline int iscntrl_l(int c, locale_t l __unused) {
  return iscntrl(c);
}

static __inline int isdigit_l(int c, locale_t l __unused) {
  return isdigit(c);
}

static __inline int isgraph_l(int c, locale_t l __unused) {
  return isgraph(c);
}

static __inline int islower_l(int c, locale_t l __unused) {
  return islower(c);
}

static __inline int isprint_l(int c, locale_t l __unused) {
  return isprint(c);
}

static __inline int ispunct_l(int c, locale_t l __unused) {
  return ispunct(c);
}

static __inline int isspace_l(int c, locale_t l __unused) {
  return isspace(c);
}

static __inline int isupper_l(int c, locale_t l __unused) {
  return isupper(c);
}

static __inline int isxdigit_l(int c, locale_t l __unused) {
  return isxdigit(c);
}

static __inline int tolower_l(int c, locale_t l __unused) {
  return tolower(c);
}

static __inline int toupper_l(int c, locale_t l __unused) {
  return toupper(c);
}

#endif /* __ANDROID_API__ < 21 */

__END_DECLS

#endif /* ANDROID_LEGACY_CTYPE_INLINES_H */
