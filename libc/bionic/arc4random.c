/*	$OpenBSD: arc4random.c,v 1.33 2014/06/13 18:58:58 deraadt Exp $	*/

/*
 * Copyright (c) 1996, David Mazieres <dm@uun.org>
 * Copyright (c) 2008, Damien Miller <djm@openbsd.org>
 * Copyright (c) 2013, Markus Friedl <markus@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * ChaCha based random number generator for OpenBSD.
 */

#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/mman.h>

#if defined(__ANDROID__)
#include <sys/stat.h>
#include <linux/random.h>
#include "private/libc_logging.h"
#include "private/thread_private.h"

#define explicit_bzero(p, s) memset(p, 0, s)

#undef MAP_ANON
#define MAP_ANON (MAP_PRIVATE | MAP_ANONYMOUS)

/*
 * XXX Should be replaced with a proper entropy measure.
 */
static int
gotdata(u_char *buf, size_t len)
{
	char	any_set = 0;
	size_t	i;

	for (i = 0; i < len; ++i)
		any_set |= buf[i];
	if (any_set == 0)
		return -1;
	return 0;
}

static int
getentropy/*_urandom*/(u_char *buf, size_t len)
{
	int save_errno = errno;

	int fd = TEMP_FAILURE_RETRY(open("/dev/urandom", O_RDONLY|O_CLOEXEC|O_NOFOLLOW, 0));
	if (fd == -1) {
		__libc_fatal("getentropy_urandom failed to open \"/dev/urandom\": %s",
				strerror(errno));
	}

	/* Lightly verify that the device node looks sane */
	struct stat st;
	if (fstat(fd, &st) == -1 || !S_ISCHR(st.st_mode)) {
		__libc_fatal("getentropy_urandom failed to fstat \"/dev/urandom\": %s",
				strerror(errno));
	}
	int cnt;
	if (ioctl(fd, RNDGETENTCNT, &cnt) == -1) {
		__libc_fatal("getentropy_urandom failed to ioctl \"/dev/urandom\": %s",
				strerror(errno));
	}
	for (size_t i = 0; i < len; ) {
		size_t wanted = len - i;
		ssize_t ret = TEMP_FAILURE_RETRY(read(fd, buf + i, wanted));

		if (ret == -1) {
			__libc_fatal("getentropy_urandom failed to read \"/dev/urandom\": %s",
					strerror(errno));
		}
		i += ret;
	}
	close(fd);
	if (gotdata(buf, len) == -1) {
		__libc_fatal("getentropy_urandom failed to get enough entropy: %s",
				strerror(errno));
	}

	errno = save_errno;
	return 0;
}
#endif /* __ANDROID__ */

#define KEYSTREAM_ONLY
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "../upstream-openbsd/lib/libc/crypt/chacha_private.h"
#pragma GCC diagnostic pop

#ifdef __GNUC__
#define inline __inline
#else				/* !__GNUC__ */
#define inline
#endif				/* !__GNUC__ */

#define KEYSZ	32
#define IVSZ	8
#define BLOCKSZ	64
#define RSBUFSZ	(16*BLOCKSZ)
static int rs_initialized;
static pid_t rs_stir_pid;
static chacha_ctx *rs;		/* chacha context for random keystream */
static u_char *rs_buf;		/* keystream blocks */
static size_t rs_have;		/* valid bytes at end of rs_buf */
static size_t rs_count;		/* bytes till reseed */

static inline void _rs_rekey(u_char *dat, size_t datlen);

static inline void
_rs_init(u_char *buf, size_t n)
{
	if (n < KEYSZ + IVSZ)
		return;

	if (rs == NULL && (rs = mmap(NULL, sizeof(*rs), PROT_READ|PROT_WRITE,
	    MAP_ANON, -1, 0)) == MAP_FAILED)
		abort();
	if (rs_buf == NULL && (rs_buf = mmap(NULL, RSBUFSZ, PROT_READ|PROT_WRITE,
	    MAP_ANON, -1, 0)) == MAP_FAILED)
		abort();

	chacha_keysetup(rs, buf, KEYSZ * 8, 0);
	chacha_ivsetup(rs, buf + KEYSZ);
}

