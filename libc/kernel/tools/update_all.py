#!/usr/bin/env python
#
import sys, cpp, kernel, glob, os, re, getopt, clean_header, subprocess
from defaults import *
from utils import *

def usage():
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
        'bionic/libc/kernel/common', etc..
""" % { "progname" : os.path.basename(sys.argv[0]) }
    sys.exit(0)

try:
    optlist, args = getopt.getopt(sys.argv[1:], '')
except:
    # unrecognized option
    sys.stderr.write("error: unrecognized option\n")
    usage()

if len(optlist) > 0 or len(args) > 2:
    usage()

modified_dir = get_kernel_headers_modified_dir()
if len(args) == 1 or len(args) == 2:
    original_dir = args[0]
    if not os.path.isdir(original_dir):
        panic("Not a directory: %s\n" % original_dir)

    if len(args) == 2:
        modified_dir = args[1]
        if not os.path.isdir(modified_dir):
            panic("Not a directory: %s\n" % modified_dir)
else:
    original_dir = get_kernel_headers_original_dir()
    if not os.path.isdir(original_dir):
        panic("Missing directory, please specify one through command-line: %s\n" % original_dir)

if not os.path.isdir(modified_dir):
    modified_dir = None

# Find all source files in 'original'.
sources = dict()
original_dir = os.path.normpath(original_dir)
original_dir_len = len(original_dir) + 1
for root, _, files in os.walk(original_dir):
    for file in files:
        _, ext = os.path.splitext(file)
        if ext == ".h":
            rel_path = os.path.normpath(os.path.join(root, file))
            rel_path = rel_path[original_dir_len:]
            # Check to see if there is a modified header to use instead.
            if modified_dir and os.path.exists(os.path.join(modified_dir, rel_path)):
                sources[rel_path] = False
            else:
                sources[rel_path] = True


b = BatchFileUpdater()

kernel_dir = get_kernel_dir()
for arch in kernel_archs:
    b.readDir(os.path.join(kernel_dir, "arch-%s" % arch))

b.readDir(os.path.join(kernel_dir, "common"))

oldlen = 120
android_root_len = len(get_android_root()) + 1
for rel_path in sorted(sources):
    if sources[rel_path]:
        src_dir = original_dir
        src_str = "<original>/"
    else:
        src_dir = modified_dir
        src_str = "<modified>/"
    dst_path, newdata = clean_header.cleanupFile(kernel_dir, src_dir, rel_path)
    if not dst_path:
        continue

    dst_path = os.path.join(kernel_dir, dst_path)
    b.readFile(dst_path)
    r = b.editFile(dst_path, newdata)
    if r == 0:
        state = "unchanged"
    elif r == 1:
        state = "edited"
    else:
        state = "added"

    # dst_path is guaranteed to include android root.
    rel_dst_path = dst_path[android_root_len:]
    str = "cleaning: %-*s -> %-*s (%s)" % (35, src_str + rel_path, 35, rel_dst_path, state)
    if sys.stdout.isatty():
        print "%-*s" % (oldlen, str),
        if (r == 0):
            print "\r",
        else:
            print "\n",
            oldlen = 0
    else:
        print str

    oldlen = len(str)

print "%-*s" % (oldlen, "Done!")

b.updateGitFiles()

sys.exit(0)
