/*  $NetBSD: res_data.c,v 1.8 2004/06/09 18:07:03 christos Exp $  */

/*
 * Copyright (c) 2004 by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1995-1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "resolv_private.h"

#include <pthread.h>

extern "C" int res_ourserver_p(const res_state, const struct sockaddr*);
extern "C" int __res_vinit(res_state, int);

class GlobalStateAccessor {
 public:
  GlobalStateAccessor() {
    pthread_mutex_lock(&mutex);
    if (!initialized) {
      init();
      initialized = true;
    }
  }

  ~GlobalStateAccessor() {
    pthread_mutex_unlock(&mutex);
  }

  __res_state* get() {
    return &state;
  }

  int init();

 private:
  static __res_state state;
  static bool initialized;
  static pthread_mutex_t mutex;
};
__res_state GlobalStateAccessor::state;
bool GlobalStateAccessor::initialized = false;
pthread_mutex_t GlobalStateAccessor::mutex = PTHREAD_MUTEX_INITIALIZER;

int GlobalStateAccessor::init() {
  // These three fields used to be statically initialized.  This made
  // it hard to use this code in a shared library.  It is necessary,
  // now that we're doing dynamic initialization here, that we preserve
  // the old semantics: if an application modifies one of these three
  // fields of _res before res_init() is called, res_init() will not
  // alter them.  Of course, if an application is setting them to
  // _zero_ before calling res_init(), hoping to override what used
  // to be the static default, we can't detect it and unexpected results
  // will follow.  Zero for any of these fields would make no sense,
  // so one can safely assume that the applications were already getting
  // unexpected results.
  // g_nres.options is tricky since some apps were known to diddle the bits
  // before res_init() was first called. We can't replicate that semantic
  // with dynamic initialization (they may have turned bits off that are
  // set in RES_DEFAULT).  Our solution is to declare such applications
  // "broken".  They could fool us by setting RES_INIT but none do (yet).
  if (!state.retrans) state.retrans = RES_TIMEOUT;
  if (!state.retry) state.retry = 4;
  if (!(state.options & RES_INIT)) state.options = RES_DEFAULT;

  // This one used to initialize implicitly to zero, so unless the app
  // has set it to something in particular, we can randomize it now.
  if (!state.id) state.id = res_randomid();

  return __res_vinit(&state, 1);
}

int res_init(void) {
  GlobalStateAccessor gsa;
  return gsa.init();
}

void p_query(const u_char* msg) {
  fp_query(msg, stdout);
}

void fp_query(const u_char* msg, FILE* file) {
  fp_nquery(msg, PACKETSZ, file);
}

void fp_nquery(const u_char* msg, int len, FILE* file) {
  GlobalStateAccessor gsa;
  res_pquery(gsa.get(), msg, len, file);
}

int
res_mkquery(int op, const char* dname, int klass, int type, const u_char* data,
            int datalen, const u_char* newrr_in, u_char* buf, int buflen) {
  GlobalStateAccessor gsa;
  return res_nmkquery(gsa.get(), op, dname, klass, type, data, datalen, newrr_in, buf, buflen);
}

int res_query(const char* name, int klass, int type, u_char* answer, int anslen) {
  GlobalStateAccessor gsa;
  return res_nquery(gsa.get(), name, klass, type, answer, anslen);
}

void res_send_setqhook(res_send_qhook hook) {
  GlobalStateAccessor gsa;
  gsa.get()->qhook = hook;
}

void res_send_setrhook(res_send_rhook hook) {
  GlobalStateAccessor gsa;
  gsa.get()->rhook = hook;
}

int res_isourserver(const struct sockaddr_in* inp) {
  GlobalStateAccessor gsa;
  return res_ourserver_p(gsa.get(), reinterpret_cast<const sockaddr*>(inp));
}

int res_send(const u_char* buf, int buflen, u_char* ans, int anssiz) {
  GlobalStateAccessor gsa;
  return res_nsend(gsa.get(), buf, buflen, ans, anssiz);
}

void res_close(void) {
  GlobalStateAccessor gsa;
  res_nclose(gsa.get());
}

int res_search(const char* name, int klass, int type, u_char* answer, int anslen) {
  GlobalStateAccessor gsa;
  return res_nsearch(gsa.get(), name, klass, type, answer, anslen);
}

int res_querydomain(const char* name, const char* domain, int klass, int type, u_char* answer,
                    int anslen) {
  GlobalStateAccessor gsa;
  return res_nquerydomain(gsa.get(), name, domain, klass, type, answer, anslen);
}

int res_opt(int a, u_char* b, int c, int d) {
  GlobalStateAccessor gsa;
  return res_nopt(gsa.get(), a, b, c, d);
}

const char* hostalias(const char* name) {
  return NULL;
}
