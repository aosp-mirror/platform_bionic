#!/usr/bin/env python

import argparse
import collections
import logging
import os
import re
import subprocess
import textwrap

from gensyscalls import SysCallsTxtParser


BPF_JGE = "BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, {0}, {1}, {2})"
BPF_ALLOW = "BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW)"


class SyscallRange(object):
  def __init__(self, name, value):
    self.names = [name]
    self.begin = value
    self.end = self.begin + 1

  def __str__(self):
    return "(%s, %s, %s)" % (self.begin, self.end, self.names)

  def add(self, name, value):
    if value != self.end:
      raise ValueError
    self.end += 1
    self.names.append(name)


def load_syscall_names_from_file(file_path, architecture):
  parser = SysCallsTxtParser()
  parser.parse_open_file(open(file_path))
  return set([x["name"] for x in parser.syscalls if x.get(architecture)])


def merge_names(base_names, whitelist_names, blacklist_names):
  if bool(blacklist_names - base_names):
    raise RuntimeError("Blacklist item not in bionic - aborting " + str(
        blacklist_names - base_names))

  return (base_names - blacklist_names) | whitelist_names


def parse_syscall_NRs(names_path):
  # The input is now the preprocessed source file. This will contain a lot
  # of junk from the preprocessor, but our lines will be in the format:
  #
  #    #define __(ARM_)?NR_${NAME} ${VALUE}
  #
  # Where ${VALUE} is a preprocessor expression.

  constant_re = re.compile(
      r'^\s*#define\s+([A-Za-z_][A-Za-z0-9_]+)\s+(.+)\s*$')
  token_re = re.compile(r'\b[A-Za-z_][A-Za-z0-9_]+\b')
  constants = {}
  with open(names_path) as f:
    for line in f:
      m = constant_re.match(line)
      if not m:
        continue
      try:
        name = m.group(1)
        # eval() takes care of any arithmetic that may be done
        value = eval(token_re.sub(lambda x: str(constants[x.group(0)]),
                                  m.group(2)))

        constants[name] = value
      except:
        logging.debug('Failed to parse %s', line)
        pass

  syscalls = {}
  for name, value in constants.iteritems():
    if not name.startswith("__NR_") and not name.startswith("__ARM_NR"):
      continue
    if name.startswith("__NR_"):
      # Remote the __NR_ prefix
      name = name[len("__NR_"):]
    syscalls[name] = value

  return syscalls


def convert_NRs_to_ranges(syscalls):
  # Sort the values so we convert to ranges and binary chop
  syscalls = sorted(syscalls, lambda x, y: cmp(x[1], y[1]))

  # Turn into a list of ranges. Keep the names for the comments
  ranges = []
  for name, value in syscalls:
    if not ranges:
      ranges.append(SyscallRange(name, value))
      continue

    last_range = ranges[-1]
    if last_range.end == value:
      last_range.add(name, value)
    else:
      ranges.append(SyscallRange(name, value))
  return ranges


# Converts the sorted ranges of allowed syscalls to a binary tree bpf
# For a single range, output a simple jump to {fail} or {allow}. We can't set
# the jump ranges yet, since we don't know the size of the filter, so use a
# placeholder
# For multiple ranges, split into two, convert the two halves and output a jump
# to the correct half
def convert_to_intermediate_bpf(ranges):
  if len(ranges) == 1:
    # We will replace {fail} and {allow} with appropriate range jumps later
    return [BPF_JGE.format(ranges[0].end, "{fail}", "{allow}") +
            ", //" + "|".join(ranges[0].names)]
  else:
    half = (len(ranges) + 1) / 2
    first = convert_to_intermediate_bpf(ranges[:half])
    second = convert_to_intermediate_bpf(ranges[half:])
    jump = [BPF_JGE.format(ranges[half].begin, len(first), 0) + ","]
    return jump + first + second


def convert_ranges_to_bpf(ranges):
  bpf = convert_to_intermediate_bpf(ranges)

  # Now we know the size of the tree, we can substitute the {fail} and {allow}
  # placeholders
  for i, statement in enumerate(bpf):
    # Replace placeholder with
    # "distance to jump to fail, distance to jump to allow"
    # We will add a kill statement and an allow statement after the tree
    # With bpfs jmp 0 means the next statement, so the distance to the end is
    # len(bpf) - i - 1, which is where we will put the kill statement, and
    # then the statement after that is the allow statement
    if "{fail}" in statement and "{allow}" in statement:
      bpf[i] = statement.format(fail=str(len(bpf) - i),
                                allow=str(len(bpf) - i - 1))

  # Add the allow calls at the end. If the syscall is not matched, we will
  # continue. This allows the user to choose to match further syscalls, and
  # also to choose the action when we want to block
  bpf.append(BPF_ALLOW + ",")

  # Add check that we aren't off the bottom of the syscalls
  bpf.insert(0, BPF_JGE.format(ranges[0].begin, 0, str(len(bpf))) + ',')
  return bpf


