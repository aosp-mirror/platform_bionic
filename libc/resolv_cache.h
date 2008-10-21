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
#ifndef _RESOLV_CACHE_H_
#define _RESOLV_CACHE_H_

#include <netdb.h>

const struct hostent  _resolv_hostent_none;
#define _RESOLV_HOSTENT_NONE  ((struct hostent*)&_resolv_hostent_none)

struct resolv_cache;  /* forward */
extern struct resolv_cache*  __get_res_cache(void);
extern struct hostent**      __get_res_cache_hostent_p(void);

extern struct resolv_cache*  _resolv_cache_get( void );

extern struct resolv_cache*  _resolv_cache_create( void );

extern void                  _resolv_cache_destroy( struct resolv_cache*  cache );

extern struct hostent*       _resolv_cache_lookup( struct resolv_cache*  cache,
                                                   const char*           name,
                                                   int                   af );

extern void                  _resolv_cache_add( struct resolv_cache*  cache,
                                                const char*           name,
                                                int                   af,
                                                struct hostent*       hp );

extern struct hostent*       _resolv_hostent_copy( struct hostent*  hp );
extern void                  _resolv_hostent_free( struct hostent*  hp );

#endif /* _RESOLV_CACHE_H_ */
