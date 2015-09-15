#!/usr/bin/env python

#------------------------------------------------------------------------------
# Description of the header clean process
#------------------------------------------------------------------------------
# Here is the list of actions performed by this script to clean the original
# kernel headers.
#
# 1. Optimize well-known macros (e.g. __KERNEL__, __KERNEL_STRICT_NAMES)
#
#     This pass gets rid of everything that is guarded by a well-known macro
#     definition. This means that a block like:
#
#        #ifdef __KERNEL__
#        ....
#        #endif
#
#     Will be totally omitted from the output. The optimizer is smart enough to
#     handle all complex C-preprocessor conditional expression appropriately.
#     This means that, for example:
#
#        #if defined(__KERNEL__) || defined(FOO)
#        ...
#        #endif
#
#     Will be transformed into:
#
#        #ifdef FOO
#        ...
#        #endif
#
#     See tools/defaults.py for the list of well-known macros used in this pass,
#     in case you need to update it in the future.
#
#     Note that this also removes any reference to a kernel-specific
#     configuration macro like CONFIG_FOO from the clean headers.
#
#
# 2. Remove variable and function declarations:
#
#   This pass scans non-directive text and only keeps things that look like a
#   typedef/struct/union/enum declaration. This allows us to get rid of any
#   variables or function declarations that should only be used within the
#   kernel anyway (and which normally *should* be guarded by an #ifdef
#   __KERNEL__ ...  #endif block, if the kernel writers were not so messy).
#
#   There are, however, a few exceptions: it is seldom useful to keep the
#   definition of some static inline functions performing very simple
#   operations. A good example is the optimized 32-bit byte-swap function
#   found in:
#
#     arch-arm/asm/byteorder.h
#
#   The list of exceptions is in tools/defaults.py in case you need to update
#   it in the future.
#
#   Note that we do *not* remove macro definitions, including these macro that
#   perform a call to one of these kernel-header functions, or even define other
#   functions. We consider it safe since userland applications have no business
#   using them anyway.
#
#
# 3. Add a standard disclaimer:
#
#   The message:
#
#   /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#
#   Is prepended to each generated header.
#------------------------------------------------------------------------------

import sys, cpp, kernel, glob, os, re, getopt
from defaults import *
from utils import *

def print_error(no_update, msg):
    if no_update:
        panic(msg)
    sys.stderr.write("warning: " + msg)


def cleanupFile(dst_dir, src_dir, rel_path, no_update = True):
    """reads an original header and perform the cleanup operation on it
       this functions returns the destination path and the clean header
       as a single string"""
    # check the header path
    full_path = os.path.join(src_dir, rel_path)

    if not os.path.exists(full_path):
        print_error(no_update, "file does not exist: '%s'\n" % full_path)
        return None, None

    if not os.path.isfile(full_path):
        print_error(no_update, "path is not a file: '%s'\n" % full_path)
        return None, None

    # convert into destination path, extracting architecture if needed
    # and the corresponding list of known static functions
    #
    arch = None
    statics = kernel_known_generic_statics
    m = re.match(r"asm-([\w\d_\+\.\-]+)(/.*)", rel_path)
    if m and m.group(1) != 'generic':
        dst_path = "arch-%s/asm/%s" % m.groups()
        arch = m.group(1)
        statics  = statics.union(kernel_known_statics.get(arch, set()))
    else:
        # process headers under the uapi directory
        # note the "asm" level has been explicitly added in the original
        # kernel header tree for architectural-dependent uapi headers
        m_uapi = re.match(r"(uapi)/([\w\d_\+\.\-]+)(/.*)", rel_path)
        if m_uapi:
            dst_path = rel_path
            m_uapi_arch = re.match(r"asm-([\w\d_\+\.\-]+)", m_uapi.group(2))
            if m_uapi_arch and m_uapi_arch.group(1) != 'generic':
                arch = m_uapi_arch.group(1)
                statics = statics.union(kernel_known_statics.get(arch, set()))
        # common headers (ie non-asm and non-uapi)
        else:
            dst_path = os.path.join("common", rel_path)

    dst_path = os.path.join(dst_dir, dst_path)

    # now, let's parse the file
    #
    parser = cpp.BlockParser()
    blocks = parser.parseFile(full_path)
    if not parser.parsed:
        print_error(no_update, "can't parse '%s'%" % full_path)
        return None, None

    macros = kernel_known_macros.copy()
    if arch and arch in kernel_default_arch_macros:
        macros.update(kernel_default_arch_macros[arch])

    if arch and arch in kernel_arch_token_replacements:
        blocks.replaceTokens(kernel_arch_token_replacements[arch])

    blocks.optimizeMacros(macros)
    blocks.optimizeIf01()
    blocks.removeVarsAndFuncs(statics)
    blocks.replaceTokens(kernel_token_replacements)
    blocks.removeMacroDefines(kernel_ignored_macros)

    out = StringOutput()
    out.write(kernel_disclaimer)
    blocks.writeWithWarning(out, kernel_warning, 4)
    return dst_path, out.get()


if __name__ == "__main__":

    def usage():
        print """\
    usage:  %s [options] <header_path>

        options:
            -v    enable verbose mode

            -u    enabled update mode
                this will try to update the corresponding 'clean header'
                if the content has changed. with this, you can pass more
                than one file on the command-line

            -k<path>  specify path of original kernel headers
            -d<path>  specify path of cleaned kernel headers

        <header_path> must be in a subdirectory of 'original'
    """ % os.path.basename(sys.argv[0])
        sys.exit(1)

    try:
        optlist, args = getopt.getopt(sys.argv[1:], 'uvk:d:')
    except:
        # unrecognized option
        sys.stderr.write("error: unrecognized option\n")
        usage()

    no_update = True
    dst_dir = get_kernel_dir()
    src_dir = get_kernel_headers_original_dir()
    for opt, arg in optlist:
        if opt == '-u':
            no_update = False
        elif opt == '-v':
            logging.basicConfig(level=logging.DEBUG)
        elif opt == '-k':
            src_dir = arg
        elif opt == '-d':
            dst_dir = arg

    if len(args) == 0:
        usage()

    if no_update:
        for path in args:
            dst_path, newdata = cleanupFile(dst_dir, src_dir, path)
            print newdata

        sys.exit(0)

    # now let's update our files.

    b = BatchFileUpdater()

    for path in args:
        dst_path, newdata = cleanupFile(dst_dir, src_dir, path, no_update)
        if not dst_path:
            continue

        b.readFile(dst_path)
        r = b.editFile(dst_path, newdata)
        if r == 0:
            r = "unchanged"
        elif r == 1:
            r = "edited"
        else:
            r = "added"

        print "cleaning: %-*s -> %-*s (%s)" % (35, path, 35, dst_path, r)


    b.updateGitFiles()

    sys.exit(0)
