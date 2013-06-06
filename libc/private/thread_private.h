/* $OpenBSD: thread_private.h,v 1.18 2006/02/22 07:16:31 otto Exp $ */

/* PUBLIC DOMAIN: No Rights Reserved. Marco S Hyman <marc@snafu.org> */

#ifndef _THREAD_PRIVATE_H_
#define _THREAD_PRIVATE_H_

#include <pthread.h>

/*
 * This file defines the thread library interface to libc.  Thread
 * libraries must implement the functions described here for proper
 * inter-operation with libc.   libc contains weak versions of the
 * described functions for operation in a non-threaded environment.
 */

/*
 * This variable is 0 until a second thread is created.
 */
extern int __isthreaded;

/*
 * helper macro to make unique names in the thread namespace
 */
#define __THREAD_NAME(name)	__CONCAT(_thread_tagname_,name)

struct __thread_private_tag_t {
    pthread_mutex_t    _private_lock;
    pthread_key_t      _private_key;
};

#define _THREAD_PRIVATE_MUTEX(name)  \
	static struct __thread_private_tag_t  __THREAD_NAME(name) = { PTHREAD_MUTEX_INITIALIZER, -1 }
#define _THREAD_PRIVATE_MUTEX_LOCK(name)  \
	pthread_mutex_lock( &__THREAD_NAME(name)._private_lock )
#define _THREAD_PRIVATE_MUTEX_UNLOCK(name) \
	pthread_mutex_unlock( &__THREAD_NAME(name)._private_lock )

void	_thread_atexit_lock(void);
void	_thread_atexit_unlock(void);

#define _ATEXIT_LOCK()		do {					\
					if (__isthreaded)		\
						_thread_atexit_lock();	\
				} while (0)
#define _ATEXIT_UNLOCK()	do {					\
					if (__isthreaded)		\
						_thread_atexit_unlock();\
				} while (0)

#endif /* _THREAD_PRIVATE_H_ */
