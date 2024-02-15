Malloc Debug
============

Malloc debug is a method of debugging native memory problems. It can help
detect memory corruption, memory leaks, and use after free issues.

This documentation describes how to enable this feature on Android N or later
versions of the Android OS. (See the "Examples" section.)

The documentation for malloc debug on older versions of Android is
[here](README_marshmallow_and_earlier.md).

When malloc debug is enabled, it works by adding a shim layer that replaces
the normal allocation calls. The replaced calls are:

* `malloc`
* `free`
* `calloc`
* `realloc`
* `posix_memalign`
* `memalign`
* `aligned_alloc`
* `malloc_usable_size`

On 32 bit systems, these two deprecated functions are also replaced:

* `pvalloc`
* `valloc`

Any errors detected by the library are reported in the log.

NOTE: There is a small behavioral change beginning in P for realloc.
Before, a realloc from one size to a smaller size would not update the
backtrace related to the allocation. Starting in P, every single realloc
call changes the backtrace for the pointer no matter whether the pointer
returned has changed or not.


Controlling Malloc Debug Behavior
---------------------------------
Malloc debug is controlled by individual options. Each option can be enabled
individually, or in a group of other options. Every single option can be
combined with every other option.

Option Descriptions
-------------------
### front\_guard[=SIZE\_BYTES]
Enables a small buffer placed before the allocated data. This is an attempt
to find memory corruption occuring to a region before the original allocation.
On first allocation, this front guard is written with a specific pattern (0xaa).
When the allocation is freed, the guard is checked to verify it has not been
modified. If any part of the front guard is modified, an error will be reported
in the log indicating what bytes changed.

If the backtrace option is also enabled, then any error message will include
the backtrace of the allocation site.

If SIZE\_BYTES is present, it indicates the number of bytes in the guard.
The default is 32 bytes, the max bytes is 16384. SIZE\_BYTES will be
padded so that it is a multiple of 8 bytes on 32 bit systems and 16 bytes
on 64 bit systems to make sure that the allocation returned is aligned
properly.

This option adds a special header to all allocations that contains the guard
and information about the original allocation.

Example error:

    04-10 12:00:45.621  7412  7412 E malloc_debug: +++ ALLOCATION 0x12345678 SIZE 100 HAS A CORRUPTED FRONT GUARD
    04-10 12:00:45.622  7412  7412 E malloc_debug:   allocation[-32] = 0x00 (expected 0xaa)
    04-10 12:00:45.622  7412  7412 E malloc_debug:   allocation[-15] = 0x02 (expected 0xaa)

### rear\_guard[=SIZE\_BYTES]
Enables a small buffer placed after the allocated data. This is an attempt
to find memory corruption occuring to a region after the original allocation.
On first allocation, this rear guard is written with a specific pattern (0xbb).
When the allocation is freed, the guard is checked to verify it has not been
modified. If any part of the rear guard is modified, an error will be reported
in the log indicating what bytes changed.

If SIZE\_BYTES is present, it indicates the number of bytes in the guard.
The default is 32 bytes, the max bytes is 16384.

This option adds a special header to all allocations that contains
information about the original allocation.

Example error:

    04-10 12:00:45.621  7412  7412 E malloc_debug: +++ ALLOCATION 0x12345678 SIZE 100 HAS A CORRUPTED REAR GUARD
    04-10 12:00:45.622  7412  7412 E malloc_debug:   allocation[130] = 0xbf (expected 0xbb)
    04-10 12:00:45.622  7412  7412 E malloc_debug:   allocation[131] = 0x00 (expected 0xbb)

### guard[=SIZE\_BYTES]
Enables both a front guard and a rear guard on all allocations.

If SIZE\_BYTES is present, it indicates the number of bytes in both guards.
The default is 32 bytes, the max bytes is 16384.

### backtrace[=MAX\_FRAMES]
Enable capturing the backtrace of each allocation site.
This option will slow down allocations by an order of magnitude. If the
system runs too slowly with this option enabled, decreasing the maximum number
of frames captured will speed the allocations up.

Note that any backtrace frames that occur within the malloc backtrace library
itself are not recorded.

If MAX\_FRAMES is present, it indicates the maximum number of frames to
capture in a backtrace. The default is 16 frames, the maximumum value
this can be set to is 256.

Before P, this option adds a special header to all allocations that contains
the backtrace and information about the original allocation. After that, this
option will not add a special header.

