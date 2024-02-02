#!/usr/bin/env python3
# Run with directory arguments from any directory, with no special setup
# required.

import os
from pathlib import Path
import re
import sys
from typing import Sequence

VERBOSE = False

copyrights = set()


def warn(s):
    sys.stderr.write("warning: %s\n" % s)


def warn_verbose(s):
    if VERBOSE:
        warn(s)


def is_interesting(path_str: str) -> bool:
    path = Path(path_str.lower())
    uninteresting_extensions = [
        ".bp",
        ".map",
        ".md",
        ".mk",
        ".py",
        ".pyc",
        ".swp",
        ".txt",
        ".xml",
    ]
    if path.suffix in uninteresting_extensions:
        return False
    if path.name in {"notice", "readme", "pylintrc"}:
        return False
    # Backup files for some editors.
    if path.match("*~"):
        return False
    return True


def is_copyright_end(line: str, first_line_was_hash: bool) -> bool:
    endings = [
        " $FreeBSD: ",
        "$Citrus$",
        "$FreeBSD$",
        "*/",
        "From: @(#)",
        # OpenBSD likes to say where stuff originally came from:
        "Original version ID:",
        "\t$Citrus: ",
        "\t$NetBSD: ",
        "\t$OpenBSD: ",
        "\t@(#)",
        "\tcitrus Id: ",
        "\tfrom: @(#)",
        "from OpenBSD:",
    ]
    if first_line_was_hash and not line:
        return True

    for ending in endings:
        if ending in line:
            return True

    return False


def extract_copyright_at(lines: Sequence[str], i: int) -> int:
    first_line_was_hash = lines[i].startswith("#")

    # Do we need to back up to find the start of the copyright header?
    start = i
    if not first_line_was_hash:
        while start > 0:
            if "/*" in lines[start - 1]:
                break
            start -= 1

    # Read comment lines until we hit something that terminates a
    # copyright header.
    while i < len(lines):
        if is_copyright_end(lines[i], first_line_was_hash):
            break
        i += 1

    end = i

    # Trim trailing cruft.
    while end > 0:
        line = lines[end - 1]
        if line not in {
                " *", " * ===================================================="
        }:
            break
        end -= 1

    # Remove C/assembler comment formatting, pulling out just the text.
    clean_lines = []
    for line in lines[start:end]:
        line = line.replace("\t", "    ")
        line = line.replace("/* ", "")
        line = re.sub(r"^ \* ", "", line)
        line = line.replace("** ", "")
        line = line.replace("# ", "")
        if "SPDX-License-Identifier:" in line:
            continue
        if line.startswith("++Copyright++"):
            continue
        line = line.replace("--Copyright--", "")
        line = line.rstrip()
        # These come last and take care of "blank" comment lines.
        if line in {"#", " *", "**", "-"}:
            line = ""
        clean_lines.append(line)

    # Trim blank lines from head and tail.
    while clean_lines[0] == "":
        clean_lines = clean_lines[1:]
    while clean_lines[len(clean_lines) - 1] == "":
        clean_lines = clean_lines[0:(len(clean_lines) - 1)]

    copyrights.add("\n".join(clean_lines))

    return i


def do_file(path: str) -> None:
    raw = Path(path).read_bytes()
    try:
        content = raw.decode("utf-8")
    except UnicodeDecodeError:
        warn("bad UTF-8 in %s" % path)
        content = raw.decode("iso-8859-1")

    lines = content.split("\n")

    if len(lines) <= 4:
        warn_verbose("ignoring short file %s" % path)
        return

    if not "Copyright" in content:
        if "public domain" in content.lower():
            warn_verbose("ignoring public domain file %s" % path)
            return
        warn('no copyright notice found in "%s" (%d lines)' %
             (path, len(lines)))
        return

    # Manually iterate because extract_copyright_at tells us how many lines to
    # skip.
    i = 0
    while i < len(lines):
        if "Copyright" in lines[i] and not "@(#) Copyright" in lines[i]:
            i = extract_copyright_at(lines, i)
        else:
            i += 1


def do_dir(arg):
    for directory, sub_directories, filenames in os.walk(arg):
        if ".git" in sub_directories:
            sub_directories.remove(".git")
        sub_directories = sorted(sub_directories)

        for filename in sorted(filenames):
            path = os.path.join(directory, filename)
            if is_interesting(path):
                do_file(path)


def main() -> None:
    args = sys.argv[1:]
    if len(args) == 0:
        args = ["."]

    for arg in args:
        if os.path.isdir(arg):
            do_dir(arg)
        else:
            do_file(arg)

    for notice in sorted(copyrights):
        print(notice)
        print()
        print("-" * 67)
        print()


if __name__ == "__main__":
    main()