static void
_rs_stir(void)
{
	u_char rnd[KEYSZ + IVSZ];

	/* XXX */
	(void) getentropy(rnd, sizeof rnd);

	if (!rs_initialized) {
		rs_initialized = 1;
		_rs_init(rnd, sizeof(rnd));
	} else
		_rs_rekey(rnd, sizeof(rnd));
	explicit_bzero(rnd, sizeof(rnd));

	/* invalidate rs_buf */
	rs_have = 0;
	memset(rs_buf, 0, RSBUFSZ);

	rs_count = 1600000;
}

static inline void
_rs_stir_if_needed(size_t len)
{
	pid_t pid = getpid();

	if (rs_count <= len || !rs_initialized || rs_stir_pid != pid) {
		rs_stir_pid = pid;
		_rs_stir();
	} else
		rs_count -= len;
}

static inline void
_rs_rekey(u_char *dat, size_t datlen)
{
#ifndef KEYSTREAM_ONLY
	memset(rs_buf, 0,RSBUFSZ);
#endif
	/* fill rs_buf with the keystream */
	chacha_encrypt_bytes(rs, rs_buf, rs_buf, RSBUFSZ);
	/* mix in optional user provided data */
	if (dat) {
		size_t i, m;

		m = MIN(datlen, KEYSZ + IVSZ);
		for (i = 0; i < m; i++)
			rs_buf[i] ^= dat[i];
	}
	/* immediately reinit for backtracking resistance */
	_rs_init(rs_buf, KEYSZ + IVSZ);
	memset(rs_buf, 0, KEYSZ + IVSZ);
	rs_have = RSBUFSZ - KEYSZ - IVSZ;
}

static inline void
_rs_random_buf(void *_buf, size_t n)
{
	u_char *buf = (u_char *)_buf;
	size_t m;

	_rs_stir_if_needed(n);
	while (n > 0) {
		if (rs_have > 0) {
			m = MIN(n, rs_have);
			memcpy(buf, rs_buf + RSBUFSZ - rs_have, m);
			memset(rs_buf + RSBUFSZ - rs_have, 0, m);
			buf += m;
			n -= m;
			rs_have -= m;
		}
		if (rs_have == 0)
			_rs_rekey(NULL, 0);
	}
}

static inline void
_rs_random_u32(u_int32_t *val)
{
	_rs_stir_if_needed(sizeof(*val));
	if (rs_have < sizeof(*val))
		_rs_rekey(NULL, 0);
	memcpy(val, rs_buf + RSBUFSZ - rs_have, sizeof(*val));
	memset(rs_buf + RSBUFSZ - rs_have, 0, sizeof(*val));
	rs_have -= sizeof(*val);
}

u_int32_t
arc4random(void)
{
	u_int32_t val;

	_ARC4_LOCK();
	_rs_random_u32(&val);
	_ARC4_UNLOCK();
	return val;
}

void
arc4random_buf(void *buf, size_t n)
{
	_ARC4_LOCK();
	_rs_random_buf(buf, n);
	_ARC4_UNLOCK();
}

/*
 * Calculate a uniformly distributed random number less than upper_bound
 * avoiding "modulo bias".
 *
 * Uniformity is achieved by generating new random numbers until the one
 * returned is outside the range [0, 2**32 % upper_bound).  This
 * guarantees the selected random number will be inside
 * [2**32 % upper_bound, 2**32) which maps back to [0, upper_bound)
 * after reduction modulo upper_bound.
 */
u_int32_t
arc4random_uniform(u_int32_t upper_bound)
{
	u_int32_t r, min;

	if (upper_bound < 2)
		return 0;

	/* 2**32 % x == (2**32 - x) % x */
	min = -upper_bound % upper_bound;

	/*
	 * This could theoretically loop forever but each retry has
	 * p > 0.5 (worst case, usually far better) of selecting a
	 * number inside the range we need, so it should rarely need
	 * to re-roll.
	 */
	for (;;) {
		r = arc4random();
		if (r >= min)
			break;
	}

	return r % upper_bound;
}
