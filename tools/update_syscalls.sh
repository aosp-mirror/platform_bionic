#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/..
./libc/tools/gensyscalls.py

git diff --exit-code HEAD libc/arch-*/syscalls/
exit $?
