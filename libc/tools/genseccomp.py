#!/usr/bin/env python
import os
from subprocess import Popen, PIPE
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


def get_names(syscall_files, architecture):
  syscalls = []
  for syscall_file in syscall_files:
    parser = SysCallsTxtParser()
    parser.parse_open_file(syscall_file)
    syscalls += parser.syscalls

  # Select only elements matching required architecture
  syscalls = [x for x in syscalls if architecture in x and x[architecture]]

  # We only want the name
  return [x["name"] for x in syscalls]


def convert_names_to_NRs(names, header_dir):
  # Run preprocessor over the __NR_syscall symbols, including unistd.h,
  # to get the actual numbers
  prefix = "__SECCOMP_"  # prefix to ensure no name collisions
  cpp = Popen(["../../prebuilts/clang/host/linux-x86/clang-stable/bin/clang",
               "-E", "-nostdinc", "-I" + header_dir, "-Ikernel/uapi/", "-"],
              stdin=PIPE, stdout=PIPE)
  cpp.stdin.write("#include <asm/unistd.h>\n")
  for name in names:
    # In SYSCALLS.TXT, there are two arm-specific syscalls whose names start
    # with __ARM__NR_. These we must simply write out as is.
    if not name.startswith("__ARM_NR_"):
      cpp.stdin.write(prefix + name + ", __NR_" + name + "\n")
    else:
      cpp.stdin.write(prefix + name + ", " + name + "\n")
  content = cpp.communicate()[0].split("\n")

  # The input is now the preprocessed source file. This will contain a lot
  # of junk from the preprocessor, but our lines will be in the format:
  #
  #     __SECCOMP_${NAME}, (0 + value)

  syscalls = []
  for line in content:
    if not line.startswith(prefix):
      continue

    # We might pick up extra whitespace during preprocessing, so best to strip.
    name, value = [w.strip() for w in line.split(",")]
    name = name[len(prefix):]

    # Note that some of the numbers were expressed as base + offset, so we
    # need to eval, not just int
    value = eval(value)
    syscalls.append((name, value))

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

  # Add check that we aren't off the bottom of the syscalls
  bpf.insert(0, BPF_JGE.format(ranges[0].begin, 0, str(len(bpf))) + ',')

  # Add the allow calls at the end. If the syscall is not matched, we will
  # continue. This allows the user to choose to match further syscalls, and
  # also to choose the action when we want to block
  bpf.append(BPF_ALLOW + ",")
  return bpf


def convert_bpf_to_output(bpf, architecture):
  header = textwrap.dedent("""\
    // Autogenerated file - edit at your peril!!

    #include <linux/filter.h>
    #include <errno.h>

    #include "seccomp_bpfs.h"
    const sock_filter {architecture}_filter[] = {{
    """).format(architecture=architecture)

  footer = textwrap.dedent("""\

    }};

    const size_t {architecture}_filter_size = sizeof({architecture}_filter) / sizeof(struct sock_filter);
    """).format(architecture=architecture)
  return header + "\n".join(bpf) + footer


def construct_bpf(syscall_files, architecture, header_dir):
  names = get_names(syscall_files, architecture)
  syscalls = convert_names_to_NRs(names, header_dir)
  ranges = convert_NRs_to_ranges(syscalls)
  bpf = convert_ranges_to_bpf(ranges)
  return convert_bpf_to_output(bpf, architecture)


android_syscall_files = ["SYSCALLS.TXT", "SECCOMP_WHITELIST.TXT"]
arm_headers = "kernel/uapi/asm-arm"
arm64_headers = "kernel/uapi/asm-arm64"
arm_architecture = "arm"
arm64_architecture = "arm64"


ANDROID_SYSCALL_FILES = ["SYSCALLS.TXT", "SECCOMP_WHITELIST.TXT"]

POLICY_CONFIGS = [("arm", "kernel/uapi/asm-arm"),
                  ("arm64", "kernel/uapi/asm-arm64")]


def set_dir():
  # Set working directory for predictable results
  os.chdir(os.path.join(os.environ["ANDROID_BUILD_TOP"], "bionic/libc"))


def main():
  set_dir()
  for arch, header_path in POLICY_CONFIGS:
    files = [open(filename) for filename in ANDROID_SYSCALL_FILES]
    output = construct_bpf(files, arch, header_path)

    # And output policy
    existing = ""
    output_path = "seccomp/{}_policy.cpp".format(arch)
    if os.path.isfile(output_path):
      existing = open(output_path).read()
    if output == existing:
      print "File " + output_path + " not changed."
    else:
      with open(output_path, "w") as output_file:
        output_file.write(output)
      print "Generated file " + output_path


if __name__ == "__main__":
  main()
