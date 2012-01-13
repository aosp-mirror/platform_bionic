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
#ifndef _RESOLV_IFACE_H
#define _RESOLV_IFACE_H

/* This header contains declarations related to per-interface DNS
 * server selection. They are used by system/netd/ and should not be
 * exposed by the C library's public NDK headers.
 *
 * NOTE: <resolv.h> contains the same declarations, this will be removed
 *        when we change system/netd to use this header instead.
 */
#include <sys/cdefs.h>
#include <netinet/in.h>

__BEGIN_DECLS

/* Use a guard macro until we remove the same definitions from <resolv.h> */
#ifndef _BIONIC_RESOLV_IFACE_FUNCTIONS_DECLARED
#define _BIONIC_RESOLV_IFACE_FUNCTIONS_DECLARED

/* Set name of default interface */
extern void _resolv_set_default_iface(const char* ifname);

/* set name servers for an interface */
extern void _resolv_set_nameservers_for_iface(const char* ifname, char** servers, int numservers);

/* tell resolver of the address of an interface */
extern void _resolv_set_addr_of_iface(const char* ifname, struct in_addr* addr);

/* flush the cache associated with the default interface */
extern void _resolv_flush_cache_for_default_iface();

/* flush the cache associated with a certain interface */
extern void _resolv_flush_cache_for_iface(const char* ifname);

#endif /* _BIONIC_RESOLV_IFACE_FUNCTIONS_DECLARED */

__END_DECLS

#endif /* _RESOLV_IFACE_H */
