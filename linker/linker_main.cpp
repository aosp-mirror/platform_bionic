/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "linker_main.h"

#include <link.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/prctl.h>

#include "linker.h"
#include "linker_auxv.h"
#include "linker_cfi.h"
#include "linker_debug.h"
#include "linker_debuggerd.h"
#include "linker_gdb_support.h"
#include "linker_globals.h"
#include "linker_phdr.h"
#include "linker_relocate.h"
#include "linker_relocs.h"
#include "linker_tls.h"
#include "linker_utils.h"

#include "private/KernelArgumentBlock.h"
#include "private/bionic_call_ifunc_resolver.h"
#include "private/bionic_globals.h"
#include "private/bionic_tls.h"

#include "android-base/unique_fd.h"
#include "android-base/strings.h"
#include "android-base/stringprintf.h"

#include <async_safe/log.h>
#include <bionic/libc_init_common.h>
#include <bionic/pthread_internal.h>

#include <vector>

__LIBC_HIDDEN__ extern "C" void _start();

static ElfW(Addr) get_elf_exec_load_bias(const ElfW(Ehdr)* elf);

static void get_elf_base_from_phdr(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                   ElfW(Addr)* base, ElfW(Addr)* load_bias);

static void set_bss_vma_name(soinfo* si);

void __libc_init_mte(const memtag_dynamic_entries_t* memtag_dynamic_entries, const void* phdr_start,
                     size_t phdr_count, uintptr_t load_bias, void* stack_top);

// These should be preserved static to avoid emitting
// RELATIVE relocations for the part of the code running
// before linker links itself.

// TODO (dimtiry): remove somain, rename solist to solist_head
static soinfo* solist;
static soinfo* sonext;
static soinfo* somain; // main process, always the one after libdl_info
static soinfo* solinker;
static soinfo* vdso; // vdso if present

void solist_add_soinfo(soinfo* si) {
  sonext->next = si;
  sonext = si;
}

bool solist_remove_soinfo(soinfo* si) {
  soinfo *prev = nullptr, *trav;
  for (trav = solist; trav != nullptr; trav = trav->next) {
    if (trav == si) {
      break;
    }
    prev = trav;
  }

  if (trav == nullptr) {
    // si was not in solist
    PRINT("name \"%s\"@%p is not in solist!", si->get_realpath(), si);
    return false;
  }

  // prev will never be null, because the first entry in solist is
  // always the static libdl_info.
  CHECK(prev != nullptr);
  prev->next = si->next;
  if (si == sonext) {
    sonext = prev;
  }

  return true;
}

soinfo* solist_get_head() {
  return solist;
}

soinfo* solist_get_somain() {
  return somain;
}

soinfo* solist_get_vdso() {
  return vdso;
}

bool g_is_ldd;
int g_ld_debug_verbosity;

static std::vector<std::string> g_ld_preload_names;

static std::vector<soinfo*> g_ld_preloads;

static void parse_path(const char* path, const char* delimiters,
                       std::vector<std::string>* resolved_paths) {
  std::vector<std::string> paths;
  split_path(path, delimiters, &paths);
  resolve_paths(paths, resolved_paths);
}

static void parse_LD_LIBRARY_PATH(const char* path) {
  std::vector<std::string> ld_libary_paths;
  parse_path(path, ":", &ld_libary_paths);
  g_default_namespace.set_ld_library_paths(std::move(ld_libary_paths));
}

static void parse_LD_PRELOAD(const char* path) {
  g_ld_preload_names.clear();
  if (path != nullptr) {
    // We have historically supported ':' as well as ' ' in LD_PRELOAD.
    g_ld_preload_names = android::base::Split(path, " :");
    g_ld_preload_names.erase(std::remove_if(g_ld_preload_names.begin(), g_ld_preload_names.end(),
                                            [](const std::string& s) { return s.empty(); }),
                             g_ld_preload_names.end());
  }
}

// An empty list of soinfos
static soinfo_list_t g_empty_list;

