#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/..
python3 ./libc/tools/generate_notice.py libc libm > libc/NOTICE
if [ $? -ne 0 ]; then
  >&2 echo NOTICE file generation failed
  exit 1
fi

git diff --exit-code HEAD libc/NOTICE
exit $?
