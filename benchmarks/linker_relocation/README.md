# Dynamic Linker Relocation Benchmark

This benchmark measures the time spent in the dynamic linker to load a large set
of shared objects with many symbols and relocations. It mimics the work involved
in loading a fixed version of `libandroid_servers.so`.

## Running the benchmark

To run the benchmark, build the `linker-reloc-bench` target, sync `data`, and
run the benchmark from `/data/benchmarktest[64]/linker-reloc-bench`.

There is also a `run_bench_with_ninja.sh` script that uses the
`gen_bench.py --ninja` mode to generate a benchmark. It's useful for
experimentation. The `--cc` and `--linker` flags allow swapping out different
static and dynamic linkers.

## Regenerating the synthetic benchmark

`regen/dump_relocs.py` scans an ELF file and its dependencies, outputting a JSON
dump, then `regen/gen_bench.py` processes the JSON file into benchmark code.
`gen_bench.py` has two modes:

 - (default) generate Android.bp and source files
 - with `--ninja`: generate a build.ninja instead, and build a set of ELF file
   outputs