static void add_vdso() {
  ElfW(Ehdr)* ehdr_vdso = reinterpret_cast<ElfW(Ehdr)*>(getauxval(AT_SYSINFO_EHDR));
  if (ehdr_vdso == nullptr) {
    return;
  }

  soinfo* si = soinfo_alloc(&g_default_namespace, "[vdso]", nullptr, 0, 0);

  si->phdr = reinterpret_cast<ElfW(Phdr)*>(reinterpret_cast<char*>(ehdr_vdso) + ehdr_vdso->e_phoff);
  si->phnum = ehdr_vdso->e_phnum;
  si->base = reinterpret_cast<ElfW(Addr)>(ehdr_vdso);
  si->size = phdr_table_get_load_size(si->phdr, si->phnum);
  si->load_bias = get_elf_exec_load_bias(ehdr_vdso);

  si->prelink_image();
  si->link_image(SymbolLookupList(si), si, nullptr, nullptr);
  // prevents accidental unloads...
  si->set_dt_flags_1(si->get_dt_flags_1() | DF_1_NODELETE);
  si->set_linked();
  si->call_constructors();

  vdso = si;
}

// Initializes an soinfo's link_map_head field using other fields from the
// soinfo (phdr, phnum, load_bias). The soinfo's realpath must not change after
// this function is called.
static void init_link_map_head(soinfo& info) {
  auto& map = info.link_map_head;
  map.l_addr = info.load_bias;
  map.l_name = const_cast<char*>(info.get_realpath());
  phdr_table_get_dynamic_section(info.phdr, info.phnum, info.load_bias, &map.l_ld, nullptr);
}

extern "C" int __system_properties_init(void);

struct ExecutableInfo {
  std::string path;
  struct stat file_stat;
  const ElfW(Phdr)* phdr;
  size_t phdr_count;
  ElfW(Addr) entry_point;
  bool should_pad_segments;
};

static ExecutableInfo get_executable_info(const char* arg_path) {
  ExecutableInfo result = {};
  char const* exe_path = "/proc/self/exe";

  // Stat "/proc/self/exe" instead of executable_path because
  // the executable could be unlinked by this point and it should
  // not cause a crash (see http://b/31084669)
  if (TEMP_FAILURE_RETRY(stat(exe_path, &result.file_stat) == -1)) {
    // Fallback to argv[0] for the case where /proc isn't available
    if (TEMP_FAILURE_RETRY(stat(arg_path, &result.file_stat) == -1)) {
      async_safe_fatal("unable to stat either \"/proc/self/exe\" or \"%s\": %s",
          arg_path, strerror(errno));
    }
    exe_path = arg_path;
  }

  // Path might be a symlink; we need the target so that we get the right
  // linker configuration later.
  char sym_path[PATH_MAX];
  result.path = std::string(realpath(exe_path, sym_path) != nullptr ? sym_path : exe_path);

  result.phdr = reinterpret_cast<const ElfW(Phdr)*>(getauxval(AT_PHDR));
  result.phdr_count = getauxval(AT_PHNUM);
  result.entry_point = getauxval(AT_ENTRY);
  return result;
}

#if defined(__LP64__)
static char kFallbackLinkerPath[] = "/system/bin/linker64";
#else
static char kFallbackLinkerPath[] = "/system/bin/linker";
#endif

__printflike(1, 2)
static void __linker_error(const char* fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  async_safe_format_fd_va_list(STDERR_FILENO, fmt, ap);
  va_end(ap);

  va_start(ap, fmt);
  async_safe_format_log_va_list(ANDROID_LOG_FATAL, "linker", fmt, ap);
  va_end(ap);

  _exit(EXIT_FAILURE);
}

static void __linker_cannot_link(const char* argv0) {
  __linker_error("CANNOT LINK EXECUTABLE \"%s\": %s\n",
                 argv0,
                 linker_get_error_buffer());
}

