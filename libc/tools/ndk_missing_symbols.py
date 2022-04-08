#!/usr/bin/env python2
#
# Copyright (C) 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the 'License');
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an 'AS IS' BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import os
import subprocess
import tempfile

import symbols


def adb_pull(src, dst):
    with open(os.devnull, 'w') as devnull:
        subprocess.check_call(['adb', 'pull', src, dst],
                              stdout=devnull, stderr=devnull)


def main():
    tmp_dir = tempfile.mkdtemp()
    adb_pull('/system/lib/libc.so', tmp_dir)
    adb_pull('/system/lib/libm.so', tmp_dir)

    current = symbols.GetFromAndroidSo(['libc.so', 'libm.so'])
    device = (symbols.GetFromElf(os.path.join(tmp_dir, 'libc.so')) |
              symbols.GetFromElf(os.path.join(tmp_dir, 'libm.so')))
    compat_lib = symbols.GetFromAndroidStaticLib(['libc_ndk.a'])

    missing_symbols = current - device
    compat_not_covered = missing_symbols - compat_lib

    for symbol in sorted(compat_not_covered):
        print symbol


if __name__ == '__main__':
    main()
