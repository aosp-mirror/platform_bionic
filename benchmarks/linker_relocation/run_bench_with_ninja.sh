#!/bin/bash -e
#
# Copyright (C) 2019 The Android Open Source Project
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

top=$(cd $(dirname $0) && pwd)

usage() {
cat <<EOF
Usage: $0 [options]

Toolchain:
  --cc CC           C compiler driver to compiler the benchmark with

Run config:
  --cpu-mask MASK   MASK is a hex mask of CPU affinity passed to taskset
  --linker LINKER   Run the benchmark using a specific dynamic linker
  --adb             Run the test using adb

Timing options:
  --multitime N     Use multitime to run the benchmark N iterations
  --time CMD        Use the given time command

Misc:
  --keep-tmp-dir    Don't delete the temporary directory on exit

Examples:
  --cc "\$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android29-clang
        -fuse-ld=lld -Wl,--pack-dyn-relocs=android+relr -Wl,--use-android-relr-tags"
EOF
  exit 0
}

cc=
cpu_mask=0
linker=
use_adb=0
time_cmd=
keep_tmp_dir=0

need_arg() {
  if [ $2 -eq 0 ]; then
    echo "error: $1 needs an argument"
    exit 1
  fi
}

while [ $# -gt 0 ]; do
  arg=$1
  shift
  case "$arg" in
    --cc)           need_arg "$arg" "$#"; cc=$1; shift;;
    --cpu-mask)     need_arg "$arg" "$#"; cpu_mask=$1; shift;;
    --linker)       need_arg "#arg" "$#"; linker=$1; shift;;
    --adb)          use_adb=1;;
    --multitime)    need_arg "$arg" "$#"; time_cmd=--multitime=$1; shift;;
    --time)         need_arg "$arg" "$#"; time_cmd=--time=$1; shift;;
    --keep-tmp-dir) keep_tmp_dir=1;;
    --help)         usage;;
    *)
      echo "error: unrecognized option: $arg"
      exit 1
      ;;
  esac
done

# If no --cc is specified, use gcc because that's likely available.
if [ "$cc" == "" ]; then
  cc='gcc -ldl -pthread'
fi

case "$time_cmd" in
  "") time_cmd=time;;
  --time=*) time_cmd="${time_cmd#--time=}";;
  --multitime=*)
    iter="${time_cmd#--multitime=}"
    if [ $use_adb -eq 1 ]; then
      time_cmd="/data/local/tmp/multitime"
    else
      time_cmd="multitime"
    fi
    time_cmd="$time_cmd -n$iter -s0"
    ;;
esac

# Generate the JSON file as a side effect of regenerating the benchmark.
json_file=$top/tmp/libandroid_servers_arm64.json
if [ ! -e $json_file ]; then
  $top/gen_bench.sh --keep-tmp-dir --skip-gen-bench
fi

tmp_dir=$(mktemp -d)
work_dir=$tmp_dir/linker-reloc-bench

python3 -B $top/regen/gen_bench.py --ninja --cc "$cc" $json_file $work_dir

env_setup=()
if [ $use_adb -eq 1 ]; then
  target_dir=/data/local/tmp/linker-reloc-bench
  adb shell rm -rf $target_dir
  (cd $tmp_dir; tar -c linker-reloc-bench/linker_reloc_bench_main linker-reloc-bench/*.so \
    | gzip -1 -c | adb shell 'cd /data/local/tmp && tar xz')
  env_setup+=( adb shell LD_LIBRARY_PATH=$target_dir )
else
  target_dir=$work_dir
  env_setup+=( env LD_LIBRARY_PATH=$target_dir )
fi

main_cmd=()
if [ "$linker" != "" ]; then
  main_cmd+=( $linker )
fi
main_cmd+=( $target_dir/linker_reloc_bench_main )

# Run the program once to warm the cache.
echo "+ ${env_setup[@]}" "${main_cmd[@]}"
       "${env_setup[@]}" "${main_cmd[@]}"

# Run the benchmark proper.
run_cmd=()
if [ $cpu_mask -ne 0 ]; then
  run_cmd+=( taskset $cpu_mask )
fi
run_cmd+=( $time_cmd )

echo "+ ${env_setup[@]}" "${run_cmd[@]}" "${main_cmd[@]}"
       "${env_setup[@]}" "${run_cmd[@]}" "${main_cmd[@]}"

if [ $keep_tmp_dir -eq 0 ]; then
  rm -fr $tmp_dir
fi
