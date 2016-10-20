#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/..
./libc/tools/generate-NOTICE.py libc libdl libm linker libstdc++ > libc/NOTICE

git diff --exit-code HEAD libc/NOTICE
exit $?
