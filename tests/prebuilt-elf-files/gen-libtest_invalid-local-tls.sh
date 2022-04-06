#!/bin/bash
#
# Bionic doesn't support the references to STB_LOCAL symbols of type STT_TLS
# and STT_SECTION that ld.gold generates. Set NDK21E to the path to a copy of
# NDK r21e, which still has ld.gold (unlike the platform build or newer NDKs).

set -e

cat >test.c <<EOF
  static __thread int tls_var_1;
  extern __thread int tls_var_2;
  int* getaddr1() { return &tls_var_1; }
  int* getaddr2() { return &tls_var_2; }
EOF
cat >test2.c <<EOF
  __attribute__((visibility("hidden"))) __thread int tls_var_2;
EOF

build() {
  arch=$1
  target=$2
  $NDK21E/toolchains/llvm/prebuilt/linux-x86_64/bin/clang -O2 --target=$target \
      -fpic -shared -o $arch/libtest_invalid-local-tls.so -fno-emulated-tls \
      -fuse-ld=gold test.c test2.c
}

build arm armv7a-linux-androideabi29
build arm64 aarch64-linux-android29
build x86 i686-linux-android29
build x86_64 x86_64-linux-android29
