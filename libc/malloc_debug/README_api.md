Native Memory Tracking using libc Callbacks
-------------------------------------------
Malloc debug can be used to get information on all of the live allocations
in a process. The libc library in Android exports two calls that can be
used to gather this data from a process. This tracking can be enabled using
either the backtrace option or the backtrace\_enabled\_on\_signal option.

The function to gather the data:

<pre>
<b>
extern "C" void get_malloc_leak_info(uint8_t** info, size_t* overall_size, size_t* info_size, size_t* total_memory, size_t* backtrace_size);
</b>
</pre>

<i>info</i> is set to a buffer allocated by the call that contains all of
the allocation information.
<i>overall\_size</i> is set to the total size of the buffer returned. If this
<i>info\_size</i>
value is zero, then there are no allocation being tracked.
<i>total\_memory</i> is set to the sum of all allocation sizes that are live at
the point of the function call. This does not include the memory allocated
by the malloc debug library itself.
<i>backtrace\_size</i> is set to the maximum number of backtrace entries
that are present for each allocation.

In order to free the buffer allocated by the function, call:

<pre>
<b>
extern "C" void free_malloc_leak_info(uint8_t* info);
</b>
</pre>

### Format of info Buffer
<pre>
size_t size_of_original_allocation
size_t num_backtrace_frames
uintptr_t pc1
uintptr_t pc2
uintptr_t pc3
.
.
.
</pre>

The number of <i>uintptr\_t</i> values is determined by the value
<i>backtrace\_size</i> as returned by the original call to
<i>get\_malloc\_leak\_info</i>. This value is not variable, it is the same
for all the returned data. The value
<i>num\_backtrace\_frames</i> contains the real number of frames found. The
extra frames are set to zero. Each <i>uintptr\_t</i> is a pc of the callstack.
The calls from within the malloc debug library are automatically removed.

For 32 bit systems, <i>size\_t</i> and <i>uintptr\_t</i> are both 4 byte values.

For 64 bit systems, <i>size\_t</i> and <i>uintptr\_t</i> are both 8 byte values.

The total number of these structures returned in <i>info</i> is
<i>overall\_size</i> divided by <i>info\_size</i>.

Note, the size value in each allocation data structure will have bit 31 set
if this allocation was created by the Zygote process. This helps to distinguish
between native allocations created by the application.
