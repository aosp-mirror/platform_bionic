Android Dynamic Linker Design Notes
===================================

Introduction:
-------------

This document provides several notes related to the design of the Android
dynamic linker.


Prelinking:
-----------

System libraries in Android are internally prelinked, which means that
any internal relocations within them are stripped from the corresponding
shared object, in order to reduce size and speed up loading.

Such libraries can only be loaded at the very specific virtual memory address
they have been prelinked to (during the build process). The list of prelinked
system libraries and their corresponding virtual memory address is found in
the file:

   build/core/prelink-linux-<arch>.map

It should be updated each time a new system library is added to the
system.

The prelink step happens at build time, and uses the 'soslim' and 'apriori'
tools:

  - 'apriori' is the real prelink tool which removes relocations from the
    shared object, however, it must be given a list of symbols to remove
    from the file.

  - 'soslim' is used to find symbols in an executable ELF file
    and generate a list that can be passed to 'apriori'.

By default, these tools are only used to remove internal symbols from
libraries, though they have been designed to allow more aggressive
optimizations (e.g. 'global' prelinking and symbol stripping, which
prevent replacing individual system libraries though).

You can disable prelinking at build time by modifying your Android.mk with
a line like:

    LOCAL_PRELINK_MODULE := false


Initialization and Termination functions:
-----------------------------------------

The Unix Sys V Binary Interface standard states that an
executable can have the following entries in its .dynamic
section:

  DT_INIT
      Points to the address of an initialization function
      that must be called when the file is loaded.

  DT_INIT_ARRAY
      Points to an array of function addresses that must be
      called, in-order, to perform initialization. Some of
      the entries in the array can be 0 or -1, and should
      be ignored.

      Note: this is generally stored in a .init_array section

  DT_INIT_ARRAYSZ
      The size of the DT_INITARRAY, if any

  DT_FINI
      Points to the address of a finalization function which
      must be called when the file is unloaded or the process
      terminated.

  DT_FINI_ARRAY
      Same as DT_INITARRAY but for finalizers. Note that the
      functions must be called in reverse-order though

      Note: this is generally stroed in a .fini_array section

  DT_FINI_ARRAYSZ
      Size of FT_FINIARRAY

  DT_PREINIT_ARRAY
      An array similar to DT_INIT_ARRAY which must *only* be
      present in executables, not shared libraries, which contains
      a list of functions that need to be called before any other
      initialization function (i.e. DT_INIT and/or DT_INIT_ARRAY)

      Note: this is generally stroed in a .preinit_array section

  DT_PREINIT_ARRAYSZ
      The size of DT_PREINIT_ARRAY

If both a DT_INIT and DT_INITARRAY entry are present, the DT_INIT
function must be called before the DT_INITARRAY functions.

Consequently, the DT_FINIARRAY must be parsed in reverse order before
the DT_FINI function, if both are available.

Note that the implementation of static C++ constructors is very
much processor dependent, and may use different ELF sections.

On the ARM (see "C++ ABI for ARM" document), the static constructors
must be called explicitely from the DT_INIT_ARRAY, and each one of them
shall register a destructor by calling the special __eabi_atexit()
function (provided by the C library). The DT_FINI_ARRAY is not used
by static C++ destructors.

On x86, the lists of constructors and destructors are placed in special
sections named ".ctors" and ".dtors", and the DT_INIT / DT_FINI functions
are in charge of calling them explicitely.


C Library Usage Restrictions:
-----------------------------

The dynamic linker executable (/system/bin/linker) is built using the
static version of the C library (libc.a), in order to use various functions
and system calls provided by it.

However, it will normally, at runtime, map the shared library version
of the C library (/system/lib/libc.so) as well in the process' address
space. This means that:

- any global variable defined by the C library will appear twice in
  the process address space, at different addresses.

- some functions will be duplicated too, though those that refer to
  global variables will refer to distinct addresses.

This can lead to subtle conflicts, typically for process-specific data that
is managed through the kernel. A good example is the handling of the
end of the data segment, which is normally done through the 'sbrk' or
'brk' system call by the malloc implementation.

If two similar, but distinct, malloc implementations run at the same time,
and if each one thinks it exclusively manages some process settings, hideous
corruption or crashes may occur.

For this very reason, THE DYNAMIC LINKER CANNOT USE malloc()/free() !
That's why it is linked to a special version of the C library that will
abort when any of these functions (or calloc()/realloc()) is called.

Moreover, it cannot use any C library feature that could use these
indirectly. Experience as shown that this meant:

- avoiding any FILE* - related stdio function (fopen, fread, fprintf, etc...)
- avoiding snprintf() with any floating-point formatter ("%f", "%g")

There are probably other cases that haven't been discovered yet, so the
code needs to be very frugal in its use of the C library.

This also explains why the linker's tracing macros are all disabled by
default. Enabling them sometimes creates problems, depending on the process
being loaded, so they should be considered an experimental feature for now.
