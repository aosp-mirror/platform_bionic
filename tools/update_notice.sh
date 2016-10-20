#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/..
NOTICES="libc/NOTICE libdl/NOTICE libm/NOTICE linker/NOTICE libstdc++/NOTICE"
for notice in $NOTICES; do
  ./libc/tools/generate-NOTICE.py $(dirname $notice) > $notice
done

git diff --exit-code HEAD $NOTICES
exit $?
