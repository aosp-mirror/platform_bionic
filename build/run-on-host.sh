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

    NATIVETEST=${TARGET_OUT_DATA}/nativetest
    if [ "${BITS}" = 64 ]; then
        NATIVETEST=${NATIVETEST}64
    fi

    if [ ${TARGET_ARCH} = arm -o ${TARGET_ARCH} = mips -o ${TARGET_ARCH} = x86 ]; then
        LINKER=${TARGET_OUT_EXECUTABLES}/linker
    else
        LINKER="${TARGET_OUT_EXECUTABLES}/linker64 ${TARGET_OUT_EXECUTABLES}/linker"
    fi

    if [ ${TARGET_ARCH} = x86 -o ${TARGET_ARCH} = x86_64 ]; then
        m -j ${LINKER} ${TARGET_OUT}/etc/hosts ${TARGET_OUT_EXECUTABLES}/sh $@

        if [ ! -d /system ]; then
            echo "Attempting to create /system";
            sudo mkdir -p -m 0777 /system;
        fi
        (
            cd ${ANDROID_BUILD_TOP}
            mkdir -p ${TARGET_OUT_DATA}/local/tmp
            ln -fs `realpath ${TARGET_OUT}/bin` /system/
            ln -fs `realpath ${TARGET_OUT}/etc` /system/
            ln -fs `realpath ${TARGET_OUT}/lib` /system/
            if [ -d "${TARGET_OUT}/lib64" ]; then
                ln -fs `realpath ${TARGET_OUT}/lib64` /system/;
            fi
        )
    fi
}
