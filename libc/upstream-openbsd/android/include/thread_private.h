/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _THREAD_PRIVATE_H_
#define _THREAD_PRIVATE_H_

#include <pthread.h>

/* Note that these aren't compatible with the usual OpenBSD ones which lazy-initialize! */
#define _MUTEX_LOCK(l) pthread_mutex_lock((pthread_mutex_t*) l)
#define _MUTEX_UNLOCK(l) pthread_mutex_unlock((pthread_mutex_t*) l)

#endif /* _THREAD_PRIVATE_H_ */
