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

#ifndef ANDROID_LEGACY_WCTYPE_INLINES_H
#define ANDROID_LEGACY_WCTYPE_INLINES_H

#include <sys/cdefs.h>
#include <wctype.h>

__BEGIN_DECLS

#if __ANDROID_API__ < 21

static __inline int iswalnum_l(wint_t wc, locale_t l __unused) {
  return iswalnum(wc);
}

static __inline int iswalpha_l(wint_t wc, locale_t l __unused) {
  return iswalpha(wc);
}

static __inline int iswblank_l(wint_t wc, locale_t l __unused) {
  return iswblank(wc);
}

static __inline int iswcntrl_l(wint_t wc, locale_t l __unused) {
  return iswcntrl(wc);
}

static __inline int iswdigit_l(wint_t wc, locale_t l __unused) {
  return iswdigit(wc);
}

static __inline int iswgraph_l(wint_t wc, locale_t l __unused) {
  return iswgraph(wc);
}

static __inline int iswlower_l(wint_t wc, locale_t l __unused) {
  return iswlower(wc);
}

static __inline int iswprint_l(wint_t wc, locale_t l __unused) {
  return iswprint(wc);
}

static __inline int iswpunct_l(wint_t wc, locale_t l __unused) {
  return iswpunct(wc);
}

static __inline int iswspace_l(wint_t wc, locale_t l __unused) {
  return iswspace(wc);
}

static __inline int iswupper_l(wint_t wc, locale_t l __unused) {
  return iswupper(wc);
}

static __inline int iswxdigit_l(wint_t wc, locale_t l __unused) {
  return iswxdigit(wc);
}

static __inline wint_t towlower_l(wint_t wc, locale_t l __unused) {
  return towlower(wc);
}

static __inline wint_t towupper_l(wint_t wc, locale_t l __unused) {
  return towupper(wc);
}

#endif /* __ANDROID_API__ < 21 */

__END_DECLS

#endif /* ANDROID_LEGACY_WCTYPE_INLINES_H */
