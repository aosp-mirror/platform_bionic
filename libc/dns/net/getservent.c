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

#include <netdb.h>

#include <endian.h>
#include <stdlib.h>
#include <string.h>

#include "resolv_static.h"
#include "services.h"

struct servent* getservent_r(res_static rs) {
    const char*  p;
    const char*  q;
    int          namelen;
    int          nn,count;
    int          total = 0;
    int          port;
    char*        p2;

    p = rs->servent_ptr;
    if (p == NULL)
        p = _services;
    else if (p[0] == 0)
        return NULL;

    /* first compute the total size */
    namelen = p[0];
    total  += namelen + 1;
    q       = p + 1 + namelen + 3;  /* skip name + port + proto */
    count   = q[0];   /* get aliascount */
    q      += 1;

    total += (count+1)*sizeof(char*);
    for (nn = 0; nn < count; nn++) {
        int  len2 = q[0];
        total += 1 + len2;
        q     += 1 + len2;
    }

    /* reallocate the thread-specific servent struct */
    p2 = realloc( (char*)rs->servent.s_aliases, total );
    if (p2 == NULL)
        return NULL;

    /* now write to it */
    rs->servent.s_aliases = (char**) p2;
    p2                   += (count+1)*sizeof(char*);
    rs->servent.s_name    = p2;
    p2                   += namelen + 1;
    rs->servent.s_proto   = p2;

    /* copy name + port + setup protocol */
    memcpy( rs->servent.s_name, p+1, namelen );
    rs->servent.s_name[namelen] = 0;
    p += 1 + namelen;

    /* s_port must be in network byte order */
    port = ((((unsigned char*)p)[0] << 8) |
             ((unsigned char*)p)[1]);

    rs->servent.s_port  = htons(port);
    rs->servent.s_proto = p[2] == 't' ? "tcp" : "udp";
    p += 4;  /* skip port(2) + proto(1) + aliascount(1) */

    for (nn = 0; nn < count; nn++) {
        int  len2 = p[0];
        rs->servent.s_aliases[nn] = p2;
        memcpy( p2, p+1, len2 );
        p2[len2] = 0;
        p2 += len2 + 1;
        p  += len2 + 1;
    }
    rs->servent.s_aliases[nn] = NULL;

    rs->servent_ptr = p;

    return &rs->servent;
}

void setservent(int stayopen) {
  endservent();
}

void endservent(void) {
  res_static rs = __res_get_static();
  if (rs) rs->servent_ptr = NULL;
}

struct servent* getservent(void) {
  res_static rs = __res_get_static();
  return rs ? getservent_r(rs) : NULL;
}

struct servent* getservbyname(const char* name, const char* proto) {
  res_static rs = __res_get_static();
  if (rs == NULL) return NULL;

  const char* old_servent_ptr = rs->servent_ptr;
  rs->servent_ptr = NULL;
  struct servent* s;
  while ((s = getservent_r(rs)) != NULL) {
    if (strcmp(s->s_name, name) == 0 && (proto == NULL || strcmp(s->s_proto, proto) == 0)) {
      break;
    }
  }
  rs->servent_ptr = old_servent_ptr;
  return s;
}

struct servent* getservbyport(int port, const char* proto) {
  res_static rs = __res_get_static();
  if (rs == NULL) return NULL;

  const char* old_servent_ptr = rs->servent_ptr;
  rs->servent_ptr = NULL;
  struct servent* s;
  while ((s = getservent_r(rs)) != NULL) {
    if (s->s_port == port && (proto == NULL || strcmp(s->s_proto, proto) == 0)) {
      break;
    }
  }
  rs->servent_ptr = old_servent_ptr;
  return s;
}
