# All the defaults used to generate the cleaned-up uapi headers for bionic.

# the list of include directories that belong to the kernel
# tree. used when looking for sources...
kernel_dirs = [ "linux", "asm", "asm-generic", "mtd" ]

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
    "__HAVE_BUILTIN_BSWAP16__": "1",
    "__HAVE_BUILTIN_BSWAP32__": "1",
    "__HAVE_BUILTIN_BSWAP64__": "1",
    # Use this to remove the struct __kernel_old_timeval definition.
    # Otherwise, there will be two struct timeval definitions when
    # __kernel_old_timeval is renamed to timeval.
    "__kernel_old_timeval": "1",
    }

# This is the set of known kernel data structures we want to remove from
# the final headers. If the map value is non-empty, that means that in
# addition to removing the structure, add a #include to the file.
kernel_structs_to_remove = {
    # Remove these structures since they are still the same as
    # timeval, itimerval.
    "__kernel_old_timeval": None,
    "__kernel_old_itimerval": None,
    # Replace all of the below structures with #include <bits/STRUCT.h>
    "__kernel_sockaddr_storage": "bits/sockaddr_storage.h",
    "epoll_event": "bits/epoll_event.h",
    "flock": "bits/flock.h",
    "flock64": "bits/flock64.h",
    "in_addr": "bits/in_addr.h",
    "ip_mreq_source": "bits/ip_mreq_source.h",
    "ip_msfilter": "bits/ip_msfilter.h",
    "tcphdr": "bits/tcphdr.h",
    "timespec": "bits/timespec.h",
    }

# define to true if you want to remove all defined(CONFIG_FOO) tests
# from the clean headers. testing shows that this is not strictly necessary
# but just generates cleaner results
kernel_remove_config_macros = True

# Maps an architecture to a set of default macros that would be provided by
# the toolchain's preprocessor. Currently only used to remove confusing
# big-endian junk from the 32-bit arm headers.
kernel_default_arch_macros = {
    "arm": {"__ARMEB__": kCppUndefinedMacro, "__ARM_EABI__": "1"},
    }

# Replace tokens in the output according to this mapping.
kernel_token_replacements = {
    # The kernel usage of __unused for unused struct fields conflicts with the macro defined in <sys/cdefs.h>.
    "__unused": "__linux_unused",
    # The kernel usage of C++ keywords causes problems for C++ code so rename.
    "class": "__linux_class",
    "private": "__linux_private",
    "virtual": "__linux_virtual",
    # The non-64 stuff is legacy; msqid64_ds/ipc64_perm is what userspace wants.
    "msqid_ds": "__kernel_legacy_msqid_ds",
    "semid_ds": "__kernel_legacy_semid_ds",
    "shmid_ds": "__kernel_legacy_shmid_ds",
    "ipc_perm": "__kernel_legacy_ipc_perm",
    # The kernel semun isn't usable (https://github.com/android-ndk/ndk/issues/400).
    "semun": "__kernel_legacy_semun",
    # The kernel's _NSIG/NSIG are one less than the userspace value, so we need to move them aside.
    "_NSIG": "_KERNEL__NSIG",
    "NSIG": "_KERNEL_NSIG",
    # The kernel's SIGRTMIN/SIGRTMAX are absolute limits; userspace steals a few.
    "SIGRTMIN": "__SIGRTMIN",
    "SIGRTMAX": "__SIGRTMAX",
    # We want to support both BSD and Linux member names in struct udphdr.
    "udphdr": "__kernel_udphdr",
    # This causes problems when trying to export the headers for the ndk.
    "__attribute_const__": "__attribute__((__const__))",
    # The kernel started using struct __kernel_old_timeval in some places,
    # which is the exact same as struct timeval. Replace that name with
    # timeval so that kernel structures all use the same named structure.
    # If struct __kernel_old_timeval and struct timeval become different,
    # then a different solution needs to be implemented.
    "__kernel_old_timeval": "timeval",
    # Do the same for __kernel_old_itimerval as for timeval.
    "__kernel_old_itimerval": "itimerval",
    # Do the same for __kernel_sockaddr_storage.
    "__kernel_sockaddr_storage": "sockaddr_storage",
    # Replace __packed with __attribute__((__packed__)) to avoid depending
    # on sys/cdefs.h
    "__packed": "__attribute__((__packed__))",
    # Remove unused macros (http://b/262917450).
    "__force": "",
    "__user": "",
    # Rename the kernel's sigaction so we can expose our POSIX one publicly,
    # but translate to the kernel's one internally.
    "sigaction": "__kernel_sigaction",
    }


# Static inline functions that we want to keep.
kernel_known_generic_statics = set(
        [
          "ipt_get_target",  # uapi/linux/netfilter_ipv4/ip_tables.h
          "ip6t_get_target", # uapi/linux/netfilter_ipv6/ip6_tables.h
          # Byte swapping inlines from uapi/linux/swab.h
          # The below functions are the ones we are guaranting we export.
          "__swab16",
          "__swab32",
          "__swab64",
          "__swab16p",
          "__swab32p",
          "__swab64p",
          "__swab16s",
          "__swab32s",
          "__swab64s",
          "__swahw32",
          "__swahb32",
          "__swahw32p",
          "__swahb32p",
          "__swahw32s",
          "__swahb32s",
          # These are required to support the above functions.
          "__fswahw32",
          "__fswahb32",
          # This is used by various macros in <linux/ioprio.h>.
          "ioprio_value",

          # Contact opensource-licensing@ before adding to this set.
        ]
    )