// Load an executable. Normally the kernel has already loaded the executable when the linker
// starts. The linker can be invoked directly on an executable, though, and then the linker must
// load it. This function doesn't load dependencies or resolve relocations.
static ExecutableInfo load_executable(const char* orig_path) {
  ExecutableInfo result = {};

  if (orig_path[0] != '/') {
    __linker_error("error: expected absolute path: \"%s\"\n", orig_path);
  }

  off64_t file_offset;
  android::base::unique_fd fd(open_executable(orig_path, &file_offset, &result.path));
  if (fd.get() == -1) {
    __linker_error("error: unable to open file \"%s\"\n", orig_path);
  }

  if (TEMP_FAILURE_RETRY(fstat(fd.get(), &result.file_stat)) == -1) {
    __linker_error("error: unable to stat \"%s\": %s\n", result.path.c_str(), strerror(errno));
  }

  ElfReader elf_reader;
  if (!elf_reader.Read(result.path.c_str(), fd.get(), file_offset, result.file_stat.st_size)) {
    __linker_error("error: %s\n", linker_get_error_buffer());
  }
  address_space_params address_space;
  if (!elf_reader.Load(&address_space)) {
    __linker_error("error: %s\n", linker_get_error_buffer());
  }

  result.phdr = elf_reader.loaded_phdr();
  result.phdr_count = elf_reader.phdr_count();
  result.entry_point = elf_reader.entry_point();
  result.should_pad_segments = elf_reader.should_pad_segments();
  return result;
}

static void platform_properties_init() {
#if defined(__aarch64__)
  const unsigned long hwcap2 = getauxval(AT_HWCAP2);
  g_platform_properties.bti_supported = (hwcap2 & HWCAP2_BTI) != 0;
#endif
}

