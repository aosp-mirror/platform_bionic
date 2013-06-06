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
extern void _resolv_set_nameservers_for_iface(const char* ifname, const char** servers, int numservers,
        const char *domains);

/* tell resolver of the address of an interface */
extern void _resolv_set_addr_of_iface(const char* ifname, struct in_addr* addr);

/* flush the cache associated with the default interface */
extern void _resolv_flush_cache_for_default_iface();

/* flush the cache associated with a certain interface */
extern void _resolv_flush_cache_for_iface(const char* ifname);

/* set a pid to use the name servers of the specified interface */
extern void _resolv_set_iface_for_pid(const char* ifname, int pid);

/* clear pid from being associated with an interface */
extern void _resolv_clear_iface_for_pid(int pid);

/* clear the entire mapping of pids to interfaces. */
extern void _resolv_clear_iface_pid_mapping();

/** Gets the name of the interface to which the pid is attached.
 *  On error, -1 is returned.
 *  If no interface is found, 0 is returned and buff is set to empty ('\0').
 *  If an interface is found, the name is copied to buff and the length of the name is returned.
 *  Arguments:   pid The pid to find an interface for
 *               buff A buffer to copy the result to
 *               buffLen Length of buff. An interface is at most IF_NAMESIZE in length */
extern int _resolv_get_pids_associated_interface(int pid, char* buff, int buffLen);


/** set a uid range to use the name servers of the specified interface
 *  If [low,high] overlaps with an already existing rule -1 is returned */
extern int _resolv_set_iface_for_uid_range(const char* ifname, int uid_start, int uid_end);

/* clear a uid range from being associated with an interface
 * If the range given is not mapped -1 is returned. */
extern int _resolv_clear_iface_for_uid_range(int uid_start, int uid_end);

/* clear the entire mapping of uid ranges to interfaces. */
extern void _resolv_clear_iface_uid_range_mapping();

/** Gets the name of the interface to which the uid is attached.
 *  On error, -1 is returned.
 *  If no interface is found, 0 is returned and buff is set to empty ('\0').
 *  If an interface is found, the name is copied to buff and the length of the name is returned.
 *  Arguments:   uid The uid to find an interface for
 *               buff A buffer to copy the result to
 *               buffLen Length of buff. An interface is at most IF_NAMESIZE in length */
extern int _resolv_get_uids_associated_interface(int uid, char* buff, int buffLen);

#endif /* _BIONIC_RESOLV_IFACE_FUNCTIONS_DECLARED */

__END_DECLS

#endif /* _RESOLV_IFACE_H */
