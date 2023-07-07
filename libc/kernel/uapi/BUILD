# Copyright (C) 2022 The Android Open Source Project
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

load("//build/bazel/rules/apis:cc_api_contribution.bzl", "cc_api_headers")

package(default_visibility = ["//bionic/libc:__pkg__"])

cc_api_headers(
    name = "libc_kernel_uapi_headers",
    hdrs = glob(["**/*.h"]),
    system = True,
)

cc_api_headers(
    name = "libc_kernel_uapi_asm_arm_headers",
    hdrs = glob(["asm-arm/**/*.h"]),
    arch = "arm",
    include_dir = "asm-arm",
    system = True,
)

cc_api_headers(
    name = "libc_kernel_uapi_asm_arm64_headers",
    hdrs = glob(["asm-arm64/**/*.h"]),
    arch = "arm64",
    include_dir = "asm-arm64",
    system = True,
)

cc_api_headers(
    name = "libc_kernel_uapi_asm_x86_headers",
    hdrs = glob(["asm-x86/**/*.h"]),
    arch = "x86",
    include_dir = "asm-x86",
    system = True,
)

cc_api_headers(
    name = "libc_kernel_uapi_asm_x86_64_headers",
    hdrs = glob(["asm-x86_64/**/*.h"]),
    arch = "x86_64",
    include_dir = "asm-x86_64",
    system = True,
)
