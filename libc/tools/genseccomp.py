#!/usr/bin/env python3

import argparse
import logging
import operator
import os
import re
import sys
import textwrap

from gensyscalls import SysCallsTxtParser


BPF_JGE = "BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, {0}, {1}, {2})"
BPF_JEQ = "BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K, {0}, {1}, {2})"
BPF_ALLOW = "BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW)"


class SyscallRange:
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
  return {x["name"] for x in parser.syscalls if x.get(architecture)}


def load_syscall_priorities_from_file(file_path):
  format_re = re.compile(r'^\s*([A-Za-z_][A-Za-z0-9_]+)\s*$')
  priorities = []
  with open(file_path) as priority_file:
    for line in priority_file:
      match = format_re.match(line)
      if match is None:
        continue
      try:
        name = match.group(1)
        priorities.append(name)
      except IndexError:
        # TODO: This should be impossible becauase it wouldn't have matched?
        logging.exception('Failed to parse %s from %s', line, file_path)

  return priorities


def merge_names(base_names, allowlist_names, blocklist_names):
  if bool(blocklist_names - base_names):
    raise RuntimeError("blocklist item not in bionic - aborting " + str(
        blocklist_names - base_names))

  return (base_names - blocklist_names) | allowlist_names


def extract_priority_syscalls(syscalls, priorities):
  # Extract syscalls that are not in the priority list
  other_syscalls = \
    [syscall for syscall in syscalls if syscall[0] not in priorities]
  # For prioritized syscalls, keep the order in which they appear in th
  # priority list
  syscall_dict = {syscall[0]: syscall[1] for syscall in syscalls}
  priority_syscalls = []
  for name in priorities:
    if name in syscall_dict.keys():
      priority_syscalls.append((name, syscall_dict[name]))
  return priority_syscalls, other_syscalls


def parse_syscall_NRs(names_path):
  # The input is now the preprocessed source file. This will contain a lot
  # of junk from the preprocessor, but our lines will be in the format:
  #
  #    #define __(ARM_)?NR_${NAME} ${VALUE}
  #
  # Where ${VALUE} is a preprocessor expression.
  #
  # Newer architectures have things like this though:
  #
  #    #define __NR3264_fcntl 25
  #    #define __NR_fcntl __NR3264_fcntl
  #
  # So we need to keep track of the __NR3264_* constants and substitute them.

  line_re = re.compile(r'^# \d+ ".*".*')
  undef_re = re.compile(r'^#undef\s.*')
  define_re = re.compile(r'^\s*#define\s+([A-Za-z0-9_(,)]+)(?:\s+(.+))?\s*$')
  token_re = re.compile(r'\b[A-Za-z_][A-Za-z0-9_]+\b')
  constants = {}
  nr3264s = {}
  with open(names_path) as f:
    for line in f:
      line = line.strip()
      m = define_re.match(line)
      if m:
        name = m.group(1)
        value = m.group(2)
        if name.startswith('__NR3264'):
          nr3264s[name] = value
        elif name.startswith('__NR_') or name.startswith('__ARM_NR_'):
          if value in nr3264s:
            value = nr3264s[value]
          # eval() takes care of any arithmetic that may be done
          value = eval(token_re.sub(lambda x: str(constants[x.group(0)]), value))

          constants[name] = value
      else:
        if not line_re.match(line) and not undef_re.match(line) and line:
          print('%s: failed to parse line `%s`' % (names_path, line))
          sys.exit(1)

  syscalls = {}
  for name, value in constants.items():
    # Remove the __NR_ prefix.
    # TODO: why not __ARM_NR too?
    if name.startswith("__NR_"):
      name = name[len("__NR_"):]
    syscalls[name] = value

  return syscalls


def convert_NRs_to_ranges(syscalls):
  # Sort the values so we convert to ranges and binary chop
  syscalls = sorted(syscalls, key=operator.itemgetter(1))

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

  half = (len(ranges) + 1) // 2
  first = convert_to_intermediate_bpf(ranges[:half])
  second = convert_to_intermediate_bpf(ranges[half:])
  jump = [BPF_JGE.format(ranges[half].begin, len(first), 0) + ","]
  return jump + first + second


