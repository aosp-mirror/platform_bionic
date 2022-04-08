#!/usr/bin/env python
# Unit tests for genseccomp.py

import cStringIO
import textwrap
import unittest

import genseccomp

class TestGenseccomp(unittest.TestCase):
  def setUp(self):
    genseccomp.set_dir()

  def get_config(self, arch):
    for i in genseccomp.POLICY_CONFIGS:
      if i[0] == arch:
        return i
    self.fail("No such architecture")

  def get_headers(self, arch):
    return self.get_config(arch)[1]

  def get_switches(self, arch):
    return self.get_config(arch)[2]

  def test_get_names(self):
    bionic = cStringIO.StringIO(textwrap.dedent("""\
int __llseek:_llseek(int, unsigned long, unsigned long, off64_t*, int) arm,x86
int         fchown:fchown(int, uid_t, gid_t)    arm64,x86_64
    """))

    whitelist = cStringIO.StringIO(textwrap.dedent("""\
ssize_t     read(int, void*, size_t)        all
    """))

    empty = cStringIO.StringIO(textwrap.dedent("""\
    """))

    names = genseccomp.get_names([bionic, whitelist, empty], "arm")
    bionic.seek(0)
    whitelist.seek(0)
    empty.seek(0)
    names64 = genseccomp.get_names([bionic, whitelist, empty], "arm64")
    bionic.seek(0)
    whitelist.seek(0)
    empty.seek(0)

    self.assertIn("fchown", names64)
    self.assertNotIn("fchown", names)
    self.assertIn("_llseek", names)
    self.assertNotIn("_llseek", names64)
    self.assertIn("read", names)
    self.assertIn("read", names64)

    # Blacklist item must be in bionic
    blacklist = cStringIO.StringIO(textwrap.dedent("""\
int         fchown2:fchown2(int, uid_t, gid_t)    arm64,x86_64
    """))
    with self.assertRaises(RuntimeError):
      genseccomp.get_names([bionic, whitelist, blacklist], "arm")
    bionic.seek(0)
    whitelist.seek(0)
    blacklist.seek(0)

    # Test blacklist item is removed
    blacklist = cStringIO.StringIO(textwrap.dedent("""\
int         fchown:fchown(int, uid_t, gid_t)    arm64,x86_64
    """))
    names = genseccomp.get_names([bionic, whitelist, blacklist], "arm64")
    bionic.seek(0)
    whitelist.seek(0)
    blacklist.seek(0)
    self.assertIn("read", names)
    self.assertNotIn("fchown", names)

    # Blacklist item must not be in whitelist
    whitelist = cStringIO.StringIO(textwrap.dedent("""\
int         fchown:fchown(int, uid_t, gid_t)    arm64,x86_64
    """))
    with self.assertRaises(RuntimeError):
      genseccomp.get_names([empty, whitelist, blacklist], "arm")
    empty.seek(0)
    whitelist.seek(0)
    blacklist.seek(0)

    # No dups in bionic and whitelist
    whitelist = cStringIO.StringIO(textwrap.dedent("""\
int __llseek:_llseek(int, unsigned long, unsigned long, off64_t*, int) arm,x86
    """))
    with self.assertRaises(RuntimeError):
      genseccomp.get_names([bionic, whitelist, empty], "arm")
    bionic.seek(0)
    whitelist.seek(0)
    empty.seek(0)

  def test_convert_names_to_NRs(self):
    self.assertEquals(genseccomp.convert_names_to_NRs(["open"],
                                                      self.get_headers("arm"),
                                                      self.get_switches("arm")),
                      [("open", 5)])

    self.assertEquals(genseccomp.convert_names_to_NRs(["__ARM_NR_set_tls"],
                                                      self.get_headers("arm"),
                                                      self.get_switches("arm")),
                      [('__ARM_NR_set_tls', 983045)])

    self.assertEquals(genseccomp.convert_names_to_NRs(["openat"],
                                                      self.get_headers("arm64"),
                                                      self.get_switches("arm64")),
                      [("openat", 56)])

    self.assertEquals(genseccomp.convert_names_to_NRs(["openat"],
                                                      self.get_headers("x86"),
                                                      self.get_switches("x86")),
                      [("openat", 295)])

    self.assertEquals(genseccomp.convert_names_to_NRs(["openat"],
                                                      self.get_headers("x86_64"),
                                                      self.get_switches("x86_64")),
                      [("openat", 257)])


  def test_convert_NRs_to_ranges(self):
    ranges = genseccomp.convert_NRs_to_ranges([("b", 2), ("a", 1)])
    self.assertEquals(len(ranges), 1)
    self.assertEquals(ranges[0].begin, 1)
    self.assertEquals(ranges[0].end, 3)
    self.assertItemsEqual(ranges[0].names, ["a", "b"])

    ranges = genseccomp.convert_NRs_to_ranges([("b", 3), ("a", 1)])
    self.assertEquals(len(ranges), 2)
    self.assertEquals(ranges[0].begin, 1)
    self.assertEquals(ranges[0].end, 2)
    self.assertItemsEqual(ranges[0].names, ["a"])
    self.assertEquals(ranges[1].begin, 3)
    self.assertEquals(ranges[1].end, 4)
    self.assertItemsEqual(ranges[1].names, ["b"])

  def test_convert_to_intermediate_bpf(self):
    ranges = genseccomp.convert_NRs_to_ranges([("b", 2), ("a", 1)])
    bpf = genseccomp.convert_to_intermediate_bpf(ranges)
    self.assertEquals(bpf, ['BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 3, {fail}, {allow}), //a|b'])

    ranges = genseccomp.convert_NRs_to_ranges([("b", 3), ("a", 1)])
    bpf = genseccomp.convert_to_intermediate_bpf(ranges)
    self.assertEquals(bpf, ['BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 3, 1, 0),',
                            'BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 2, {fail}, {allow}), //a',
                            'BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4, {fail}, {allow}), //b'])

  def test_convert_ranges_to_bpf(self):
    ranges = genseccomp.convert_NRs_to_ranges([("b", 2), ("a", 1)])
    bpf = genseccomp.convert_ranges_to_bpf(ranges)
    self.assertEquals(bpf, ['BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 1, 0, 2),',
                            'BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 3, 1, 0), //a|b',
                            'BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),'])

    ranges = genseccomp.convert_NRs_to_ranges([("b", 3), ("a", 1)])
    bpf = genseccomp.convert_ranges_to_bpf(ranges)
    self.assertEquals(bpf, ['BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 1, 0, 4),',
                            'BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 3, 1, 0),',
                            'BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 2, 2, 1), //a',
                            'BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4, 1, 0), //b',
                            'BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),'])

  def test_convert_bpf_to_output(self):
    output = genseccomp.convert_bpf_to_output(["line1", "line2"], "arm")
    expected_output = textwrap.dedent("""\
    // Autogenerated file - edit at your peril!!

    #include <linux/filter.h>
    #include <errno.h>

    #include "seccomp_bpfs.h"
    const sock_filter arm_filter[] = {
    line1
    line2
    };

    const size_t arm_filter_size = sizeof(arm_filter) / sizeof(struct sock_filter);
    """)
    self.assertEquals(output, expected_output)

  def test_construct_bpf(self):
    syscalls = cStringIO.StringIO(textwrap.dedent("""\
    int __llseek:_llseek(int, unsigned long, unsigned long, off64_t*, int) arm,x86
    int         fchown:fchown(int, uid_t, gid_t)    arm64,x86_64
    """))

    whitelist = cStringIO.StringIO(textwrap.dedent("""\
    ssize_t     read(int, void*, size_t)        all
    """))

    blacklist = cStringIO.StringIO(textwrap.dedent("""\
    """))

    syscall_files = [syscalls, whitelist, blacklist]
    output = genseccomp.construct_bpf(syscall_files, "arm", self.get_headers("arm"),
                                      self.get_switches("arm"))

    expected_output = textwrap.dedent("""\
    // Autogenerated file - edit at your peril!!

    #include <linux/filter.h>
    #include <errno.h>

    #include "seccomp_bpfs.h"
    const sock_filter arm_filter[] = {
    BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 3, 0, 4),
    BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 140, 1, 0),
    BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4, 2, 1), //read
    BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 141, 1, 0), //_llseek
    BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),
    };

    const size_t arm_filter_size = sizeof(arm_filter) / sizeof(struct sock_filter);
    """)
    self.assertEquals(output, expected_output)


if __name__ == '__main__':
  unittest.main()
