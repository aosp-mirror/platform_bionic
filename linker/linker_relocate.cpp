/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "linker_relocate.h"

#include <elf.h>
#include <link.h>

#include <type_traits>

#include "linker.h"
#include "linker_debug.h"
#include "linker_globals.h"
#include "linker_gnu_hash.h"
#include "linker_phdr.h"
#include "linker_relocs.h"
#include "linker_reloc_iterators.h"
#include "linker_sleb128.h"
#include "linker_soinfo.h"
#include "private/bionic_globals.h"

static bool is_tls_reloc(ElfW(Word) type) {
  switch (type) {
    case R_GENERIC_TLS_DTPMOD:
    case R_GENERIC_TLS_DTPREL:
    case R_GENERIC_TLS_TPREL:
    case R_GENERIC_TLSDESC:
      return true;
    default:
      return false;
  }
}

class Relocator {
 public:
  Relocator(const VersionTracker& version_tracker, const SymbolLookupList& lookup_list)
      : version_tracker(version_tracker), lookup_list(lookup_list)
  {}

  soinfo* si = nullptr;
  const char* si_strtab = nullptr;
  size_t si_strtab_size = 0;
  ElfW(Sym)* si_symtab = nullptr;

  const VersionTracker& version_tracker;
  const SymbolLookupList& lookup_list;

  // Cache key
  ElfW(Word) cache_sym_val = 0;
  // Cache value
  const ElfW(Sym)* cache_sym = nullptr;
  soinfo* cache_si = nullptr;

  std::vector<TlsDynamicResolverArg>* tlsdesc_args;
  std::vector<std::pair<TlsDescriptor*, size_t>> deferred_tlsdesc_relocs;
  size_t tls_tp_base = 0;

  __attribute__((always_inline))
  const char* get_string(ElfW(Word) index) {
    if (__predict_false(index >= si_strtab_size)) {
      async_safe_fatal("%s: strtab out of bounds error; STRSZ=%zd, name=%d",
                       si->get_realpath(), si_strtab_size, index);
    }
    return si_strtab + index;
  }
};

template <bool DoLogging>
__attribute__((always_inline))
static inline bool lookup_symbol(Relocator& relocator, uint32_t r_sym, const char* sym_name,
                                 soinfo** found_in, const ElfW(Sym)** sym) {
  if (r_sym == relocator.cache_sym_val) {
    *found_in = relocator.cache_si;
    *sym = relocator.cache_sym;
    count_relocation_if<DoLogging>(kRelocSymbolCached);
  } else {
    const version_info* vi = nullptr;
    if (!relocator.si->lookup_version_info(relocator.version_tracker, r_sym, sym_name, &vi)) {
      return false;
    }

    soinfo* local_found_in = nullptr;
    const ElfW(Sym)* local_sym = soinfo_do_lookup(sym_name, vi, &local_found_in, relocator.lookup_list);

    relocator.cache_sym_val = r_sym;
    relocator.cache_si = local_found_in;
    relocator.cache_sym = local_sym;
    *found_in = local_found_in;
    *sym = local_sym;
  }

  if (*sym == nullptr) {
    if (ELF_ST_BIND(relocator.si_symtab[r_sym].st_info) != STB_WEAK) {
      DL_ERR("cannot locate symbol \"%s\" referenced by \"%s\"...", sym_name, relocator.si->get_realpath());
      return false;
    }
  }

  count_relocation_if<DoLogging>(kRelocSymbol);
  return true;
}

enum class RelocMode {
  // Fast path for JUMP_SLOT relocations.
  JumpTable,
  // Fast path for typical relocations: ABSOLUTE, GLOB_DAT, or RELATIVE.
  Typical,
  // Handle all relocation types, relocations in text sections, and statistics/tracing.
  General,
};

struct linker_stats_t {
  int count[kRelocMax];
};

static linker_stats_t linker_stats;

void count_relocation(RelocationKind kind) {
  ++linker_stats.count[kind];
}

void print_linker_stats() {
  PRINT("RELO STATS: %s: %d abs, %d rel, %d symbol (%d cached)",
         g_argv[0],
         linker_stats.count[kRelocAbsolute],
         linker_stats.count[kRelocRelative],
         linker_stats.count[kRelocSymbol],
         linker_stats.count[kRelocSymbolCached]);
}

static bool process_relocation_general(Relocator& relocator, const rel_t& reloc);

