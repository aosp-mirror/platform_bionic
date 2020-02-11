/*
 * Copyright (C) 2012 The Android Open Source Project
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

// Deliberately overwrite the stack canary.
__attribute__((noinline)) void modify_stack_protector_test() {
  // We can't use memset here because it's fortified, and we want to test
  // the line of defense *after* that.
  // Without volatile, the generic x86/x86-64 targets don't write to the stack.
  volatile char* p;
  p = reinterpret_cast<volatile char*>(&p + 1);
  *p = '\0';
}
