#!/usr/bin/env python3
#
# Copyright (C) 2019 The Android Open Source Project
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

# Generate a benchmark using a JSON dump of ELF file symbols and relocations.

import argparse
import codecs
import json
import math
import os
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
import textwrap
import typing
from enum import Enum
from typing import Dict, List, Optional, Set
from subprocess import PIPE, DEVNULL
from pathlib import Path

from common_types import LoadedLibrary, SymbolRef, SymKind, bfs_walk, json_to_elf_tree


g_obfuscate = True
g_benchmark_name = 'linker_reloc_bench'


kBionicSonames: Set[str] = set([
    'libc.so',
    'libdl.so',
    'libdl_android.so',
    'libm.so',
    'ld-android.so',
])

# Skip these symbols so the benchmark runs on multiple C libraries (glibc, Bionic, musl).
kBionicIgnoredSymbols: Set[str] = set([
    '__FD_ISSET_chk',
    '__FD_SET_chk',
    '__assert',
    '__assert2',
    '__b64_ntop',
    '__cmsg_nxthdr',
    '__cxa_thread_atexit_impl',
    '__errno',
    '__gnu_basename',
    '__gnu_strerror_r',
    '__memcpy_chk',
    '__memmove_chk',
    '__memset_chk',
    '__open_2',
    '__openat_2',
    '__pread64_chk',
    '__pread_chk',
    '__read_chk',
    '__readlink_chk',
    '__register_atfork',
    '__sF',
    '__strcat_chk',
    '__strchr_chk',
    '__strcpy_chk',
    '__strlcat_chk',
    '__strlcpy_chk',
    '__strlen_chk',
    '__strncat_chk',
    '__strncpy_chk',
    '__strncpy_chk2',
    '__strrchr_chk',
    '__system_property_area_serial',
    '__system_property_find',
    '__system_property_foreach',
    '__system_property_get',
    '__system_property_read',
    '__system_property_serial',
    '__system_property_set',
    '__umask_chk',
    '__vsnprintf_chk',
    '__vsprintf_chk',
    'android_dlopen_ext',
    'android_set_abort_message',
    'arc4random_buf',
    'dl_unwind_find_exidx',
    'fts_close',
    'fts_open',
    'fts_read',
    'fts_set',
    'getprogname',
    'gettid',
    'isnanf',
    'lseek64',
    'lstat64',
    'mallinfo',
    'malloc_info',
    'pread64',
    'pthread_gettid_np',
    'pwrite64',
    'res_mkquery',
    'strlcpy',
    'strtoll_l',
    'strtoull_l',
    'tgkill',
])


Definitions = Dict[str, LoadedLibrary]

def build_symbol_index(lib: LoadedLibrary) -> Definitions:
    defs: Dict[str, LoadedLibrary] = {}
    for lib in bfs_walk(lib):
        for sym in lib.syms.values():
            if not sym.defined: continue
            defs.setdefault(sym.name, lib)
    return defs


def check_rels(root: LoadedLibrary, defs: Definitions) -> None:
    # Find every symbol for every relocation in the load group.
    has_missing = False
    for lib in bfs_walk(root):
        rels = lib.rels
        for sym in rels.got + rels.jump_slots + [sym for off, sym in rels.symbolic]:
            if sym.name not in defs:
                if sym.is_weak:
                    pass # print('info: weak undefined', lib.soname, r)
                else:
                    print(f'error: {lib.soname}: unresolved relocation to {sym.name}')
                    has_missing = True
    if has_missing: sys.exit('error: had unresolved relocations')


# Obscure names to avoid polluting Android code search.
def rot13(text: str) -> str:
    if g_obfuscate:
        result = codecs.getencoder("rot-13")(text)[0]
        assert isinstance(result, str)
        return result
    else:
        return text


