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

#ifndef SYSTEM_PROPERTIES_CONTEXTS_PRE_SPLIT_H
#define SYSTEM_PROPERTIES_CONTEXTS_PRE_SPLIT_H

#include <sys/mman.h>

#include "contexts.h"
#include "prop_area.h"
#include "prop_info.h"
#include "system_property_globals.h"

class ContextsPreSplit : public Contexts {
 public:
  virtual ~ContextsPreSplit() override {
  }

  // We'll never initialize this legacy option as writable, so don't even check the arg.
  virtual bool Initialize(bool) override {
    __system_property_area__ = prop_area::map_prop_area(property_filename);
    return __system_property_area__ != nullptr;
  }

  virtual prop_area* GetPropAreaForName(const char*) override {
    return __system_property_area__;
  }

  virtual void ForEach(void (*propfn)(const prop_info* pi, void* cookie), void* cookie) override {
    __system_property_area__->foreach (propfn, cookie);
  }

  // This is a no-op for pre-split properties as there is only one property file and it is
  // accessible by all domains
  virtual void ResetAccess() override {
  }

  virtual void FreeAndUnmap() override {
    if (__system_property_area__ != nullptr) {
      munmap(__system_property_area__, pa_size);
      __system_property_area__ = nullptr;
    }
  }
};

#endif
