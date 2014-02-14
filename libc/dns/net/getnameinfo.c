/*	$NetBSD: getnameinfo.c,v 1.53 2012/09/26 23:13:00 christos Exp $	*/
/*	$KAME: getnameinfo.c,v 1.45 2000/09/25 22:43:56 itojun Exp $	*/

/*
 * Copyright (c) 2000 Ben Harris.
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Issues to be discussed:
 * - Thread safe-ness must be checked
 * - RFC2553 says that we should raise error on short buffer.  X/Open says
 *   we need to truncate the result.  We obey RFC2553 (and X/Open should be
 *   modified).  ipngwg rough consensus seems to follow RFC2553.
 * - What is "local" in NI_FQDN?
 * - NI_NAMEREQD and NI_NUMERICHOST conflict with each other.
 * - (KAME extension) always attach textual scopeid (fe80::1%lo0), if
 *   sin6_scope_id is filled - standardization status?
 *   XXX breaks backward compat for code that expects no scopeid.
 *   beware on merge.
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$NetBSD: getnameinfo.c,v 1.53 2012/09/26 23:13:00 christos Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <net/if.h>
#include <net/if_ieee1394.h>
#include <net/if_types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/nameser.h>
#include "resolv_netid.h"
#include "resolv_private.h"
#include <sys/system_properties.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>

static const struct afd {
	int		a_af;
	socklen_t	a_addrlen;
	socklen_t	a_socklen;
	int		a_off;
} afdl [] = {
#ifdef INET6
	{PF_INET6, sizeof(struct in6_addr), sizeof(struct sockaddr_in6),
		offsetof(struct sockaddr_in6, sin6_addr)},
#endif
	{PF_INET, sizeof(struct in_addr), sizeof(struct sockaddr_in),
		offsetof(struct sockaddr_in, sin_addr)},
	{0, 0, 0, 0},
};

struct sockinet {
	u_char	si_len;
	u_char	si_family;
	u_short	si_port;
};

static int getnameinfo_inet(const struct sockaddr *, socklen_t, char *,
    socklen_t, char *, socklen_t, int, unsigned, unsigned);
#ifdef INET6
static int ip6_parsenumeric(const struct sockaddr *, const char *, char *,
				 socklen_t, int);
static int ip6_sa2str(const struct sockaddr_in6 *, char *, size_t, int);
#endif
static int getnameinfo_local(const struct sockaddr *, socklen_t, char *,
    socklen_t, char *, socklen_t, int);

/*
 * Top-level getnameinfo() code.  Look at the address family, and pick an
 * appropriate function to call.
 */
int getnameinfo(const struct sockaddr* sa, socklen_t salen, char* host, size_t hostlen,
		char* serv, size_t servlen, int flags)
{
	return android_getnameinfofornet(sa, salen, host, hostlen, serv, servlen, flags,
			NETID_UNSET, MARK_UNSET);
}

int android_getnameinfofornet(const struct sockaddr* sa, socklen_t salen, char* host,
		size_t hostlen, char* serv, size_t servlen, int flags, unsigned netid,
		unsigned mark)
{
	switch (sa->sa_family) {
	case AF_INET:
	case AF_INET6:
		return getnameinfo_inet(sa, salen, host, hostlen,
				serv, servlen, flags, netid, mark);
	case AF_LOCAL:
		return getnameinfo_local(sa, salen, host, hostlen,
		    serv, servlen, flags);
	default:
		return EAI_FAMILY;
	}
}

/*
 * getnameinfo_local():
 * Format an local address into a printable format.
 */
/* ARGSUSED */
static int
getnameinfo_local(const struct sockaddr *sa, socklen_t salen,
    char *host, socklen_t hostlen, char *serv, socklen_t servlen,
    int flags __attribute__((unused)))
{
       const struct sockaddr_un *sun =
           (const struct sockaddr_un *)(const void *)sa;

       if (salen < (socklen_t) offsetof(struct sockaddr_un, sun_path)) {
           return EAI_FAMILY;
       }

       if (serv != NULL && servlen > 0)
               serv[0] = '\0';

       if (host && hostlen > 0)
               strlcpy(host, sun->sun_path,
                   MIN((socklen_t) sizeof(sun->sun_path) + 1, hostlen));

       return 0;
}

/*
 * getnameinfo_inet():
 * Format an IPv4 or IPv6 sockaddr into a printable string.
 */
