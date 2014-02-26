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

#include <errno.h>
#include <pthread.h>

static pthread_mutex_t gAtForkListMutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;

struct atfork_t {
  atfork_t* next;
  atfork_t* prev;

  void (*prepare)(void);
  void (*child)(void);
  void (*parent)(void);
};

struct atfork_list_t {
  atfork_t* first;
  atfork_t* last;
};

static atfork_list_t gAtForkList = { NULL, NULL };

void __bionic_atfork_run_prepare() {
  // We will lock this here, and unlock it in the parent and child functions.
  // This ensures that nobody can modify the handler array between the calls
  // to the prepare and parent/child handlers.
  //
  // TODO: If a handler mucks with the list, it could cause problems.  Right
  //       now it's ok because all they can do is add new items to the end
  //       of the list, but if/when we implement cleanup in dlclose() things
  //       will get more interesting...
  pthread_mutex_lock(&gAtForkListMutex);

  // Call pthread_atfork() prepare handlers. POSIX states that the prepare
  // handlers should be called in the reverse order of the parent/child
  // handlers, so we iterate backwards.
  for (atfork_t* it = gAtForkList.last; it != NULL; it = it->prev) {
    if (it->prepare != NULL) {
      it->prepare();
    }
  }
}

void __bionic_atfork_run_child() {
  for (atfork_t* it = gAtForkList.first; it != NULL; it = it->next) {
    if (it->child != NULL) {
      it->child();
    }
  }

  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&gAtForkListMutex, &attr);
}

void __bionic_atfork_run_parent() {
  for (atfork_t* it = gAtForkList.first; it != NULL; it = it->next) {
    if (it->parent != NULL) {
      it->parent();
    }
  }

  pthread_mutex_unlock(&gAtForkListMutex);
}

int pthread_atfork(void (*prepare)(void), void (*parent)(void), void(*child)(void)) {
  atfork_t* entry = reinterpret_cast<atfork_t*>(malloc(sizeof(atfork_t)));
  if (entry == NULL) {
    return ENOMEM;
  }

  entry->prepare = prepare;
  entry->parent = parent;
  entry->child = child;

  pthread_mutex_lock(&gAtForkListMutex);

  // Append 'entry' to the list.
  entry->next = NULL;
  entry->prev = gAtForkList.last;
  if (entry->prev != NULL) {
    entry->prev->next = entry;
  }
  if (gAtForkList.first == NULL) {
    gAtForkList.first = entry;
  }
  gAtForkList.last = entry;

  pthread_mutex_unlock(&gAtForkListMutex);

  return 0;
}
