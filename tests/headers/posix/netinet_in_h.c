/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <netinet/in.h>

#include "header_checks.h"

static void netinet_in_h() {
  TYPE(in_port_t);
  TYPE(in_addr_t);
  TYPE(sa_family_t);
  TYPE(uint8_t);
  TYPE(uint32_t);

  TYPE(struct in_addr);
  STRUCT_MEMBER(struct in_addr, in_addr_t, s_addr);

  TYPE(struct sockaddr_in);
  STRUCT_MEMBER(struct sockaddr_in, sa_family_t, sin_family);
  STRUCT_MEMBER(struct sockaddr_in, in_port_t, sin_port);
  STRUCT_MEMBER(struct sockaddr_in, struct in_addr, sin_addr);

  TYPE(struct in6_addr);
  STRUCT_MEMBER_ARRAY(struct in6_addr, uint8_t/*[]*/, s6_addr);

  TYPE(struct sockaddr_in6);
  STRUCT_MEMBER(struct sockaddr_in6, sa_family_t, sin6_family);
  STRUCT_MEMBER(struct sockaddr_in6, in_port_t, sin6_port);
  STRUCT_MEMBER(struct sockaddr_in6, uint32_t, sin6_flowinfo);
  STRUCT_MEMBER(struct sockaddr_in6, struct in6_addr, sin6_addr);
  STRUCT_MEMBER(struct sockaddr_in6, uint32_t, sin6_scope_id);

  struct in6_addr any_global = in6addr_any;
  struct in6_addr any_macro = IN6ADDR_ANY_INIT;
  struct in6_addr loop_global = in6addr_loopback;
  struct in6_addr loop_macro = IN6ADDR_LOOPBACK_INIT;

  TYPE(struct ipv6_mreq);
  STRUCT_MEMBER(struct ipv6_mreq, struct in6_addr, ipv6mr_multiaddr);
#if defined(__BIONIC__) // Currently comes from uapi header.
  STRUCT_MEMBER(struct ipv6_mreq, int, ipv6mr_interface);
#else
  STRUCT_MEMBER(struct ipv6_mreq, unsigned, ipv6mr_interface);
#endif

  MACRO(IPPROTO_IP);
  MACRO(IPPROTO_IPV6);
  MACRO(IPPROTO_ICMP);
  MACRO(IPPROTO_RAW);
  MACRO(IPPROTO_TCP);
  MACRO(IPPROTO_UDP);

  MACRO(INADDR_ANY);
  MACRO(INADDR_BROADCAST);

  MACRO_VALUE(INET_ADDRSTRLEN, 16);

  FUNCTION(htonl, uint32_t (*f)(uint32_t));
  FUNCTION(htons, uint16_t (*f)(uint16_t));
  FUNCTION(ntohl, uint32_t (*f)(uint32_t));
  FUNCTION(ntohs, uint16_t (*f)(uint16_t));

  MACRO_VALUE(INET6_ADDRSTRLEN, 46);

  MACRO(IPV6_JOIN_GROUP);
  MACRO(IPV6_LEAVE_GROUP);
  MACRO(IPV6_MULTICAST_HOPS);
  MACRO(IPV6_MULTICAST_IF);
  MACRO(IPV6_MULTICAST_LOOP);
  MACRO(IPV6_UNICAST_HOPS);
  MACRO(IPV6_V6ONLY);

#if !defined(IN6_IS_ADDR_UNSPECIFIED)
#error IN6_IS_ADDR_UNSPECIFIED
#endif
#if !defined(IN6_IS_ADDR_LOOPBACK)
#error IN6_IS_ADDR_LOOPBACK
#endif
#if !defined(IN6_IS_ADDR_MULTICAST)
#error IN6_IS_ADDR_MULTICAST
#endif
#if !defined(IN6_IS_ADDR_LINKLOCAL)
#error IN6_IS_ADDR_LINKLOCAL
#endif
#if !defined(IN6_IS_ADDR_SITELOCAL)
#error IN6_IS_ADDR_SITELOCAL
#endif
#if !defined(IN6_IS_ADDR_V4MAPPED)
#error IN6_IS_ADDR_V4MAPPED
#endif
#if !defined(IN6_IS_ADDR_V4COMPAT)
#error IN6_IS_ADDR_V4COMPAT
#endif
#if !defined(IN6_IS_ADDR_MC_NODELOCAL)
#error IN6_IS_ADDR_MC_NODELOCAL
#endif
#if !defined(IN6_IS_ADDR_MC_LINKLOCAL)
#error IN6_IS_ADDR_MC_LINKLOCAL
#endif
#if !defined(IN6_IS_ADDR_MC_SITELOCAL)
#error IN6_IS_ADDR_MC_SITELOCAL
#endif
#if !defined(IN6_IS_ADDR_MC_ORGLOCAL)
#error IN6_IS_ADDR_MC_ORGLOCAL
#endif
#if !defined(IN6_IS_ADDR_MC_GLOBAL)
#error IN6_IS_ADDR_MC_GLOBAL
#endif
}
