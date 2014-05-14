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

static pthread_mutex_t g_atfork_list_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
static atfork_list_t g_atfork_list = { NULL, NULL };

void __bionic_atfork_run_prepare() {
  // We lock the atfork list here, unlock it in the parent, and reset it in the child.
  // This ensures that nobody can modify the handler array between the calls
  // to the prepare and parent/child handlers.
  //
  // TODO: If a handler tries to mutate the list, they'll block. We should probably copy
  // the list before forking, and have prepare, parent, and child all work on the consistent copy.
  pthread_mutex_lock(&g_atfork_list_mutex);

  // Call pthread_atfork() prepare handlers. POSIX states that the prepare
  // handlers should be called in the reverse order of the parent/child
  // handlers, so we iterate backwards.
  for (atfork_t* it = g_atfork_list.last; it != NULL; it = it->prev) {
    if (it->prepare != NULL) {
      it->prepare();
    }
  }
}

void __bionic_atfork_run_child() {
  for (atfork_t* it = g_atfork_list.first; it != NULL; it = it->next) {
    if (it->child != NULL) {
      it->child();
    }
  }

  g_atfork_list_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
}

void __bionic_atfork_run_parent() {
  for (atfork_t* it = g_atfork_list.first; it != NULL; it = it->next) {
    if (it->parent != NULL) {
      it->parent();
    }
  }

  pthread_mutex_unlock(&g_atfork_list_mutex);
}

int pthread_atfork(void (*prepare)(void), void (*parent)(void), void(*child)(void)) {
  atfork_t* entry = reinterpret_cast<atfork_t*>(malloc(sizeof(atfork_t)));
  if (entry == NULL) {
    return ENOMEM;
  }

  entry->prepare = prepare;
  entry->parent = parent;
  entry->child = child;

  pthread_mutex_lock(&g_atfork_list_mutex);

  // Append 'entry' to the list.
  entry->next = NULL;
  entry->prev = g_atfork_list.last;
  if (entry->prev != NULL) {
    entry->prev->next = entry;
  }
  if (g_atfork_list.first == NULL) {
    g_atfork_list.first = entry;
  }
  g_atfork_list.last = entry;

  pthread_mutex_unlock(&g_atfork_list_mutex);

  return 0;
}
