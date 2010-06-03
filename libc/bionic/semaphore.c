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
#include <semaphore.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/atomics.h>
#include <time.h>
#include <cutils/atomic-inline.h>
#include <bionic_futex.h>

/* Use the lower 31-bits for the counter, and the high bit for
 * the shared flag.
 */
#define SEM_VALUE_MASK  0x7fffffff
#define SEM_SHARED_MASK 0x80000000

#define SEM_GET_SHARED(sem)  ((sem)->count & SEM_SHARED_MASK)
#define SEM_GET_VALUE(sem)   ((sem)->count & SEM_VALUE_MASK)

int sem_init(sem_t *sem, int pshared, unsigned int value)
{
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* ensure that 'value' can be stored in the semaphore */
    if ((value & SEM_VALUE_MASK) != value) {
        errno = EINVAL;
        return -1;
    }

    sem->count = value;
    if (pshared != 0)
        sem->count |= SEM_SHARED_MASK;

    return 0;
}


int sem_destroy(sem_t *sem)
{
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    if ((sem->count & SEM_VALUE_MASK) == 0) {
        errno = EBUSY;
        return -1;
    }
    sem->count = 0;
    return 0;
}


sem_t *sem_open(const char *name, int oflag, ...)
{
    name=name;
    oflag=oflag;

    errno = ENOSYS;
    return SEM_FAILED;
}


int sem_close(sem_t *sem)
{
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    errno = ENOSYS;
    return -1;
}


int sem_unlink(const char * name)
{
    errno = ENOSYS;
    return -1;
}


/* Return 0 if a semaphore's value is 0
 * Otherwise, decrement the value and return the old value.
 */
static int
__sem_dec_if_positive(volatile unsigned int *pvalue)
{
    unsigned int  shared = (*pvalue & SEM_SHARED_MASK);
    unsigned int  old;

    do {
        old = (*pvalue & SEM_VALUE_MASK);
    }
    while ( old != 0 &&
            __atomic_cmpxchg((int)(old|shared),
                             (int)((old-1)|shared),
                             (volatile int*)pvalue) != 0 );

    return old;
}

/* Increment the value of a semaphore atomically.
 * NOTE: the value will wrap above SEM_VALUE_MASK
 */
static int
__sem_inc(volatile unsigned int *pvalue)
{
    unsigned int  shared = (*pvalue & SEM_SHARED_MASK);
    unsigned int  old;

    do {
        old = (*pvalue & SEM_VALUE_MASK);
    } while ( __atomic_cmpxchg((int)(old|shared),
                               (int)(((old+1)&SEM_VALUE_MASK)|shared),
                               (volatile int*)pvalue) != 0);
    return old;
}

/* lock a semaphore */
int sem_wait(sem_t *sem)
{
    unsigned shared;

    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }

    shared = SEM_GET_SHARED(sem);

    for (;;) {
        if (__sem_dec_if_positive(&sem->count))
            break;

        __futex_wait_ex(&sem->count, shared, shared, NULL);
    }
    ANDROID_MEMBAR_FULL();
    return 0;
}

int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout)
{
    int  ret;
    unsigned int shared;

    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* POSIX says we need to try to decrement the semaphore
     * before checking the timeout value */
    if (__sem_dec_if_positive(&sem->count)) {
        ANDROID_MEMBAR_FULL();
        return 0;
    }

    /* check it as per Posix */
    if (abs_timeout == NULL    ||
        abs_timeout->tv_sec < 0 ||
        abs_timeout->tv_nsec < 0 ||
        abs_timeout->tv_nsec >= 1000000000)
    {
        errno = EINVAL;
        return -1;
    }

    shared = SEM_GET_SHARED(sem);

    for (;;) {
        struct timespec ts;
        int             ret;

        /* Posix mandates CLOCK_REALTIME here */
        clock_gettime( CLOCK_REALTIME, &ts );
        ts.tv_sec  = abs_timeout->tv_sec - ts.tv_sec;
        ts.tv_nsec = abs_timeout->tv_nsec - ts.tv_nsec;
        if (ts.tv_nsec < 0) {
            ts.tv_nsec += 1000000000;
            ts.tv_sec  -= 1;
        }

        if (ts.tv_sec < 0 || ts.tv_nsec < 0) {
            errno = ETIMEDOUT;
            return -1;
        }

        ret = __futex_wait_ex(&sem->count, shared, shared, &ts);

        /* return in case of timeout or interrupt */
        if (ret == -ETIMEDOUT || ret == -EINTR) {
            errno = -ret;
            return -1;
        }

        if (__sem_dec_if_positive(&sem->count)) {
            ANDROID_MEMBAR_FULL();
            break;
        }
    }
    return 0;
}

/* unlock a semaphore */
int sem_post(sem_t *sem)
{
    unsigned int shared;

    if (sem == NULL)
        return EINVAL;

    shared = SEM_GET_SHARED(sem);

    ANDROID_MEMBAR_FULL();
    if (__sem_inc(&sem->count) >= 0)
        __futex_wake_ex(&sem->count, shared, 1);

    return 0;
}

int  sem_trywait(sem_t *sem)
{
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (__sem_dec_if_positive(&sem->count) > 0) {
        ANDROID_MEMBAR_FULL();
        return 0;
    } else {
        errno = EAGAIN;
        return -1;
    }
}

int  sem_getvalue(sem_t *sem, int *sval)
{
    if (sem == NULL || sval == NULL) {
        errno = EINVAL;
        return -1;
    }

    *sval = SEM_GET_VALUE(sem);
    return 0;
}