static ElfW(Addr) linker_main(KernelArgumentBlock& args, const char* exe_to_load) {
  ProtectedDataGuard guard;

#if TIMING
  struct timeval t0, t1;
  gettimeofday(&t0, 0);
#endif

  // Sanitize the environment.
  __libc_init_AT_SECURE(args.envp);

  // Initialize system properties
  __system_properties_init(); // may use 'environ'

  // Initialize platform properties.
  platform_properties_init();

  // Register the debuggerd signal handler.
  linker_debuggerd_init();

  g_linker_logger.ResetState();

  // Enable debugging logs?
  const char* LD_DEBUG = getenv("LD_DEBUG");
  if (LD_DEBUG != nullptr) {
    g_ld_debug_verbosity = atoi(LD_DEBUG);
  }

  if (getenv("LD_SHOW_AUXV") != nullptr) ld_show_auxv(args.auxv);

#if defined(__LP64__)
  INFO("[ Android dynamic linker (64-bit) ]");
#else
  INFO("[ Android dynamic linker (32-bit) ]");
#endif

  // These should have been sanitized by __libc_init_AT_SECURE, but the test
  // doesn't cost us anything.
  const char* ldpath_env = nullptr;
  const char* ldpreload_env = nullptr;
  if (!getauxval(AT_SECURE)) {
    ldpath_env = getenv("LD_LIBRARY_PATH");
    if (ldpath_env != nullptr) {
      INFO("[ LD_LIBRARY_PATH set to \"%s\" ]", ldpath_env);
    }
    ldpreload_env = getenv("LD_PRELOAD");
    if (ldpreload_env != nullptr) {
      INFO("[ LD_PRELOAD set to \"%s\" ]", ldpreload_env);
    }
  }

  const ExecutableInfo exe_info = exe_to_load ? load_executable(exe_to_load) :
                                                get_executable_info(args.argv[0]);

  INFO("[ Linking executable \"%s\" ]", exe_info.path.c_str());

  // Initialize the main exe's soinfo.
  soinfo* si = soinfo_alloc(&g_default_namespace,
                            exe_info.path.c_str(), &exe_info.file_stat,
                            0, RTLD_GLOBAL);
  somain = si;
  si->phdr = exe_info.phdr;
  si->phnum = exe_info.phdr_count;
  si->set_should_pad_segments(exe_info.should_pad_segments);
  get_elf_base_from_phdr(si->phdr, si->phnum, &si->base, &si->load_bias);
  si->size = phdr_table_get_load_size(si->phdr, si->phnum);
  si->dynamic = nullptr;
  si->set_main_executable();
  init_link_map_head(*si);

  set_bss_vma_name(si);

  // Use the executable's PT_INTERP string as the solinker filename in the
  // dynamic linker's module list. gdb reads both PT_INTERP and the module list,
  // and if the paths for the linker are different, gdb will report that the
  // PT_INTERP linker path was unloaded once the module list is initialized.
  // There are three situations to handle:
  //  - the APEX linker (/system/bin/linker[64] -> /apex/.../linker[64])
  //  - the ASAN linker (/system/bin/linker_asan[64] -> /apex/.../linker[64])
  //  - the bootstrap linker (/system/bin/bootstrap/linker[64])
  const char *interp = phdr_table_get_interpreter_name(somain->phdr, somain->phnum,
                                                       somain->load_bias);
  if (interp == nullptr) {
    // This case can happen if the linker attempts to execute itself
    // (e.g. "linker64 /system/bin/linker64").
    interp = kFallbackLinkerPath;
  }
  solinker->set_realpath(interp);
  init_link_map_head(*solinker);

#if defined(__aarch64__)
  if (exe_to_load == nullptr) {
    // Kernel does not add PROT_BTI to executable pages of the loaded ELF.
    // Apply appropriate protections here if it is needed.
    auto note_gnu_property = GnuPropertySection(somain);
    if (note_gnu_property.IsBTICompatible() &&
        (phdr_table_protect_segments(somain->phdr, somain->phnum, somain->load_bias,
                                     somain->should_pad_segments(), &note_gnu_property) < 0)) {
      __linker_error("error: can't protect segments for \"%s\": %s", exe_info.path.c_str(),
                     strerror(errno));
    }
  }
#endif

  // Register the main executable and the linker upfront to have
  // gdb aware of them before loading the rest of the dependency
  // tree.
  //
  // gdb expects the linker to be in the debug shared object list.
  // Without this, gdb has trouble locating the linker's ".text"
  // and ".plt" sections. Gdb could also potentially use this to
  // relocate the offset of our exported 'rtld_db_dlactivity' symbol.
  //
  insert_link_map_into_debug_map(&si->link_map_head);
  insert_link_map_into_debug_map(&solinker->link_map_head);

  add_vdso();

  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(si->base);

  // For security reasons we dropped non-PIE support in API level 21,
  // and the NDK no longer supports earlier API levels.
  if (elf_hdr->e_type != ET_DYN) {
    __linker_error("error: %s: Android only supports position-independent "
                   "executables (-fPIE)\n", exe_info.path.c_str());
  }

  // Use LD_LIBRARY_PATH and LD_PRELOAD (but only if we aren't setuid/setgid).
  parse_LD_LIBRARY_PATH(ldpath_env);
  parse_LD_PRELOAD(ldpreload_env);

  std::vector<android_namespace_t*> namespaces = init_default_namespaces(exe_info.path.c_str());

  if (!si->prelink_image()) __linker_cannot_link(g_argv[0]);

  // add somain to global group
  si->set_dt_flags_1(si->get_dt_flags_1() | DF_1_GLOBAL);
  // ... and add it to all other linked namespaces
  for (auto linked_ns : namespaces) {
    if (linked_ns != &g_default_namespace) {
      linked_ns->add_soinfo(somain);
      somain->add_secondary_namespace(linked_ns);
    }
  }

  linker_setup_exe_static_tls(g_argv[0]);

  // Load ld_preloads and dependencies.
  std::vector<const char*> needed_library_name_list;
  size_t ld_preloads_count = 0;

  for (const auto& ld_preload_name : g_ld_preload_names) {
    needed_library_name_list.push_back(ld_preload_name.c_str());
    ++ld_preloads_count;
  }

  for_each_dt_needed(si, [&](const char* name) {
    needed_library_name_list.push_back(name);
  });

  const char** needed_library_names = &needed_library_name_list[0];
  size_t needed_libraries_count = needed_library_name_list.size();

  if (needed_libraries_count > 0 &&
      !find_libraries(&g_default_namespace,
                      si,
                      needed_library_names,
                      needed_libraries_count,
                      nullptr,
                      &g_ld_preloads,
                      ld_preloads_count,
                      RTLD_GLOBAL,
                      nullptr,
                      true /* add_as_children */,
                      &namespaces)) {
    __linker_cannot_link(g_argv[0]);
  } else if (needed_libraries_count == 0) {
    if (!si->link_image(SymbolLookupList(si), si, nullptr, nullptr)) {
      __linker_cannot_link(g_argv[0]);
    }
    si->increment_ref_count();
  }

  // Exit early for ldd. We don't want to run the code that was loaded, so skip
  // the constructor calls. Skip CFI setup because it would call __cfi_init in
  // libdl.so.
  if (g_is_ldd) _exit(EXIT_SUCCESS);

#if defined(__aarch64__)
  // This has to happen after the find_libraries, which will have collected any possible
  // libraries that request memtag_stack in the dynamic section.
  __libc_init_mte(somain->memtag_dynamic_entries(), somain->phdr, somain->phnum, somain->load_bias,
                  args.argv);
#endif

  linker_finalize_static_tls();
  __libc_init_main_thread_final();

  if (!get_cfi_shadow()->InitialLinkDone(solist)) __linker_cannot_link(g_argv[0]);

  si->call_pre_init_constructors();
  si->call_constructors();

#if TIMING
  gettimeofday(&t1, nullptr);
  PRINT("LINKER TIME: %s: %d microseconds", g_argv[0],
        static_cast<int>(((static_cast<long long>(t1.tv_sec) * 1000000LL) +
                          static_cast<long long>(t1.tv_usec)) -
                         ((static_cast<long long>(t0.tv_sec) * 1000000LL) +
                          static_cast<long long>(t0.tv_usec))));
#endif
#if STATS
  print_linker_stats();
#endif
#if TIMING || STATS
  fflush(stdout);
#endif

  // We are about to hand control over to the executable loaded.  We don't want
  // to leave dirty pages behind unnecessarily.
  purge_unused_memory();

  ElfW(Addr) entry = exe_info.entry_point;
  TRACE("[ Ready to execute \"%s\" @ %p ]", si->get_realpath(), reinterpret_cast<void*>(entry));
  return entry;
}

