/*
 * Copyright (C) 2014 The Android Open Source Project

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

#ifndef __ANDROID_DLEXT_H__
#define __ANDROID_DLEXT_H__

#include <stddef.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

/* bitfield definitions for android_dlextinfo.flags */
enum {
  /* Mask of valid bits */
  ANDROID_DLEXT_VALID_FLAG_BITS = 0,
};

typedef struct {
  int     flags;
} android_dlextinfo;

extern void* android_dlopen_ext(const char* filename, int flag, const android_dlextinfo* extinfo);

__END_DECLS

#endif /* __ANDROID_DLEXT_H__ */
