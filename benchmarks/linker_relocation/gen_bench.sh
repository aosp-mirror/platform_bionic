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

bid=6079926

keep_tmp_dir=0
skip_gen_bench=0
for arg in "$@"; do
  case "$arg" in
    --keep-tmp-dir)
      keep_tmp_dir=1
      ;;
    --skip-gen-bench)
      skip_gen_bench=1
      ;;
    *)
      echo "error: unrecognized option: $arg"
      exit 1
      ;;
  esac
done

work_dir=$top/tmp
mkdir -p $work_dir
cd $work_dir

archive=aosp_arm64-target_files-$bid.zip
if [ ! -f $archive ]; then
  /google/data/ro/projects/android/fetch_artifact --bid $bid --target aosp_arm64-userdebug $archive
fi
if [ -e SYSTEM ]; then
  rm -rf SYSTEM
fi

echo 'Unzipping system files...'
unzip -q $archive \
  'SYSTEM/system_ext/apex/com.android.art.debug/*' \
  'SYSTEM/system_ext/apex/com.android.runtime/*' \
  'SYSTEM/lib64/*'

echo 'Dumping relocations...'
python3 -B $top/regen/dump_relocs.py \
  -L $work_dir/SYSTEM/system_ext/apex/com.android.art.debug/lib64 \
  -L $work_dir/SYSTEM/system_ext/apex/com.android.runtime/lib64/bionic \
  -L $work_dir/SYSTEM/lib64 \
  $work_dir/SYSTEM/lib64/libandroid_servers.so \
  $work_dir/libandroid_servers_arm64.json

if [ $skip_gen_bench -eq 0 ]; then
  echo 'Generating benchmark...'
  python3 -B $top/regen/gen_bench.py $work_dir/libandroid_servers_arm64.json $top/gen
fi

if [ $keep_tmp_dir -eq 0 ]; then
  rm -rf $work_dir
fi
