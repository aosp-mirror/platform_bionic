/*
 * Copyright (C) 2016 The Android Open Source Project
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

#pragma once

/**
 * @file sys/shm.h
 * @brief System V shared memory. Not useful on Android because it's disallowed by SELinux.
 */

#include <sys/cdefs.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include <linux/shm.h>

#define shmid_ds shmid64_ds
#define SHMLBA 4096

__BEGIN_DECLS

typedef unsigned long shmatt_t;

/** Not useful on Android; disallowed by SELinux. */
void* _Nonnull shmat(int __shm_id, const void* _Nullable __addr, int __flags) __INTRODUCED_IN(26);
/** Not useful on Android; disallowed by SELinux. */
int shmctl(int __shm_id, int __cmd, struct shmid_ds* _Nullable __buf) __INTRODUCED_IN(26);
/** Not useful on Android; disallowed by SELinux. */
int shmdt(const void* _Nonnull __addr) __INTRODUCED_IN(26);
/** Not useful on Android; disallowed by SELinux. */
int shmget(key_t __key, size_t __size, int __flags) __INTRODUCED_IN(26);

__END_DECLS