def make_asm_file(lib: LoadedLibrary, is_main: bool, out_filename: Path, map_out_filename: Path,
                  defs: Definitions) -> bool:

    def trans_sym(name: str, ver: Optional[str]) -> Optional[str]:
        nonlocal defs
        d = defs.get(name)
        if d is not None and d.soname in kBionicSonames:
            if name in kBionicIgnoredSymbols: return None
            # Discard relocations to newer Bionic symbols, because there aren't many of them, and
            # they would limit where the benchmark can run.
            if ver == 'LIBC': return name
            return None
        return 'b_' + rot13(name)

    versions: Dict[Optional[str], List[str]] = {}

    with open(out_filename, 'w') as out:
        out.write(f'// AUTO-GENERATED BY {os.path.basename(__file__)} -- do not edit manually\n')
        out.write(f'#include "{g_benchmark_name}_asm.h"\n')
        out.write('.data\n')
        out.write('.p2align 4\n')

        if is_main:
            out.write('.text\n' 'MAIN\n')

        for d in lib.syms.values():
            if not d.defined: continue
            sym = trans_sym(d.name, None)
            if sym is None: continue
            if d.kind == SymKind.Func:
                out.write('.text\n'
                          f'.globl {sym}\n'
                          f'.type {sym},%function\n'
                          f'{sym}:\n'
                          'nop\n')
            else: # SymKind.Var
                out.write('.data\n'
                          f'.globl {sym}\n'
                          f'.type {sym},%object\n'
                          f'{sym}:\n'
                          f'.space __SIZEOF_POINTER__\n')
            versions.setdefault(d.ver_name, []).append(sym)

        out.write('.text\n')
        for r in lib.rels.jump_slots:
            sym = trans_sym(r.name, r.ver)
            if sym is None: continue
            if r.is_weak: out.write(f'.weak {sym}\n')
            out.write(f'CALL({sym})\n')
        out.write('.text\n')
        for r in lib.rels.got:
            sym = trans_sym(r.name, r.ver)
            if sym is None: continue
            if r.is_weak: out.write(f'.weak {sym}\n')
            out.write(f'GOT_RELOC({sym})\n')

        out.write('.data\n')
        out.write('local_label:\n')

        image = []
        for off in lib.rels.relative:
            image.append((off, f'DATA_WORD(local_label)\n'))
        for off, r in lib.rels.symbolic:
            sym = trans_sym(r.name, r.ver)
            if sym is None: continue
            text = f'DATA_WORD({sym})\n'
            if r.is_weak: text += f'.weak {sym}\n'
            image.append((off, text))
        image.sort()

        cur_off = 0
        for off, text in image:
            if cur_off < off:
                out.write(f'.space (__SIZEOF_POINTER__ * {off - cur_off})\n')
                cur_off = off
            out.write(text)
            cur_off += 1

    has_map_file = False
    if len(versions) > 0 and list(versions.keys()) != [None]:
        has_map_file = True
        with open(map_out_filename, 'w') as out:
            if None in versions:
                print(f'error: {out_filename} has both unversioned and versioned symbols')
                print(versions.keys())
                sys.exit(1)
            for ver in sorted(versions.keys()):
                assert ver is not None
                out.write(f'{rot13(ver)} {{\n')
                if len(versions[ver]) > 0:
                    out.write('  global:\n')
                    out.write(''.join(f'    {x};\n' for x in versions[ver]))
                out.write(f'}};\n')

    return has_map_file


class LibNames:
    def __init__(self, root: LoadedLibrary):
        self._root = root
        self._names: Dict[LoadedLibrary, str] = {}
        all_libs = [x for x in bfs_walk(root) if x is not root and x.soname not in kBionicSonames]
        num_digits = math.ceil(math.log10(len(all_libs) + 1))
        if g_obfuscate:
            self._names = {x : f'{i:0{num_digits}}' for i, x in enumerate(all_libs)}
        else:
            self._names = {x : re.sub(r'\.so$', '', x.soname) for x in all_libs}

    def name(self, lib: LoadedLibrary) -> str:
        if lib is self._root:
            return f'{g_benchmark_name}_main'
        else:
            return f'lib{g_benchmark_name}_{self._names[lib]}'


