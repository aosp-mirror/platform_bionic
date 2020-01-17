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

# Scan an ELF file and its tree of DT_NEEDED ELF files, and dump out a JSON file listing:
#  - each ELF file
#  - its DT_NEEDED entries
#  - its defined symbols
#  - its relocations

import argparse
import json
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
from typing import Any, Set, List, Dict, Optional
from subprocess import PIPE, DEVNULL
from pathlib import Path

from common_types import LoadedLibrary, SymBind, SymKind, DynSymbol, DynSymbols, Relocations, \
    SymbolRef, bfs_walk, elf_tree_to_json


g_readelf_cache: Dict[str, str] = {}
g_path_to_soname_cache: Dict[Path, str] = {}

def do_readelf_query(arguments: List[str]) -> List[str]:
    cmdline = ['llvm-readelf'] + arguments
    key = repr(cmdline)
    if key in g_readelf_cache: return g_readelf_cache[key].splitlines()
    out = subprocess.run(cmdline, check=True, stdout=PIPE).stdout.decode()
    g_readelf_cache[key] = out
    return out.splitlines()


def get_elf_soname(path: Path) -> str:
    if path in g_path_to_soname_cache: return g_path_to_soname_cache[path]
    out = do_readelf_query(['-d', str(path)])
    for line in out:
        m = re.search(r'\(SONAME\)\s+Library soname: \[(.+)\]$', line)
        if not m: continue
        result = m.group(1)
        break
    else:
        result = os.path.basename(path)
    g_path_to_soname_cache[path] = result
    return result


def get_elf_needed(path: Path) -> List[str]:
    result = []
    out = do_readelf_query(['-d', str(path)])
    for line in out:
        m = re.search(r'\(NEEDED\)\s+Shared library: \[(.+)\]$', line)
        if not m: continue
        result.append(m.group(1))
    return result


kSymbolMatcher = re.compile(r'''
    \s+ (\d+) : \s*                 # number
    [0-9a-f]+ \s+                   # value
    [0-9a-f]+ \s+                   # size
    (FUNC|IFUNC|OBJECT|NOTYPE) \s+  # type
    (GLOBAL|WEAK) \s+               # bind
    \w+ \s+                         # vis
    (\d+|UND) \s+                   # ndx
    ([\.\w]+)                       # name
    (?:(@@?)(\w+))?                 # version
    $
''', re.VERBOSE)


def get_dyn_symbols(path: Path) -> DynSymbols:
    kind_lookup = {
        'FUNC': SymKind.Func,
        'IFUNC': SymKind.Func,
        'OBJECT': SymKind.Var,
        'NOTYPE': SymKind.Func,
    }
    bind_lookup = { 'GLOBAL': SymBind.Global, 'WEAK': SymBind.Weak }

    result = {}
    out = do_readelf_query(['--dyn-syms', str(path)])
    for line in out:
        m = kSymbolMatcher.match(line)
        if not m:
            # gLinux currently has a version of llvm-readelf whose output is very different from
            # the current versions of llvm-readelf (or GNU readelf).
            if 'Symbol table of .gnu.hash for image:' in line:
                sys.exit(f'error: obsolete version of llvm-readelf')
            continue

        num, kind, bind, ndx, name, ver_type, ver_name = m.groups()

        if name == '__cfi_check':
            # The linker gives an error like:
            #    CANNOT LINK EXECUTABLE "/data/local/tmp/out-linker-bench/b_libandroid_servers": unaligned __cfi_check in the library "(null)"
            # I am probably breaking some kind of CFI invariant, so strip these out for now.
            continue

        result[int(num)] = DynSymbol(name, kind_lookup[kind], bind_lookup[bind], ndx != 'UND',
                                     ver_type, ver_name)

    return result


kRelocationMatcher = re.compile(r'''
    ([0-9a-f]+) \s+     # offset
    ([0-9a-f]+) \s+     # info
    (\w+)               # type
    (?:
        \s+ [0-9a-f]+ \s+       # symbol value
        ([\.\w]+)               # symbol name
        (?: @@? ([\.\w]+) )?    # version
    )?
    \b
''', re.VERBOSE)


def scan_relocations(path: Path, syms: DynSymbols) -> Relocations:
    result: Relocations = Relocations()
    out = do_readelf_query(['-r', str(path)])
    for line in out:
        m = kRelocationMatcher.match(line)
        if not m: continue

        offset_str, info_str, reloc_name, sym_name, ver = m.groups()

        if len(offset_str) == 8:
            offset = int(offset_str, 16) // 4
            sym_idx = int(info_str, 16) >> 8
        elif len(offset_str) == 16:
            offset = int(offset_str, 16) // 8
            sym_idx = int(info_str, 16) >> 32
        else:
            sys.exit(f'error: invalid offset length: {repr(offset_str)}')

        # TODO: R_ARM_IRELATIVE doesn't work, so skip it.
        if reloc_name == 'R_ARM_IRELATIVE': continue

        if reloc_name in ['R_ARM_RELATIVE', 'R_AARCH64_RELATIVE']:
            assert sym_name is None
            result.relative.append(offset)
        else:
            if sym_name is None:
                sys.exit(f'error: missing symbol for reloc {m.groups()} in {path}')

            is_weak = syms[sym_idx].bind == SymBind.Weak
            symbol = SymbolRef(sym_name, is_weak, ver)

            if reloc_name in ['R_ARM_JUMP_SLOT', 'R_AARCH64_JUMP_SLOT']:
                result.jump_slots.append(symbol)
            elif reloc_name in ['R_ARM_GLOB_DAT', 'R_AARCH64_GLOB_DAT']:
                result.got.append(symbol)
            elif reloc_name in ['R_ARM_ABS32', 'R_AARCH64_ABS64']:
                result.symbolic.append((offset, symbol))
            else:
                sys.exit(f'error: unrecognized reloc {m.groups()} in {path}')

    return result


def load_elf_tree(search_path: List[Path], path: Path) -> LoadedLibrary:

    libraries: Dict[str, LoadedLibrary] = {}

    def find_library(needed: str) -> Optional[LoadedLibrary]:
        nonlocal libraries

        if needed in libraries: return libraries[needed]

        for candidate_dir in search_path:
            candidate_path = candidate_dir / needed
            if candidate_path.exists():
                return load(candidate_path)

        sys.exit(f'error: missing DT_NEEDED lib {needed}!')

    def load(path: Path) -> LoadedLibrary:
        nonlocal libraries

        lib = LoadedLibrary()
        lib.soname = get_elf_soname(path)
        if lib.soname in libraries: sys.exit(f'soname already loaded: {lib.soname}')
        libraries[lib.soname] = lib

        lib.syms = get_dyn_symbols(path)
        lib.rels = scan_relocations(path, lib.syms)

        for needed in get_elf_needed(path):
            needed_lib = find_library(needed)
            if needed_lib is not None:
                lib.needed.append(needed_lib)

        return lib

    return load(path)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=str)
    parser.add_argument('output', type=str)
    parser.add_argument('-L', dest='search_path', metavar='PATH', action='append', type=str, default=[])

    args = parser.parse_args()
    search_path = [Path(p) for p in args.search_path]

    with open(Path(args.output), 'w') as f:
        root = load_elf_tree(search_path, Path(args.input))
        json.dump(elf_tree_to_json(root), f, sort_keys=True, indent=2)


if __name__ == '__main__':
    main()