# Converts the prioritized syscalls to a bpf list that  is prepended to the
# tree generated by convert_to_intermediate_bpf(). If we hit one of these
# syscalls, shortcut to the allow statement at the bottom of the tree
# immediately
def convert_priority_to_intermediate_bpf(priority_syscalls):
  result = []
  for syscall in priority_syscalls:
    result.append(BPF_JEQ.format(syscall[1], "{allow}", 0) +
                  ", //" + syscall[0])
  return result


def convert_ranges_to_bpf(ranges, priority_syscalls):
  bpf = convert_priority_to_intermediate_bpf(priority_syscalls) + \
    convert_to_intermediate_bpf(ranges)

  # Now we know the size of the tree, we can substitute the {fail} and {allow}
  # placeholders
  for i, statement in enumerate(bpf):
    # Replace placeholder with
    # "distance to jump to fail, distance to jump to allow"
    # We will add a kill statement and an allow statement after the tree
    # With bpfs jmp 0 means the next statement, so the distance to the end is
    # len(bpf) - i - 1, which is where we will put the kill statement, and
    # then the statement after that is the allow statement
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


def construct_bpf(syscalls, architecture, name_modifier, priorities):
  priority_syscalls, other_syscalls = \
    extract_priority_syscalls(syscalls, priorities)
  ranges = convert_NRs_to_ranges(other_syscalls)
  bpf = convert_ranges_to_bpf(ranges, priority_syscalls)
  return convert_bpf_to_output(bpf, architecture, name_modifier)


def gen_policy(name_modifier, out_dir, base_syscall_file, syscall_files,
               syscall_NRs, priority_file):
  for arch in syscall_NRs.keys():
    base_names = load_syscall_names_from_file(base_syscall_file, arch)
    allowlist_names = set()
    blocklist_names = set()
    for f in syscall_files:
      if "blocklist" in f.lower():
        blocklist_names |= load_syscall_names_from_file(f, arch)
      else:
        allowlist_names |= load_syscall_names_from_file(f, arch)
    priorities = []
    if priority_file:
      priorities = load_syscall_priorities_from_file(priority_file)

    allowed_syscalls = []
    for name in sorted(merge_names(base_names, allowlist_names, blocklist_names)):
      try:
        allowed_syscalls.append((name, syscall_NRs[arch][name]))
      except:
        logging.exception("Failed to find %s in %s (%s)", name, arch, syscall_NRs[arch])
        raise
    output = construct_bpf(allowed_syscalls, arch, name_modifier, priorities)

    # And output policy
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
                            "One of {app,system}."))
  parser.add_argument("--out-dir",
                      help="The output directory for the policy files")
  parser.add_argument("base_file", metavar="base-file", type=str,
                      help="The path of the base syscall list (SYSCALLS.TXT).")
  parser.add_argument("files", metavar="FILE", type=str, nargs="+",
                      help=("The path of the input files. In order to "
                            "simplify the build rules, it can take any of the "
                            "following files: \n"
                            "* /blocklist.*\\.txt$/ syscall blocklist.\n"
                            "* /allowlist.*\\.txt$/ syscall allowlist.\n"
                            "* /priority.txt$/ priorities for bpf rules.\n"
                            "* otherwise, syscall name-number mapping.\n"))
  args = parser.parse_args()

  if args.verbose:
    logging.basicConfig(level=logging.DEBUG)
  else:
    logging.basicConfig(level=logging.INFO)

  syscall_files = []
  priority_file = None
  syscall_NRs = {}
  for filename in args.files:
    if filename.lower().endswith('.txt'):
      if filename.lower().endswith('priority.txt'):
        priority_file = filename
      else:
        syscall_files.append(filename)
    else:
      m = re.search(r"libseccomp_gen_syscall_nrs_([^/]+)", filename)
      syscall_NRs[m.group(1)] = parse_syscall_NRs(filename)

  gen_policy(name_modifier=args.name_modifier, out_dir=args.out_dir,
             syscall_NRs=syscall_NRs, base_syscall_file=args.base_file,
             syscall_files=syscall_files, priority_file=priority_file)


if __name__ == "__main__":
  main()