/* Compute the load-bias of an existing executable. This shall only
 * be used to compute the load bias of an executable or shared library
 * that was loaded by the kernel itself.
 *
 * Input:
 *    elf    -> address of ELF header, assumed to be at the start of the file.
 * Return:
 *    load bias, i.e. add the value of any p_vaddr in the file to get
 *    the corresponding address in memory.
 */
static ElfW(Addr) get_elf_exec_load_bias(const ElfW(Ehdr)* elf) {
  ElfW(Addr) offset = elf->e_phoff;
  const ElfW(Phdr)* phdr_table =
      reinterpret_cast<const ElfW(Phdr)*>(reinterpret_cast<uintptr_t>(elf) + offset);
  const ElfW(Phdr)* phdr_end = phdr_table + elf->e_phnum;

  for (const ElfW(Phdr)* phdr = phdr_table; phdr < phdr_end; phdr++) {
    if (phdr->p_type == PT_LOAD) {
      return reinterpret_cast<ElfW(Addr)>(elf) + phdr->p_offset - phdr->p_vaddr;
    }
  }
  return 0;
}

/* Find the load bias and base address of an executable or shared object loaded
 * by the kernel. The ELF file's PHDR table must have a PT_PHDR entry.
 *
 * A VDSO doesn't have a PT_PHDR entry in its PHDR table.
 */
static void get_elf_base_from_phdr(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                   ElfW(Addr)* base, ElfW(Addr)* load_bias) {
  for (size_t i = 0; i < phdr_count; ++i) {
    if (phdr_table[i].p_type == PT_PHDR) {
      *load_bias = reinterpret_cast<ElfW(Addr)>(phdr_table) - phdr_table[i].p_vaddr;
      *base = reinterpret_cast<ElfW(Addr)>(phdr_table) - phdr_table[i].p_offset;
      return;
    }
  }
  async_safe_fatal("Could not find a PHDR: broken executable?");
}

/*
 * Set anonymous VMA name for .bss section.  For DSOs loaded by the linker, this
 * is done by ElfReader.  This function is here for DSOs loaded by the kernel,
 * namely the linker itself and the main executable.
 */