As of P, this option will also enable dumping backtrace heap data to a
file when the process receives the signal SIGRTMAX - 17 ( which is 47 on
Android devices). The format of this dumped data is the same format as
that dumped when running am dumpheap -n. The default is to dump this data
to the file /data/local/tmp/backtrace\_heap.**PID**.txt. This is useful when
used with native only executables that run for a while since these processes
are not spawned from a zygote process.

Note that when the signal is received, the heap is not dumped until the next
malloc/free occurs.

### backtrace\_enable\_on\_signal[=MAX\_FRAMES]
Enable capturing the backtrace of each allocation site. If the
backtrace capture is toggled when the process receives the signal
SIGRTMAX - 19 (which is 45 on Android devices). When this
option is used alone, backtrace capture starts out disabled until the signal
is received. If both this option and the backtrace option are set, then
backtrace capture is enabled until the signal is received.

If MAX\_FRAMES is present, it indicates the maximum number of frames to
capture in a backtrace. The default is 16 frames, the maximumum value
this can be set to is 256.

Before P, this option adds a special header to all allocations that contains
the backtrace and information about the original allocation. After that, this
option will not add a special header.

### backtrace\_dump\_on\_exit
As of P, when the backtrace option has been enabled, this causes the backtrace
dump heap data to be dumped to a file when the program exits. If the backtrace
option has not been enabled, this does nothing. The default is to dump this
to the file named /data/local/tmp/backtrace\_heap.**PID**.exit.txt.

The file location can be changed by setting the backtrace\_dump\_prefix
option.

### backtrace\_dump\_prefix
As of P, when one of the backtrace options has been enabled, this sets the
prefix used for dumping files when the signal SIGRTMAX - 17 is received or when
the program exits and backtrace\_dump\_on\_exit is set.

The default is /data/local/tmp/backtrace\_heap.

When this value is changed from the default, then the filename chosen
on the signal will be backtrace\_dump\_prefix.**PID**.txt. The filename chosen
when the program exits will be backtrace\_dump\_prefix.**PID**.exit.txt.

### backtrace\_min\_size=ALLOCATION\_SIZE\_BYTES
As of U, setting this in combination with the backtrace option means
that only allocations of a size greater than or equal to
**ALLOCATION\_SIZE\_BYTES** will be backtraced. When used in combination
with the backtrace\_max\_size option, then allocations greater than or
equal to backtrace\_min\_size and less than or equal to
backtrace\_max\_size will be backtraced. The backtrace\_size option
overrides this option, and should not be used at the same time.

