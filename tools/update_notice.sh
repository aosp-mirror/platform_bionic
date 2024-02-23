#!/bin/bash
set -x
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/..
python3 ./libc/tools/generate_notice.py benchmarks > benchmarks/NOTICE
python3 ./libc/tools/generate_notice.py libc > libc/NOTICE
python3 ./libc/tools/generate_notice.py libdl > libdl/NOTICE
python3 ./libc/tools/generate_notice.py libfdtrack > libfdtrack/NOTICE
python3 ./libc/tools/generate_notice.py libm > libm/NOTICE
python3 ./libc/tools/generate_notice.py libstdc++ > libstdc++/NOTICE
python3 ./libc/tools/generate_notice.py linker > linker/NOTICE
python3 ./libc/tools/generate_notice.py tests > tests/NOTICE
python3 ./libc/tools/generate_notice.py tools > tools/NOTICE
git diff --exit-code HEAD */NOTICE
