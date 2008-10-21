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

#include "resolv_cache.h"
#include <stdlib.h>
#include <time.h>
#include "pthread.h"

/* this code implements a small DNS resolver cache for all gethostbyname* functions
 *
 * the cache is shared between all threads of the current process, but the result of
 * a succesful lookup is always copied to a thread-local variable to honor the persistence
 * rules of the gethostbyname*() APIs.
 */

/* the name of an environment variable that will be checked the first time this code is called
 * if its value is "0", then the resolver cache is disabled.
 */
#define  CONFIG_ENV  "BIONIC_DNSCACHE"

/* entries older than CONFIG_SECONDS seconds are always discarded. otherwise we could keep
 * stale addresses in the cache and be oblivious to DNS server changes
 */
#define  CONFIG_SECONDS    (60*10)    /* 10 minutes */

/* maximum number of entries kept in the cache. be frugal, this is the C library
 * this MUST BE A POWER OF 2
 */
#define  CONFIG_MAX_ENTRIES   128

/****************************************************************************/
/****************************************************************************/
/*****                                                                  *****/
/*****                                                                  *****/
/*****                                                                  *****/
/****************************************************************************/
/****************************************************************************/

#define  DEBUG  0

#if DEBUG
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
static void
xlog( const char*  fmt, ... )
{
    static int  fd = -2;
    int         ret;

    if (fd == -2) {
        do {
            fd = open( "/data/dns.log", O_CREAT | O_APPEND | O_WRONLY, 0666 );
        } while (fd < 0 && errno == EINTR);
    }

    if (fd >= 0) {
        char  temp[128];
        va_list  args;
        va_start(args, fmt);
        vsnprintf( temp, sizeof(temp), fmt, args);
        va_end(args);

        do {
            ret = write( fd, temp, strlen(temp) );
        } while (ret == -1 && errno == EINTR);
    }
}
#define  XLOG(...)   xlog(__VA_ARGS__)
#else
#define  XLOG(...)   ((void)0)
#endif


static time_t
_time_now( void )
{
    struct timeval  tv;

    gettimeofday( &tv, NULL );
    return tv.tv_sec;
}

/****************************************************************************/
/****************************************************************************/
/*****                                                                  *****/
/*****                                                                  *****/
/*****                                                                  *****/
/****************************************************************************/
/****************************************************************************/

/* used to define the content of _RESOLV_HOSTENT_NONE
 */
const struct hostent  _resolv_hostent_none_cst = {
    NULL,
    NULL,
    AF_INET,
    4,
    NULL
};

struct hostent*
_resolv_hostent_copy( struct hostent*  hp )
{
    struct hostent*  dst;
    int              nn, len;
    char*            p;
    int              num_aliases = 0, num_addresses = 0;

    if (hp == NULL)
        return NULL;

    if (hp == _RESOLV_HOSTENT_NONE)
        return hp;

    len  = sizeof(*hp);
    len += strlen(hp->h_name) + 1;

    if (hp->h_aliases != NULL) {
        for (nn = 0; hp->h_aliases[nn] != NULL; nn++)
            len += sizeof(char*) + strlen(hp->h_aliases[nn]) + 1;
        num_aliases = nn;
    }
    len += sizeof(char*);

    for (nn = 0; hp->h_addr_list[nn] != NULL; nn++) {
        len += sizeof(char*) + hp->h_length;
    }
    num_addresses = nn;
    len += sizeof(char*);

    dst = malloc( len );
    if (dst == NULL)
        return NULL;

    dst->h_aliases   = (char**)(dst + 1);
    dst->h_addr_list = dst->h_aliases + num_aliases + 1;
    dst->h_length    = hp->h_length;
    dst->h_addrtype  = hp->h_addrtype;

    p = (char*)(dst->h_addr_list + num_addresses + 1);

    /* write the addresses first, to help with alignment issues */
    for (nn = 0; nn < num_addresses; nn++) {
        dst->h_addr_list[nn] = p;
        len = hp->h_length;
        memcpy( p, hp->h_addr_list[nn], len );
        p += len;
    }
    dst->h_addr_list[nn] = NULL;

    for (nn = 0; nn < num_aliases; nn++) {
        dst->h_aliases[nn] = p;
        len = strlen(hp->h_aliases[nn]) + 1;
        memcpy( p, hp->h_aliases[nn], len );
        p += len;
    }
    dst->h_aliases[nn] = NULL;

    dst->h_name      = p;
    len = strlen(hp->h_name) + 1;
    memcpy(p, hp->h_name, len);
    p += len;

    return dst;
}


