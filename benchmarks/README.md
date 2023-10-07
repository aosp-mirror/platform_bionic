# Bionic Benchmarks

[TOC]

## libc benchmarks (bionic-benchmarks)

`bionic-benchmarks` is a command line tool for measuring the runtimes of libc functions. It is built
on top of [Google Benchmark](https://github.com/google/benchmark) with some additions to organize
tests into suites.

### Device benchmarks

    $ mmma bionic/benchmarks
    $ adb root
    $ adb sync data
    $ adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks
    $ adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks

By default, `bionic-benchmarks` runs all of the benchmarks in alphabetical order. Pass
`--benchmark_filter=getpid` to run just the benchmarks with "getpid" in their name.

### Host benchmarks

See the `benchmarks/run-on-host.sh` script. The host benchmarks can be run with 32-bit or 64-bit
Bionic, or the host glibc.

### XML suites

Suites are stored in the `suites/` directory and can be chosen with the command line flag
`--bionic_xml`.

To choose a specific XML file, use the `--bionic_xml=FILE.XML` option. By default, this option
searches for the XML file in the `suites/` directory. If it doesn't exist in that directory, then
the file will be found as relative to the current directory. If the option specifies the full path
to an XML file such as `/data/nativetest/suites/example.xml`, it will be used as-is.

If no XML file is specified through the command-line option, the default is to use `suites/full.xml`.
However, for the host bionic benchmarks (`bionic-benchmarks-glibc`), the default is to use
`suites/host.xml`.

### XML suite format

The format for a benchmark is:

```
<fn>
    <name>BM_sample_benchmark</name>
    <cpu><optional_cpu_to_lock></cpu>
    <iterations><optional_iterations_to_run></iterations>
    <args><space separated list of function args|shorthand></args>
</fn>
```

XML-specified values for iterations and cpu take precedence over those specified via command line
(via `--bionic_iterations` and `--bionic_cpu`, respectively.)

To make small changes in runs, you can also schedule benchmarks by passing in their name and a
space-separated list of arguments via the `--bionic_extra` command line flag, e.g.
`--bionic_extra="BM_string_memcpy AT_COMMON_SIZES"` or `--bionic_extra="BM_string_memcmp 32 8 8"`

Note that benchmarks will run normally if extra arguments are passed in, and it will fail
with a segfault if too few are passed in.

### Shorthand

For the sake of brevity, multiple runs can be scheduled in one XML element by putting one of the
following in the args field:

    NUM_PROPS
    MATH_COMMON
    AT_ALIGNED_<ONE|TWO>BUF
    AT_<any power of two between 2 and 16384>_ALIGNED_<ONE|TWO>BUF
    AT_COMMON_SIZES

Definitions for these can be found in bionic_benchmarks.cpp, and example usages can be found in
the suites directory.

### Unit Tests

`bionic-benchmarks` also has its own set of unit tests, which can be run from the binary in
`/data/nativetest[64]/bionic-benchmarks-tests`

## Process startup time (bionic-spawn-benchmarks)

The `spawn/` subdirectory has a few benchmarks measuring the time used to start simple programs
(e.g. Toybox's `true` and `sh -c true`). Run it on a device like so:

    m bionic-spawn-benchmarks
    adb root
    adb sync data
    adb shell /data/benchmarktest/bionic-spawn-benchmarks/bionic-spawn-benchmarks
    adb shell /data/benchmarktest64/bionic-spawn-benchmarks/bionic-spawn-benchmarks

Google Benchmark reports both a real-time figure ("Time") and a CPU usage figure. For these
benchmarks, the CPU measurement only counts time spent in the thread calling `posix_spawn`, not that
spent in the spawned process. The real-time is probably more useful, and it is the figure used to
determine the iteration count.

Locking the CPU frequency seems to improve the results of these benchmarks significantly, and it
reduces variability.

## Google Benchmark notes

### Repetitions

Google Benchmark uses two settings to control how many times to run each benchmark, "iterations" and
"repetitions". By default, the repetition count is one. Google Benchmark runs the benchmark a few
times to determine a sufficiently-large iteration count.

Google Benchmark can optionally run a benchmark run repeatedly and report statistics (median, mean,
standard deviation) for the runs. To do so, pass the `--benchmark_repetitions` option, e.g.:

    # ./bionic-benchmarks --benchmark_filter=BM_stdlib_strtoll --benchmark_repetitions=4
    ...
    -------------------------------------------------------------------
    Benchmark                         Time             CPU   Iterations
    -------------------------------------------------------------------
    BM_stdlib_strtoll              27.7 ns         27.7 ns     25290525
    BM_stdlib_strtoll              27.7 ns         27.7 ns     25290525
    BM_stdlib_strtoll              27.7 ns         27.7 ns     25290525
    BM_stdlib_strtoll              27.8 ns         27.7 ns     25290525
    BM_stdlib_strtoll_mean         27.7 ns         27.7 ns            4
    BM_stdlib_strtoll_median       27.7 ns         27.7 ns            4
    BM_stdlib_strtoll_stddev      0.023 ns        0.023 ns            4

There are 4 runs, each with 25290525 iterations. Measurements for the individual runs can be
suppressed if they aren't needed:

    # ./bionic-benchmarks --benchmark_filter=BM_stdlib_strtoll --benchmark_repetitions=4 --benchmark_report_aggregates_only
    ...
    -------------------------------------------------------------------
    Benchmark                         Time             CPU   Iterations
    -------------------------------------------------------------------
    BM_stdlib_strtoll_mean         27.8 ns         27.7 ns            4
    BM_stdlib_strtoll_median       27.7 ns         27.7 ns            4
    BM_stdlib_strtoll_stddev      0.043 ns        0.043 ns            4

### CPU frequencies

To get consistent results between runs, it can sometimes be helpful to restrict a benchmark to
specific cores, or to lock cores at specific frequencies. Some phones have a big.LITTLE core setup,
or at least allow some cores to run at higher frequencies than others.

A core can be selected for `bionic-benchmarks` using the `--bionic_cpu` option or using the
`taskset` utility. e.g. A Pixel 3 device has 4 Kryo 385 Silver cores followed by 4 Gold cores:

    blueline:/ # /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=BM_stdlib_strtoll --bionic_cpu=0
    ...
    ------------------------------------------------------------
    Benchmark                  Time             CPU   Iterations
    ------------------------------------------------------------
    BM_stdlib_strtoll       64.2 ns         63.6 ns     11017493

    blueline:/ # /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=BM_stdlib_strtoll --bionic_cpu=4
    ...
    ------------------------------------------------------------
    Benchmark                  Time             CPU   Iterations
    ------------------------------------------------------------
    BM_stdlib_strtoll       21.8 ns         21.7 ns     33167103

A similar result can be achieved using `taskset`. The first parameter is a bitmask of core numbers
to pass to `sched_setaffinity`:

    blueline:/ # taskset f /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=BM_stdlib_strtoll
    ...
    ------------------------------------------------------------
    Benchmark                  Time             CPU   Iterations
    ------------------------------------------------------------
    BM_stdlib_strtoll       64.3 ns         63.6 ns     10998697

    blueline:/ # taskset f0 /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --benchmark_filter=BM_stdlib_strtoll
    ...
    ------------------------------------------------------------
    Benchmark                  Time             CPU   Iterations
    ------------------------------------------------------------
    BM_stdlib_strtoll       21.3 ns         21.2 ns     33094801

To lock the CPU frequency, use the sysfs interface at `/sys/devices/system/cpu/cpu*/cpufreq/`.
Changing the scaling governor to `performance` suppresses the warning that Google Benchmark
otherwise prints:

    ***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.

Some devices have a `perf-setup.sh` script that locks CPU and GPU frequencies. Some TradeFed
benchmarks appear to be using the script. For more information:
 * run `get_build_var BOARD_PERFSETUP_SCRIPT`
 * run `m perf-setup` to install the script into `${OUT}/data/local/tmp/perf-setup.sh`
 * see: https://android.googlesource.com/platform/platform_testing/+/refs/heads/main/scripts/perf-setup/
