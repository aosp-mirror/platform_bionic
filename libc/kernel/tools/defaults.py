# this module contains all the defaults used by the generation of cleaned-up headers
# for the Bionic C library
#

import time, os, sys
from utils import *

# the list of supported architectures
kernel_archs = [ 'arm', 'arm64', 'mips', 'x86' ]

# the list of include directories that belong to the kernel
# tree. used when looking for sources...
kernel_dirs = [ "linux", "asm", "asm-generic", "mtd" ]

# path to the directory containing the original kernel headers
kernel_original_path = os.path.normpath( find_program_dir() + '/../../../../external/kernel-headers/original' )

# path to the default location of the cleaned-up headers
kernel_cleaned_path = os.path.normpath( find_program_dir() + '/..' )

# a special value that is used to indicate that a given macro is known to be
# undefined during optimization
kCppUndefinedMacro = "<<<undefined>>>"

# this is the set of known macros we want to totally optimize out from the
# final headers
kernel_known_macros = {
    "__KERNEL__": kCppUndefinedMacro,
    "__KERNEL_STRICT_NAMES":"1",
    "__CHECKER__": kCppUndefinedMacro,
    "__CHECK_ENDIAN__": kCppUndefinedMacro,
    "CONFIG_64BIT": "__LP64__",
    "CONFIG_X86_32": "__i386__",
    "__EXPORTED_HEADERS__": "1",
    }

# define to true if you want to remove all defined(CONFIG_FOO) tests
# from the clean headers. testing shows that this is not strictly necessary
# but just generates cleaner results
kernel_remove_config_macros = True

# maps an architecture to a set of default macros that would be provided by
# toolchain preprocessor
kernel_default_arch_macros = {
    "arm": {"__ARMEB__": kCppUndefinedMacro, "__ARM_EABI__": "1"},
    "arm64": {},
    "mips": {"CONFIG_32BIT":"1", "__MIPSEB__": kCppUndefinedMacro, "__MIPSEL__": "1"},
    "x86": {},
    }

kernel_arch_token_replacements = {
    "arm": {},
    "arm64": {},
    "mips": {"off_t":"__kernel_off_t"},
    "x86": {},
    }

# Replace tokens in the output according to this mapping
kernel_token_replacements = {
    "asm": "__asm__",
    # The kernel usage of __unused for unused struct fields conflicts with the macro defined in <sys/cdefs.h>.
    "__unused": "__linux_unused",
    # The kernel's _NSIG/NSIG are one less than the userspace value, so we need to move them aside.
    "_NSIG": "_KERNEL__NSIG",
    "NSIG": "_KERNEL_NSIG",
    # The kernel's SIGRTMIN/SIGRTMAX are absolute limits; userspace steals a few.
    "SIGRTMIN": "__SIGRTMIN",
    "SIGRTMAX": "__SIGRTMAX",
    }

# this is the set of known static inline functions that we want to keep
# in the final ARM headers. this is only used to keep optimized byteswapping
# static functions and stuff like that.
# TODO: this isn't working!
kernel_known_arm_statics = set(
        [ "___arch__swab32",    # asm-arm/byteorder.h
        ]
    )

kernel_known_arm64_statics = set(
        [
        ]
    )

kernel_known_mips_statics = set(
        [
        ]
    )

kernel_known_x86_statics = set(
        [ "___arch__swab32",  # asm-x86/byteorder.h
          "___arch__swab64",  # asm-x86/byteorder.h
        ]
    )

kernel_known_generic_statics = set(
        [
          "ipt_get_target",  # uapi/linux/netfilter_ipv4/ip_tables.h
          "ip6t_get_target", # uapi/linux/netfilter_ipv6/ip6_tables.h
        ]
    )

# this maps an architecture to the set of static inline functions that
# we want to keep in the final headers
#
kernel_known_statics = {
        "arm" : kernel_known_arm_statics,
        "arm64" : kernel_known_arm64_statics,
        "mips" : kernel_known_mips_statics,
        "x86" : kernel_known_x86_statics,
    }

# this is a list of macros which we want to specifically exclude from
# the generated files.
#
kernel_ignored_macros = set(
        [ "MAXHOSTNAMELEN",  # for some reason, Linux defines it to 64
                             # while most of the BSD code expects this to be 256
                             # so ignore the kernel-provided definition and
                             # define it in the Bionic headers instead
        ]
    )

# this is the standard disclaimer
#
kernel_disclaimer = """\
/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
"""

# This is the warning line that will be inserted every N-th line in the output
kernel_warning = """\
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
"""