void
_resolv_hostent_free( struct hostent*  hp )
{
    if (hp && hp != _RESOLV_HOSTENT_NONE)
        free(hp);
}

/****************************************************************************/
/****************************************************************************/
/*****                                                                  *****/
/*****                                                                  *****/
/*****                                                                  *****/
/****************************************************************************/
/****************************************************************************/

typedef struct Entry {
    unsigned int     hash;
    const char*      name;
    short            af;
    short            index;
    struct Entry*    mru_prev;
    struct Entry*    mru_next;
    time_t           when;
    struct hostent*  hp;
} Entry;


static void
entry_free( Entry*  e )
{
    /* everything is allocated in a single memory block */
    if (e) {
        _resolv_hostent_free(e->hp);
        free(e);
    }
}

static void
entry_init_key( Entry*  e, const char*  name, int  af )
{
    unsigned     h = 0;
    const char*  p = name;

    /* compute hash */
    p = name;
    while (*p) {
        h = h*33 + *p++;
    }
    h += af*17;

    e->hash = h;
    e->name = name;
    e->af   = (short) af;
}


static __inline__ void
entry_mru_remove( Entry*  e )
{
    e->mru_prev->mru_next = e->mru_next;
    e->mru_next->mru_prev = e->mru_prev;
}

static __inline__ void
entry_mru_add( Entry*  e, Entry*  list )
{
    Entry*  first = list->mru_next;

    e->mru_next = first;
    e->mru_prev = list;

    list->mru_next  = e;
    first->mru_prev = e;
}


static Entry*
entry_alloc( const char*  name, int  af, int  index, struct hostent*  hp )
{
    Entry*  e;
    int     num_aliases   = 0;
    int     num_addresses = 0;
    char**  aliases;
    char**  addresses;

    /* compute the length of the memory block that will contain everything */
    int   len = sizeof(*e) + strlen(name)+1;

    e = malloc(len);
    if (e == NULL)
        return e;

    entry_init_key(e, name, af);

    e->mru_next = e->mru_prev = e;
    e->index    = (short) index;
    e->when     = _time_now();
    e->hp       = _resolv_hostent_copy(hp);

    if (e->hp == NULL) {
        free(e);
        return NULL;
    }

    e->name = (char*)(e+1);
    len     = strlen(name)+1;
    memcpy( (char*)e->name, name, len );
    return e;
}


static __inline__ int
entry_equals( const Entry*  e1, const Entry*  e2 )
{
    return e1->hash == e2->hash && e1->af == e2->af && !strcmp( e1->name, e2->name );
}

/****************************************************************************/
/****************************************************************************/
/*****                                                                  *****/
/*****                                                                  *****/
/*****                                                                  *****/
/****************************************************************************/
/****************************************************************************/

#define  MAX_HASH_ENTRIES   (2*CONFIG_MAX_ENTRIES)

typedef struct resolv_cache {
    int               num_entries;
    Entry             mru_list;
    pthread_mutex_t   lock;
    int               disabled;
    Entry*            entries[ MAX_HASH_ENTRIES ];      /* hash-table of pointers to entries */
} Cache;


void
_resolv_cache_destroy( struct resolv_cache*  cache )
{
    if (cache != NULL) {
        int  nn;
        for (nn = 0; nn < MAX_HASH_ENTRIES; nn++) {
            entry_free(cache->entries[nn]);
        }
        pthread_mutex_destroy(&cache->lock);
        free(cache);
    }
}


struct resolv_cache*
_resolv_cache_create( void )
{
    struct resolv_cache*  cache;

    cache = calloc(sizeof(*cache), 1);
    if (cache) {
        const char*  env = getenv(CONFIG_ENV);

        if (env && atoi(env) == 0)
            cache->disabled = 1;

        pthread_mutex_init( &cache->lock, NULL );
        cache->mru_list.mru_prev = cache->mru_list.mru_next = &cache->mru_list;
        XLOG("%s: cache=%p %s\n", __FUNCTION__, cache, cache->disabled ? "disabled" : "enabled" );
    }
    return cache;
}


