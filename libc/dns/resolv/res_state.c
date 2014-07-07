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
#include <sys/cdefs.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <netdb.h>
#include "resolv_private.h"
#include "resolv_cache.h"
#include <pthread.h>
#include <stdlib.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

/* Set to 1 to enable debug traces */
#define DEBUG 0

#if DEBUG
#  include "private/libc_logging.h"
#  include <unistd.h>  /* for gettid() */
#  define D(...) __libc_format_log(ANDROID_LOG_DEBUG,"libc", __VA_ARGS__)
#else
#  define D(...)  do{}while(0)
#endif

static pthread_key_t   _res_key;
static pthread_once_t  _res_once = PTHREAD_ONCE_INIT;

typedef struct {
    int                  _h_errno;
    // TODO: Have one __res_state per network so we don't have to repopulate frequently.
    struct __res_state  _nres[1];
    unsigned             _serial;
    struct prop_info*   _pi;
    struct res_static   _rstatic[1];
} _res_thread;

static _res_thread*
_res_thread_alloc(void)
{
    _res_thread*  rt = calloc(1, sizeof(*rt));

    if (rt) {
        rt->_h_errno = 0;
        /* Special system property which tracks any changes to 'net.*'. */
        rt->_serial = 0;
        rt->_pi = (struct prop_info*) __system_property_find("net.change");
        if (rt->_pi) {
            rt->_serial = __system_property_serial(rt->_pi);
        }
        memset(rt->_rstatic, 0, sizeof rt->_rstatic);
    }
    return rt;
}

static void
_res_static_done( res_static  rs )
{
    /* fortunately, there is nothing to do here, since the
     * points in h_addr_ptrs and host_aliases should all
     * point to 'hostbuf'
     */
    if (rs->hostf) {  /* should not happen in theory, but just be safe */
        fclose(rs->hostf);
        rs->hostf = NULL;
    }
    free(rs->servent.s_aliases);
}

static void
_res_thread_free( void*  _rt )
{
    _res_thread*  rt = _rt;

    D("%s: rt=%p for thread=%d", __FUNCTION__, rt, gettid());

    _res_static_done(rt->_rstatic);
    res_ndestroy(rt->_nres);
    free(rt);
}

static void
_res_init_key( void )
{
    pthread_key_create( &_res_key, _res_thread_free );
}

static _res_thread*
_res_thread_get(void)
{
    _res_thread*  rt;
    pthread_once( &_res_once, _res_init_key );
    rt = pthread_getspecific( _res_key );

    if (rt != NULL) {
        /* We already have one thread-specific DNS state object.
         * Check the serial value for any changes to net.* properties */
        D("%s: Called for tid=%d rt=%p rt->pi=%p rt->serial=%d",
           __FUNCTION__, gettid(), rt, rt->_pi, rt->_serial);
        if (rt->_pi == NULL) {
            /* The property wasn't created when _res_thread_get() was
             * called the last time. This should only happen very
             * early during the boot sequence. First, let's try to see if it
             * is here now. */
            rt->_pi = (struct prop_info*) __system_property_find("net.change");
            if (rt->_pi == NULL) {
                /* Still nothing, return current state */
                D("%s: exiting for tid=%d rt=%d since system property not found",
                  __FUNCTION__, gettid(), rt);
                return rt;
            }
        }
        if (rt->_serial == __system_property_serial(rt->_pi)) {
            /* Nothing changed, so return the current state */
            D("%s: tid=%d rt=%p nothing changed, returning",
              __FUNCTION__, gettid(), rt);
            return rt;
        }
        /* Update the recorded serial number, and go reset the state */
        rt->_serial = __system_property_serial(rt->_pi);
        goto RESET_STATE;
    }

    /* It is the first time this function is called in this thread,
     * we need to create a new thread-specific DNS resolver state. */
    rt = _res_thread_alloc();
    if (rt == NULL) {
        return NULL;
    }
    pthread_setspecific( _res_key, rt );
    D("%s: tid=%d Created new DNS state rt=%p",
      __FUNCTION__, gettid(), rt);

RESET_STATE:
    /* Reset the state, note that res_ninit() can now properly reset
     * an existing state without leaking memory.
     */
    D("%s: tid=%d, rt=%p, resetting DNS state (options RES_INIT=%d)",
      __FUNCTION__, gettid(), rt, (rt->_nres->options & RES_INIT) != 0);
    if ( res_ninit( rt->_nres ) < 0 ) {
        /* This should not happen */
        D("%s: tid=%d rt=%p, woot, res_ninit() returned < 0",
          __FUNCTION__, gettid(), rt);
        _res_thread_free(rt);
        pthread_setspecific( _res_key, NULL );
        return NULL;
    }
    return rt;
}

__LIBC_HIDDEN__
struct __res_state _nres;

#if 0
struct resolv_cache*
__get_res_cache(void)
{
    _res_thread*  rt = _res_thread_get();

    if (!rt)
        return NULL;

    if (!rt->_cache) {
        rt->_cache = _resolv_cache_create();
    }
    return rt->_cache;
}
#endif

int*
__get_h_errno(void)
{
    _res_thread*  rt    = _res_thread_get();
    static int    panic = NETDB_INTERNAL;

    return rt ? &rt->_h_errno : &panic;
}

res_state
__res_get_state(void)
{
    _res_thread*  rt = _res_thread_get();

    return rt ? rt->_nres : NULL;
}

void
__res_put_state(res_state res __unused)
{
    /* nothing to do */
}

res_static
__res_get_static(void)
{
    _res_thread*  rt = _res_thread_get();

    return rt ? rt->_rstatic : NULL;
}
