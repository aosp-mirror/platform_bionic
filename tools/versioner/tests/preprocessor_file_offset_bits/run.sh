set -e

rm -rf out
versioner headers -i -o out
diff -q -w -B out expected
