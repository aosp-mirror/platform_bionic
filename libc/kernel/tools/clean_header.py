#!/usr/bin/env python
#

import sys, cpp, kernel, glob, os, re, getopt
from defaults import *
from utils import *

noUpdate = 1

def cleanupFile(path, original_path):
    """reads an original header and perform the cleanup operation on it
       this functions returns the destination path and the clean header
       as a single string"""
    # check the header path
    src_path = path

    if not os.path.exists(src_path):
        if noUpdate:
            panic( "file does not exist: '%s'\n" % path )
        sys.stderr.write( "warning: file does not exit: %s\n" % path )
        return None, None

    if not os.path.isfile(src_path):
        if noUpdate:
            panic( "path is not a file: '%s'\n" % path )
        sys.stderr.write( "warning: not a file: %s\n" % path )
        return None, None

    if os.path.commonprefix( [ src_path, original_path ] ) != original_path:
        if noUpdate:
            panic( "file is not in 'original' directory: %s\n" % path );
        sys.stderr.write( "warning: file not in 'original' ignored: %s\n" % path )
        return None, None

    src_path = src_path[len(original_path):]
    if len(src_path) > 0 and src_path[0] == '/':
        src_path = src_path[1:]

    if len(src_path) == 0:
        panic( "oops, internal error, can't extract correct relative path\n" )

    # convert into destination path, extracting architecture if needed
    # and the corresponding list of known static functions
    #
    arch = None
    statics = kernel_known_generic_statics
    m = re.match(r"asm-([\w\d_\+\.\-]+)(/.*)", src_path)
    if m and m.group(1) != 'generic':
        dst_path = "arch-%s/asm/%s" % m.groups()
        arch     = m.group(1)
        statics  = statics.union( kernel_known_statics.get( arch, set() ) )
    else:
        # process headers under the uapi directory
        # note the "asm" level has been explicitly added in the original
        # kernel header tree for architectural-dependent uapi headers
        m_uapi = re.match(r"(uapi)/([\w\d_\+\.\-]+)(/.*)", src_path)
        if m_uapi:
            dst_path = src_path
            m_uapi_arch = re.match(r"asm-([\w\d_\+\.\-]+)", m_uapi.group(2))
            if m_uapi_arch and m_uapi_arch.group(1) != 'generic':
                arch     = m_uapi_arch.group(1)
                statics  = statics.union( kernel_known_statics.get( arch, set() ) )
        # common headers (ie non-asm and non-uapi)
        else:
            dst_path = "common/" + src_path

    dst_path = os.path.normpath( kernel_cleaned_path + "/" + dst_path )

    # now, let's parse the file
    #
    blocks = cpp.BlockParser().parseFile(path)
    if not blocks:
        sys.stderr.write( "error: can't parse '%s'" % path )
        sys.exit(1)

    macros = kernel_known_macros.copy()
    if arch and arch in kernel_default_arch_macros:
        macros.update(kernel_default_arch_macros[arch])

    if arch and arch in kernel_arch_token_replacements:
        blocks.replaceTokens( kernel_arch_token_replacements[arch] )

    blocks.optimizeMacros( macros )
    blocks.optimizeIf01()
    blocks.removeVarsAndFuncs( statics )
    blocks.replaceTokens( kernel_token_replacements )
    blocks.removeComments()
    blocks.removeMacroDefines( kernel_ignored_macros )
    blocks.removeWhiteSpace()

    out = StringOutput()
    out.write( kernel_disclaimer )
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
        optlist, args = getopt.getopt( sys.argv[1:], 'uvk:d:' )
    except:
        # unrecognized option
        sys.stderr.write( "error: unrecognized option\n" )
        usage()

    for opt, arg in optlist:
        if opt == '-u':
            noUpdate = 0
        elif opt == '-v':
            verbose = 1
            D_setlevel(1)
        elif opt == '-k':
            kernel_original_path = arg
        elif opt == '-d':
            kernel_cleaned_path = arg

    if len(args) == 0:
        usage()

    if noUpdate:
        for path in args:
            dst_path, newdata = cleanupFile(path,kernel_original_path)
            print newdata

        sys.exit(0)

    # now let's update our files.

    b = BatchFileUpdater()

    for path in args:
        dst_path, newdata = cleanupFile(path,kernel_original_path)
        if not dst_path:
            continue

        b.readFile( dst_path )
        r = b.editFile( dst_path, newdata )
        if r == 0:
            r = "unchanged"
        elif r == 1:
            r = "edited"
        else:
            r = "added"

        print "cleaning: %-*s -> %-*s (%s)" % ( 35, path, 35, dst_path, r )


    b.updateGitFiles()

    sys.exit(0)
