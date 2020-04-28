#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/..
./libc/tools/genversion-scripts.py

git diff --exit-code HEAD libc/*.map libdl/*.map libm/*.map linker/*.map
exit $?
