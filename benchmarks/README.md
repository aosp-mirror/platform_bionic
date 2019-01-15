Bionic Benchmarks
=================

Bionic benchmarks is a command line tool for measuring the runtimes of libc functions. It is built
on top of [Google benchmarks](https://github.com/google/benchmark) with some additions to organize
tests into suites.

Running the benchmarks
----------------------

### Device benchmarks

    $ mma
    $ adb remount
    $ adb sync
    $ adb shell /data/benchmarktest/bionic-benchmarks/bionic-benchmarks
    $ adb shell /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks

When operated without specifying an xml file, the default is to run all
of the benchmarks in alphabetical order.

You can use `--benchmark_filter=getpid` to just run benchmarks with "getpid"
in their name.

### Host benchmarks

See the benchmarks/run-on-host.sh script. The host benchmarks can be run
with 32 bit or 64 bit bionic, or the host glibc.

## Suites

Suites are stored in the `suites/` directory and can be chosen with the command line flag
'--bionic_xml'.

To choose a specific xml file, use the `--bionic_xml=FILE.XML` option. By default, this
option searches for the xml file in the `suites/` directory. If it doesn't exist
in that directory then the file will be found as relative to the current
directory. If the option specifies the full path to an xml file such as
`/data/nativetest/suites/example.xml`, it will be used as is.

If no xml file is specified through the command-line option, the default is to use `suites/full.xml`.
However, for the host bionic benchmarks (bionic-benchmarks-glibc), the default
is to use `suites/host.xml`.

### Format

The format for a benchmark is:

```
<fn>
    <name>BM_sample_benchmark</name>
    <cpu><optional_cpu_to_lock></cpu>
    <iterations><optional_iterations_to_run></iterations>
    <args><space separated list of function args|shorthand></args>
</fn>
```

xml-specified values for iterations and cpu take precedence over those specified via command line
(via '--bionic_iterations' and '--bionic_cpu', respectively.)

To make small changes in runs, you can also schedule benchmarks by passing in their name and a
space-separated list of arguments via the 'bionic_extra' command line flag, e.g.
'--bionic_extra="BM_string_memcpy AT_COMMON_SIZES"' or '--bionic_extra="BM_string_memcmp 32 8 8"'

Note that benchmarks will run normally if extra arguments are passed in, and it will fail
with a segfault if too few are passed in.

### Shorthand

For the sake of brevity, multiple runs can be scheduled in one xml element by putting one of the
following in the args field:

    NUM_PROPS
    MATH_COMMON
    AT_ALIGNED_<ONE|TWO>BUF
    AT_<any power of two between 2 and 16384>_ALIGNED_<ONE|TWO>BUF
    AT_COMMON_SIZES

Definitions for these can be found in bionic_benchmarks.cpp, and example usages can be found in
the suites directory.

### Unit Tests

Bionic benchmarks also has its own set of unit tests, which can be run from the binary in
`/data/nativetest[64]/bionic-benchmarks-tests`
