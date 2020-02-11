# Native Memory Allocator Verification
This document describes how to verify the native memory allocator on Android.
This procedure should be followed when upgrading or moving to a new allocator.
A small minor upgrade might not need to run all of the benchmarks, however,
at least the
[SQL Allocation Trace Benchmark](#sql-allocation-trace-benchmark),
[Memory Replay Benchmarks](#memory-replay-benchmarks) and
[Performance Trace Benchmarks](#performance-trace-benchmarks) should be run.

It is important to note that there are two modes for a native allocator
to run in on Android. The first is the normal allocator, the second is
called the svelte config, which is designed to run on memory constrained
systems and be a bit slower, but take less RSS. To enable the svelte config,
add this line to the `BoardConfig.mk` for the given target:

    MALLOC_SVELTE := true

The `BoardConfig.mk` file is usually found in the directory
`device/<DEVICE_NAME>/` or in a sub directory.

When evaluating a native allocator, make sure that you benchmark both
versions.

## Android Extensions
Android supports a few non-standard functions and mallopt controls that
a native allocator needs to implement.

### Iterator Functions
These are functions that are used to implement a memory leak detector
called `libmemunreachable`.

#### malloc\_disable
This function, when called, should pause all threads that are making a
call to an allocation function (malloc/free/etc). When a call
is made to `malloc_enable`, the paused threads should start running again.

#### malloc\_enable
This function, when called, does nothing unless there was a previous call
to `malloc_disable`. This call will unpause any thread which is making
a call to an allocation function (malloc/free/etc) when `malloc_disable`
was called previously.

#### malloc\_iterate
This function enumerates all of the allocations currently live in the
system. It is meant to be called after a call to `malloc_disable` to
prevent further allocations while this call is being executed. To
see what is expected for this function, the best description is the
tests for this funcion in `bionic/tests/malloc_itearte_test.cpp`.

### Mallopt Extensions
These are mallopt options that Android requires for a native allocator
to work efficiently.

#### M\_DECAY\_TIME
When set to zero, `mallopt(M_DECAY_TIME, 0)`, it is expected that an
allocator will attempt to purge and release any unused memory back to the
kernel on free calls. This is important in Android to avoid consuming extra
RSS.

When set to non-zero, `mallopt(M_DECAY_TIME, 1)`, an allocator can delay the
purge and release action. The amount of delay is up to the allocator
implementation, but it should be a reasonable amount of time. The jemalloc
allocator was implemented to have a one second delay.

The drawback to this option is that most allocators do not have a separate
thread to handle the purge, so the decay is only handled when an
allocation operation occurs. For server processes, this can mean that
RSS is slightly higher when the server is waiting for the next connection
and no other allocation calls are made. The `M_PURGE` option is used to
force a purge in this case.

For all applications on Android, the call `mallopt(M_DECAY_TIME, 1)` is
made by default. The idea is that it allows application frees to run a
bit faster, while only increasing RSS a bit.

#### M\_PURGE
When called, `mallopt(M_PURGE, 0)`, an allocator should purge and release
any unused memory immediately. The argument for this call is ignored. If
possible, this call should clear thread cached memory if it exists. The
idea is that this can be called to purge memory that has not been
purged when `M_DECAY_TIME` is set to one. This is useful if you have a
server application that does a lot of native allocations and the
application wants to purge that memory before waiting for the next connection.

## Correctness Tests
These are the tests that should be run to verify an allocator is
working properly according to Android.

### Bionic Unit Tests
The bionic unit tests contain a small number of allocator tests. These
tests are primarily verifying Android extensions and non-standard behavior
of allocation routines such as what happens when a non-power of two alignment
is passed to memalign.

To run all of the compliance tests:

    adb shell /data/nativetest64/bionic-unit-tests/bionic-unit-tests --gtest_filter="malloc*"
    adb shell /data/nativetest/bionic-unit-tests/bionic-unit-tests --gtest_filter="malloc*"

The allocation tests are not meant to be complete, so it is expected
that a native allocator will have its own set of tests that can be run.

### Libmemunreachable Tests
The libmemunreachable tests verify that the iterator functions are working
properly.

To run all of the tests:

    adb shell /data/nativetest64/memunreachable_binder_test/memunreachable_binder_test
    adb shell /data/nativetest/memunreachable_binder_test/memunreachable_binder_test
    adb shell /data/nativetest64/memunreachable_test/memunreachable_test
    adb shell /data/nativetest/memunreachable_test/memunreachable_test
    adb shell /data/nativetest64/memunreachable_unit_test/memunreachable_unit_test
    adb shell /data/nativetest/memunreachable_unit_test/memunreachable_unit_test

### CTS Entropy Test
In addition to the bionic tests, there is also a CTS test that is designed
to verify that the addresses returned by malloc are sufficiently randomized
to help defeat potential security bugs.

Run this test thusly:

    atest AslrMallocTest

If there are multiple devices connected to the system, use `-s <SERIAL>`
to specify a device.

## Performance
There are multiple different ways to evaluate the performance of a native
allocator on Android. One is allocation speed in various different scenarios,
another is total RSS taken by the allocator.

The last is virtual address space consumed in 32 bit applications. There is
a limited amount of address space available in 32 bit apps, and there have
been allocator bugs that cause memory failures when too much virtual
address space is consumed. For 64 bit executables, this can be ignored.

### Bionic Benchmarks
These are the microbenchmarks that are part of the bionic benchmarks suite of
benchmarks. These benchmarks can be built using this command:

    mmma -j bionic/benchmarks

These benchmarks are only used to verify the speed of the allocator and
ignore anything related to RSS and virtual address space consumed.

For all of these benchmark runs, it can be useful to add these two options:

    --benchmark_repetitions=XX
    --benchmark_report_aggregates_only=true

This will run the benchmark XX times and then give a mean, median, and stddev
and helps to get a number that can be compared to the new allocator.

In addition, there is another option:

    --bionic_cpu=XX

Which will lock the benchmark to only run on core XX. This also avoids
any issue related to the code migrating from one core to another
with different characteristics. For example, on a big-little cpu, if the
benchmark moves from big to little or vice-versa, this can cause scores
to fluctuate in indeterminate ways.

For most runs, the best set of options to add is:

    --benchmark_repetitions=10 --benchmark_report_aggregates_only=true --bionic_cpu=3

On most phones with a big-little cpu, the third core is the little core.
Choosing to run on the little core can tend to highlight any performance
differences.

#### Allocate/Free Benchmarks
These are the benchmarks to verify the allocation speed of a loop doing a
single allocation, touching every page in the allocation to make it resident
and then freeing the allocation.

To run the benchmarks with `mallopt(M_DECAY_TIME, 0)`, use these commands:

    adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=stdlib_malloc_free_default
    adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --benchmark_filter=malloc_free_default

To run the benchmarks with `mallopt(M_DECAY_TIME, 1)`, use these commands:

    adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=stdlib_malloc_free_decay1
    adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --benchmark_filter=malloc_free_decay1

The last value in the output is the size of the allocation in bytes. It is
useful to look at these kinds of benchmarks to make sure that there are
no outliers, but these numbers should not be used to make a final decision.
If these numbers are slightly worse than the current allocator, the
single thread numbers from trace data is a better representative of
real world situations.

#### Multiple Allocations Retained Benchmarks
These are the benchmarks that examine how the allocator handles multiple
allocations of the same size at the same time.

The first set of these benchmarks does a set number of 8192 byte allocations
in one loop, and then frees all of the allocations at the end of the loop.
Only the time it takes to do the allocations is recorded, the frees are not
counted. The value of 8192 was chosen since the jemalloc native allocator
had issues with this size. It is possible other sizes might show different
results, but, as mentioned before, these microbenchmark numbers should
not be used as absolutes for determining if an allocator is worth using.

This benchmark is designed to verify that there is no performance issue
related to having multiple allocations alive at the same time.

To run the benchmarks with `mallopt(M_DECAY_TIME, 0)`, use these commands:

    adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=stdlib_malloc_multiple_8192_allocs_default
    adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --benchmark_filter=stdlib_malloc_multiple_8192_allocs_default

To run the benchmarks with `mallopt(M_DECAY_TIME, 1)`, use these commands:

    adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=stdlib_malloc_multiple_8192_allocs_decay1
    adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --benchmark_filter=stdlib_malloc_multiple_8192_allocs_decay1

For these benchmarks, the last parameter is the total number of allocations to
do in each loop.

The other variation of this benchmark is to always do forty allocations in
each loop, but vary the size of the forty allocations. As with the other
benchmark, only the time it takes to do the allocations is tracked, the
frees are not counted. Forty allocations is an arbitrary number that could
be modified in the future. It was chosen because a version of the native
allocator, jemalloc, showed a problem at forty allocations.

To run the benchmarks with `mallopt(M_DECAY_TIME, 0)`, use these commands:

    adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=stdlib_malloc_forty_default
    adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --benchmark_filter=stdlib_malloc_forty_default

To run the benchmarks with `mallopt(M_DECAY_TIME, 1)`, use these command:

    adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=stdlib_malloc_forty_decay1
    adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --benchmark_filter=stdlib_malloc_forty_decay1

For these benchmarks, the last parameter in the output is the size of the
allocation in bytes.

As with the other microbenchmarks, an allocator with numbers in the same
proximity of the current values is usually sufficient to consider making
a switch. The trace benchmarks are more important than these benchmarks
since they simulate real world allocation profiles.

#### SQL Allocation Trace Benchmark
This benchmark is a trace of the allocations performed when running
the SQLite BenchMark app.

This benchmark is designed to verify that the allocator will be performant
in a real world allocation scenario. SQL operations were chosen as a
benchmark because these operations tend to do lots of malloc/realloc/free
calls, and they tend to be on the critical path of applications.

To run the benchmarks with `mallopt(M_DECAY_TIME, 0)`, use these commands:

    adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=malloc_sql_trace_default
    adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --benchmark_filter=malloc_sql_trace_default

To run the benchmarks with `mallopt(M_DECAY_TIME, 1)`, use these commands:

    adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=malloc_sql_trace_decay1
    adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --benchmark_filter=malloc_sql_trace_decay1

These numbers should be as performant as the current allocator.

#### mallinfo Benchmark
This benchmark only verifies that mallinfo is still close to the performance
of the current allocator.

To run the benchmark, use these commands:

    adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=BM_mallinfo
    adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --benchmark_filter=BM_mallinfo

Calls to mallinfo are used in ART so a new allocator is required to be
nearly as performant as the current allocator.

#### mallopt M\_PURGE Benchmark
This benchmark tracks the cost of calling `mallopt(M_PURGE, 0)`. As with the
mallinfo benchmark, it's not necessary for this to be better than the previous
allocator, only that the performance be in the same order of magnitude.

To run the benchmark, use these commands:

    adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=BM_mallopt_purge
    adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --benchmark_filter=BM_mallopt_purge

These calls are used to free unused memory pages back to the kernel.

### Memory Trace Benchmarks
These benchmarks measure all three axes of a native allocator, RSS, virtual
address space consumed, speed of allocation. They are designed to
run on a trace of the allocations from a real world application or system
process.

To build this benchmark:

    mmma -j system/extras/memory_replay

This will build two executables:

    /system/bin/memory_replay32
    /system/bin/memory_replay64

And these two benchmark executables:

    /data/benchmarktest64/trace_benchmark/trace_benchmark
    /data/benchmarktest/trace_benchmark/trace_benchmark

#### Memory Replay Benchmarks
These benchmarks display RSS, virtual memory consumed (VA space), and do a
bit of performance testing on actual traces taken from running applications.

The trace data includes what thread does each operation, so the replay
mechanism will simulate this by creating threads and replaying the operations
on a thread as if it was rerunning the real trace. The only issue is that
this is a worst case scenario for allocations happening at the same time
in all threads since it collapses all of the allocation operations to occur
one after another. This will cause a lot of threads allocating at the same
time. The trace data does not include timestamps,
so it is not possible to create a completely accurate replay.

To generate these traces, see the [Malloc Debug documentation](https://android.googlesource.com/platform/bionic/+/master/libc/malloc_debug/README.md),
the option [record\_allocs](https://android.googlesource.com/platform/bionic/+/master/libc/malloc_debug/README.md#record_allocs_total_entries).

To run these benchmarks, first copy the trace files to the target using
these commands:

    adb shell push system/extras/traces /data/local/tmp

Since all of the traces come from applications, the `memory_replay` program
will always call `mallopt(M_DECAY_TIME, 1)' before running the trace.

Run the benchmark thusly:

    adb shell memory_replay64 /data/local/tmp/traces/XXX.zip
    adb shell memory_replay32 /data/local/tmp/traces/XXX.zip

Where XXX.zip is the name of a zipped trace file. The `memory_replay`
program also can process text files, but all trace files are currently
checked in as zip files.

Every 100000 allocation operations, a dump of the RSS and VA space will be
performed. At the end, a final RSS and VA space number will be printed.
For the most part, the intermediate data can be ignored, but it is always
a good idea to look over the data to verify that no strange spikes are
occurring.

The performance number is a measure of the time it takes to perform all of
the allocation calls (malloc/memalign/posix_memalign/realloc/free/etc).
For any call that allocates a pointer, the time for the call and the time
it takes to make the pointer completely resident in memory is included.

The performance numbers for these runs tend to have a wide variability so
they should not be used as absolute value for comparison against the
current allocator. But, they should be in the same range as the current
values.

When evaluating an allocator, one of the most important traces is the
camera.txt trace. The camera application does very large allocations,
and some allocators might leave large virtual address maps around
rather than delete them. When that happens, it can lead to allocation
failures and would cause the camera app to abort/crash. It is
important to verify that when running this trace using the 32 bit replay
executable, the virtual address space consumed is not much larger than the
current allocator. A small increase (on the order of a few MBs) would be okay.

There is no specific benchmark for memory fragmentation, instead, the RSS
when running the memory traces acts as a proxy for this. An allocator that
is fragmenting badly will show an increase in RSS. The best trace for
tracking fragmentation is system\_server.txt which is an extremely long
trace (~13 million operations). The total number of live allocations goes
up and down a bit, but stays mostly the same so an allocator that fragments
badly would likely show an abnormal increase in RSS on this trace.

NOTE: When a native allocator calls mmap, it is expected that the allocator
will name the map using the call:

    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, <PTR>, <SIZE>, "libc_malloc");

If the native allocator creates a different name, then it necessary to
modify the file:

    system/extras/memory_replay/NativeInfo.cpp

The `GetNativeInfo` function needs to be modified to include the name
of the maps that this allocator includes.

In addition, in order for the frameworks code to keep track of the memory
of a process, any named maps must be added to the file:

    frameworks/base/core/jni/android_os_Debug.cpp

Modify the `load_maps` function and add a check of the new expected name.

#### Performance Trace Benchmarks
This is a benchmark that treats the trace data as if all allocations
occurred in a single thread. This is the scenario that could
happen if all of the allocations are spaced out in time so no thread
every does an allocation at the same time as another thread.

Run these benchmarks thusly:

    adb shell /data/benchmarktest64/trace_benchmark/trace_benchmark
    adb shell /data/benchmarktest/trace_benchmark/trace_benchmark

When run without any arguments, the benchmark will run over all of the
traces and display data. It takes many minutes to complete these runs in
order to get as accurate a number as possible.
