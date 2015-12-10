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
#include <stdlib.h>

#include "private/bionic_macros.h"

struct atfork_t {
  atfork_t* next;
  atfork_t* prev;

  void (*prepare)(void);
  void (*child)(void);
  void (*parent)(void);

  void* dso_handle;
};

class atfork_list_t {
 public:
  constexpr atfork_list_t() : first_(nullptr), last_(nullptr) {}

  template<typename F>
  void walk_forward(F f) {
    for (atfork_t* it = first_; it != nullptr; it = it->next) {
      f(it);
    }
  }

  template<typename F>
  void walk_backwards(F f) {
    for (atfork_t* it = last_; it != nullptr; it = it->prev) {
      f(it);
    }
  }

  void push_back(atfork_t* entry) {
    entry->next = nullptr;
    entry->prev = last_;
    if (entry->prev != nullptr) {
      entry->prev->next = entry;
    }
    if (first_ == nullptr) {
      first_ = entry;
    }
    last_ = entry;
  }

  template<typename F>
  void remove_if(F predicate) {
    atfork_t* it = first_;
    while (it != nullptr) {
      if (predicate(it)) {
        atfork_t* entry = it;
        it = it->next;
        remove(entry);
      } else {
        it = it->next;
      }
    }
  }

 private:
  void remove(atfork_t* entry) {
    if (entry->prev != nullptr) {
      entry->prev->next = entry->next;
    } else {
      first_ = entry->next;
    }

    if (entry->next != nullptr) {
      entry->next->prev = entry->prev;
    } else {
      last_ = entry->prev;
    }

    free(entry);
  }

  atfork_t* first_;
  atfork_t* last_;

  DISALLOW_COPY_AND_ASSIGN(atfork_list_t);
};

static pthread_mutex_t g_atfork_list_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
static atfork_list_t g_atfork_list;

void __bionic_atfork_run_prepare() {
  // We lock the atfork list here, unlock it in the parent, and reset it in the child.
  // This ensures that nobody can modify the handler array between the calls
  // to the prepare and parent/child handlers.
  pthread_mutex_lock(&g_atfork_list_mutex);

  // Call pthread_atfork() prepare handlers. POSIX states that the prepare
  // handlers should be called in the reverse order of the parent/child
  // handlers, so we iterate backwards.
  g_atfork_list.walk_backwards([](atfork_t* it) {
    if (it->prepare != nullptr) {
      it->prepare();
    }
  });
}

void __bionic_atfork_run_child() {
  g_atfork_list.walk_forward([](atfork_t* it) {
    if (it->child != nullptr) {
      it->child();
    }
  });

  g_atfork_list_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
}

void __bionic_atfork_run_parent() {
  g_atfork_list.walk_forward([](atfork_t* it) {
    if (it->parent != nullptr) {
      it->parent();
    }
  });

  pthread_mutex_unlock(&g_atfork_list_mutex);
}

// __register_atfork is the name used by glibc
extern "C" int __register_atfork(void (*prepare)(void), void (*parent)(void),
                                 void(*child)(void), void* dso) {
  atfork_t* entry = reinterpret_cast<atfork_t*>(malloc(sizeof(atfork_t)));
  if (entry == nullptr) {
    return ENOMEM;
  }

  entry->prepare = prepare;
  entry->parent = parent;
  entry->child = child;
  entry->dso_handle = dso;

  pthread_mutex_lock(&g_atfork_list_mutex);

  g_atfork_list.push_back(entry);

  pthread_mutex_unlock(&g_atfork_list_mutex);

  return 0;
}

extern "C" __LIBC_HIDDEN__ void __unregister_atfork(void* dso) {
  pthread_mutex_lock(&g_atfork_list_mutex);
  g_atfork_list.remove_if([&](const atfork_t* entry) {
    return entry->dso_handle == dso;
  });
  pthread_mutex_unlock(&g_atfork_list_mutex);
}

