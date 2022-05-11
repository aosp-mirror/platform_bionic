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

#pragma once

// Some FreeBSD source includes <complex.h> and assumes <math.h> from that.
#include <math.h>
#include <float.h>

#define __weak_reference(sym,alias)     \
    __asm__(".weak " #alias);           \
    __asm__(".equ "  #alias ", " #sym)

#define __strong_reference(sym,aliassym) \
    extern __typeof (sym) aliassym __attribute__ ((__alias__ (#sym)))

#define __warn_references(sym,msg) /* ignored */

// digittoint is in BSD's <ctype.h>, but not ours, so we have a secret
// implementation in libm. We reuse parts of libm in the NDK's
// libandroid_support, where it's a static library, so we want all our
// "hidden" functions start with a double underscore --- being HIDDEN
// in the ELF sense is not sufficient.
#define digittoint __libm_digittoint
int digittoint(char ch);

// Similarly rename _scan_nan.
#define _scan_nan __libm_scan_nan

// FreeBSD exports these in <math.h> but we don't.
double cospi(double);
double sinpi(double);