static void set_bss_vma_name(soinfo* si) {
  for (size_t i = 0; i < si->phnum; ++i) {
    auto phdr = &si->phdr[i];

    if (phdr->p_type != PT_LOAD) {
      continue;
    }

    ElfW(Addr) seg_start = phdr->p_vaddr + si->load_bias;
    ElfW(Addr) seg_page_end = page_end(seg_start + phdr->p_memsz);
    ElfW(Addr) seg_file_end = page_end(seg_start + phdr->p_filesz);

    if (seg_page_end > seg_file_end) {
      prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME,
            reinterpret_cast<void*>(seg_file_end), seg_page_end - seg_file_end,
            ".bss");
    }
  }
}

#if defined(USE_RELA)
using RelType = ElfW(Rela);
const unsigned kRelTag = DT_RELA;
const unsigned kRelSzTag = DT_RELASZ;
#else
using RelType = ElfW(Rel);
const unsigned kRelTag = DT_REL;
const unsigned kRelSzTag = DT_RELSZ;
#endif

extern __LIBC_HIDDEN__ ElfW(Ehdr) __ehdr_start;

static void call_ifunc_resolvers_for_section(RelType* begin, RelType* end) {
  auto ehdr = reinterpret_cast<ElfW(Addr)>(&__ehdr_start);
  for (RelType *r = begin; r != end; ++r) {
    if (ELFW(R_TYPE)(r->r_info) != R_GENERIC_IRELATIVE) {
      continue;
    }
    ElfW(Addr)* offset = reinterpret_cast<ElfW(Addr)*>(ehdr + r->r_offset);
#if defined(USE_RELA)
    ElfW(Addr) resolver = ehdr + r->r_addend;
#else
    ElfW(Addr) resolver = ehdr + *offset;
#endif
    *offset = __bionic_call_ifunc_resolver(resolver);
  }
}

static void relocate_linker() {
  // The linker should only have relative relocations (in RELR) and IRELATIVE
  // relocations. Find the IRELATIVE relocations using the DT_JMPREL and
  // DT_PLTRELSZ, or DT_RELA/DT_RELASZ (DT_REL/DT_RELSZ on ILP32).
  auto ehdr = reinterpret_cast<ElfW(Addr)>(&__ehdr_start);
  auto* phdr = reinterpret_cast<ElfW(Phdr)*>(ehdr + __ehdr_start.e_phoff);
  for (size_t i = 0; i != __ehdr_start.e_phnum; ++i) {
    if (phdr[i].p_type != PT_DYNAMIC) {
      continue;
    }
    auto *dyn = reinterpret_cast<ElfW(Dyn)*>(ehdr + phdr[i].p_vaddr);
    ElfW(Addr) relr = 0, relrsz = 0, pltrel = 0, pltrelsz = 0, rel = 0, relsz = 0;
    for (size_t j = 0, size = phdr[i].p_filesz / sizeof(ElfW(Dyn)); j != size; ++j) {
      const auto tag = dyn[j].d_tag;
      const auto val = dyn[j].d_un.d_ptr;
      // We don't currently handle IRELATIVE relocations in DT_ANDROID_REL[A].
      // We disabled DT_ANDROID_REL[A] at build time; verify that it was actually disabled.
      CHECK(tag != DT_ANDROID_REL && tag != DT_ANDROID_RELA);
      if (tag == DT_RELR || tag == DT_ANDROID_RELR) {
        relr = val;
      } else if (tag == DT_RELRSZ || tag == DT_ANDROID_RELRSZ) {
        relrsz = val;
      } else if (tag == DT_JMPREL) {
        pltrel = val;
      } else if (tag == DT_PLTRELSZ) {
        pltrelsz = val;
      } else if (tag == kRelTag) {
        rel = val;
      } else if (tag == kRelSzTag) {
        relsz = val;
      }
    }
    // Apply RELR relocations first so that the GOT is initialized for ifunc
    // resolvers.
    if (relr && relrsz) {
      relocate_relr(reinterpret_cast<ElfW(Relr*)>(ehdr + relr),
                    reinterpret_cast<ElfW(Relr*)>(ehdr + relr + relrsz), ehdr);
    }
    if (pltrel && pltrelsz) {
      call_ifunc_resolvers_for_section(reinterpret_cast<RelType*>(ehdr + pltrel),
                                       reinterpret_cast<RelType*>(ehdr + pltrel + pltrelsz));
    }
    if (rel && relsz) {
      call_ifunc_resolvers_for_section(reinterpret_cast<RelType*>(ehdr + rel),
                                       reinterpret_cast<RelType*>(ehdr + rel + relsz));
    }
  }
}

