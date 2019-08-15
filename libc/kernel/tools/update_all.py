#!/usr/bin/env python
#
import sys, cpp, kernel, glob, os, re, getopt, clean_header, subprocess, shutil
from defaults import *
from utils import *

def Usage():
    print """\
  usage: %(progname)s [kernel-original-path] [kernel-modified-path]

    this program is used to update all the auto-generated clean headers
    used by the Bionic C library. it assumes the following:

      - a set of source kernel headers is located in
        'external/kernel-headers/original', relative to the current
        android tree

      - a set of manually modified kernel header files located in
        'external/kernel-headers/modified', relative to the current
        android tree

      - the clean headers will be placed in 'bionic/libc/kernel/arch-<arch>/asm',
        'bionic/libc/kernel/android', etc..
""" % { "progname" : os.path.basename(sys.argv[0]) }
    sys.exit(0)

def ProcessFiles(updater, original_dir, modified_dir, src_rel_dir, update_rel_dir):
    # Delete the old headers before updating to the new headers.
    update_dir = os.path.join(get_kernel_dir(), update_rel_dir)
    shutil.rmtree(update_dir)
    os.mkdir(update_dir, 0755)

    src_dir = os.path.normpath(os.path.join(original_dir, src_rel_dir))
    src_dir_len = len(src_dir) + 1
    mod_src_dir = os.path.join(modified_dir, src_rel_dir)
    update_dir = os.path.join(get_kernel_dir(), update_rel_dir)

    kernel_dir = get_kernel_dir()
    for root, _, files in os.walk(src_dir):
        for file in sorted(files):
            _, ext = os.path.splitext(file)
            if ext != ".h":
                continue
            src_file = os.path.normpath(os.path.join(root, file))
            rel_path = src_file[src_dir_len:]
            # Check to see if there is a modified header to use instead.
            if os.path.exists(os.path.join(mod_src_dir, rel_path)):
                src_file = os.path.join(mod_src_dir, rel_path)
                src_str = os.path.join("<modified>", src_rel_dir, rel_path)
            else:
                src_str = os.path.join("<original>", src_rel_dir, rel_path)
            dst_file = os.path.join(update_dir, rel_path)
            new_data = clean_header.cleanupFile(dst_file, src_file, rel_path)
            if not new_data:
                continue
            updater.readFile(dst_file)
            ret_val = updater.editFile(dst_file, new_data)
            if ret_val == 0:
                state = "unchanged"
            elif ret_val == 1:
                state = "edited"
            else:
                state = "added"
            update_path = os.path.join(update_rel_dir, rel_path)
            print "cleaning %s -> %s (%s)" % (src_str, update_path, state)


# This lets us support regular system calls like __NR_write and also weird
# ones like __ARM_NR_cacheflush, where the NR doesn't come at the start.
def make__NR_name(name):
    if name.startswith('__ARM_NR_'):
        return name
    else:
        return '__NR_%s' % (name)


# Scan Linux kernel asm/unistd.h files containing __NR_* constants
# and write out equivalent SYS_* constants for glibc source compatibility.
def GenerateGlibcSyscallsHeader(updater):
    libc_root = '%s/bionic/libc/' % os.environ['ANDROID_BUILD_TOP']

    # Collect the set of all syscalls for all architectures.
    syscalls = set()
    pattern = re.compile(r'^\s*#\s*define\s*__NR_([a-z_]\S+)')
    for unistd_h in ['kernel/uapi/asm-generic/unistd.h',
                     'kernel/uapi/asm-arm/asm/unistd.h',
                     'kernel/uapi/asm-arm/asm/unistd-common.h',
                     'kernel/uapi/asm-arm/asm/unistd-eabi.h',
                     'kernel/uapi/asm-arm/asm/unistd-oabi.h',
                     'kernel/uapi/asm-x86/asm/unistd_32.h',
                     'kernel/uapi/asm-x86/asm/unistd_64.h',
                     'kernel/uapi/asm-x86/asm/unistd_x32.h']:
        for line in open(os.path.join(libc_root, unistd_h)):
            m = re.search(pattern, line)
            if m:
                nr_name = m.group(1)
                if 'reserved' not in nr_name and 'unused' not in nr_name:
                    syscalls.add(nr_name)

    # Create a single file listing them all.
    # Note that the input files include #if trickery, so even for a single
    # architecture we don't know exactly which ones are available.
    # https://b.corp.google.com/issues/37110151
    content = '/* Generated file. Do not edit. */\n'
    content += '#pragma once\n'

    for syscall in sorted(syscalls):
        nr_name = make__NR_name(syscall)
        content += '#if defined(%s)\n' % nr_name
        content += '  #define SYS_%s %s\n' % (syscall, nr_name)
        content += '#endif\n'

    syscall_file = os.path.join(libc_root, 'include/bits/glibc-syscalls.h')
    updater.readFile(syscall_file)
    updater.editFile(syscall_file, content)


try:
    optlist, args = getopt.getopt(sys.argv[1:], '')
except:
    # Unrecognized option
    sys.stderr.write("error: unrecognized option\n")
    Usage()

if len(optlist) > 0 or len(args) > 2:
    Usage()

if len(args) > 0:
    original_dir = args[0]
else:
    original_dir = get_kernel_headers_original_dir()

if len(args) > 1:
    modified_dir = args[1]
else:
    modified_dir = get_kernel_headers_modified_dir()

if not os.path.isdir(original_dir):
    panic("The kernel directory %s is not a directory\n" % original_dir)

if not os.path.isdir(modified_dir):
    panic("The kernel modified directory %s is not a directory\n" % modified_dir)

updater = BatchFileUpdater()

# Process the original uapi headers first.
ProcessFiles(updater, original_dir, modified_dir, "uapi", "uapi"),

# Now process the special files.
ProcessFiles(updater, original_dir, modified_dir, "scsi", os.path.join("android", "scsi", "scsi"))

updater.updateGitFiles()

# Now re-generate the <bits/glibc-syscalls.h> from the new uapi headers.
updater = BatchFileUpdater()
GenerateGlibcSyscallsHeader(updater)
updater.updateGitFiles()
