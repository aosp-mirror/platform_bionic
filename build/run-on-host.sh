# source this script in bash

source ${ANDROID_BUILD_TOP}/build/envsetup.sh

# See envsetup.sh for an example of using --dumpvars-mode to set shell variables.
eval "$(cd ${ANDROID_BUILD_TOP}; build/soong/soong_ui.bash --dumpvars-mode --vars="\
    TARGET_ARCH \
    TARGET_OUT \
    TARGET_OUT_EXECUTABLES \
    TARGET_OUT_DATA \
    HOST_OS \
    HOST_ARCH \
    HOST_OUT \
    ")"

function prepare()
{
    local BITS=$1
    shift

    BENCHMARKS=${TARGET_OUT_DATA}/benchmarktest
    NATIVETEST=${TARGET_OUT_DATA}/nativetest
    if [ "${BITS}" = 64 ]; then
        BENCHMARKS=${BENCHMARKS}64
        NATIVETEST=${NATIVETEST}64
    fi

    if [ ${TARGET_ARCH} = x86 -o ${TARGET_ARCH} = x86_64 ]; then
        m -j MODULES-IN-bionic MODULES-IN-external-icu MODULES-IN-external-mksh ${TARGET_OUT}/etc/hosts "$@"

        if [ ! -d /system ]; then
            echo "Attempting to create /system"
            sudo mkdir -p -m 0777 /system
            mkdir -p -m 0777 /system/bin
            mkdir -p -m 0777 /system/lib
            mkdir -p -m 0777 /system/lib64
        fi
        (
            function make_link() {
                dir=$1
                tgt=$2
                name=`basename ${tgt}`
                src=$dir/$name
                if [ -e $tgt ]; then
                    ln -sfT `realpath ${tgt}` $src
                fi
            }
            cd ${ANDROID_BUILD_TOP}
            mkdir -p ${TARGET_OUT_DATA}/local/tmp
            for i in ${TARGET_OUT}/bin/bootstrap/* ${TARGET_OUT}/bin/*; do
                make_link /system/bin ${i}
            done
            ln -fs `realpath ${TARGET_OUT}/etc` /system/
            for i in ${TARGET_OUT}/lib/bootstrap/* ${TARGET_OUT}/lib/*; do
                make_link /system/lib ${i}
            done
            for i in ${TARGET_OUT}/lib64/bootstrap/* ${TARGET_OUT}/lib64/*; do
                make_link /system/lib64 ${i}
            done
        )
    fi
}
