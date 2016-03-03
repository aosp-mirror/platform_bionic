/*
 * Copyright (C) 2014 The Android Open Source Project

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

#ifndef __ANDROID_DLEXT_H__
#define __ANDROID_DLEXT_H__

#include <stddef.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>  /* for off64_t */

__BEGIN_DECLS

/* bitfield definitions for android_dlextinfo.flags */
enum {
  /* When set, the reserved_addr and reserved_size fields must point to an
   * already-reserved region of address space which will be used to load the
   * library if it fits. If the reserved region is not large enough, the load
   * will fail.
   */
  ANDROID_DLEXT_RESERVED_ADDRESS      = 0x1,

  /* As DLEXT_RESERVED_ADDRESS, but if the reserved region is not large enough,
   * the linker will choose an available address instead.
   */
  ANDROID_DLEXT_RESERVED_ADDRESS_HINT = 0x2,

  /* When set, write the GNU RELRO section of the mapped library to relro_fd
   * after relocation has been performed, to allow it to be reused by another
   * process loading the same library at the same address. This implies
   * ANDROID_DLEXT_USE_RELRO.
   */
  ANDROID_DLEXT_WRITE_RELRO           = 0x4,

  /* When set, compare the GNU RELRO section of the mapped library to relro_fd
   * after relocation has been performed, and replace any relocated pages that
   * are identical with a version mapped from the file.
   */
  ANDROID_DLEXT_USE_RELRO             = 0x8,

  /* Instruct dlopen to use library_fd instead of opening file by name.
   * The filename parameter is still used to identify the library.
   */
  ANDROID_DLEXT_USE_LIBRARY_FD        = 0x10,

  /* If opening a library using library_fd read it starting at library_fd_offset.
   * This flag is only valid when ANDROID_DLEXT_USE_LIBRARY_FD is set.
   */
  ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET    = 0x20,

  /* When set, do not check if the library has already been loaded by file stat(2)s.
   *
   * This flag allows forced loading of the library in the case when for some
   * reason multiple ELF files share the same filename (because the already-loaded
   * library has been removed and overwritten, for example).
   *
   * Note that if the library has the same dt_soname as an old one and some other
   * library has the soname in DT_NEEDED list, the first one will be used to resolve any
   * dependencies.
   */
  ANDROID_DLEXT_FORCE_LOAD = 0x40,

  /* When set, if the minimum p_vaddr of the ELF file's PT_LOAD segments is non-zero,
   * the dynamic linker will load it at that address.
   *
   * This flag is for ART internal use only.
   */
  ANDROID_DLEXT_FORCE_FIXED_VADDR = 0x80,

  /* Instructs dlopen to load the library at the address specified by reserved_addr.
   *
   * The difference between ANDROID_DLEXT_LOAD_AT_FIXED_ADDRESS and ANDROID_DLEXT_RESERVED_ADDRESS
   * is that for ANDROID_DLEXT_LOAD_AT_FIXED_ADDRESS the linker reserves memory at reserved_addr
   * whereas for ANDROID_DLEXT_RESERVED_ADDRESS the linker relies on the caller to reserve the memory.
   *
   * This flag can be used with ANDROID_DLEXT_FORCE_FIXED_VADDR; when ANDROID_DLEXT_FORCE_FIXED_VADDR
   * is set and load_bias is not 0 (load_bias is min(p_vaddr) of PT_LOAD segments) this flag is ignored.
   * This is implemented this way because the linker has to pick one address over the other and this
   * way is more convenient for art. Note that ANDROID_DLEXT_FORCE_FIXED_VADDR does not generate
   * an error when min(p_vaddr) is 0.
   *
   * Cannot be used with ANDROID_DLEXT_RESERVED_ADDRESS or ANDROID_DLEXT_RESERVED_ADDRESS_HINT.
   *
   * This flag is for ART internal use only.
   */
  ANDROID_DLEXT_LOAD_AT_FIXED_ADDRESS = 0x100,

  /* This flag used to load library in a different namespace. The namespace is
   * specified in library_namespace.
   */
  ANDROID_DLEXT_USE_NAMESPACE = 0x200,

