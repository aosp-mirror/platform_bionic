/*
 * Copyright (C) 2011 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _BIONIC_TIME_H
#define _BIONIC_TIME_H

#include <time.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#ifndef _BIONIC_STRFTIME_TZ_DECLARED
#define _BIONIC_STRFTIME_TZ_DECLARED

struct strftime_locale {
    const char *  mon[12];
    const char *  month[12];
    const char *  standalone_month[12];
    const char *  wday[7];
    const char *  weekday[7];
    const char *  X_fmt;
    const char *  x_fmt;
    const char *  c_fmt;
    const char *  am;
    const char *  pm;
    const char *  date_fmt;
};

/*
 * Note: you should consider these extensions deprecated and use managed code or icu4c instead.
 */
extern size_t strftime_tz(char* s, size_t max, const char* format, const struct tm* tm, const struct strftime_locale* lc);
extern time_t mktime_tz(struct tm* const tmp, char const* tz);
extern void localtime_tz(const time_t* const timep, struct tm* tmp, const char* tz);

#endif /* _BIONIC_STRFTIME_TZ_DECLARED */

__END_DECLS

#endif /* _BIONIC_TIME_H */
