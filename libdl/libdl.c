/*
 * Copyright (C) 2007 The Android Open Source Project
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

/* These are stubs for functions that are actually defined
 * in the dynamic linker (dlfcn.c), and hijacked at runtime.
 */
void *dlopen(const char *filename, int flag) { return 0; }
char *dlerror(void) { return 0; }
void *dlsym(void *handle, const char *symbol) { return 0; }
int dlclose(void *handle) { return 0; }
void *dl_unwind_find_exidx(void *pc, int *pcount) { return 0; }
