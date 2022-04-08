#!/system/bin/sh

# Rather than have ldd and ldd64, this script does the right thing depending
# on the argument.

function error() {
  echo "$1"
  exit 1
}

[ $# -eq 1 ] || error "usage: ldd FILE"

what=$(file -L "$1")
case "$what" in
  *32-bit*)
    linker --list "$1"
    ;;
  *64-bit*)
    linker64 --list "$1"
    ;;
  *)
    error "$what"
    ;;
esac
