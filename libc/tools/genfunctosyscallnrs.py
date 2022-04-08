#!/usr/bin/env python

import argparse
import collections
import logging
import os
import re
import subprocess
import textwrap

from gensyscalls import SupportedArchitectures, SysCallsTxtParser
from genseccomp import parse_syscall_NRs

def load_syscall_names_from_file(file_path, architecture):
  parser = SysCallsTxtParser()
  parser.parse_open_file(open(file_path))
  arch_map = {}
  for syscall in parser.syscalls:
    if syscall.get(architecture):
      arch_map[syscall["func"]] = syscall["name"];

  return arch_map

def gen_syscall_nrs(out_file, base_syscall_file, syscall_NRs):
  for arch in SupportedArchitectures:
    base_names = load_syscall_names_from_file(base_syscall_file, arch)

    for func,syscall in base_names.iteritems():
      out_file.write("#define __" + arch + "_" + func + " " + str(syscall_NRs[arch][syscall]) + ";\n")

def main():
  parser = argparse.ArgumentParser(
      description="Generates a mapping of bionic functions to system call numbers per architecture.")
  parser.add_argument("--verbose", "-v", help="Enables verbose logging.")
  parser.add_argument("--out-dir",
                      help="The output directory for the output files")
  parser.add_argument("base_file", metavar="base-file", type=str,
                      help="The path of the base syscall list (SYSCALLS.TXT).")
  parser.add_argument("files", metavar="FILE", type=str, nargs="+",
                      help=("A syscall name-number mapping file for an architecture.\n"))
  args = parser.parse_args()

  if args.verbose:
    logging.basicConfig(level=logging.DEBUG)
  else:
    logging.basicConfig(level=logging.INFO)

  syscall_files = []
  syscall_NRs = {}
  for filename in args.files:
    m = re.search(r"libseccomp_gen_syscall_nrs_([^/]+)", filename)
    syscall_NRs[m.group(1)] = parse_syscall_NRs(filename)

  output_path = os.path.join(args.out_dir, "func_to_syscall_nrs.h")
  with open(output_path, "w") as output_file:
    gen_syscall_nrs(out_file=output_file,
             syscall_NRs=syscall_NRs, base_syscall_file=args.base_file)

if __name__ == "__main__":
  main()
