#!/usr/bin/env python

# This tool is used to generate the version scripts for libc, libm, libdl,
# and libstdc++ for every architecture.

# usage: generate-version-script.py ARCH INPUT OUTPUT

import sys

def has_arch_tags(tags):
  for arch in ["arm", "arm64", "x86", "x86_64"]:
    if arch in tags:
      return True
  return False

def GenerateVersionScript(arch, in_filename, out_filename):
  with open(out_filename, "w") as fout:
    with open(in_filename, "r") as fin:
      for line in fin:
        index = line.find("#")
        if index != -1:
          tags = line[index+1:].split()
          if arch not in tags and has_arch_tags(tags):
            continue
        fout.write(line)

arch = sys.argv[1]
in_filename = sys.argv[2]
out_filename = sys.argv[3]
GenerateVersionScript(arch, in_filename, out_filename)