static int
getnameinfo_inet(const struct sockaddr* sa, socklen_t salen,
       char *host, socklen_t hostlen,
       char *serv, socklen_t servlen,
       int flags, unsigned netid, unsigned mark)
{
	const struct afd *afd;
	struct servent *sp;
	struct hostent *hp;
	u_short port;
	int family, i;
	const char *addr;
	uint32_t v4a;
	char numserv[512];
	char numaddr[512];

	/* sa is checked below */
	/* host may be NULL */
	/* serv may be NULL */

	if (sa == NULL)
		return EAI_FAIL;

	family = sa->sa_family;
	for (i = 0; afdl[i].a_af; i++)
		if (afdl[i].a_af == family) {
			afd = &afdl[i];
			goto found;
		}
	return EAI_FAMILY;

 found:
	// http://b/1889275: callers should be allowed to provide too much
	// space, but not too little.
	if (salen < afd->a_socklen) {
		return EAI_FAMILY;
	}

	/* network byte order */
	port = ((const struct sockinet *)(const void *)sa)->si_port;
	addr = (const char *)(const void *)sa + afd->a_off;

	if (serv == NULL || servlen == 0) {
		/*
		 * do nothing in this case.
		 * in case you are wondering if "&&" is more correct than
		 * "||" here: rfc2553bis-03 says that serv == NULL OR
		 * servlen == 0 means that the caller does not want the result.
		 */
	} else {
		if (flags & NI_NUMERICSERV)
			sp = NULL;
		else {
			sp = getservbyport(port,
				(flags & NI_DGRAM) ? "udp" : "tcp");
		}
		if (sp) {
			if (strlen(sp->s_name) + 1 > (size_t)servlen)
				return EAI_MEMORY;
			strlcpy(serv, sp->s_name, servlen);
		} else {
			snprintf(numserv, sizeof(numserv), "%u", ntohs(port));
			if (strlen(numserv) + 1 > (size_t)servlen)
				return EAI_MEMORY;
			strlcpy(serv, numserv, servlen);
		}
	}

	switch (sa->sa_family) {
	case AF_INET:
		v4a = (uint32_t)
		    ntohl(((const struct sockaddr_in *)
		    (const void *)sa)->sin_addr.s_addr);
		if (IN_MULTICAST(v4a) || IN_EXPERIMENTAL(v4a))
			flags |= NI_NUMERICHOST;
		v4a >>= IN_CLASSA_NSHIFT;
		if (v4a == 0)
			flags |= NI_NUMERICHOST;
		break;
#ifdef INET6
	case AF_INET6:
	    {
		const struct sockaddr_in6 *sin6;
		sin6 = (const struct sockaddr_in6 *)(const void *)sa;
		switch (sin6->sin6_addr.s6_addr[0]) {
		case 0x00:
			if (IN6_IS_ADDR_V4MAPPED(&sin6->sin6_addr))
				;
			else if (IN6_IS_ADDR_LOOPBACK(&sin6->sin6_addr))
				;
			else
				flags |= NI_NUMERICHOST;
			break;
		default:
			if (IN6_IS_ADDR_LINKLOCAL(&sin6->sin6_addr)) {
				flags |= NI_NUMERICHOST;
			}
			else if (IN6_IS_ADDR_MULTICAST(&sin6->sin6_addr))
				flags |= NI_NUMERICHOST;
			break;
		}
	    }
		break;
#endif
	}
	if (host == NULL || hostlen == 0) {
		/*
		 * do nothing in this case.
		 * in case you are wondering if "&&" is more correct than
		 * "||" here: rfc2553bis-03 says that host == NULL or
		 * hostlen == 0 means that the caller does not want the result.
		 */
	} else if (flags & NI_NUMERICHOST) {
		size_t numaddrlen;

		/* NUMERICHOST and NAMEREQD conflicts with each other */
		if (flags & NI_NAMEREQD)
			return EAI_NONAME;

		switch(afd->a_af) {
#ifdef INET6
		case AF_INET6:
		{
			int error;

			if ((error = ip6_parsenumeric(sa, addr, host,
						      hostlen, flags)) != 0)
				return(error);
			break;
		}
#endif
		default:
			if (inet_ntop(afd->a_af, addr, numaddr, sizeof(numaddr))
			    == NULL)
				return EAI_SYSTEM;
			numaddrlen = strlen(numaddr);
			if (numaddrlen + 1 > (size_t)hostlen) /* don't forget terminator */
				return EAI_MEMORY;
			strlcpy(host, numaddr, hostlen);
			break;
		}
	} else {
		hp = android_gethostbyaddrfornet_proxy(addr, afd->a_addrlen, afd->a_af, netid);
		if (hp) {
#if 0
			/*
			 * commented out, since "for local host" is not
			 * implemented here - see RFC2553 p30
			 */
			if (flags & NI_NOFQDN) {
				char *p;
				p = strchr(hp->h_name, '.');
				if (p)
					TODO: Before uncommenting rewrite to avoid modifying hp.
					*p = '\0';
			}
#endif
			if (strlen(hp->h_name) + 1 > (size_t)hostlen) {
				return EAI_MEMORY;
			}
			strlcpy(host, hp->h_name, hostlen);
		} else {
			if (flags & NI_NAMEREQD)
				return EAI_NONAME;
			switch(afd->a_af) {
#ifdef INET6
			case AF_INET6:
			{
				int error;

				if ((error = ip6_parsenumeric(sa, addr, host,
							      hostlen,
							      flags)) != 0)
					return(error);
				break;
			}
#endif
			default:
				if (inet_ntop(afd->a_af, addr, host,
				    hostlen) == NULL)
					return EAI_SYSTEM;
				break;
			}
		}
	}
	return(0);
}

