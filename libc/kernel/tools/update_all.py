#!/usr/bin/env python
#
import sys, cpp, kernel, glob, os, re, getopt, clean_header
from defaults import *
from utils import *

def usage():
    print """\
  usage: %(progname)s [kernel-original-path]

    this program is used to update all the auto-generated clean headers
    used by the Bionic C library. it assumes the following:

      - a set of source kernel headers is located in '../original',
        relative to the program's directory

      - the clean headers will be placed in '../arch-<arch>/asm',
        '../common/linux', '../common/asm-generic', etc..
""" % { "progname" : os.path.basename(sys.argv[0]) }
    sys.exit(0)

try:
    optlist, args = getopt.getopt( sys.argv[1:], '' )
except:
    # unrecognized option
    sys.stderr.write( "error: unrecognized option\n" )
    usage()

if len(optlist) > 0 or len(args) > 1:
    usage()

progdir = find_program_dir()

if len(args) == 1:
    original_dir = args[0]
    if not os.path.isdir(original_dir):
        panic( "Not a directory: %s\n" % original_dir )
else:
    original_dir = kernel_original_path
    if not os.path.isdir(original_dir):
        panic( "Missing directory, please specify one through command-line: %s\n" % original_dir )

# find all source files in 'original'
#
sources = []
for root, dirs, files in os.walk( original_dir ):
    for file in files:
        base, ext = os.path.splitext(file)
        if ext == ".h":
            sources.append( "%s/%s" % (root,file) )

b = BatchFileUpdater()

for arch in kernel_archs:
    b.readDir( os.path.normpath( progdir + "/../arch-%s" % arch ) )

b.readDir( os.path.normpath( progdir + "/../common" ) )

#print "OLD " + repr(b.old_files)

oldlen = 120
for path in sources:
    dst_path, newdata = clean_header.cleanupFile(path, original_dir)
    if not dst_path:
        continue

    b.readFile( dst_path )
    r = b.editFile( dst_path, newdata )
    if r == 0:
        state = "unchanged"
    elif r == 1:
        state = "edited"
    else:
        state = "added"

    str = "cleaning: %-*s -> %-*s (%s)" % ( 35, "<original>" + path[len(original_dir):], 35, dst_path, state )
    if sys.stdout.isatty():
        print "%-*s" % (oldlen,str),
        if (r == 0):
            print "\r",
        else:
            print "\n",
            oldlen = 0
    else:
        print str

    oldlen = len(str)

print "%-*s" % (oldlen,"Done!")

b.updateGitFiles()

sys.exit(0)
