#!/usr/bin/env python3

"""Generate the compilation target feature printing source code.

The source code for detecting target features is heavily redundant and
copy-pasted, and is easier to maintain using a generative script.

This script creates the source and the include files in its current
directory.
"""

import argparse
from pathlib import Path
from typing import Dict, List, Iterable

_CPP_BOILERPLATE: str = """\
#include <stdio.h>

#define TO_STRING_EXP(DEF) #DEF
#define TO_STRING(DEF) TO_STRING_EXP(DEF)
"""

_FEATURES = {
    "Aarch64": [
        "__ARM_FEATURE_AES",
        "__ARM_FEATURE_BTI",
        "__ARM_FEATURE_CRC32",
        "__ARM_FEATURE_CRYPTO",
        "__ARM_FEATURE_PAC_DEFAULT",
        "__ARM_FEATURE_SHA2",
        "__ARM_FEATURE_SHA3",
        "__ARM_FEATURE_SHA512",
    ],
    "Arm32": [
        "__ARM_ARCH_ISA_THUMB",
        "__ARM_FEATURE_AES",
        "__ARM_FEATURE_BTI",
        "__ARM_FEATURE_CRC32",
        "__ARM_FEATURE_CRYPTO",
        "__ARM_FEATURE_PAC_DEFAULT",
        "__ARM_FEATURE_SHA2",
    ],
    "X86": [
        "__AES__",
        "__AVX__",
        "__CRC32__",
        "__POPCNT__",
        "__SHA512__",
        "__SHA__",
    ],
    "Riscv": [
        "__riscv_vector",
    ],
}


def _make_function_sig(name: str) -> str:
    return f"void print{name}TargetFeatures()"


def check_template(define: str) -> List[str]:
    return [
        f"#if defined({define})",
        f'  printf("%s=%s\\n", TO_STRING_EXP({define}), TO_STRING({define}));',
        "#else",
        f'  printf("%s not defined\\n", TO_STRING_EXP({define}));',
        "#endif",
    ]


def generate_cpp_file(define_mapping: Dict[str, List[str]]) -> List[str]:
    out: List[str] = _CPP_BOILERPLATE.split("\n")
    for target, defines in define_mapping.items():
        out.append("")
        out.extend(generate_print_function(target, defines))
    return out


def generate_print_function(name: str, defines: List[str]) -> List[str]:
    """Generate a print<DEFINE>TargetFeatures function."""
    function_body = [_make_function_sig(name) + " {"]
    for d in defines:
        function_body.extend(check_template(d))
    function_body.append("}")
    return function_body


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "cpp_in",
        type=Path,
        help="Output path to generate the cpp file.",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    printer_cpp_filepath = args.cpp_in
    printer_cpp_filepath.write_text(
        "\n".join(generate_cpp_file(_FEATURES)), encoding="utf-8"
    )


if __name__ == "__main__":
    main()