template <RelocMode Mode>
__attribute__((always_inline))
static bool process_relocation_impl(Relocator& relocator, const rel_t& reloc) {
  constexpr bool IsGeneral = Mode == RelocMode::General;

  void* const rel_target = reinterpret_cast<void*>(reloc.r_offset + relocator.si->load_bias);
  const uint32_t r_type = ELFW(R_TYPE)(reloc.r_info);
  const uint32_t r_sym = ELFW(R_SYM)(reloc.r_info);

  soinfo* found_in = nullptr;
  const ElfW(Sym)* sym = nullptr;
  const char* sym_name = nullptr;
  ElfW(Addr) sym_addr = 0;

  if (r_sym != 0) {
    sym_name = relocator.get_string(relocator.si_symtab[r_sym].st_name);
  }

  // While relocating a DSO with text relocations (obsolete and 32-bit only), the .text segment is
  // writable (but not executable). To call an ifunc, temporarily remap the segment as executable
  // (but not writable). Then switch it back to continue applying relocations in the segment.
#if defined(__LP64__)
  const bool handle_text_relocs = false;
  auto protect_segments = []() { return true; };
  auto unprotect_segments = []() { return true; };
#else
  const bool handle_text_relocs = IsGeneral && relocator.si->has_text_relocations;
  auto protect_segments = [&]() {
    // Make .text executable.
    if (phdr_table_protect_segments(relocator.si->phdr, relocator.si->phnum,
                                    relocator.si->load_bias) < 0) {
      DL_ERR("can't protect segments for \"%s\": %s",
             relocator.si->get_realpath(), strerror(errno));
      return false;
    }
    return true;
  };
  auto unprotect_segments = [&]() {
    // Make .text writable.
    if (phdr_table_unprotect_segments(relocator.si->phdr, relocator.si->phnum,
                                      relocator.si->load_bias) < 0) {
      DL_ERR("can't unprotect loadable segments for \"%s\": %s",
             relocator.si->get_realpath(), strerror(errno));
      return false;
    }
    return true;
  };
#endif

  auto trace_reloc = [](const char* fmt, ...) __printflike(2, 3) {
    if (IsGeneral &&
        g_ld_debug_verbosity > LINKER_VERBOSITY_TRACE &&
        DO_TRACE_RELO) {
      va_list ap;
      va_start(ap, fmt);
      linker_log_va_list(LINKER_VERBOSITY_TRACE, fmt, ap);
      va_end(ap);
    }
  };

  // Skip symbol lookup for R_GENERIC_NONE relocations.
  if (__predict_false(r_type == R_GENERIC_NONE)) {
    trace_reloc("RELO NONE");
    return true;
  }

#if defined(USE_RELA)
  auto get_addend_rel   = [&]() -> ElfW(Addr) { return reloc.r_addend; };
  auto get_addend_norel = [&]() -> ElfW(Addr) { return reloc.r_addend; };
#else
  auto get_addend_rel   = [&]() -> ElfW(Addr) { return *static_cast<ElfW(Addr)*>(rel_target); };
  auto get_addend_norel = [&]() -> ElfW(Addr) { return 0; };
#endif

  if (IsGeneral && is_tls_reloc(r_type)) {
    if (r_sym == 0) {
      // By convention in ld.bfd and lld, an omitted symbol on a TLS relocation
      // is a reference to the current module.
      found_in = relocator.si;
    } else if (ELF_ST_BIND(relocator.si_symtab[r_sym].st_info) == STB_LOCAL) {
      // In certain situations, the Gold linker accesses a TLS symbol using a
      // relocation to an STB_LOCAL symbol in .dynsym of either STT_SECTION or
      // STT_TLS type. Bionic doesn't support these relocations, so issue an
      // error. References:
      //  - https://groups.google.com/d/topic/generic-abi/dJ4_Y78aQ2M/discussion
      //  - https://sourceware.org/bugzilla/show_bug.cgi?id=17699
      sym = &relocator.si_symtab[r_sym];
      DL_ERR("unexpected TLS reference to local symbol \"%s\" in \"%s\": sym type %d, rel type %u",
             sym_name, relocator.si->get_realpath(), ELF_ST_TYPE(sym->st_info), r_type);
      return false;
    } else if (!lookup_symbol<IsGeneral>(relocator, r_sym, sym_name, &found_in, &sym)) {
      return false;
    }
    if (found_in != nullptr && found_in->get_tls() == nullptr) {
      // sym_name can be nullptr if r_sym is 0. A linker should never output an ELF file like this.
      DL_ERR("TLS relocation refers to symbol \"%s\" in solib \"%s\" with no TLS segment",
             sym_name, found_in->get_realpath());
      return false;
    }
    if (sym != nullptr) {
      if (ELF_ST_TYPE(sym->st_info) != STT_TLS) {
        // A toolchain should never output a relocation like this.
        DL_ERR("reference to non-TLS symbol \"%s\" from TLS relocation in \"%s\"",
               sym_name, relocator.si->get_realpath());
        return false;
      }
      sym_addr = sym->st_value;
    }
  } else {
    if (r_sym == 0) {
      // Do nothing.
    } else {
      if (!lookup_symbol<IsGeneral>(relocator, r_sym, sym_name, &found_in, &sym)) return false;
      if (sym != nullptr) {
        const bool should_protect_segments = handle_text_relocs &&
                                             found_in == relocator.si &&
                                             ELF_ST_TYPE(sym->st_info) == STT_GNU_IFUNC;
        if (should_protect_segments && !protect_segments()) return false;
        sym_addr = found_in->resolve_symbol_address(sym);
        if (should_protect_segments && !unprotect_segments()) return false;
      } else if constexpr (IsGeneral) {
        // A weak reference to an undefined symbol. We typically use a zero symbol address, but
        // use the relocation base for PC-relative relocations, so that the value written is zero.
        switch (r_type) {
#if defined(__x86_64__)
          case R_X86_64_PC32:
            sym_addr = reinterpret_cast<ElfW(Addr)>(rel_target);
            break;
#elif defined(__i386__)
          case R_386_PC32:
            sym_addr = reinterpret_cast<ElfW(Addr)>(rel_target);
            break;
#endif
        }
      }
    }
  }

  if constexpr (IsGeneral || Mode == RelocMode::JumpTable) {
    if (r_type == R_GENERIC_JUMP_SLOT) {
      count_relocation_if<IsGeneral>(kRelocAbsolute);
      const ElfW(Addr) result = sym_addr + get_addend_norel();
      trace_reloc("RELO JMP_SLOT %16p <- %16p %s",
                  rel_target, reinterpret_cast<void*>(result), sym_name);
      *static_cast<ElfW(Addr)*>(rel_target) = result;
      return true;
    }
  }

  if constexpr (IsGeneral || Mode == RelocMode::Typical) {
    // Almost all dynamic relocations are of one of these types, and most will be
    // R_GENERIC_ABSOLUTE. The platform typically uses RELR instead, but R_GENERIC_RELATIVE is
    // common in non-platform binaries.
    if (r_type == R_GENERIC_ABSOLUTE) {
      count_relocation_if<IsGeneral>(kRelocAbsolute);
      const ElfW(Addr) result = sym_addr + get_addend_rel();
      trace_reloc("RELO ABSOLUTE %16p <- %16p %s",
                  rel_target, reinterpret_cast<void*>(result), sym_name);
      *static_cast<ElfW(Addr)*>(rel_target) = result;
      return true;
    } else if (r_type == R_GENERIC_GLOB_DAT) {
      // The i386 psABI specifies that R_386_GLOB_DAT doesn't have an addend. The ARM ELF ABI
      // document (IHI0044F) specifies that R_ARM_GLOB_DAT has an addend, but Bionic isn't adding
      // it.
      count_relocation_if<IsGeneral>(kRelocAbsolute);
      const ElfW(Addr) result = sym_addr + get_addend_norel();
      trace_reloc("RELO GLOB_DAT %16p <- %16p %s",
                  rel_target, reinterpret_cast<void*>(result), sym_name);
      *static_cast<ElfW(Addr)*>(rel_target) = result;
      return true;
    } else if (r_type == R_GENERIC_RELATIVE) {
      // In practice, r_sym is always zero, but if it weren't, the linker would still look up the
      // referenced symbol (and abort if the symbol isn't found), even though it isn't used.
      count_relocation_if<IsGeneral>(kRelocRelative);
      const ElfW(Addr) result = relocator.si->load_bias + get_addend_rel();
      trace_reloc("RELO RELATIVE %16p <- %16p",
                  rel_target, reinterpret_cast<void*>(result));
      *static_cast<ElfW(Addr)*>(rel_target) = result;
      return true;
    }
  }

  if constexpr (!IsGeneral) {
    // Almost all relocations are handled above. Handle the remaining relocations below, in a
    // separate function call. The symbol lookup will be repeated, but the result should be served
    // from the 1-symbol lookup cache.
    return process_relocation_general(relocator, reloc);
  }

  switch (r_type) {
    case R_GENERIC_IRELATIVE:
      // In the linker, ifuncs are called as soon as possible so that string functions work. We must
      // not call them again. (e.g. On arm32, resolving an ifunc changes the meaning of the addend
      // from a resolver function to the implementation.)
      if (!relocator.si->is_linker()) {
        count_relocation_if<IsGeneral>(kRelocRelative);
        const ElfW(Addr) ifunc_addr = relocator.si->load_bias + get_addend_rel();
        trace_reloc("RELO IRELATIVE %16p <- %16p",
                    rel_target, reinterpret_cast<void*>(ifunc_addr));
        if (handle_text_relocs && !protect_segments()) return false;
        const ElfW(Addr) result = call_ifunc_resolver(ifunc_addr);
        if (handle_text_relocs && !unprotect_segments()) return false;
        *static_cast<ElfW(Addr)*>(rel_target) = result;
      }
      break;
    case R_GENERIC_COPY:
      // Copy relocations allow read-only data or code in a non-PIE executable to access a
      // variable from a DSO. The executable reserves extra space in its .bss section, and the
      // linker copies the variable into the extra space. The executable then exports its copy
      // to interpose the copy in the DSO.
      //
      // Bionic only supports PIE executables, so copy relocations aren't supported. The ARM and
      // AArch64 ABI documents only allow them for ET_EXEC (non-PIE) objects. See IHI0056B and
      // IHI0044F.
      DL_ERR("%s COPY relocations are not supported", relocator.si->get_realpath());
      return false;
    case R_GENERIC_TLS_TPREL:
      count_relocation_if<IsGeneral>(kRelocRelative);
      {
        ElfW(Addr) tpoff = 0;
        if (found_in == nullptr) {
          // Unresolved weak relocation. Leave tpoff at 0 to resolve
          // &weak_tls_symbol to __get_tls().
        } else {
          CHECK(found_in->get_tls() != nullptr); // We rejected a missing TLS segment above.
          const TlsModule& mod = get_tls_module(found_in->get_tls()->module_id);
          if (mod.static_offset != SIZE_MAX) {
            tpoff += mod.static_offset - relocator.tls_tp_base;
          } else {
            DL_ERR("TLS symbol \"%s\" in dlopened \"%s\" referenced from \"%s\" using IE access model",
                   sym_name, found_in->get_realpath(), relocator.si->get_realpath());
            return false;
          }
        }
        tpoff += sym_addr + get_addend_rel();
        trace_reloc("RELO TLS_TPREL %16p <- %16p %s",
                    rel_target, reinterpret_cast<void*>(tpoff), sym_name);
        *static_cast<ElfW(Addr)*>(rel_target) = tpoff;
      }
      break;
    case R_GENERIC_TLS_DTPMOD:
      count_relocation_if<IsGeneral>(kRelocRelative);
      {
        size_t module_id = 0;
        if (found_in == nullptr) {
          // Unresolved weak relocation. Evaluate the module ID to 0.
        } else {
          CHECK(found_in->get_tls() != nullptr); // We rejected a missing TLS segment above.
          module_id = found_in->get_tls()->module_id;
        }
        trace_reloc("RELO TLS_DTPMOD %16p <- %zu %s",
                    rel_target, module_id, sym_name);
        *static_cast<ElfW(Addr)*>(rel_target) = module_id;
      }
      break;
    case R_GENERIC_TLS_DTPREL:
      count_relocation_if<IsGeneral>(kRelocRelative);
      {
        const ElfW(Addr) result = sym_addr + get_addend_rel();
        trace_reloc("RELO TLS_DTPREL %16p <- %16p %s",
                    rel_target, reinterpret_cast<void*>(result), sym_name);
        *static_cast<ElfW(Addr)*>(rel_target) = result;
      }
      break;

#if defined(__aarch64__)
    // Bionic currently only implements TLSDESC for arm64. This implementation should work with
    // other architectures, as long as the resolver functions are implemented.
    case R_GENERIC_TLSDESC:
      count_relocation_if<IsGeneral>(kRelocRelative);
      {
        ElfW(Addr) addend = reloc.r_addend;
        TlsDescriptor* desc = static_cast<TlsDescriptor*>(rel_target);
        if (found_in == nullptr) {
          // Unresolved weak relocation.
          desc->func = tlsdesc_resolver_unresolved_weak;
          desc->arg = addend;
          trace_reloc("RELO TLSDESC %16p <- unresolved weak, addend 0x%zx %s",
                      rel_target, static_cast<size_t>(addend), sym_name);
        } else {
          CHECK(found_in->get_tls() != nullptr); // We rejected a missing TLS segment above.
          size_t module_id = found_in->get_tls()->module_id;
          const TlsModule& mod = get_tls_module(module_id);
          if (mod.static_offset != SIZE_MAX) {
            desc->func = tlsdesc_resolver_static;
            desc->arg = mod.static_offset - relocator.tls_tp_base + sym_addr + addend;
            trace_reloc("RELO TLSDESC %16p <- static (0x%zx - 0x%zx + 0x%zx + 0x%zx) %s",
                        rel_target, mod.static_offset, relocator.tls_tp_base,
                        static_cast<size_t>(sym_addr), static_cast<size_t>(addend),
                        sym_name);
          } else {
            relocator.tlsdesc_args->push_back({
              .generation = mod.first_generation,
              .index.module_id = module_id,
              .index.offset = sym_addr + addend,
            });
            // Defer the TLSDESC relocation until the address of the TlsDynamicResolverArg object
            // is finalized.
            relocator.deferred_tlsdesc_relocs.push_back({
              desc, relocator.tlsdesc_args->size() - 1
            });
            const TlsDynamicResolverArg& desc_arg = relocator.tlsdesc_args->back();
            trace_reloc("RELO TLSDESC %16p <- dynamic (gen %zu, mod %zu, off %zu) %s",
                        rel_target, desc_arg.generation, desc_arg.index.module_id,
                        desc_arg.index.offset, sym_name);
          }
        }
      }
      break;
#endif  // defined(__aarch64__)

#if defined(__x86_64__)
    case R_X86_64_32:
      count_relocation_if<IsGeneral>(kRelocAbsolute);
      {
        const Elf32_Addr result = sym_addr + reloc.r_addend;
        trace_reloc("RELO R_X86_64_32 %16p <- 0x%08x %s",
                    rel_target, result, sym_name);
        *static_cast<Elf32_Addr*>(rel_target) = result;
      }
      break;
    case R_X86_64_PC32:
      count_relocation_if<IsGeneral>(kRelocRelative);
      {
        const ElfW(Addr) target = sym_addr + reloc.r_addend;
        const ElfW(Addr) base = reinterpret_cast<ElfW(Addr)>(rel_target);
        const Elf32_Addr result = target - base;
        trace_reloc("RELO R_X86_64_PC32 %16p <- 0x%08x (%16p - %16p) %s",
                    rel_target, result, reinterpret_cast<void*>(target),
                    reinterpret_cast<void*>(base), sym_name);
        *static_cast<Elf32_Addr*>(rel_target) = result;
      }
      break;
#elif defined(__i386__)
    case R_386_PC32:
      count_relocation_if<IsGeneral>(kRelocRelative);
      {
        const ElfW(Addr) target = sym_addr + get_addend_rel();
        const ElfW(Addr) base = reinterpret_cast<ElfW(Addr)>(rel_target);
        const ElfW(Addr) result = target - base;
        trace_reloc("RELO R_386_PC32 %16p <- 0x%08x (%16p - %16p) %s",
                    rel_target, result, reinterpret_cast<void*>(target),
                    reinterpret_cast<void*>(base), sym_name);
        *static_cast<ElfW(Addr)*>(rel_target) = result;
      }
      break;
#endif
    default:
      DL_ERR("unknown reloc type %d in \"%s\"", r_type, relocator.si->get_realpath());
      return false;
  }
  return true;
}