# Generate a ninja file directly that builds the benchmark using a C compiler driver and ninja.
# Using a driver directly can be faster than building with Soong, and it allows testing
# configurations that Soong can't target, like musl.
def make_ninja_benchmark(root: LoadedLibrary, defs: Definitions, cc: str, out: Path) -> None:

    lib_names = LibNames(root)

    def lib_dso_name(lib: LoadedLibrary) -> str:
        return lib_names.name(lib) + '.so'

    ninja = open(out / 'build.ninja', 'w')
    include_path = os.path.relpath(os.path.dirname(__file__) + '/../include', out)
    common_flags = f"-Wl,-rpath-link,. -lm -I{include_path}"
    ninja.write(textwrap.dedent(f'''\
        rule exe
            command = {cc} -fpie -pie $in -o $out {common_flags} $extra_args
        rule dso
            command = {cc} -fpic -shared $in -o $out -Wl,-soname,$out {common_flags} $extra_args
    '''))

    for lib in bfs_walk(root):
        if lib.soname in kBionicSonames: continue

        lib_base_name = lib_names.name(lib)
        asm_name = lib_base_name + '.S'
        map_name = lib_base_name + '.map'
        asm_path = out / asm_name
        map_path = out / map_name

        has_map_file = make_asm_file(lib, lib is root, asm_path, map_path, defs)
        needed = ' '.join([lib_dso_name(x) for x in lib.needed if x.soname not in kBionicSonames])

        if lib is root:
            ninja.write(f'build {lib_base_name}: exe {asm_name} {needed}\n')
        else:
            ninja.write(f'build {lib_dso_name(lib)}: dso {asm_name} {needed}\n')
        if has_map_file:
            ninja.write(f'    extra_args = -Wl,--version-script={map_name}\n')

    ninja.close()

    subprocess.run(['ninja', '-C', str(out), lib_names.name(root)], check=True)


def make_soong_benchmark(root: LoadedLibrary, defs: Definitions, out: Path) -> None:

    lib_names = LibNames(root)

    bp = open(out / 'Android.bp', 'w')
    bp.write(f'// AUTO-GENERATED BY {os.path.basename(__file__)} -- do not edit\n')

    bp.write(f'package {{ default_applicable_licenses: ["bionic_benchmarks_license"], }}\n')
    bp.write(f'cc_defaults {{\n')
    bp.write(f'    name: "{g_benchmark_name}_all_libs",\n')
    bp.write(f'    runtime_libs: [\n')
    for lib in bfs_walk(root):
        if lib.soname in kBionicSonames: continue
        if lib is root: continue
        bp.write(f'        "{lib_names.name(lib)}",\n')
    bp.write(f'    ],\n')
    bp.write(f'}}\n')

    for lib in bfs_walk(root):
        if lib.soname in kBionicSonames: continue

        lib_base_name = lib_names.name(lib)
        asm_name = lib_base_name + '.S'
        map_name = lib_base_name + '.map'
        asm_path = out / asm_name
        map_path = out / map_name

        has_map_file = make_asm_file(lib, lib is root, asm_path, map_path, defs)

        if lib is root:
            bp.write(f'cc_binary {{\n')
            bp.write(f'    defaults: ["{g_benchmark_name}_binary"],\n')
        else:
            bp.write(f'cc_test_library {{\n')
            bp.write(f'    defaults: ["{g_benchmark_name}_library"],\n')
        bp.write(f'    name: "{lib_base_name}",\n')
        bp.write(f'    srcs: ["{asm_name}"],\n')
        bp.write(f'    shared_libs: [\n')
        for need in lib.needed:
            if need.soname in kBionicSonames: continue
            bp.write(f'        "{lib_names.name(need)}",\n')
        bp.write(f'    ],\n')
        if has_map_file:
            bp.write(f'    version_script: "{map_name}",\n')
        bp.write('}\n')

    bp.close()


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=str)
    parser.add_argument('out_dir', type=str)
    parser.add_argument('--ninja', action='store_true',
                        help='Generate a benchmark using a compiler and ninja rather than Soong')
    parser.add_argument('--cc',
                        help='For --ninja, a target-specific C clang driver and flags (e.g. "'
                             '$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android29-clang'
                             ' -fuse-ld=lld")')

    args = parser.parse_args()

    if args.ninja:
        if args.cc is None: sys.exit('error: --cc required with --ninja')

    out = Path(args.out_dir)
    with open(Path(args.input)) as f:
        root = json_to_elf_tree(json.load(f))
    defs = build_symbol_index(root)
    check_rels(root, defs)

    if out.exists(): shutil.rmtree(out)
    os.makedirs(str(out))

    if args.ninja:
        make_ninja_benchmark(root, defs, args.cc, out)
    else:
        make_soong_benchmark(root, defs, out)


if __name__ == '__main__':
    main()
