Malloc Debug
============

Malloc debug is a method of debugging native memory problems. It can help
detect memory corruption, memory leaks, and use after free issues.

This documentation describes how to enable this feature on versions of
the Android OS, Marshmallow or older. Note: malloc debug was full of bugs
and was not fully functional until KitKat, so using it on a version older
than that is not guaranteed to work at all.

The documentation for malloc debug on newer versions of Android is
[here](README.md).

On these old versions of the OS, you must be able to set system properties
using the setprop command from the shell. This requires the ability to
run as root on the device.

When malloc debug is enabled, it works by adding a shim layer that replaces
the normal allocation calls. The replaced calls are:

* `malloc`
* `free`
* `calloc`
* `realloc`
* `posix_memalign`
* `memalign`
* `malloc_usable_size`

On 32 bit systems, these two deprecated functions are also replaced:

* `pvalloc`
* `valloc`

Any errors detected by the library are reported in the log.

Controlling Malloc Debug Behavior
---------------------------------
Malloc debug is controlled by a system property that takes a numeric value
named libc.debug.malloc. It has only a few distinct modes that enables a
set of different malloc debug checks at once.

Value 1
--------
When enabled, this value creates a special header to all allocations
that contains information about the allocation.

### Backtrace at Allocation Creation
Enable capturing the backtrace of each allocation site. Only the
first 16 frames of the backtrace will be captured.
This option will slow down allocations by an order of magnitude, and
might cause timeouts when trying to start a device.

### Track Live Allocations
All of the currently live allocations will be tracked and can be retrieved
by a call to get\_malloc\_leak\_info (see README\_api.md for details).

Note: If multiple allocations have the same exact backtrace, then only one
entry is returned in the list.

Value 5
-------
When enabled, this value does not create a special header. It only modifies
the content of allocations.

Whenever an allocation is created, initialize the data with a known
pattern (0xeb). This does not happen for the calloc calls.
Whenever an allocation is freed, write a known pattern over the data (0xef).

Value 10
--------
When enabled, this value creates a special header to all allocations
that contains information about the allocation.

This value enables everything enabled with value 1 plus these other options.

### Allocation Guards
A 32 byte buffer is placed before the returned allocation (known as
a front guard). This buffer is filled with the pattern (0xaa). In addition,
a 32 byte buffer is placed after the data for the returned allocation (known
as a rear guard). This buffer is filled with the pattern (0xbb).

When the allocation is freed, both of these guards are verified to contain
the expected patterns. If not, then an error message is printed to the log.

### Free Memory Tracking
When a pointer is freed, do not free the memory right away, but add it to
a list of freed allocations. In addition to being added to the list, the
entire allocation is filled with the value 0xef, and the backtrace at
the time of the free is recorded. As with the backtrace on allocation,
only up to 16 frames will be recorded.

When the list of freed allocations reaches 100, the oldest allocation
on the list is removed and verified that it still contains the pattern 0xef.
If the entire allocation is not filled with this value, an error is printed
to the log.

### Log Leaks
When the program completes, all of the allocations that are still live
are printed to the log as leaks. This isn't very useful since it tends
to display a lot of false positive because many programs do not free
everything before terminating.

Option 20
---------
Do not use this option value, it only works on the emulator. It has not
been verified, so it may or may not work.

Enable on Certain Processes
---------------------------
Using the special system property, libc.debug.malloc.program, will
cause malloc debug to only be used on processes with that name. For example,
if the property is set to ls, then only the program named ls will have malloc
debug enabled.

Examples
========
Enable malloc debug for all allocations for all processes:

    adb shell stop
    adb shell setprop libc.debug.malloc 1
    adb shell start

Enable malloc debug for a particular process:

    adb shell setprop libc.debug.malloc.program ls
    adb shell setprop libc.debug.malloc 10
    adb shell ls /data/local/tmp
