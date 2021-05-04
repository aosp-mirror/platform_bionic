#pragma once

#include <netdb.h>
#include <sys/cdefs.h>

/* this structure contains all the variables that were declared
 * 'static' in the original NetBSD resolver code.
 *
 * this caused vast amounts of crashes and memory corruptions
 * when the resolver was being used by multiple threads.
 *
 * (note: the OpenBSD/FreeBSD resolver has similar 'issues')
 */

#define	MAXALIASES	35
#define	MAXADDRS	35

__BEGIN_DECLS

struct res_static {
  char* h_addr_ptrs[MAXADDRS + 1];
  char* host_aliases[MAXALIASES];
  char hostbuf[8 * 1024];
  u_int32_t host_addr[16 / sizeof(u_int32_t)]; /* IPv4 or IPv6 */
  FILE* hostf;
  int stayopen;
  const char* servent_ptr;
  struct servent servent;
  struct hostent host;
};

struct res_static* __res_get_static(void);

__END_DECLS
