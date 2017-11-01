Native Memory Tracking using libc Callbacks
-------------------------------------------
Malloc debug can be used to get information on all of the live allocations
in a process. The libc library in Android exports two calls that can be
used to gather this data from a process. This tracking can be enabled using
either the backtrace option or the backtrace\_enabled\_on\_signal option.

The function to gather the data:

`extern "C" void get_malloc_leak_info(uint8_t** info, size_t* overall_size, size_t* info_size, size_t* total_memory, size_t* backtrace_size);`

*info* is set to a buffer allocated by the call that contains all of
the allocation information.
*overall\_size* is set to the total size of the buffer returned. If this
*info\_size*
value is zero, then there are no allocation being tracked.
*total\_memory* is set to the sum of all allocation sizes that are live at
the point of the function call. This does not include the memory allocated
by the malloc debug library itself.
*backtrace\_size* is set to the maximum number of backtrace entries
that are present for each allocation.

In order to free the buffer allocated by the function, call:

`extern "C" void free_malloc_leak_info(uint8_t* info);`

### Format of info Buffer
    size_t size_of_original_allocation
    size_t num_allocations
    uintptr_t pc1
    uintptr_t pc2
    uintptr_t pc3
    .
    .
    .

The number of *uintptr\_t* values is determined by the value
*backtrace\_size* as returned by the original call to
*get\_malloc\_leak\_info*. This value is not variable, it is the same
for all the returned data. The value
*num\_allocations* contains the total number of allocations with the same
backtrace and size as this allocation. On Android Nougat, this value was
incorrectly set to the number of frames in the backtrace.
Each *uintptr\_t* is a pc of the callstack. If the total number
of backtrace entries is less than *backtrace\_size*, the rest of the
entries are zero.
The calls from within the malloc debug library are automatically removed.

For 32 bit systems, *size\_t* and *uintptr\_t* are both 4 byte values.

For 64 bit systems, *size\_t* and *uintptr\_t* are both 8 byte values.

The total number of these structures returned in *info* is
*overall\_size* divided by *info\_size*.

Note, the size value in each allocation data structure will have bit 31 set
if this allocation was created in a process forked from the Zygote process.
This helps to distinguish between native allocations created by the application.
