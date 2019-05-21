#!/bin/bash -e

source ${ANDROID_BUILD_TOP}/build/envsetup.sh

TARGET_ARCH=$(get_build_var TARGET_ARCH)
TARGET_OUT=$(get_build_var TARGET_OUT)
TARGET_OUT_EXECUTABLES=$(get_build_var TARGET_OUT_EXECUTABLES)
TARGET_OUT_DATA=$(get_build_var TARGET_OUT_DATA)
HOST_OS=$(get_build_var HOST_OS)
HOST_ARCH=$(get_build_var HOST_ARCH)
HOST_OUT=$(get_build_var HOST_OUT)

function prepare()
{
    BITS=$1
    shift

    BENCHMARKS=${TARGET_OUT_DATA}/benchmarktest
    NATIVETEST=${TARGET_OUT_DATA}/nativetest
    if [ "${BITS}" = 64 ]; then
        BENCHMARKS=${BENCHMARKS}64
        NATIVETEST=${NATIVETEST}64
    fi

    if [ ${TARGET_ARCH} = x86 -o ${TARGET_ARCH} = x86_64 ]; then
        m -j MODULES-IN-bionic MODULES-IN-external-icu MODULES-IN-external-mksh ${TARGET_OUT}/etc/hosts $@

        if [ ! -d /system ]; then
            echo "Attempting to create /system";
            sudo mkdir -p -m 0777 /system;
            mkdir -p -m 0777 /system/bin;
            mkdir -p -m 0777 /system/lib;
            mkdir -p -m 0777 /system/lib64;
        fi
        (
            cd ${ANDROID_BUILD_TOP}
            mkdir -p ${TARGET_OUT_DATA}/local/tmp
            for i in ${TARGET_OUT}/bin/bootstrap/* ${TARGET_OUT}/bin/*; do
              ln -fs `realpath ${i}` /system/bin/
            done
            ln -fs `realpath ${TARGET_OUT}/etc` /system/
            for i in ${TARGET_OUT}/lib/bootstrap/* ${TARGET_OUT}/lib/*; do
              ln -fs `realpath ${i}` /system/lib/
            done
            for i in ${TARGET_OUT}/lib64/bootstrap/* ${TARGET_OUT}/lib64/*; do
              ln -fs `realpath ${i}` /system/lib64/
            done
        )
    fi
}