// Usable before ifunc resolvers have been called. This function is compiled with -ffreestanding.
static void linker_memclr(void* dst, size_t cnt) {
  for (size_t i = 0; i < cnt; ++i) {
    reinterpret_cast<char*>(dst)[i] = '\0';
  }
}

// Detect an attempt to run the linker on itself. e.g.:
//   /system/bin/linker64 /system/bin/linker64
// Use priority-1 to run this constructor before other constructors.
__attribute__((constructor(1))) static void detect_self_exec() {
  // Normally, the linker initializes the auxv global before calling its
  // constructors. If the linker loads itself, though, the first loader calls
  // the second loader's constructors before calling __linker_init.
  if (__libc_shared_globals()->auxv != nullptr) {
    return;
  }
#if defined(__i386__)
  // We don't have access to the auxv struct from here, so use the int 0x80
  // fallback.
  __libc_sysinfo = reinterpret_cast<void*>(__libc_int0x80);
#endif
  __linker_error("error: linker cannot load itself\n");
}

static ElfW(Addr) __attribute__((noinline))
__linker_init_post_relocation(KernelArgumentBlock& args, soinfo& linker_so);

/*
 * This is the entry point for the linker, called from begin.S. This
 * method is responsible for fixing the linker's own relocations, and
 * then calling __linker_init_post_relocation().
 *
 * Because this method is called before the linker has fixed it's own
 * relocations, any attempt to reference an extern variable, extern
 * function, or other GOT reference will generate a segfault.
 */
extern "C" ElfW(Addr) __linker_init(void* raw_args) {
  // Unlock the loader mutex immediately before transferring to the executable's
  // entry point. This must happen after destructors are called in this function
  // (e.g. ~soinfo), so declare this variable very early.
  struct DlMutexUnlocker {
    ~DlMutexUnlocker() { pthread_mutex_unlock(&g_dl_mutex); }
  } unlocker;

  // Initialize TLS early so system calls and errno work.
  KernelArgumentBlock args(raw_args);
  bionic_tcb temp_tcb __attribute__((uninitialized));
  linker_memclr(&temp_tcb, sizeof(temp_tcb));
  __libc_init_main_thread_early(args, &temp_tcb);

  // When the linker is run by itself (rather than as an interpreter for
  // another program), AT_BASE is 0.
  ElfW(Addr) linker_addr = getauxval(AT_BASE);
  if (linker_addr == 0) {
    // The AT_PHDR and AT_PHNUM aux values describe this linker instance, so use
    // the phdr to find the linker's base address.
    ElfW(Addr) load_bias;
    get_elf_base_from_phdr(
      reinterpret_cast<ElfW(Phdr)*>(getauxval(AT_PHDR)), getauxval(AT_PHNUM),
      &linker_addr, &load_bias);
  }

  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(linker_addr);
  ElfW(Phdr)* phdr = reinterpret_cast<ElfW(Phdr)*>(linker_addr + elf_hdr->e_phoff);

  // Relocate the linker. This step will initialize the GOT, which is needed for
  // accessing non-hidden global variables. (On some targets, the stack
  // protector uses GOT accesses rather than TLS.) Relocating the linker will
  // also call the linker's ifunc resolvers so that string.h functions can be
  // used.
  relocate_linker();

  soinfo tmp_linker_so(nullptr, nullptr, nullptr, 0, 0);

  tmp_linker_so.base = linker_addr;
  tmp_linker_so.size = phdr_table_get_load_size(phdr, elf_hdr->e_phnum);
  tmp_linker_so.load_bias = get_elf_exec_load_bias(elf_hdr);
  tmp_linker_so.dynamic = nullptr;
  tmp_linker_so.phdr = phdr;
  tmp_linker_so.phnum = elf_hdr->e_phnum;
  tmp_linker_so.set_linker_flag();

  if (!tmp_linker_so.prelink_image()) __linker_cannot_link(args.argv[0]);
  if (!tmp_linker_so.link_image(SymbolLookupList(&tmp_linker_so), &tmp_linker_so, nullptr, nullptr)) __linker_cannot_link(args.argv[0]);

  return __linker_init_post_relocation(args, tmp_linker_so);
}

