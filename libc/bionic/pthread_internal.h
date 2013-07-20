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
#ifndef _PTHREAD_INTERNAL_H_
#define _PTHREAD_INTERNAL_H_

#include <pthread.h>
#include <stdbool.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

typedef struct pthread_internal_t
{
    struct pthread_internal_t*  next;
    struct pthread_internal_t*  prev;
    pthread_attr_t              attr;
    pid_t                       tid;
    bool                        allocated_on_heap;
    pthread_cond_t              join_cond;
    void*                       return_value;
    int                         internal_flags;
    __pthread_cleanup_t*        cleanup_stack;
    void**                      tls;         /* thread-local storage area */

    void* alternate_signal_stack;

    /*
     * The dynamic linker implements dlerror(3), which makes it hard for us to implement this
     * per-thread buffer by simply using malloc(3) and free(3).
     */
#define __BIONIC_DLERROR_BUFFER_SIZE 512
    char dlerror_buffer[__BIONIC_DLERROR_BUFFER_SIZE];
} pthread_internal_t;

int _init_thread(pthread_internal_t* thread, bool add_to_thread_list);
void __init_tls(pthread_internal_t* thread);
void _pthread_internal_add(pthread_internal_t* thread);
pthread_internal_t* __get_thread(void);

__LIBC_HIDDEN__ void pthread_key_clean_all(void);
__LIBC_HIDDEN__ void _pthread_internal_remove_locked(pthread_internal_t* thread);

/* Has the thread been detached by a pthread_join or pthread_detach call? */
#define PTHREAD_ATTR_FLAG_DETACHED      0x00000001

/* Was the thread's stack allocated by the user rather than by us? */
#define PTHREAD_ATTR_FLAG_USER_STACK    0x00000002

/* Has the thread been joined by another thread? */
#define PTHREAD_ATTR_FLAG_JOINED        0x00000004

/* Has the thread already exited but not been joined? */
#define PTHREAD_ATTR_FLAG_ZOMBIE        0x00000008

__LIBC_HIDDEN__ extern pthread_internal_t* gThreadList;
__LIBC_HIDDEN__ extern pthread_mutex_t gThreadListLock;

/* needed by fork.c */
extern void __timer_table_start_stop(int  stop);
extern void __bionic_atfork_run_prepare();
extern void __bionic_atfork_run_child();
extern void __bionic_atfork_run_parent();

__END_DECLS

#endif /* _PTHREAD_INTERNAL_H_ */
