rm -rf out
set -e
versioner headers -i -o out
diff -q -w -B out expected
