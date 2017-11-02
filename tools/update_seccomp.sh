#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/..
./libc/tools/genseccomp.py

git diff --exit-code HEAD libc/seccomp/
exit $?
