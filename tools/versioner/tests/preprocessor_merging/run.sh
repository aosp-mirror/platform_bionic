rm -rf out
set -e
versioner headers -a 9 -a 10 -a 11 -i -o out
diff -q -w -B out expected