#ifdef INET6
static int
ip6_parsenumeric(const struct sockaddr *sa, const char *addr, char *host,
       socklen_t hostlen, int flags)
{
	size_t numaddrlen;
	char numaddr[512];

	assert(sa != NULL);
	assert(addr != NULL);
	assert(host != NULL);

	if (inet_ntop(AF_INET6, addr, numaddr, sizeof(numaddr)) == NULL)
		return EAI_SYSTEM;

	numaddrlen = strlen(numaddr);
	if (numaddrlen + 1 > (size_t)hostlen) /* don't forget terminator */
		return EAI_OVERFLOW;
	strlcpy(host, numaddr, hostlen);

	if (((const struct sockaddr_in6 *)(const void *)sa)->sin6_scope_id) {
		char zonebuf[MAXHOSTNAMELEN];
		int zonelen;

		zonelen = ip6_sa2str(
		    (const struct sockaddr_in6 *)(const void *)sa,
		    zonebuf, sizeof(zonebuf), flags);
		if (zonelen < 0)
			return EAI_OVERFLOW;
		if ((size_t) zonelen + 1 + numaddrlen + 1 > (size_t)hostlen)
			return EAI_OVERFLOW;
		/* construct <numeric-addr><delim><zoneid> */
		memcpy(host + numaddrlen + 1, zonebuf,
		    (size_t)zonelen);
		host[numaddrlen] = SCOPE_DELIMITER;
		host[numaddrlen + 1 + zonelen] = '\0';
	}

	return 0;
}

/* ARGSUSED */
static int
ip6_sa2str(const struct sockaddr_in6 *sa6, char *buf, size_t bufsiz, int flags)
{
	unsigned int ifindex;
	const struct in6_addr *a6;
	int n;

	assert(sa6 != NULL);
	assert(buf != NULL);

	ifindex = (unsigned int)sa6->sin6_scope_id;
	a6 = &sa6->sin6_addr;

#ifdef NI_NUMERICSCOPE
	if ((flags & NI_NUMERICSCOPE) != 0) {
		n = snprintf(buf, bufsiz, "%u", sa6->sin6_scope_id);
		if (n < 0 || n >= bufsiz)
			return -1;
		else
			return n;
	}
#endif

	/* if_indextoname() does not take buffer size.  not a good api... */
	if ((IN6_IS_ADDR_LINKLOCAL(a6) || IN6_IS_ADDR_MC_LINKLOCAL(a6)) &&
	    bufsiz >= IF_NAMESIZE) {
		char *p = if_indextoname(ifindex, buf);
		if (p) {
			return(strlen(p));
		}
	}

	/* last resort */
	n = snprintf(buf, bufsiz, "%u", sa6->sin6_scope_id);
	if (n < 0 || (size_t) n >= bufsiz)
		return -1;
	else
		return n;
}
#endif /* INET6 */