This option can also be used in combination with other tools such
as [libmemunreachable](https://android.googlesource.com/platform/system/memory/libmemunreachable/+/main/README.md)
to only get backtraces for sizes of allocations listed as being leaked.

### backtrace\_max\_size=ALLOCATION\_SIZE\_BYTES
As of U, setting this in combination with the backtrace option means
that only allocations of a size less than or equal to
**ALLOCATION\_SIZE\_BYTES** will be backtraced. When used in combination
with the backtrace\_min\_size option, then allocations greater than or
equal to backtrace\_min\_size and less than or equal to
backtrace\_max\_size will be backtraced. The backtrace\_size option
overrides this option, and should not be used at the same time.

This option can also be used in combination with other tools such
as [libmemunreachable](https://android.googlesource.com/platform/system/memory/libmemunreachable/+/main/README.md)
to only get backtraces for sizes of allocations listed as being leaked.

### backtrace\_size=ALLOCATION\_SIZE\_BYTES
As of U, setting this in combination with the backtrace option means
that only allocations of size **ALLOCATION\_SIZE\_BYTES** will be backtraced.
This option overrides the backtrace\_min\_size and the backtrace\_max\_size.

This option can also be used in combination with other tools such
as [libmemunreachable](https://android.googlesource.com/platform/system/memory/libmemunreachable/+/main/README.md)
to only get backtraces for sizes of allocations listed as being leaked.

### backtrace\_full
As of Q, any time that a backtrace is gathered, a different algorithm is used
that is extra thorough and can unwind through Java frames. This will run
slower than the normal backtracing function.

### bt, bt\_dmp\_on\_ex, bt\_dmp\_pre, bt\_en\_on\_sig, bt\_full, bt\_max\_sz, bt\_min\_sz, bt\_sz
As of U, add shorter aliases for backtrace related options to avoid property length restrictions.

| Alias           | Option                        |
|:----------------|:------------------------------|
| bt              | backtrace                     |
| bt\_dmp\_on\_ex | backtrace\_dump\_on\_exit     |
| bt\_dmp\_pre    | backtrace\_dump\_prefix       |
| bt\_en\_on\_sig | backtrace\_enable\_on\_signal |
| bt\_full        | backtrace\_full               |
| bt\_max\_sz     | backtrace\_max\_size          |
| bt\_min\_sz     | backtrace\_min\_size          |
| bt\_sz          | backtrace\_size               |

### check\_unreachable\_on\_signal
As of Android U, this option will trigger a check for unreachable memory
in a process. Specifically, if the signal SIGRTMAX - 16 (which is 48 on
Android devices). The best way to see the exact signal being used is to
enable the verbose option then look at the log for the message:

    Run: 'kill -48 <PID>' to check for unreachable memory.

When the signal is received, the actual unreachable check only triggers
on the next allocation that happens in the process (malloc/free, etc).

If a process is not doing any allocations, it can be forced to trigger when
running:

    debuggerd -b <PID>

**NOTE**: The unreachable check can fail for protected processes, so it
might be necessary to run:

    setenforce 0

To get the unreachable data.

### fill\_on\_alloc[=MAX\_FILLED\_BYTES]
Any allocation routine, other than calloc, will result in the allocation being
filled with the value 0xeb. When doing a realloc to a larger size, the bytes
above the original usable size will be set to 0xeb.

If MAX\_FILLED\_BYTES is present, it will only fill up to the specified number
of bytes in the allocation. The default is to fill the entire allocation.

### fill\_on\_free[=MAX\_FILLED\_BYTES]
When an allocation is freed, fill it with 0xef.

If MAX\_FILLED\_BYTES is present, it will only fill up to the specified number
of bytes in the allocation. The default is to fill the entire allocation.

### fill[=MAX\_FILLED\_BYTES]
This enables both the fill\_on\_alloc option and the fill\_on\_free option.

If MAX\_FILLED\_BYTES is present, it will only fill up to the specified number
of bytes in the allocation. The default is to fill the entire allocation.

### expand\_alloc[=EXPAND\_BYTES]
Add an extra amount to allocate for every allocation.

If XX is present, it is the number of bytes to expand the allocation by.
The default is 16 bytes, the max bytes is 16384.

### free\_track[=ALLOCATION\_COUNT]
When a pointer is freed, do not free the memory right away, but add it to
a list of freed allocations. In addition to being added to the list, the
entire allocation is filled with the value 0xef, and the backtrace at
the time of the free is recorded. The backtrace recording is completely
separate from the backtrace option, and happens automatically if this
option is enabled. By default, a maximum of 16 frames will be recorded,
but this value can be changed using the free\_track\_backtrace\_num\_frames
option. It can also be completely disabled by setting the option to zero.
See the full description of this option below.

When the list is full, an allocation is removed from the list and is
checked to make sure that none of the contents have been modified since
being placed on the list. When the program terminates, all of the allocations
left on the list are verified.

If ALLOCATION\_COUNT is present, it indicates the total number of allocations
in the list. The default is to record 100 freed allocations, the max
allocations to record is 16384.

Before P, this option adds a special header to all allocations that contains
the backtrace and information about the original allocation. After that, this
option will not add a special header.

Example error:

    04-15 12:00:31.304  7412  7412 E malloc_debug: +++ ALLOCATION 0x12345678 USED AFTER FREE
    04-15 12:00:31.305  7412  7412 E malloc_debug:   allocation[20] = 0xaf (expected 0xef)
    04-15 12:00:31.305  7412  7412 E malloc_debug:   allocation[99] = 0x12 (expected 0xef)
    04-15 12:00:31.305  7412  7412 E malloc_debug: Backtrace at time of free:
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #00  pc 00029310  /system/lib/libc.so
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #01  pc 00021438  /system/lib/libc.so (newlocale+160)
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #02  pc 000a9e38  /system/lib/libc++.so
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #03  pc 000a28a8  /system/lib/libc++.so

In addition, there is another type of error message that can occur if
an allocation has a special header applied, and the header is corrupted
before the verification occurs. This is the error message that will be found
in the log:

    04-15 12:00:31.604  7412  7412 E malloc_debug: +++ ALLOCATION 0x12345678 HAS CORRUPTED HEADER TAG 0x1cc7dc00 AFTER FREE

### free\_track\_backtrace\_num\_frames[=MAX\_FRAMES]
This option only has meaning if free\_track is set. It indicates how many
backtrace frames to capture when an allocation is freed.

If MAX\_FRAMES is present, it indicates the number of frames to capture.
If the value is set to zero, then no backtrace will be captured when the
allocation is freed. The default is to record 16 frames, the max number of
frames to to record is 256.

### leak\_track
Track all live allocations. When the program terminates, all of the live
allocations will be dumped to the log. If the backtrace option was enabled,
then the log will include the backtrace of the leaked allocations. This
option is not useful when enabled globally because a lot of programs do not
free everything before the program terminates.

Before P, this option adds a special header to all allocations that contains
the backtrace and information about the original allocation. After that, this
option will not add a special header.

Example leak error found in the log:

    04-15 12:35:33.304  7412  7412 E malloc_debug: +++ APP leaked block of size 100 at 0x2be3b0b0 (leak 1 of 2)
    04-15 12:35:33.304  7412  7412 E malloc_debug: Backtrace at time of allocation:
    04-15 12:35:33.305  7412  7412 E malloc_debug:           #00  pc 00029310  /system/lib/libc.so
    04-15 12:35:33.305  7412  7412 E malloc_debug:           #01  pc 00021438  /system/lib/libc.so (newlocale+160)
    04-15 12:35:33.305  7412  7412 E malloc_debug:           #02  pc 000a9e38  /system/lib/libc++.so
    04-15 12:35:33.305  7412  7412 E malloc_debug:           #03  pc 000a28a8  /system/lib/libc++.so
    04-15 12:35:33.305  7412  7412 E malloc_debug: +++ APP leaked block of size 24 at 0x7be32380 (leak 2 of 2)
    04-15 12:35:33.305  7412  7412 E malloc_debug: Backtrace at time of allocation:
    04-15 12:35:33.305  7412  7412 E malloc_debug:           #00  pc 00029310  /system/lib/libc.so
    04-15 12:35:33.305  7412  7412 E malloc_debug:           #01  pc 00021438  /system/lib/libc.so (newlocale+160)
    04-15 12:35:33.305  7412  7412 E malloc_debug:           #02  pc 000a9e38  /system/lib/libc++.so
    04-15 12:35:33.305  7412  7412 E malloc_debug:           #03  pc 000a28a8  /system/lib/libc++.so

### log\_allocator\_stats\_on\_signal
As of Android V, this option will trigger a call to:

    mallopt(M_LOG_STATS, 0);

When a process receives the signal SIGRTMAX - 15 (which is 49 on Android
devices). The mallopt call is not async safe and is not called from the
signal handler directly. Instead, the next time any allocation call occurs,
the mallopt is called.

### record\_allocs[=TOTAL\_ENTRIES]
Keep track of every allocation/free made on every thread and dump them
to a file when the signal SIGRTMAX - 18 (which is 46 on Android devices)
is received.

If TOTAL\_ENTRIES is set, then it indicates the total number of
allocation/free records that can be retained. If the number of records
reaches the TOTAL\_ENTRIES value, then any further allocations/frees are
not recorded. The default value is 8,000,000 and the maximum value this
can be set to is 50,000,000.

Once the signal is received, and the current records are written to the
file, all current records are deleted. Any allocations/frees occuring while
the data is being dumped to the file are ignored.

**NOTE**: This option is not available until the O release of Android.

The allocation data is written in a human readable format. Every line begins
with the THREAD\_ID returned by gettid(), which is the thread that is making
the allocation/free. If a new thread is created, no special line is added
to the file. However, when a thread completes, a special entry is added to
the file indicating this.

The thread complete line is:

**THREAD\_ID**: thread\_done 0x0

Example:

    187: thread_done 0x0

Below is how each type of allocation/free call ends up in the file dump.

pointer = malloc(size)

**THREAD\_ID**: malloc pointer size

Example:

    186: malloc 0xb6038060 20

free(pointer)

**THREAD\_ID**: free pointer

Example:

    186: free 0xb6038060

pointer = calloc(nmemb, size)

**THREAD\_ID**: calloc pointer nmemb size

Example:

    186: calloc 0xb609f080 32 4

new\_pointer = realloc(old\_pointer, size)

**THREAD\_ID**: realloc new\_pointer old\_pointer size

Example:

    186: realloc 0xb609f080 0xb603e9a0 12

pointer = memalign(alignment, size)

**THREAD\_ID**: memalign pointer alignment size

pointer = aligned\_alloc(alignment, size)

**THREAD\_ID**: memalign pointer alignment size

posix\_memalign(&pointer, alignment, size)

**THREAD\_ID**: memalign pointer alignment size

Example:

    186: memalign 0x85423660 16 104

pointer = valloc(size)

**THREAD\_ID**: memalign pointer 4096 size

Example:

    186: memalign 0x85423660 4096 112

pointer = pvalloc(size)

**THREAD\_ID**: memalign pointer 4096 <b>SIZE\_ROUNDED\_UP\_TO\_4096</b>

Example:

    186: memalign 0x85423660 4096 8192

### record\_allocs\_file[=FILE\_NAME]
This option only has meaning if record\_allocs is set. It indicates the
file where the recorded allocations will be found.

If FILE\_NAME is set, then it indicates where the record allocation data
will be placed.

**NOTE**: This option is not available until the O release of Android.

### verify\_pointers
Track all live allocations to determine if a pointer is used that does not
exist. This option is a lightweight way to verify that all
free/malloc\_usable\_size/realloc calls are passed valid pointers.

Example error:

    04-15 12:00:31.304  7412  7412 E malloc_debug: +++ ALLOCATION 0x12345678 UNKNOWN POINTER (free)
    04-15 12:00:31.305  7412  7412 E malloc_debug: Backtrace at time of failure:
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #00  pc 00029310  /system/lib/libc.so
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #01  pc 00021438  /system/lib/libc.so (newlocale+160)
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #02  pc 000a9e38  /system/lib/libc++.so
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #03  pc 000a28a8  /system/lib/libc++.so

Where the name of the function varies depending on the function that called
with a bad pointer. Only three functions do this checking: free,
malloc\_usable\_size, realloc.

**NOTE**: This option is not available until the P release of Android.

### abort\_on\_error
When malloc debug detects an error, abort after sending the error
log message.

**NOTE**: If leak\_track is enabled, no abort occurs if leaks have been
detected when the process is exiting.

### verbose
As of Android Q, all info messages will be turned off by default. For example,
in Android P and older, enabling malloc debug would result in this message
in the log:

    08-16 15:54:16.060 26947 26947 I libc    : /system/bin/app_process64: malloc debug enabled

In android Q, this message will not be displayed because these info messages
slow down process start up. However, if you want to re-enable these messages,
add the verbose option. All of the "Run XXX" messages are also silenced unless
the verbose option is specified. This is an example of the type
of messages that are no longer displayed:

    09-10 01:03:50.070   557   557 I malloc_debug: /system/bin/audioserver: Run: 'kill -47 557' to dump the backtrace.

Additional Errors
-----------------
There are a few other error messages that might appear in the log.

### Use After Free
    04-15 12:00:31.304  7412  7412 E malloc_debug: +++ ALLOCATION 0x12345678 USED AFTER FREE (free)
    04-15 12:00:31.305  7412  7412 E malloc_debug: Backtrace of original free:
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #00  pc 00029310  /system/lib/libc.so
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #01  pc 00021438  /system/lib/libc.so (newlocale+160)
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #02  pc 000a9e38  /system/lib/libc++.so
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #03  pc 000a28a8  /system/lib/libc++.so
    04-15 12:00:31.305  7412  7412 E malloc_debug: Backtrace at time of failure:
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #00  pc 00029310  /system/lib/libc.so
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #01  pc 00021438  /system/lib/libc.so (newlocale+160)
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #02  pc 000a9e38  /system/lib/libc++.so
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #03  pc 000a28a8  /system/lib/libc++.so

This indicates that code is attempting to free an already freed pointer. The
name in parenthesis indicates that the application called the function
*free* with the bad pointer.

For example, this message:

    04-15 12:00:31.304  7412  7412 E malloc_debug: +++ ALLOCATION 0x12345678 USED AFTER FREE (realloc)

Would indicate that the application called the *realloc* function
with an already freed pointer.

### Invalid Tag
    04-15 12:00:31.304  7412  7412 E malloc_debug: +++ ALLOCATION 0x12345678 HAS INVALID TAG 1ee7d000 (malloc_usable_size)
    04-15 12:00:31.305  7412  7412 E malloc_debug: Backtrace at time of failure:
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #00  pc 00029310  /system/lib/libc.so
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #01  pc 00021438  /system/lib/libc.so (newlocale+160)
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #02  pc 000a9e38  /system/lib/libc++.so
    04-15 12:00:31.305  7412  7412 E malloc_debug:           #03  pc 000a28a8  /system/lib/libc++.so

This indicates that a function (malloc\_usable\_size) was called with
a pointer that is either not allocated memory, or that the memory of
the pointer has been corrupted.

As with the other error message, the function in parenthesis is the
function that was called with the bad pointer.

Backtrace Heap Dump Format
==========================

This section describes the format of the backtrace heap dump. This data is
generated by am dumpheap -n or, as of P, by the signal or on exit.

The data has this header:

    Android Native Heap Dump v1.0

    Total memory: XXXX
    Allocation records: YYYY
    Backtrace size: ZZZZ

Total memory is the total of all of the currently live allocations.
Allocation records is the total number of allocation records.
Backtrace size is the maximum number of backtrace frames that can be present.

Following this header are two different sections, the first section is the
allocation records, the second section is the map data.

The allocation record data has this format:

    z ZYGOTE_CHILD_ALLOC  sz    ALLOCATION_SIZE  num  NUM_ALLOCATIONS bt FRAMES

ZYGOTE\_CHILD\_ALLOC is either 0 or 1. 0 means this was allocated by the
zygote process or in a process not spawned from the zygote. 1 means this
was allocated by an application after it forked off from the zygote process.

ALLOCATION\_SIZE is the size of the allocation.
NUM\_ALLOCATIONS is the number of allocations that have this size and have the
same backtrace.
FRAMES is a list of instruction pointers that represent the backtrace of the
allocation.

Example:

    z 0  sz      400  num    1  bt 0000a230 0000b500
    z 1  sz      500  num    3  bt 0000b000 0000c000

The first allocation record was created by the zygote of size 400 only one
with this backtrace/size and a backtrace of 0xa230, 0xb500.
The second allocation record was create by an application spawned from the
zygote of size 500, where there are three of these allocation with the same
backtrace/size and a backtrace of 0xb000, 0xc000.

The final section is the map data for the process:

    MAPS
    7fe9181000-7fe91a2000 rw-p 00000000 00:00 0                              /system/lib/libc.so
    .
    .
    .
    END

The map data is simply the output of /proc/PID/maps. This data can be used to
decode the frames in the backtraces.

There are now multiple versions of the file:

Android P produces version v1.1 of the heap dump.

    Android Native Heap Dump v1.1

The only difference between v1.0 and v1.1 is that the NUM\_ALLOCATIONS
value is always accurate in v1.1. A previous version of malloc debug set
NUM\_ALLOCATIONS to an incorrect value. For heap dump v1.0, the
NUM\_ALLOCATIONS value should be treated as always 1 no matter what is
actually present.

Android Q introduces v1.2 of the heap dump. The new header looks like this:

    Android Native Heap Dump v1.2

    Build fingerprint: 'google/taimen/taimen:8.1.0/OPM2.171026.006.C1/4769658:user/release-keys'

The new line fingerprint line is the contents of the ro.build.fingerprint
property.

The new version no longer 0 pads the backtrace addresses. In v1.0/v1.1:

    z 0  sz      400  num    1  bt 0000a230 0000b500

While v1.2:

    z 0  sz      400  num    1  bt a230 b500

In addition, when the new option backtrace\_full is used, another line will
be added to every backtrace line. The line will be:

      bt_info {"MAP_NAME" RELATIVE_TO_MAP_PC "FUNCTION_NAME" FUNCTION_OFFSET} ...

For each backtrace pc, there will be one element in braces.

MAP\_NAME is the name of the map in which the backtrace pc exists. If there is
no valid map name, this will be empty.
RELATIVE\_TO\_MAP\_PC is the hexadecimal value of the relative pc to the map.
FUNCTION\_NAME the name of the function for this pc. If there is no valid
function name, then it will be empty.
FUNCTION\_OFFSET the hexadecimal offset from the beginning of the function. If
the FUNCTION\_NAME is empty, then this value will always be zero.

An example of this new format:

    z 0  sz      400  num    1  bt a2a0 b510
      bt_info {"/system/libc.so" 2a0 "abort" 24} {"/system/libutils.so" 510 "" 0}

In this example, the first backtrace frame has a pc of 0xa2a0 and is in the
map named /system/libc.so which starts at 0xa000. The relative pc is 0x2a0,
and it is in the function abort + 0x24.
The second backtrace frame has a pc of 0xb510 and is in the map named
/system/libutils.so which starts at 0xb000. The relative pc is 0x510 and
it is in an unknown function.

Examples
========

### For platform developers

Enable backtrace tracking of all allocation for all processes:

    adb shell stop
    adb shell setprop libc.debug.malloc.options backtrace
    adb shell start

Enable backtrace tracking for a specific process (ls):

    adb shell setprop libc.debug.malloc.options backtrace
    adb shell setprop libc.debug.malloc.program ls
    adb shell ls

Enable backtrace tracking for the zygote and zygote based processes:

    adb shell stop
    adb shell setprop libc.debug.malloc.program app_process
    adb shell setprop libc.debug.malloc.options backtrace
    adb shell start

Enable multiple options (backtrace and guard):

    adb shell stop
    adb shell setprop libc.debug.malloc.options "\"backtrace guard\""
    adb shell start

Note: The two levels of quoting in the adb shell command is necessary.
The outer layer of quoting is for the shell on the host, to ensure that the
inner layer of quoting is sent to the device, to make 'backtrace guard'
a single argument.

Enable malloc debug using an environment variable (pre-O Android release):

    adb shell
    # setprop libc.debug.malloc.env_enabled 1
    # setprop libc.debug.malloc.options backtrace
    # export LIBC_DEBUG_MALLOC_ENABLE=1
    # ls

Enable malloc debug using an environment variable (Android O or later):

    adb shell
    # export LIBC_DEBUG_MALLOC_OPTIONS=backtrace
    # ls

Any process spawned from this shell will run with malloc debug enabled
using the backtrace option.

    adb shell stop
    adb shell setprop libc.debug.malloc.options backtrace
    adb shell start
    adb shell am dumpheap -n <PID_TO_DUMP> /data/local/tmp/heap.txt

It is possible to use the backtrace\_enable\_on\_signal option as well,
but, obviously, it must be enabled through the signal before the file will
contain any data.

### For app developers

App developers should check the NDK documentation about
[wrap.sh](https://developer.android.com/ndk/guides/wrap-script.html)
for the best way to use malloc debug in Android O or later on non-rooted
devices.

**NOTE**: Android 12 introduced a bug that can cause the wrap.\<APP\> property to
no longer work. Use the commands below so that the wrap.\<APP\> instructions will work:

    adb shell setprop dalvik.vm.force-java-zygote-fork-loop true
    adb shell stop
    adb shell start

If you do have a rooted device, you can enable malloc debug for a specific
program/application (Android O or later):

    adb shell setprop wrap.<APP> '"LIBC_DEBUG_MALLOC_OPTIONS=backtrace logwrapper"'

If you need to enable multiple options using this method, then you can set
them like so:

    adb shell setprop wrap.<APP> '"LIBC_DEBUG_MALLOC_OPTIONS=backtrace\ leak_track\ fill logwrapper"'

For example, to enable malloc debug for the google search box (Android O or later):

    adb shell setprop wrap.com.google.android.googlequicksearchbox '"LIBC_DEBUG_MALLOC_OPTIONS=backtrace logwrapper"'
    adb shell am force-stop com.google.android.googlequicksearchbox

If you are setting multiple options and the app does not appear to start
properly, check the logcat looking for this message
(`adb logcat -d | grep "malloc debug"`):

    08-16 15:54:16.060 26947 26947 I libc    : /system/bin/app_process64: malloc debug enabled

If you do not see this message, then the wrap property was not set correctly.
Run:

    adb shell getprop | grep wrap

And verify that any spaces are properly escaped.

NOTE: On pre-O versions of the Android OS, property names had a length limit
of 32. This meant that to create a wrap property with the name of the app, it
was necessary to truncate the name to fit. On O, property names can be
an order of magnitude larger, so there should be no need to truncate the name
at all.

To detect leaks while an app is running:

    adb shell dumpsys meminfo --unreachable <PID_OF_APP>

Without also enabling malloc debug, this command will only tell
you whether it can detect leaked memory, not where those leaks are
occurring. If you enable malloc debug with the backtrace option for your
app before running the dumpsys command, you'll get backtraces showing
where the memory was allocated.

For backtraces from your app to be useful, you'll want to keep the
symbols in your app's shared libraries rather than stripping them. That
way you'll see the location of the leak directly without having to use
something like the <code>ndk-stack</code> tool.
