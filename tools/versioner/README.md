## versioner
Use clang to verify the correctness of bionic's availability attributes against the NDK platform definitions.

#### Build
Build with `LLVM_BUILD_HOST_TOOLS=true mma -j48`

#### Use
`versioner -p platforms current dependencies`
