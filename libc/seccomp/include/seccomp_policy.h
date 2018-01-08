/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef SECCOMP_POLICY_H
#define SECCOMP_POLICY_H

#include <stddef.h>
#include <linux/filter.h>

// TODO(victorhsieh): remove once the callers are switched to the new API.
bool set_seccomp_filter();

bool set_app_seccomp_filter();
bool set_system_seccomp_filter();
bool set_global_seccomp_filter();

#endif
