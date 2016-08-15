set -e

function run_test {
  SRC=$1
  DST=$2
  rm -rf $2
  versioner $1 -i -o $2
  diff -q -w -B $2 expected
}

run_test headers out
run_test headers/ out
run_test headers out/
run_test headers/ out/

run_test `pwd`/headers out
run_test `pwd`/headers/ out
run_test `pwd`/headers out/
run_test `pwd`/headers/ out/

run_test headers `pwd`/out
run_test headers/ `pwd`/out
run_test headers `pwd`/out/
run_test headers/ `pwd`/out/

run_test `pwd`/headers `pwd`/out
run_test `pwd`/headers/ `pwd`/out
run_test `pwd`/headers `pwd`/out/
run_test `pwd`/headers/ `pwd`/out/
