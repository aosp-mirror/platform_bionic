// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <netdb.h>

#include "header_checks.h"

static void netdb_h() {
  TYPE(struct hostent);
  STRUCT_MEMBER(struct hostent, char*, h_name);
  STRUCT_MEMBER(struct hostent, char**, h_aliases);
  STRUCT_MEMBER(struct hostent, int, h_addrtype);
  STRUCT_MEMBER(struct hostent, int, h_length);
  STRUCT_MEMBER(struct hostent, char**, h_addr_list);

  TYPE(struct netent);
  STRUCT_MEMBER(struct netent, char*, n_name);
  STRUCT_MEMBER(struct netent, char**, n_aliases);
  STRUCT_MEMBER(struct netent, int, n_addrtype);
  STRUCT_MEMBER(struct netent, uint32_t, n_net);

  TYPE(uint32_t);

  TYPE(struct protoent);
  STRUCT_MEMBER(struct protoent, char*, p_name);
  STRUCT_MEMBER(struct protoent, char**, p_aliases);
  STRUCT_MEMBER(struct protoent, int, p_proto);


  TYPE(struct servent);
  STRUCT_MEMBER(struct servent, char*, s_name);
  STRUCT_MEMBER(struct servent, char**, s_aliases);
  STRUCT_MEMBER(struct servent, int, s_port);
  STRUCT_MEMBER(struct servent, char*, s_proto);

  MACRO(IPPORT_RESERVED);

  TYPE(struct addrinfo);
  STRUCT_MEMBER(struct addrinfo, int, ai_flags);
  STRUCT_MEMBER(struct addrinfo, int, ai_family);
  STRUCT_MEMBER(struct addrinfo, int, ai_socktype);
  STRUCT_MEMBER(struct addrinfo, int, ai_protocol);
  STRUCT_MEMBER(struct addrinfo, socklen_t, ai_addrlen);
  STRUCT_MEMBER(struct addrinfo, struct sockaddr*, ai_addr);
  STRUCT_MEMBER(struct addrinfo, char*, ai_canonname);
  STRUCT_MEMBER(struct addrinfo, struct addrinfo*, ai_next);

  MACRO(AI_PASSIVE);
  MACRO(AI_CANONNAME);
  MACRO(AI_NUMERICHOST);
  MACRO(AI_NUMERICSERV);
  MACRO(AI_V4MAPPED);
  MACRO(AI_ALL);
  MACRO(AI_ADDRCONFIG);

  MACRO(NI_NOFQDN);
  MACRO(NI_NUMERICHOST);
  MACRO(NI_NAMEREQD);
  MACRO(NI_NUMERICSERV);
#if !defined(__BIONIC__) && !defined(__GLIBC__)
  MACRO(NI_NUMERICSCOPE);
#endif
  MACRO(NI_DGRAM);

  MACRO(EAI_AGAIN);
  MACRO(EAI_BADFLAGS);
  MACRO(EAI_FAIL);
  MACRO(EAI_FAMILY);
  MACRO(EAI_MEMORY);
  MACRO(EAI_NONAME);
  MACRO(EAI_SERVICE);
  MACRO(EAI_SOCKTYPE);
  MACRO(EAI_SYSTEM);
  MACRO(EAI_OVERFLOW);

  FUNCTION(endhostent, void (*f)(void));
  FUNCTION(endnetent, void (*f)(void));
  FUNCTION(endprotoent, void (*f)(void));
  FUNCTION(endservent, void (*f)(void));
  FUNCTION(freeaddrinfo, void (*f)(struct addrinfo*));
  FUNCTION(gai_strerror, const char* (*f)(int));
  FUNCTION(getaddrinfo, int (*f)(const char*, const char*, const struct addrinfo*, struct addrinfo**));
  FUNCTION(gethostent, struct hostent* (*f)(void));
#if defined(__BIONIC__) // Historical ABI accident.
  FUNCTION(getnameinfo, int (*f)(const struct sockaddr*, socklen_t, char*, size_t, char*, size_t, int));
#else
  FUNCTION(getnameinfo, int (*f)(const struct sockaddr*, socklen_t, char*, socklen_t, char*, socklen_t, int));
#endif
  FUNCTION(getnetbyaddr, struct netent* (*f)(uint32_t, int));
  FUNCTION(getnetbyname, struct netent* (*f)(const char*));
  FUNCTION(getnetent, struct netent* (*f)(void));
  FUNCTION(getprotobyname, struct protoent* (*f)(const char*));
  FUNCTION(getprotobynumber, struct protoent* (*f)(int));
  FUNCTION(getprotoent, struct protoent* (*f)(void));
  FUNCTION(getservbyname, struct servent* (*f)(const char*, const char*));
  FUNCTION(getservbyport, struct servent* (*f)(int, const char*));
  FUNCTION(getservent, struct servent* (*f)(void));
  FUNCTION(sethostent, void (*f)(int));
  FUNCTION(setnetent, void (*f)(int));
  FUNCTION(setprotoent, void (*f)(int));
  FUNCTION(setservent, void (*f)(int));

  TYPE(socklen_t);
}
