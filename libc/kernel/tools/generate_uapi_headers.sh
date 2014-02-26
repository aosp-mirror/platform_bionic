#!/bin/bash
#
# Copyright (C) 2013 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

###  Usage: generate_uapi_headers.sh [<options>]
###
###  This script is used to get a copy of the uapi kernel headers
###  from an android kernel tree and copies them into an android source
###  tree without any processing. The script also creates all of the
###  generated headers and copies them into the android source tree.
###
###  Options:
###   --skip-generation
###     Skip the step that generates all of the include files.
###   --download-kernel
###     Automatically create a temporary git repository and check out the
###     Android kernel source code.
###   --use-kernel-dir <DIR>
###     Do not check out the kernel source, use the kernel directory
###     pointed to by <DIR>.

# Terminate the script if any command fails.
set -eE

TMPDIR=""
ANDROID_DIR=""
KERNEL_VERSION="android-3.10"
KERNEL_DIR=""
KERNEL_DOWNLOAD=0
ARCH_LIST=("arm" "arm64" "mips" "x86")
ANDROID_KERNEL_DIR="external/kernel-headers/original"
SKIP_GENERATION=0

function cleanup () {
  if [[ "${TMPDIR}" =~ /tmp ]] && [[ -d "${TMPDIR}" ]]; then
    echo "Removing temporary directory ${TMPDIR}"
    rm -rf "${TMPDIR}"
    TMPDIR=""
  fi
}

function usage () {
  grep '^###' $0 | sed -e 's/^###//'
}

function copy_hdrs () {
  local src_dir=$1
  local tgt_dir=$2
  local dont_copy_dirs=$3

  mkdir -p ${tgt_dir}

  local search_dirs=()

  # This only works if none of the filenames have spaces.
  for file in $(ls -d ${src_dir}/* 2> /dev/null); do
    if [[ -d "${file}" ]]; then
      search_dirs+=("${file}")
    elif [[ -f  "${file}" ]] && [[ "${file}" =~ .h$ ]]; then
      cp ${file} ${tgt_dir}
    fi
  done

  if [[ "${dont_copy_dirs}" == "" ]]; then
    for dir in "${search_dirs[@]}"; do
      copy_hdrs "${dir}" ${tgt_dir}/$(basename ${dir})
    done
  fi
}

function copy_if_exists () {
  local check_dir=$1
  local src_dir=$2
  local tgt_dir=$3

  mkdir -p ${tgt_dir}

  # This only works if none of the filenames have spaces.
  for file in $(ls -d ${src_dir}/* 2> /dev/null); do
    if [[ -f  "${file}" ]] && [[ "${file}" =~ .h$ ]]; then
      # Check that this file exists in check_dir.
      header=$(basename ${file})
      if [[ -f "${check_dir}/${header}" ]]; then
        cp ${file} ${tgt_dir}
      fi
    fi
  done
}

trap cleanup EXIT
# This automatically triggers a call to cleanup.
trap "exit 1" HUP INT TERM TSTP

while [ $# -gt 0 ]; do
  case "$1" in
    "--skip-generation")
      SKIP_GENERATION=1
      ;;
    "--download-kernel")
      KERNEL_DOWNLOAD=1
      ;;
    "--use-kernel-dir")
      if [[ $# -lt 2 ]]; then
        echo "--use-kernel-dir requires an argument."
        exit 1
      fi
      shift
      KERNEL_DIR="$1"
      KERNEL_DOWNLOAD=0
      ;;
    "-h" | "--help")
      usage
      exit 1
      ;;
    "-"*)
      echo "Error: Unrecognized option $1"
      usage
      exit 1
      ;;
    *)
      echo "Error: Extra arguments on the command-line."
      usage
      exit 1
      ;;
  esac
  shift
done

ANDROID_KERNEL_DIR="${ANDROID_BUILD_TOP}/${ANDROID_KERNEL_DIR}"
if [[ "${ANDROID_BUILD_TOP}" == "" ]]; then
  echo "ANDROID_BUILD_TOP is not set, did you run lunch?"
  exit 1
elif [[ ! -d "${ANDROID_KERNEL_DIR}" ]]; then
  echo "${ANDROID_BUILD_TOP} doesn't appear to be the root of an android tree."
  echo "  ${ANDROID_KERNEL_DIR} is not a directory."
  exit 1
fi

if [[ ${KERNEL_DOWNLOAD} -eq 1 ]]; then
  TMPDIR=$(mktemp -d /tmp/android_kernelXXXXXXXX)
  cd "${TMPDIR}"
  echo "Fetching android kernel source ${KERNEL_VERSION}"
  git clone https://android.googlesource.com/kernel/common.git
  cd common
  git checkout "${KERNEL_VERSION}"
  KERNEL_DIR="${TMPDIR}"
elif [[ "${KERNEL_DIR}" == "" ]]; then
  echo "Must specify one of --use-kernel-dir or --download-kernel."
  exit 1
elif [[ ! -d "${KERNEL_DIR}" ]] || [[ ! -d "${KERNEL_DIR}/common" ]]; then
  echo "The kernel directory $KERNEL_DIR or $KERNEL_DIR/common does not exist."
  exit 1
else
  cd "${KERNEL_DIR}/common"
fi

if [[ ${SKIP_GENERATION} -eq 0 ]]; then
  # Build all of the generated headers.
  for arch in "${ARCH_LIST[@]}"; do
    echo "Generating headers for arch ${arch}"
    make ARCH=${arch} headers_install
  done
fi

# Copy all of the include/uapi files to the kernel headers uapi directory.
copy_hdrs "${KERNEL_DIR}/common/include/uapi" "${ANDROID_KERNEL_DIR}/uapi"

# Copy the staging files to uapi/linux.
copy_hdrs "${KERNEL_DIR}/common/drivers/staging/android/uapi" \
          "${ANDROID_KERNEL_DIR}/uapi/linux" "no-copy-dirs"

# Copy the generated headers.
copy_hdrs "${KERNEL_DIR}/common/include/generated/uapi" \
          "${ANDROID_KERNEL_DIR}/uapi"

for arch in "${ARCH_LIST[@]}"; do
  # Copy arch headers.
  copy_hdrs "${KERNEL_DIR}/common/arch/${arch}/include/uapi" \
            "${ANDROID_KERNEL_DIR}/uapi/asm-${arch}"
  # Copy the generated arch headers.
  copy_hdrs "${KERNEL_DIR}/common/arch/${arch}/include/generated/uapi" \
            "${ANDROID_KERNEL_DIR}/uapi/asm-${arch}"

  # Special copy of generated header files from arch/<ARCH>/generated/asm that
  # also exist in uapi/asm-generic.
  copy_if_exists "${KERNEL_DIR}/common/include/uapi/asm-generic" \
                 "${KERNEL_DIR}/common/arch/${arch}/include/generated/asm" \
                 "${ANDROID_KERNEL_DIR}/uapi/asm-${arch}/asm"
done
