/*
 * Copyright (C) 2008 The Android Open Source Project
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
#include <unistd.h>
#include <sys/cachectl.h>

#ifdef DEBUG
#include <private/libc_logging.h>
#define  XLOG(...) __libc_format_log(ANDROID_LOG_DEBUG,"libc-cacheflush",__VA_ARGS__)
#endif

/*
 * Linux historically defines a cacheflush(3) routine for MIPS
 * with this signature:
 * int cacheflush(char *addr, int nbytes, int cache);
 *
 * Android defines an alternate cacheflush routine which exposes the
 * ARM system call interface:
 * int cacheflush (long start, long end, long flags)
 *
 * This is an attempt to maintain compatibility between the historical MIPS
 * usage for software previously ported to MIPS and Android specific
 * uses of cacheflush()
 *
 * Use the gcc __clear_cache builtin if possible. This will generate inline synci
 * instructions if available or call _flush_cache(start, len, BCACHE) directly
 */

#if defined (__GNUC__)
#define GCC_VERSION ((__GNUC__*10000) + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__)
#endif

/* This is the Android signature */
int cacheflush (long start, long end, long flags)
{
	if (end < start) {
		/*
		 * It looks like this is really MIPS style cacheflush call
		 * start => addr
		 * end => nbytes
		 */
#ifdef DEBUG
		static int warned = 0;
		if (!warned) {
			XLOG("called with (start,len) instead of (start,end)");
			warned = 1;
		}
#endif
		end += start;
	}

#if !defined(ARCH_MIPS_USE_FLUSHCACHE_SYSCALL) && \
	defined(GCC_VERSION) && (GCC_VERSION >= 40300)

#if (__mips_isa_rev >= 2) && (GCC_VERSION < 40403)
	/*
	 * Modify "start" and "end" to avoid GCC 4.3.0-4.4.2 bug in
	 * mips_expand_synci_loop that may execute synci one more time.
	 * "start" points to the first byte of the cache line.
	 * "end" points to the last byte of the line before the last cache line.
	 * Because size is always a multiple of 4, this is safe to set
	 * "end" to the last byte.
	 */
	{
		int lineSize;
		asm("rdhwr %0, $1" : "=r" (lineSize));
		start = start & (-lineSize);
		end = (end & (-lineSize)) - 1;
	}
#endif
	__builtin___clear_cache((char *)start, (char *)end);
#else
	_flush_cache((char *)start, end-start, BCACHE);
#endif
	return 0;
}
