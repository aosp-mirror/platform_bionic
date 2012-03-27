#!/usr/bin/env python
#
# this program is used to find source code that includes linux kernel headers directly
# (e.g. with #include <linux/...> or #include <asm/...>)
#
# then it lists them on the standard output.

import sys, cpp, glob, os, re, getopt, kernel
from utils import *
from defaults import *

program_dir = find_program_dir()

wanted_archs   = kernel_archs
wanted_config  = None

def usage():
    print """\
  usage:  find_headers.py [options] <kernel-root> (file|directory|@listfile)+

     options:
        -c <file>          specify .config file (none by default)

        -a <archs>         used to specify an alternative list
                           of architectures to support
                           ('%s' by default)

        -v                 enable verbose mode

    this program is used to find all the kernel headers that are used
    by a set of source files or directories containing them. the search
    is recursive to find *all* required files.

""" % ( string.join(kernel_archs,",") )
    sys.exit(1)


try:
    optlist, args = getopt.getopt( sys.argv[1:], 'vc:d:a:k:' )
except:
    # unrecognized option
    print "error: unrecognized option"
    usage()

for opt, arg in optlist:
    if opt == '-a':
        wanted_archs = string.split(arg,',')
    elif opt == '-c':
        wanted_config = arg
    elif opt == '-v':
        kernel.verboseSearch = 1
        kernel.verboseFind   = 1
        verbose = 1
    else:
        usage()

if len(args) < 2:
    usage()

kernel_root = args[0]
if not os.path.exists(kernel_root):
    sys.stderr.write( "error: directory '%s' does not exist\n" % kernel_root )
    sys.exit(1)

if not os.path.isdir(kernel_root):
    sys.stderr.write( "error: '%s' is not a directory\n" % kernel_root )
    sys.exit(1)

if not os.path.isdir(kernel_root+"/include/linux"):
    sys.stderr.write( "error: '%s' does not have an 'include/linux' directory\n" % kernel_root )
    sys.exit(1)

if wanted_config:
    if not os.path.exists(wanted_config):
        sys.stderr.write( "error: file '%s' does not exist\n" % wanted_config )
        sys.exit(1)

    if not os.path.isfile(wanted_config):
        sys.stderr.write( "error: '%s' is not a file\n" % wanted_config )
        sys.exit(1)

# find all architectures in the kernel tree
archs   = []
for archdir in os.listdir(kernel_root+"/arch"):
    if os.path.exists("%s/arch/%s/include/asm" % (kernel_root, archdir)):
        if verbose:
            print "Found arch '%s'" % archdir
        archs.append(archdir)

# if we're using the 'kernel_headers' directory, there is only asm/
# and no other asm-<arch> directories
#
in_kernel_headers = False
if len(archs) == 0:
    # this can happen when we're using the 'kernel_headers' directory
    if os.path.isdir(kernel_root+"/asm"):
        in_kernel_headers = True
        archs = [ "arm", "mips"]

# if the user has specified some architectures with -a <archs> ensure that
# all those he wants are available from the kernel include tree
if wanted_archs != None:
    if in_kernel_headers and wanted_archs != [ "arm", "mips" ]:
        sys.stderr.write( "error: when parsing kernel_headers, only 'arm' and 'mips' architectures are supported at the moment\n" )
        sys.exit(1)
    missing = []
    for arch in wanted_archs:
        if arch not in archs:
            missing.append(arch)
    if len(missing) > 0:
        sys.stderr.write( "error: the following requested architectures are not in the kernel tree: " )
        for a in missing:
            sys.stderr.write( " %s" % a )
        sys.stderr.write( "\n" )
        sys.exit(1)

    archs = wanted_archs

# helper function used to walk the user files
def parse_file(path, parser):
    #print "parse %s" % path
    parser.parseFile(path)


# remove previous destination directory
#destdir = "/tmp/bionic-kernel-headers/"
#cleanup_dir(destdir)

# try to read the config file
try:
    cparser = kernel.ConfigParser()
    if wanted_config:
        cparser.parseFile( wanted_config )
except:
    sys.stderr.write( "error: can't parse '%s'" % wanted_config )
    sys.exit(1)

kernel_config = cparser.getDefinitions()

# first, obtain the list of kernel files used by our clients
fparser = kernel.HeaderScanner()
dir_excludes=[".repo","external/kernel-headers","ndk","out","prebuilt","bionic/libc/kernel","development/ndk","external/qemu/distrib"]
walk_source_files( args[1:], parse_file, fparser, excludes=["./"+f for f in dir_excludes] )
headers = fparser.getHeaders()
files   = fparser.getFiles()

# now recursively scan the kernel headers for additionnal sub-included headers
hparser = kernel.KernelHeaderFinder(headers,archs,kernel_root,kernel_config)
headers = hparser.scanForAllArchs()

if 0:    # just for debugging
    dumpHeaderUsers = False

    print "the following %d headers:" % len(headers)
    for h in sorted(headers):
        if dumpHeaderUsers:
            print "  %s (%s)" % (h, repr(hparser.getHeaderUsers(h)))
        else:
            print "  %s" % h

    print "are used by the following %d files:" % len(files)
    for f in sorted(files):
        print "  %s" % f

    sys.exit(0)

for h in sorted(headers):
    print "%s" % h

sys.exit(0)
