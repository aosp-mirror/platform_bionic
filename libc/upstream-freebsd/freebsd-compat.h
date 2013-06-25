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

#ifndef _BIONIC_FREEBSD_COMPAT_H_included
#define _BIONIC_FREEBSD_COMPAT_H_included

#define __USE_BSD
#define REPLACE_GETOPT

#define _close close
#define _fcntl fcntl
#define _fstat fstat
#define _open open

#define _sseek __sseek /* Needed as long as we have a mix of OpenBSD and FreeBSD stdio. */

#endif
