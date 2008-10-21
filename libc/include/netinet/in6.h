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
#ifndef _NETINET_IN6_H
#define _NETINET_IN6_H

#include <linux/in6.h>

#define IN6_IS_ADDR_UNSPECIFIED(a)	\
	((*(const uint32_t *)(const void *)(&(a)->s6_addr[0]) == 0) &&	\
	 (*(const uint32_t *)(const void *)(&(a)->s6_addr[4]) == 0) &&	\
	 (*(const uint32_t *)(const void *)(&(a)->s6_addr[8]) == 0) &&	\
	 (*(const uint32_t *)(const void *)(&(a)->s6_addr[12]) == 0))

#define IN6_IS_ADDR_V4COMPAT(a)		\
	((*(const uint32_t *)(const void *)(&(a)->s6_addr[0]) == 0) &&	\
	 (*(const uint32_t *)(const void *)(&(a)->s6_addr[4]) == 0) &&	\
	 (*(const uint32_t *)(const void *)(&(a)->s6_addr[8]) == 0) &&	\
	 (*(const uint32_t *)(const void *)(&(a)->s6_addr[12]) != 0) &&	\
	 (*(const uint32_t *)(const void *)(&(a)->s6_addr[12]) != ntohl(1)))

#define IN6_IS_ADDR_V4MAPPED(a)		      \
	((*(const uint32_t *)(const void *)(&(a)->s6_addr[0]) == 0) &&	\
	 (*(const uint32_t *)(const void *)(&(a)->s6_addr[4]) == 0) &&	\
	 (*(const uint32_t *)(const void *)(&(a)->s6_addr[8]) == ntohl(0x0000ffff)))

#define IN6_IS_ADDR_LINKLOCAL(a)	\
	(((a)->s6_addr[0] == 0xfe) && (((a)->s6_addr[1] & 0xc0) == 0x80))

#define IN6_IS_ADDR_SITELOCAL(a)	\
	(((a)->s6_addr[0] == 0xfe) && (((a)->s6_addr[1] & 0xc0) == 0xc0))

#define IN6_IS_ADDR_MULTICAST(a) \
    (((__const uint8_t *) (a))[0] == 0xff)

#define IN6_ARE_ADDR_EQUAL(a, b)			\
    (memcmp(&(a)->s6_addr[0], &(b)->s6_addr[0], sizeof(struct in6_addr)) == 0)

#endif /* _NETINET_IN6_H */
