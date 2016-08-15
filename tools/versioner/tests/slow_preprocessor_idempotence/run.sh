rm -rf out
set -e
mkdir out
versioner -o out/initial
versioner out/initial ../../dependencies -o out/second
diff -qrwB out/initial out/second
