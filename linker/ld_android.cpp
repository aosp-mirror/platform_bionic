/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <sys/cdefs.h>

extern "C" void __internal_linker_error() {
  __builtin_trap();
}

__strong_alias(__loader_android_create_namespace, __internal_linker_error);
__strong_alias(__loader_android_dlopen_ext, __internal_linker_error);
__strong_alias(__loader_android_dlwarning, __internal_linker_error);
__strong_alias(__loader_android_get_application_target_sdk_version, __internal_linker_error);
__strong_alias(__loader_android_get_LD_LIBRARY_PATH, __internal_linker_error);
__strong_alias(__loader_android_get_exported_namespace, __internal_linker_error);
__strong_alias(__loader_android_init_anonymous_namespace, __internal_linker_error);
__strong_alias(__loader_android_link_namespaces, __internal_linker_error);
__strong_alias(__loader_android_link_namespaces_all_libs, __internal_linker_error);
__strong_alias(__loader_android_set_application_target_sdk_version, __internal_linker_error);
__strong_alias(__loader_android_update_LD_LIBRARY_PATH, __internal_linker_error);
__strong_alias(__loader_cfi_fail, __internal_linker_error);
__strong_alias(__loader_dl_iterate_phdr, __internal_linker_error);
__strong_alias(__loader_dladdr, __internal_linker_error);
__strong_alias(__loader_dlclose, __internal_linker_error);
__strong_alias(__loader_dlerror, __internal_linker_error);
__strong_alias(__loader_dlopen, __internal_linker_error);
__strong_alias(__loader_dlsym, __internal_linker_error);
__strong_alias(__loader_dlvsym, __internal_linker_error);
__strong_alias(__loader_add_thread_local_dtor, __internal_linker_error);
__strong_alias(__loader_remove_thread_local_dtor, __internal_linker_error);
__strong_alias(__loader_shared_globals, __internal_linker_error);
#if defined(__arm__)
__strong_alias(__loader_dl_unwind_find_exidx, __internal_linker_error);
#endif
__strong_alias(rtld_db_dlactivity, __internal_linker_error);