/*
 * This code is called after the linker has linked itself and fixed its own
 * GOT. It is safe to make references to externs and other non-local data at
 * this point. The compiler sometimes moves GOT references earlier in a
 * function, so avoid inlining this function (http://b/80503879).
 */
static ElfW(Addr) __attribute__((noinline))
__linker_init_post_relocation(KernelArgumentBlock& args, soinfo& tmp_linker_so) {
  // Finish initializing the main thread.
  __libc_init_main_thread_late();

  // We didn't protect the linker's RELRO pages in link_image because we
  // couldn't make system calls on x86 at that point, but we can now...
  if (!tmp_linker_so.protect_relro()) __linker_cannot_link(args.argv[0]);

  // And we can set VMA name for the bss section now
  set_bss_vma_name(&tmp_linker_so);

  // Initialize the linker's static libc's globals
  __libc_init_globals();

  // A constructor could spawn a thread that calls into the loader, so as soon
  // as we've called a constructor, we need to hold the lock until transferring
  // to the entry point.
  pthread_mutex_lock(&g_dl_mutex);

  // Initialize the linker's own global variables
  tmp_linker_so.call_constructors();

  // Setting the linker soinfo's soname can allocate heap memory, so delay it until here.
  for (const ElfW(Dyn)* d = tmp_linker_so.dynamic; d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_SONAME) {
      tmp_linker_so.set_soname(tmp_linker_so.get_string(d->d_un.d_val));
    }
  }

  // When the linker is run directly rather than acting as PT_INTERP, parse
  // arguments and determine the executable to load. When it's instead acting
  // as PT_INTERP, AT_ENTRY will refer to the loaded executable rather than the
  // linker's _start.
  const char* exe_to_load = nullptr;
  if (getauxval(AT_ENTRY) == reinterpret_cast<uintptr_t>(&_start)) {
    if (args.argc == 3 && !strcmp(args.argv[1], "--list")) {
      // We're being asked to behave like ldd(1).
      g_is_ldd = true;
      exe_to_load = args.argv[2];
    } else if (args.argc <= 1 || !strcmp(args.argv[1], "--help")) {
      async_safe_format_fd(STDOUT_FILENO,
         "Usage: %s [--list] PROGRAM [ARGS-FOR-PROGRAM...]\n"
         "       %s [--list] path.zip!/PROGRAM [ARGS-FOR-PROGRAM...]\n"
         "\n"
         "A helper program for linking dynamic executables. Typically, the kernel loads\n"
         "this program because it's the PT_INTERP of a dynamic executable.\n"
         "\n"
         "This program can also be run directly to load and run a dynamic executable. The\n"
         "executable can be inside a zip file if it's stored uncompressed and at a\n"
         "page-aligned offset.\n"
         "\n"
         "The --list option gives behavior equivalent to ldd(1) on other systems.\n",
         args.argv[0], args.argv[0]);
      _exit(EXIT_SUCCESS);
    } else {
      exe_to_load = args.argv[1];
      __libc_shared_globals()->initial_linker_arg_count = 1;
    }
  }

  // store argc/argv/envp to use them for calling constructors
  g_argc = args.argc - __libc_shared_globals()->initial_linker_arg_count;
  g_argv = args.argv + __libc_shared_globals()->initial_linker_arg_count;
  g_envp = args.envp;
  __libc_shared_globals()->init_progname = g_argv[0];

  // Initialize static variables. Note that in order to
  // get correct libdl_info we need to call constructors
  // before get_libdl_info().
  sonext = solist = solinker = get_libdl_info(tmp_linker_so);
  g_default_namespace.add_soinfo(solinker);

  ElfW(Addr) start_address = linker_main(args, exe_to_load);

  INFO("[ Jumping to _start (%p)... ]", reinterpret_cast<void*>(start_address));

  // Return the address that the calling assembly stub should jump to.
  return start_address;
}
