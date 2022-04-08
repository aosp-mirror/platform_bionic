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
import glob
import os
import re
import subprocess


def GetFromTxt(txt_file):
    symbols = set()
    f = open(txt_file, 'r')
    for line in f.read().splitlines():
        symbols.add(line)
    f.close()
    return symbols


def GetFromElf(elf_file, sym_type='--dyn-syms'):
    # pylint: disable=line-too-long
    # Example readelf output:
    #     264: 0001623c         4 FUNC        GLOBAL DEFAULT        8 cabsf
    #     266: 00016244         4 FUNC        GLOBAL DEFAULT        8 dremf
    #     267: 00019018         4 OBJECT    GLOBAL DEFAULT     11 __fe_dfl_env
    #     268: 00000000         0 FUNC        GLOBAL DEFAULT    UND __aeabi_dcmplt

    r = re.compile(
        r' +\d+: [0-9a-f]+ +\d+ (I?FUNC|OBJECT) +\S+ +\S+ +\d+ (\S+)')

    symbols = set()

    output = subprocess.check_output(['readelf', sym_type, '-W', elf_file])
    for line in output.split('\n'):
        if ' HIDDEN ' in line or ' UND ' in line:
            continue
        m = r.match(line)
        if m:
            symbol = m.group(2)
            symbol = re.sub('@.*', '', symbol)
            symbols.add(symbol)

    return symbols


def GetFromAndroidStaticLib(files):
    out_dir = os.environ['ANDROID_PRODUCT_OUT']
    lib_dir = os.path.join(out_dir, 'obj')

    results = set()
    for f in files:
        static_lib_dir = os.path.join(
            lib_dir,
            'STATIC_LIBRARIES',
            '{}_intermediates'.format(os.path.splitext(f)[0]))
        results |= GetFromElf(
            os.path.join(static_lib_dir, f),
            sym_type='--syms')
    return results


def GetFromAndroidSo(files):
    out_dir = os.environ['ANDROID_PRODUCT_OUT']
    lib_dir = os.path.join(out_dir, 'system/lib64')
    if not os.path.isdir(lib_dir):
        lib_dir = os.path.join(out_dir, 'system/lib')

    results = set()
    for f in files:
        results |= GetFromElf(os.path.join(lib_dir, f))
    return results


def GetFromSystemSo(files):
    lib_dir = '/lib/x86_64-linux-gnu'
    results = set()
    for f in files:
        results |= GetFromElf(glob.glob(os.path.join(lib_dir, f))[-1])
    return results
