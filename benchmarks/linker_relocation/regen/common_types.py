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

from enum import Enum
from typing import Any, Dict, Iterator, List, Optional, Set, Tuple


class SymKind(Enum):
    Func = 1
    Var = 2

    def to_json(self) -> str:
        return {SymKind.Func: 'func', SymKind.Var: 'var'}[self]

    @staticmethod
    def from_json(obj: str) -> 'SymKind':
        return {'func': SymKind.Func, 'var': SymKind.Var}[obj]


class SymBind(Enum):
    Global = 1
    Weak = 2

    def to_json(self) -> str:
        return {SymBind.Global: 'global', SymBind.Weak: 'weak'}[self]

    @staticmethod
    def from_json(obj: str) -> 'SymBind':
        return {'global': SymBind.Global, 'weak': SymBind.Weak}[obj]


class DynSymbol:
    def __init__(self, name: str, kind: SymKind, bind: SymBind, defined: bool,
                 ver_type: Optional[str], ver_name: Optional[str]):
        assert ver_type in {None, '@', '@@'}
        self.name: str = name
        self.kind: SymKind = kind
        self.bind: SymBind = bind
        self.defined: bool = defined
        self.ver_type: Optional[str] = ver_type
        self.ver_name: Optional[str] = ver_name

    def to_json(self) -> Dict[str, Any]:
        result: Dict[str, Any] = {}
        result['name'] = self.name
        result['kind'] = self.kind.to_json()
        result['bind'] = self.bind.to_json()
        result['defined'] = self.defined
        result['ver_type'] = self.ver_type
        result['ver_name'] = self.ver_name
        return result

    @staticmethod
    def from_json(obj: Dict[str, Any]) -> 'DynSymbol':
        return DynSymbol(obj['name'],
                         SymKind.from_json(obj['kind']),
                         SymBind.from_json(obj['bind']),
                         obj['defined'],
                         obj['ver_type'],
                         obj['ver_name'])


DynSymbols = Dict[int, DynSymbol]


class SymbolRef:
    def __init__(self, name: str, is_weak: bool, ver: Optional[str]):
        self.name: str = name
        self.is_weak: bool = is_weak
        self.ver: Optional[str] = ver

    def to_json(self) -> Dict[str, Any]:
        result: Dict[str, Any] = {}
        result['name'] = self.name
        result['is_weak'] = self.is_weak
        if self.ver is not None:
            result['ver'] = self.ver
        return result

    @staticmethod
    def from_json(obj: Dict[str, Any]) -> 'SymbolRef':
        return SymbolRef(obj['name'], obj['is_weak'], obj.get('ver'))


class Relocations:
    def __init__(self):
        self.jump_slots: List[SymbolRef] = []
        self.got: List[SymbolRef] = []
        self.symbolic: List[Tuple[int, SymbolRef]] = []
        self.relative: List[int] = []

    def to_json(self) -> Dict[str, Any]:
        result: Dict[str, Any] = {}
        result['jump_slots'] = [sym.to_json() for sym in self.jump_slots]
        result['got'] = [sym.to_json() for sym in self.got]
        result['symbolic'] = [(off, sym.to_json()) for (off, sym) in self.symbolic]
        result['relative'] = self.relative
        return result

    @staticmethod
    def from_json(obj: Dict[str, Any]) -> 'Relocations':
        result = Relocations()
        result.jump_slots = [SymbolRef.from_json(sym) for sym in obj['jump_slots']]
        result.got = [SymbolRef.from_json(sym) for sym in obj['got']]
        result.symbolic = [(off, SymbolRef.from_json(sym)) for (off, sym) in obj['symbolic']]
        result.relative = obj['relative']
        return result


class LoadedLibrary:
    def __init__(self):
        self.soname: str = None
        self.syms: DynSymbols = None
        self.rels: Relocations = None
        self.needed: List[LoadedLibrary] = []

    def to_json(self) -> Dict[str, Any]:
        result: Dict[str, Any] = {}
        result['soname'] = self.soname
        result['syms'] = {name: sym.to_json() for name, sym in self.syms.items()}
        result['rels'] = self.rels.to_json()
        result['needed'] = [lib.soname for lib in self.needed]
        return result

    @staticmethod
    def from_json(obj: Dict[str, Any]) -> Tuple['LoadedLibrary', List[str]]:
        result = LoadedLibrary()
        result.soname = obj['soname']
        result.syms = {name: DynSymbol.from_json(sym) for name, sym in obj['syms'].items()}
        result.rels = Relocations.from_json(obj['rels'])
        return result, obj['needed']


def elf_tree_to_json(tree: LoadedLibrary) -> Dict[str, Any]:
    libraries: Dict[str, LoadedLibrary] = {}
    result: Dict[str, Any] = {}
    result['root'] = tree.soname
    result['libs'] = []
    for lib in bfs_walk(tree):
        result['libs'].append(lib.to_json())
    return result


def json_to_elf_tree(obj: Dict[str, Any]) -> LoadedLibrary:
    libraries: Dict[str, LoadedLibrary] = {}
    all_needed: List[Tuple[LoadedLibrary, List[str]]] = []
    for lib_obj in obj['libs']:
        lib, needed = LoadedLibrary.from_json(lib_obj)
        libraries[lib.soname] = lib
        all_needed.append((lib, needed))
    for lib, needed in all_needed:
        lib.needed = [libraries[x] for x in needed]
    return libraries[obj['root']]


def bfs_walk(tree: LoadedLibrary) -> Iterator[LoadedLibrary]:
    work_list = [tree]
    seen: Set[LoadedLibrary] = set()
    while len(work_list) > 0:
        lib = work_list.pop(0)
        if lib in seen: continue
        seen.add(lib)
        yield lib
        work_list.extend(lib.needed)
