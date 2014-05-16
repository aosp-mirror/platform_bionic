/*	$OpenBSD: atexit.c,v 1.14 2007/09/05 20:47:47 chl Exp $ */
/*
 * Copyright (c) 2002 Daniel Hartmeier
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "atexit.h"
#include "private/thread_private.h"

int __atexit_invalid = 1;
struct atexit *__atexit;

/*
 * TODO: Read this before upstreaming:
 *
 * As of Apr 2014 there is a bug regaring function type detection logic in
 * Free/Open/NetBSD implementations of __cxa_finalize().
 *
 * What it is about:
 * First of all there are two kind of atexit handlers:
 *  1) void handler(void) - this is the regular type
 *     available for to user via atexit(.) function call.
 *
 *  2) void internal_handler(void*) - this is the type
 *     __cxa_atexit() function expects. This handler is used
 *     by C++ compiler to register static destructor calls.
 *     Note that calling this function as the handler of type (1)
 *     results in incorrect this pointer in static d-tors.
 *
 * What is wrong with BSD implementations:
 *
 *  They use dso argument to identify the handler type. The problem
 *  with it is dso is also used to identify the handlers associated
 *  with particular dynamic library and allow __cxa_finalize to call correct
 *  set of functions on dlclose(). And it cannot identify both.
 *
 * What is correct way to identify function type?
 *
 *  Consider this:
 *  1. __cxa_finalize and __cxa_atexit are part of libc and do not have access to hidden
 *     &__dso_handle.
 *  2. __cxa_atexit has only 3 arguments: function pointer, function argument, dso.
 *     none of them can be reliably used to pass information about handler type.
 *  3. following http://www.codesourcery.com/cxx-abi/abi.html#dso-dtor (3.3.5.3 - B)
 *     translation of user atexit -> __cxa_atexit(f, NULL, NULL) results in crashes
 *     on exit() after dlclose() of a library with an atexit() call.
 *
 *  One way to resolve this is to always call second form of handler, which will
 *  result in storing unused argument in register/stack depending on architecture
 *  and should not present any problems.
 *
 *  Another way is to make them dso-local in one way or the other.
 */

/*
 * Function pointers are stored in a linked list of pages. The list
 * is initially empty, and pages are allocated on demand. The first
 * function pointer in the first allocated page (the last one in
 * the linked list) was reserved for the cleanup function.
 *
 * Outside the following functions, all pages are mprotect()'ed
 * to prevent unintentional/malicious corruption.
 */

/*
 * Register a function to be performed at exit or when a shared object
 * with the given dso handle is unloaded dynamically.  Also used as
 * the backend for atexit().  For more info on this API, see:
 *
 *	http://www.codesourcery.com/cxx-abi/abi.html#dso-dtor
 */
int
__cxa_atexit(void (*func)(void *), void *arg, void *dso)
{
	struct atexit *p = __atexit;
	struct atexit_fn *fnp;
	size_t pgsize = sysconf(_SC_PAGESIZE);
	int ret = -1;

	if (pgsize < sizeof(*p))
		return (-1);
	_ATEXIT_LOCK();
	p = __atexit;
	if (p != NULL) {
		if (p->ind + 1 >= p->max)
			p = NULL;
		else if (mprotect(p, pgsize, PROT_READ | PROT_WRITE))
			goto unlock;
	}
	if (p == NULL) {
		p = mmap(NULL, pgsize, PROT_READ | PROT_WRITE,
		    MAP_ANON | MAP_PRIVATE, -1, 0);
		if (p == MAP_FAILED)
			goto unlock;
		if (__atexit == NULL) {
			memset(&p->fns[0], 0, sizeof(p->fns[0]));
			p->ind = 1;
		} else
			p->ind = 0;
		p->max = (pgsize - ((char *)&p->fns[0] - (char *)p)) /
		    sizeof(p->fns[0]);
		p->next = __atexit;
		__atexit = p;
		if (__atexit_invalid)
			__atexit_invalid = 0;
	}
	fnp = &p->fns[p->ind++];
	fnp->cxa_func = func;
	fnp->fn_arg = arg;
	fnp->fn_dso = dso;
	if (mprotect(p, pgsize, PROT_READ))
		goto unlock;
	ret = 0;
unlock:
	_ATEXIT_UNLOCK();
	return (ret);
}

/*
 * Call all handlers registered with __cxa_atexit() for the shared
 * object owning 'dso'.
 * Note: if 'dso' is NULL, then all remaining handlers are called.
 */
void
__cxa_finalize(void *dso)
{
	struct atexit *p, *q, *original_atexit;
	struct atexit_fn fn;
	int n, pgsize = getpagesize(), original_ind;
	static int call_depth;

	if (__atexit_invalid)
		return;
	_ATEXIT_LOCK();
	call_depth++;

	p = original_atexit = __atexit;
	n = original_ind = p != NULL ? p->ind : 0;
	while (p != NULL) {
		if (p->fns[n].cxa_func != NULL /* not called */
				&& (dso == NULL || dso == p->fns[n].fn_dso)) { /* correct DSO */
			/*
			 * Mark handler as having been already called to avoid
			 * dupes and loops, then call the appropriate function.
			 */
			fn = p->fns[n];
			if (mprotect(p, pgsize, PROT_READ | PROT_WRITE) == 0) {
				p->fns[n].cxa_func = NULL;
				mprotect(p, pgsize, PROT_READ);
			}

			_ATEXIT_UNLOCK();
			(*fn.cxa_func)(fn.fn_arg);
			_ATEXIT_LOCK();
			// check for new atexit handlers
			if ((__atexit->ind != original_ind) || (__atexit != original_atexit)) {
				// need to restart now to preserve correct
				// call order - LIFO
				p = original_atexit = __atexit;
				n = original_ind = p->ind;
				continue;
			}
		}
		if (n == 0) {
			p = p->next;
			n = p != NULL ? p->ind : 0;
		} else {
			--n;
		}
	}

	--call_depth;

	/*
	 * If called via exit(), unmap the pages since we have now run
	 * all the handlers.  We defer this until calldepth == 0 so that
	 * we don't unmap things prematurely if called recursively.
	 */
	if (dso == NULL && call_depth == 0) {
		for (p = __atexit; p != NULL; ) {
			q = p;
			p = p->next;
			munmap(q, pgsize);
		}
		__atexit = NULL;
	}
	_ATEXIT_UNLOCK();
}

/*
 * Register the cleanup function
 */
void
__atexit_register_cleanup(void (*func)(void))
{
        struct atexit *p;
        size_t pgsize = sysconf(_SC_PAGESIZE);

        if (pgsize < sizeof(*p))
                return;
        _ATEXIT_LOCK();
        p = __atexit;
        while (p != NULL && p->next != NULL)
                p = p->next;
        if (p == NULL) {
                p = mmap(NULL, pgsize, PROT_READ | PROT_WRITE,
                    MAP_ANON | MAP_PRIVATE, -1, 0);
                if (p == MAP_FAILED)
                        goto unlock;
                p->ind = 1;
                p->max = (pgsize - ((char *)&p->fns[0] - (char *)p)) /
                    sizeof(p->fns[0]);
                p->next = NULL;
                __atexit = p;
        } else {
                if (mprotect(p, pgsize, PROT_READ | PROT_WRITE))
                        goto unlock;
        }
        p->fns[0].cxa_func = (void (*)(void*))func;
        p->fns[0].fn_arg = NULL;
        p->fns[0].fn_dso = NULL;
        mprotect(p, pgsize, PROT_READ);
unlock:
        _ATEXIT_UNLOCK();
}

