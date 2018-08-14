Validing libc Assembler Routines
================================
This document describes how to verify incoming assembler libc routines.

## Quick Start
* First, benchmark the previous version of the routine.
* Update the routine, run the bionic unit tests to verify the routine doesn't
have any bugs. See the [Testing](#Testing) section for details about how to
verify that the routine is being properly tested.
* Rerun the benchmarks using the updated image that uses the code for
the new routine. See the [Performance](#Performance) section for details about
benchmarking.
* Verify that unwind information for new routine looks sane. See the [Unwind Info](#unwind-info) section for details about how to verify this.

When benchmarking, it's best to verify on the latest Pixel device supported.
Make sure that you benchmark both the big and little cores to verify that
there is no major difference in performance on each.

Benchmark 64 bit memcmp:

    /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --bionic_xml=string.xml --benchmark_filter=memcmp

Benchmark 32 bit memcmp:

    /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --bionic_xml=string.xml --benchmark_filter=memcmp

Locking to a specific cpu:

    /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --bionic_cpu=2 --bionic_xml=string.xml --benchmark_filter=memcmp

## Performance
The bionic benchmarks are used to verify the performance of changes to
routines. For most routines, there should already be benchmarks available.

Building
--------
The bionic benchmarks are not built by default, they must be built separately
and pushed on to the device. The commands below show how to do this.

    mmma -j bionic/benchmarks
    adb sync data

Running
-------
There are two bionic benchmarks executables:

    /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks

This is for 64 bit libc routines.

    /data/benchmarktest/bionic-benchmarks/bionic-benchmarks

This is for 32 bit libc routines.

Here is an example of how the benchmark should be executed. For this
command to work, you need to change directory to one of the above
directories.

    bionic-benchmarks --bionic_xml=string.xml --benchmark_filter=memcmp

The last argument is the name of the one function that you want to
benchmark.

Almost all routines are already defined in the **string.xml** file in
**bionic/benchmarks/suites**. Look at the examples in that file to see
how to add a benchmark for a function that doesn't already exist.

It can take a long time to run these tests since it attempts to test a
large number of sizes and alignments.

Results
-------
Bionic benchmarks is based on the [Google Benchmarks](https://github.com/google/benchmark)
library. An example of the output looks like this:

    Run on (8 X 1844 MHz CPU s)
    CPU Caches:
      L1 Data 32K (x8)
      L1 Instruction 32K (x8)
      L2 Unified 512K (x2)
    ***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
    -------------------------------------------------------------------------------------------
    Benchmark                                                    Time           CPU Iterations
    -------------------------------------------------------------------------------------------
    BM_string_memcmp/1/0/0                                       6 ns          6 ns  120776418   164.641MB/s
    BM_string_memcmp/1/1/1                                       6 ns          6 ns  120856788   164.651MB/s

The smaller the time, the better the performance.

Caveats
-------
When running the benchmarks, CPU scaling is not normally enabled. This means
that if the device does not get up to the maximum cpu frequency, the results
can vary wildly. It's possible to lock the cpu to the maximum frequency, but
is beyond the scope of this document. However, most of the benchmarks max
out the cpu very quickly on Pixel devices, and don't affect the results.

Another potential issue is that the device can overheat when running the
benchmarks. To avoid this, you can run the device in a cool environment,
or choose a device that is less likely to overheat. To detect these kind
of issues, you can run a subset of the tests again. At the very least, it's
always a good idea to rerun the suite a couple of times to verify that
there isn't a high variation in the numbers.

If you want to verify a single benchmark result, you can run a single test
using a command like this:

    bionic-benchmarks --bionic_xml=string.xml --benchmark_filter=BM_string_memcmp/1/1/0

Where the argument to the filter argument is the name of the benchmark from
the output. Sometimes this filter can still match multiple benchmarks, to
guarantee that you only run the single benchmark, you can execute the benchmark
like so:

    bionic-benchmarks --bionic_xml=string.xml --benchmark_filter=BM_string_memcmp/1/1/0$

NOTE: It is assumed that these commands are executed in adb as the shell user
on device. If you are trying to run this using adb directly from a host
machine, you might need to escape the special shell characters such as **$**.

## Testing

Run the bionic tests to verify that the new routines are valid. However,
you should verify that there is coverage of the new routines. This is
especially important if this is the first time a routine is assembler.

Caveats
-------
When verifying an assembler routine that operates on buffer data (such as
memcpy/strcpy), it's important to verify these corner cases:

* Verify the routine does not read past the end of the buffers. Many
assembler routines optimize by reading multipe bytes at a time and can
read past the end. This kind of bug results in an infrequent and difficult to
diagnosis crash.
* Verify the routine handles unaligned buffers properly. Usually, a failure
can result in an unaligned exception.
* Verify the routine handles different sized buffers.

If there are not sufficient tests for a new routine, there are a set of helper
functions that can be used to verify the above corner cases. See the
header **bionic/tests/buffer\_tests.h** for these routines and look at
**bionic/tests/string\_test.cpp** for examples of how to use it.

## Unwind Info
It is also important to verify that the unwind information for these
routines are properly set up. Here is a quick checklist of what to check:

* Verify that all labels are of the format .LXXX, where XXX is any valid string
for a label. If any other label is used, entries in the symbol table
will be generated that include these labels. In that case, you will get
an unwind with incorrect function information.
* Verify that all places where pop/pushes or instructions that modify the
sp in any way have corresponding cfi information. Along with this item,
verify that when registers are pushed on the stack that there is cfi
information indicating how to get the register.
* Verify that only cfi directives are being used. This only matters for
arm32, where it's possible to use ARM specific unwind directives.

This list is not meant to be exhaustive, but a minimal set of items to verify
before submitting a new libc assembler routine. There are difficult
to verify unwind cases, such as around branches, where unwind information
can be drastically different for the target of the branch and for the
code after a branch instruction.