__attribute__((noinline))
static bool process_relocation_general(Relocator& relocator, const rel_t& reloc) {
  return process_relocation_impl<RelocMode::General>(relocator, reloc);
}

template <RelocMode Mode>
__attribute__((always_inline))
static inline bool process_relocation(Relocator& relocator, const rel_t& reloc) {
  return Mode == RelocMode::General ?
      process_relocation_general(relocator, reloc) :
      process_relocation_impl<Mode>(relocator, reloc);
}

template <RelocMode Mode>
__attribute__((noinline))
static bool plain_relocate_impl(Relocator& relocator, rel_t* rels, size_t rel_count) {
  for (size_t i = 0; i < rel_count; ++i) {
    if (!process_relocation<Mode>(relocator, rels[i])) {
      return false;
    }
  }
  return true;
}

template <RelocMode Mode>
__attribute__((noinline))
static bool packed_relocate_impl(Relocator& relocator, sleb128_decoder decoder) {
  return for_all_packed_relocs(decoder, [&](const rel_t& reloc) {
    return process_relocation<Mode>(relocator, reloc);
  });
}

static bool needs_slow_relocate_loop(const Relocator& relocator __unused) {
#if STATS
  // TODO: This could become a run-time flag.
  return true;
#endif
#if !defined(__LP64__)
  if (relocator.si->has_text_relocations) return true;
#endif
  if (g_ld_debug_verbosity > LINKER_VERBOSITY_TRACE) {
    // If linker TRACE() is enabled, then each relocation is logged.
    return true;
  }
  return false;
}

