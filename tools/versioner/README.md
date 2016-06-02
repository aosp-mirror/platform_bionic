## versioner
Use clang to verify the correctness of bionic's availability attributes against the NDK platform definitions.

#### Build
Build with `FORCE_BUILD_LLVM_COMPONENTS=true mma -j48`

#### Use
`versioner -p platforms current dependencies`
