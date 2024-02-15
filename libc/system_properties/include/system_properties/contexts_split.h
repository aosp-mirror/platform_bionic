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

#pragma once

#include "contexts.h"

struct PrefixNode;
class ContextListNode;

class ContextsSplit : public Contexts {
 public:
  virtual ~ContextsSplit() override {
  }

  virtual bool Initialize(bool writable, const char* filename, bool* fsetxattr_failed,
                          bool) override;
  virtual prop_area* GetPropAreaForName(const char* name) override;
  virtual prop_area* GetSerialPropArea() override {
    return serial_prop_area_;
  }
  virtual void ForEach(void (*propfn)(const prop_info* pi, void* cookie), void* cookie) override;
  virtual void ResetAccess() override;
  virtual void FreeAndUnmap() override;

  PrefixNode* GetPrefixNodeForName(const char* name);

 protected:
  bool MapSerialPropertyArea(bool access_rw, bool* fsetxattr_failed);
  bool InitializePropertiesFromFile(const char* filename);
  bool InitializeProperties();

  PrefixNode* prefixes_ = nullptr;
  ContextListNode* contexts_ = nullptr;
  prop_area* serial_prop_area_ = nullptr;
  const char* filename_ = nullptr;
};
