set -e

rm -rf out
versioner headers -a 9 -a 12 -a 13 -i -o out
diff -q -w -B out expected