static int
_resolv_cache_find_index( Cache*       cache,
                          const char*  name,
                          int          af )
{
    Entry  key;
    int    nn, step, tries;

    entry_init_key( &key, name, af );

    tries = MAX_HASH_ENTRIES;
    nn    = key.hash % MAX_HASH_ENTRIES;
    step  = 5;

    while (tries > 0) {
        Entry*  key2 = cache->entries[nn];

        if (key2 == NULL) {
            return -(nn + 1);
        }

        if (entry_equals( &key, key2 ) ) {
            return nn;
        }

        nn     = (nn + step) % MAX_HASH_ENTRIES;
        tries -= 1;
    }
    return -(MAX_HASH_ENTRIES+1);
}


static void
_resolv_cache_remove( struct resolv_cache*  cache,
                      Entry*                e )
{
    XLOG("%s: name='%s' af=%d\n", __FUNCTION__, e->name, e->af);
    cache->entries[ e->index ] = NULL;  /* remove from hash table */
    entry_mru_remove( e );
    entry_free( e );
    cache->num_entries -= 1;
}


struct hostent*
_resolv_cache_lookup( struct resolv_cache*  cache,
                      const char*           name,
                      int                   af )
{
    int               index;
    struct hostent*   result = NULL;

    if (cache->disabled)
        return NULL;

    pthread_mutex_lock( &cache->lock );

    XLOG( "%s: cache=%p name='%s' af=%d ", __FUNCTION__, cache, name, af );
    index = _resolv_cache_find_index( cache, name, af );
    if (index >= 0) {
        Entry*  e   = cache->entries[index];
        time_t  now = _time_now();
        struct hostent**  pht;

        /* ignore stale entries, they will be discarded in _resolv_cache_add */
        if ( (unsigned)(now - e->when) >= CONFIG_SECONDS ) {
            XLOG( " OLD\n" );
            goto Exit;
        }

        /* bump up this entry to the top of the MRU list */
        if (e != cache->mru_list.mru_next) {
            entry_mru_remove( e );
            entry_mru_add( e, &cache->mru_list );
        }

        /* now copy the result into a thread-local variable */
        pht = __get_res_cache_hostent_p();
        if (pht == NULL) {
            XLOG( " NOTLS\n" );  /* shouldn't happen */
            goto Exit;
        }

        if (pht[0]) {
            _resolv_hostent_free( pht[0] );  /* clear previous value */
            pht[0] = NULL;
        }
        result = _resolv_hostent_copy( e->hp );
        if (result == NULL) {
            XLOG( " NOMEM\n" );  /* bummer */
            goto Exit;
        }
        XLOG( " OK\n" );
        pht[0] = result;
        goto Exit;
    }
    XLOG( " KO\n" );
Exit:
    pthread_mutex_unlock( &cache->lock );
    return result;
}


void
_resolv_cache_add( struct resolv_cache*  cache,
                   const char*           name,
                   int                   af,
                   struct hostent*       hp )
{
    Entry*   e;
    int      index;

    if (cache->disabled)
        return;

    pthread_mutex_lock( &cache->lock );

    XLOG( "%s: cache=%p name='%s' af=%d\n", __FUNCTION__, cache, name, af);

    /* get rid of the oldest entry if needed */
    if (cache->num_entries >= CONFIG_MAX_ENTRIES) {
        Entry*  oldest = cache->mru_list.mru_prev;
        _resolv_cache_remove( cache, oldest );
    }

    index = _resolv_cache_find_index( cache, name, af );
    if (index >= 0) {
        /* discard stale entry */
        _resolv_cache_remove( cache, cache->entries[index] );
    } else {
        index = -(index+1);
        if (index >= MAX_HASH_ENTRIES)
            goto Exit;  /* should not happen */
    }

    e = entry_alloc( name, af, index, hp );
    if (e != NULL) {
        entry_mru_add( e, &cache->mru_list );
        cache->entries[index] = e;
        cache->num_entries   += 1;
    }
Exit:
    pthread_mutex_unlock( &cache->lock );
}

/****************************************************************************/
/****************************************************************************/
/*****                                                                  *****/
/*****                                                                  *****/
/*****                                                                  *****/
/****************************************************************************/
/****************************************************************************/

static struct resolv_cache*  _res_cache;
static pthread_once_t        _res_cache_once;

static void
_res_cache_init( void )
{
    _res_cache = _resolv_cache_create();
}


struct resolv_cache*
__get_res_cache( void )
{
    pthread_once( &_res_cache_once, _res_cache_init );
    return _res_cache;
}
