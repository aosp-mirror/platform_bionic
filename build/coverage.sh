#!/bin/bash

# This script generates coverage for bionic.
#
# Prereqs: Coverage-enabled build.
#
#   $ lunch <target>
#   $ m NATIVE_COVERAGE_PATHS="bionic" CLANG_COVERAGE=true
#   $ m NATIVE_COVERAGE_PATHS="bionic" CLANG_COVERAGE=true bionic-unit-tests
#
# Do *NOT* then rebuild at the top level, or you'll clobber the
# coverage-enabled libc!
#
# Flash image and set $ANDROID_SERIAL.
#
# Usage: $ bionic/build/coverage.sh
# Output: HTML report is generated to /tmp/bionic-coverage/html/index.html
#

eval "$(cd ${ANDROID_BUILD_TOP}; build/soong/soong_ui.bash --dumpvars-mode --vars="TARGET_ARCH TARGET_ARCH_VARIANT")"

LLVM_PROFDATA=${ANDROID_BUILD_TOP}/prebuilts/clang/host/linux-x86/llvm-binutils-stable/llvm-profdata
LLVM_COV=${ANDROID_BUILD_TOP}/prebuilts/clang/host/linux-x86/llvm-binutils-stable/llvm-cov

DEVICE_TEST_DIR32=/data/local/tmp/bionic-coverage32
DEVICE_TEST_DIR64=/data/local/tmp/bionic-coverage64
DEVICE_PROF_DIR=/data/local/tmp/bionic-profraw
HOST_PROFDATA_DIR=/tmp/bionic-coverage

# Run bionic-unit-tests
adb shell rm -rf ${DEVICE_TEST_DIR32} ${DEVICE_TEST_DIR64} ${DEVICE_PROF_DIR}
adb shell mkdir ${DEVICE_TEST_DIR32} ${DEVICE_TEST_DIR64} ${DEVICE_PROF_DIR}
adb push $OUT/data/nativetest/bionic-loader-test-libs ${DEVICE_TEST_DIR32}
adb push $OUT/data/nativetest/bionic-unit-tests ${DEVICE_TEST_DIR32}
adb push $OUT/data/nativetest64/bionic-loader-test-libs ${DEVICE_TEST_DIR64}
adb push $OUT/data/nativetest64/bionic-unit-tests ${DEVICE_TEST_DIR64}
adb shell LLVM_PROFILE_FILE=${DEVICE_PROF_DIR}/bionic-%p-%m.profraw \
  LD_LIBRARY_PATH=${DEVICE_TEST_DIR32}/bionic-loader-test-libs \
  ${DEVICE_TEST_DIR32}/bionic-unit-tests/bionic-unit-tests
adb shell LLVM_PROFILE_FILE=${DEVICE_PROF_DIR}/bionic-%p-%m.profraw \
  LD_LIBRARY_PATH=${DEVICE_TEST_DIR64}/bionic-loader-test-libs \
  ${DEVICE_TEST_DIR64}/bionic-unit-tests/bionic-unit-tests

# Pull coverage files and post-process
rm -rf ${HOST_PROFDATA_DIR}
mkdir ${HOST_PROFDATA_DIR}
adb pull ${DEVICE_PROF_DIR} ${HOST_PROFDATA_DIR}/profraws

${LLVM_PROFDATA} merge \
  --output=${HOST_PROFDATA_DIR}/bionic.profdata \
  ${HOST_PROFDATA_DIR}/profraws/*.profraw

${LLVM_COV} show \
  --instr-profile=${HOST_PROFDATA_DIR}/bionic.profdata \
  --format=html \
  $OUT/symbols/apex/com.android.runtime/lib64/bionic/libc.so \
  --object=$OUT/symbols/apex/com.android.runtime/lib64/bionic/libm.so \
  --object=$OUT/symbols/data/nativetest64/bionic-unit-tests/bionic-unit-tests \
  --object=$OUT/symbols/apex/com.android.runtime/lib/bionic/libc.so \
  --object=$OUT/symbols/apex/com.android.runtime/lib/bionic/libm.so \
  --object=$OUT/symbols/data/nativetest/bionic-unit-tests/bionic-unit-tests \
  /proc/self/cwd/bionic/libc \
  /proc/self/cwd/bionic/libm \
  --output-dir=${HOST_PROFDATA_DIR}/html \
  --show-region-summary=false