def convert_bpf_to_output(bpf, architecture, name_modifier):
  if name_modifier:
    name_modifier = name_modifier + "_"
  else:
    name_modifier = ""
  header = textwrap.dedent("""\
    // File autogenerated by {self_path} - edit at your peril!!

    #include <linux/filter.h>
    #include <errno.h>

    #include "seccomp/seccomp_bpfs.h"
    const sock_filter {architecture}_{suffix}filter[] = {{
    """).format(self_path=os.path.basename(__file__), architecture=architecture,
                suffix=name_modifier)

  footer = textwrap.dedent("""\

    }};

    const size_t {architecture}_{suffix}filter_size = sizeof({architecture}_{suffix}filter) / sizeof(struct sock_filter);
    """).format(architecture=architecture,suffix=name_modifier)
  return header + "\n".join(bpf) + footer


def construct_bpf(syscalls, architecture, name_modifier):
  ranges = convert_NRs_to_ranges(syscalls)
  bpf = convert_ranges_to_bpf(ranges)
  return convert_bpf_to_output(bpf, architecture, name_modifier)


def gen_policy(name_modifier, out_dir, base_syscall_file, syscall_files, syscall_NRs):
  for arch in ('arm', 'arm64', 'mips', 'mips64', 'x86', 'x86_64'):
    base_names = load_syscall_names_from_file(base_syscall_file, arch)
    whitelist_names = set()
    blacklist_names = set()
    for f in syscall_files:
      if "blacklist" in f.lower():
        blacklist_names |= load_syscall_names_from_file(f, arch)
      else:
        whitelist_names |= load_syscall_names_from_file(f, arch)

    allowed_syscalls = []
    for name in merge_names(base_names, whitelist_names, blacklist_names):
      try:
        allowed_syscalls.append((name, syscall_NRs[arch][name]))
      except:
        logging.exception("Failed to find %s in %s", name, arch)
        raise
    output = construct_bpf(allowed_syscalls, arch, name_modifier)

    # And output policy
    existing = ""
    filename_modifier = "_" + name_modifier if name_modifier else ""
    output_path = os.path.join(out_dir,
                               "{}{}_policy.cpp".format(arch, filename_modifier))
    with open(output_path, "w") as output_file:
      output_file.write(output)


def main():
  parser = argparse.ArgumentParser(
      description="Generates a seccomp-bpf policy")
  parser.add_argument("--verbose", "-v", help="Enables verbose logging.")
  parser.add_argument("--name-modifier",
                      help=("Specifies the name modifier for the policy. "
                            "One of {app,global,system}."))
  parser.add_argument("--out-dir",
                      help="The output directory for the policy files")
  parser.add_argument("base_file", metavar="base-file", type=str,
                      help="The path of the base syscall list (SYSCALLS.TXT).")
  parser.add_argument("files", metavar="FILE", type=str, nargs="+",
                      help=("The path of the input files. In order to "
                            "simplify the build rules, it can take any of the "
                            "following files: \n"
                            "* /blacklist.*\.txt$/ syscall blacklist.\n"
                            "* /whitelist.*\.txt$/ syscall whitelist.\n"
                            "* otherwise, syscall name-number mapping.\n"))
  args = parser.parse_args()

  if args.verbose:
    logging.basicConfig(level=logging.DEBUG)
  else:
    logging.basicConfig(level=logging.INFO)

  syscall_files = []
  syscall_NRs = {}
  for filename in args.files:
    if filename.lower().endswith('.txt'):
      syscall_files.append(filename)
    else:
      m = re.search(r"libseccomp_gen_syscall_nrs_([^/]+)", filename)
      syscall_NRs[m.group(1)] = parse_syscall_NRs(filename)

  gen_policy(name_modifier=args.name_modifier, out_dir=args.out_dir,
             syscall_NRs=syscall_NRs, base_syscall_file=args.base_file,
             syscall_files=args.files)


if __name__ == "__main__":
  main()
