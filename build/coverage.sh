#!/bin/bash

# This script generates coverage for bionic.
#
# Prereqs: Coverage-enabled build.
#   $ lunch <target>
#   $ m NATIVE_COVERAGE_PATHS="bionic" CLANG_COVERAGE=true
#   $ m NATIVE_COVERAGE_PATHS="bionic" CLANG_COVERAGE=true bionic-unit-tests
# Flash image and set $ANDROID_SERIAL
#
# Usage: $ bionic/build/coverage.sh
# Output: HTML report is generated to /tmp/bionic-coverage/html/index.html
#

eval "$(cd ${ANDROID_BUILD_TOP}; build/soong/soong_ui.bash --dumpvars-mode --vars="TARGET_ARCH")"

LLVM_PROFDATA=${ANDROID_BUILD_TOP}/prebuilts/clang/host/linux-x86/llvm-binutils-stable/llvm-profdata
LLVM_COV=${ANDROID_BUILD_TOP}/prebuilts/clang/host/linux-x86/llvm-binutils-stable/llvm-cov

DEVICE_TEST_DIR=/data/local/tmp/bionic-coverage
HOST_PROFDATA_DIR=/tmp/bionic-coverage

# Run bionic-unit-tests
adb shell rm -rf ${DEVICE_TEST_DIR}
adb shell mkdir ${DEVICE_TEST_DIR}
adb push $OUT/data/nativetest/bionic-loader-test-libs ${DEVICE_TEST_DIR}
adb push $OUT/data/nativetest/bionic-unit-tests ${DEVICE_TEST_DIR}
adb shell LLVM_PROFILE_FILE=${DEVICE_TEST_DIR}/profraws/bionic-%p-%m.profraw LD_LIBRARY_PATH=${DEVICE_TEST_DIR}/bionic-loader-test-libs ${DEVICE_TEST_DIR}/bionic-unit-tests/bionic-unit-tests

# Pull coverage files and post-process
rm -rf ${HOST_PROFDATA_DIR}
mkdir ${HOST_PROFDATA_DIR}
adb pull ${DEVICE_TEST_DIR}/profraws ${HOST_PROFDATA_DIR}/profraws


${LLVM_PROFDATA} merge \
  --output=${HOST_PROFDATA_DIR}/bionic.profdata \
  ${HOST_PROFDATA_DIR}/profraws/*.profraw

${LLVM_COV} show \
  --instr-profile=${HOST_PROFDATA_DIR}/bionic.profdata \
  --format=html \
  out/soong/.intermediates/bionic/libc/libc/android_${TARGET_ARCH}_shared_cov/unstripped/libc.so \
  --object=out/soong/.intermediates/bionic/tests/bionic-unit-tests/android_${TARGET_ARCH}_cov/unstripped/bionic-unit-tests \
  /proc/self/cwd/bionic/libc \
  --output-dir=${HOST_PROFDATA_DIR}/html \
  --show-region-summary=false