template <RelocMode OptMode, typename ...Args>
static bool plain_relocate(Relocator& relocator, Args ...args) {
  return needs_slow_relocate_loop(relocator) ?
      plain_relocate_impl<RelocMode::General>(relocator, args...) :
      plain_relocate_impl<OptMode>(relocator, args...);
}

template <RelocMode OptMode, typename ...Args>
static bool packed_relocate(Relocator& relocator, Args ...args) {
  return needs_slow_relocate_loop(relocator) ?
      packed_relocate_impl<RelocMode::General>(relocator, args...) :
      packed_relocate_impl<OptMode>(relocator, args...);
}

bool soinfo::relocate(const SymbolLookupList& lookup_list) {

  VersionTracker version_tracker;

  if (!version_tracker.init(this)) {
    return false;
  }

  Relocator relocator(version_tracker, lookup_list);
  relocator.si = this;
  relocator.si_strtab = strtab_;
  relocator.si_strtab_size = has_min_version(1) ? strtab_size_ : SIZE_MAX;
  relocator.si_symtab = symtab_;
  relocator.tlsdesc_args = &tlsdesc_args_;
  relocator.tls_tp_base = __libc_shared_globals()->static_tls_layout.offset_thread_pointer();

  if (android_relocs_ != nullptr) {
    // check signature
    if (android_relocs_size_ > 3 &&
        android_relocs_[0] == 'A' &&
        android_relocs_[1] == 'P' &&
        android_relocs_[2] == 'S' &&
        android_relocs_[3] == '2') {
      DEBUG("[ android relocating %s ]", get_realpath());

      const uint8_t* packed_relocs = android_relocs_ + 4;
      const size_t packed_relocs_size = android_relocs_size_ - 4;

      if (!packed_relocate<RelocMode::Typical>(relocator, sleb128_decoder(packed_relocs, packed_relocs_size))) {
        return false;
      }
    } else {
      DL_ERR("bad android relocation header.");
      return false;
    }
  }

  if (relr_ != nullptr) {
    DEBUG("[ relocating %s relr ]", get_realpath());
    if (!relocate_relr()) {
      return false;
    }
  }

#if defined(USE_RELA)
  if (rela_ != nullptr) {
    DEBUG("[ relocating %s rela ]", get_realpath());

    if (!plain_relocate<RelocMode::Typical>(relocator, rela_, rela_count_)) {
      return false;
    }
  }
  if (plt_rela_ != nullptr) {
    DEBUG("[ relocating %s plt rela ]", get_realpath());
    if (!plain_relocate<RelocMode::JumpTable>(relocator, plt_rela_, plt_rela_count_)) {
      return false;
    }
  }
#else
  if (rel_ != nullptr) {
    DEBUG("[ relocating %s rel ]", get_realpath());
    if (!plain_relocate<RelocMode::Typical>(relocator, rel_, rel_count_)) {
      return false;
    }
  }
  if (plt_rel_ != nullptr) {
    DEBUG("[ relocating %s plt rel ]", get_realpath());
    if (!plain_relocate<RelocMode::JumpTable>(relocator, plt_rel_, plt_rel_count_)) {
      return false;
    }
  }
#endif

  // Once the tlsdesc_args_ vector's size is finalized, we can write the addresses of its elements
  // into the TLSDESC relocations.
#if defined(__aarch64__)
  // Bionic currently only implements TLSDESC for arm64.
  for (const std::pair<TlsDescriptor*, size_t>& pair : relocator.deferred_tlsdesc_relocs) {
    TlsDescriptor* desc = pair.first;
    desc->func = tlsdesc_resolver_dynamic;
    desc->arg = reinterpret_cast<size_t>(&tlsdesc_args_[pair.second]);
  }
#endif

  return true;
}
