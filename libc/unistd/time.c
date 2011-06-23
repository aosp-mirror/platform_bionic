/*	$OpenBSD: time.c,v 1.5 2005/08/08 08:05:34 espie Exp $ */
/*
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <time.h>

time_t
time(time_t *t)
{
	struct timeval tt;
	time_t ret;

	if (gettimeofday(&tt, (struct timezone *)0) < 0)
		ret = -1;
	else
		ret = tt.tv_sec;
	if (t != NULL)
		*t = ret;
	return ret;
}

// return monotonically increasing CPU time in ticks relative to unspecified epoch
static inline clock_t clock_now(void)
{
	struct timespec tm;
	clock_gettime( CLOCK_MONOTONIC, &tm);
	return tm.tv_sec * CLOCKS_PER_SEC + (tm.tv_nsec * (CLOCKS_PER_SEC/1e9));
}

// initialized by the constructor below
static clock_t clock_start;

// called by dlopen when .so is loaded
__attribute__((constructor)) static void clock_crt0(void)
{
	clock_start = clock_now();
}

// return elapsed CPU time in clock ticks, since start of program execution
// (spec says epoch is undefined, but glibc uses crt0 as epoch)
clock_t
clock(void)
{
	// note that if we are executing in a different thread than crt0, then the
	// pthread_create that made us had a memory barrier so clock_start is defined
	return clock_now() - clock_start;
}
