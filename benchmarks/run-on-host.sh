#!/bin/bash -e

. $(dirname $0)/../build/run-on-host.sh

if [ "$1" = glibc ]; then
    m -j bionic-benchmarks-glibc
    (
        cd ${ANDROID_BUILD_TOP}
        export ANDROID_DATA=${TARGET_OUT_DATA}
        export ANDROID_ROOT=${TARGET_OUT}
        ${HOST_OUT}/nativetest64/bionic-benchmarks-glibc/bionic-benchmarks-glibc $@
    )
    exit 0
elif [ "$1" != 32 -a "$1" != 64 ]; then
    echo "Usage: $0 [ 32 | 64 | glibc ] [benchmark flags]"
    exit 1
fi

if [ ${HOST_OS}-${HOST_ARCH} = linux-x86 -o ${HOST_OS}-${HOST_ARCH} = linux-x86_64 ]; then

    prepare $1 bionic-benchmarks

    if [ ${TARGET_ARCH} = x86 -o ${TARGET_ARCH} = x86_64 ]; then
        (
            cd ${ANDROID_BUILD_TOP}
            export ANDROID_DATA=${TARGET_OUT_DATA}
            export ANDROID_ROOT=${TARGET_OUT}
            ${NATIVETEST}/bionic-benchmarks/bionic-benchmarks $@
        )
    else
        echo "$0 not supported on TARGET_ARCH=$TARGET_ARCH"
    fi
fi