  /* Mask of valid bits */
  ANDROID_DLEXT_VALID_FLAG_BITS       = ANDROID_DLEXT_RESERVED_ADDRESS |
                                        ANDROID_DLEXT_RESERVED_ADDRESS_HINT |
                                        ANDROID_DLEXT_WRITE_RELRO |
                                        ANDROID_DLEXT_USE_RELRO |
                                        ANDROID_DLEXT_USE_LIBRARY_FD |
                                        ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET |
                                        ANDROID_DLEXT_FORCE_LOAD |
                                        ANDROID_DLEXT_FORCE_FIXED_VADDR |
                                        ANDROID_DLEXT_LOAD_AT_FIXED_ADDRESS |
                                        ANDROID_DLEXT_USE_NAMESPACE,
};

struct android_namespace_t;

typedef struct {
  uint64_t flags;
  void*   reserved_addr;
  size_t  reserved_size;
  int     relro_fd;
  int     library_fd;
  off64_t library_fd_offset;
  struct android_namespace_t* library_namespace;
} android_dlextinfo;

extern void* android_dlopen_ext(const char* filename, int flag, const android_dlextinfo* extinfo);

/*
 * Initializes public and anonymous namespaces. The public_ns_sonames is the list of sonames
 * to be included into public namespace separated by colon. Example: "libc.so:libm.so:libdl.so".
 * The libraries in this list should be loaded prior to this call.
 *
 * The anon_ns_library_path is the search path for anonymous namespace. The anonymous namespace
 * is used in the case when linker cannot identify the caller of dlopen/dlsym. This happens
 * for the code not loaded by dynamic linker; for example calls from the mono-compiled code.
 */
extern bool android_init_namespaces(const char* public_ns_sonames,
                                    const char* anon_ns_library_path);


enum {
  /* A regular namespace is the namespace with a custom search path that does
   * not impose any restrictions on the location of native libraries.
   */
  ANDROID_NAMESPACE_TYPE_REGULAR = 0,

  /* An isolated namespace requires all the libraries to be on the search path
   * or under permitted_when_isolated_path. The search path is the union of
   * ld_library_path and default_library_path.
   */
  ANDROID_NAMESPACE_TYPE_ISOLATED = 1,

  /* The shared namespace clones the list of libraries of the caller namespace upon creation
   * which means that they are shared between namespaces - the caller namespace and the new one
   * will use the same copy of a library if it was loaded prior to android_create_namespace call.
   *
   * Note that libraries loaded after the namespace is created will not be shared.
   *
   * Shared namespaces can be isolated or regular. Note that they do not inherit the search path nor
   * permitted_path from the caller's namespace.
   */
  ANDROID_NAMESPACE_TYPE_SHARED = 2,
  ANDROID_NAMESPACE_TYPE_SHARED_ISOLATED = ANDROID_NAMESPACE_TYPE_SHARED |
                                           ANDROID_NAMESPACE_TYPE_ISOLATED,
};

/*
 * Creates new linker namespace.
 * ld_library_path and default_library_path represent the search path
 * for the libraries in the namespace.
 *
 * The libraries in the namespace are searched by folowing order:
 * 1. ld_library_path (Think of this as namespace-local LD_LIBRARY_PATH)
 * 2. In directories specified by DT_RUNPATH of the "needed by" binary.
 * 3. deault_library_path (This of this as namespace-local default library path)
 *
 * When type is ANDROID_NAMESPACE_TYPE_ISOLATED the resulting namespace requires all of
 * the libraries to be on the search path or under the permitted_when_isolated_path;
 * the search_path is ld_library_path:default_library_path. Note that the
 * permitted_when_isolated_path path is not part of the search_path and
 * does not affect the search order. It is a way to allow loading libraries from specific
 * locations when using absolute path.
 * If a library or any of its dependencies are outside of the permitted_when_isolated_path
 * and search_path, and it is not part of the public namespace dlopen will fail.
 */
extern struct android_namespace_t* android_create_namespace(const char* name,
                                                            const char* ld_library_path,
                                                            const char* default_library_path,
                                                            uint64_t type,
                                                            const char* permitted_when_isolated_path);

__END_DECLS

#endif /* __ANDROID_DLEXT_H__ */
