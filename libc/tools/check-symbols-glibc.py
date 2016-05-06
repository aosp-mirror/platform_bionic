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
# pylint: disable=bad-indentation,bad-continuation
import glob
import os
import re
import sys

import symbols

only_unwanted = False
if len(sys.argv) > 1:
  if sys.argv[1] in ('-u', '--unwanted'):
    only_unwanted = True

toolchain = os.environ['ANDROID_TOOLCHAIN']
arch = re.sub(r'.*/linux-x86/([^/]+)/.*', r'\1', toolchain)
if arch == 'aarch64':
  arch = 'arm64'

def MangleGlibcNameToBionic(name):
  if name in glibc_to_bionic_names:
    return glibc_to_bionic_names[name]
  return name

def GetNdkIgnored(arch):  # pylint: disable=redefined-outer-name
  ignored_symbols = set()
  files = glob.glob('%s/ndk/build/tools/unwanted-symbols/%s/*' %
                    (os.getenv('ANDROID_BUILD_TOP'), arch))
  for f in files:
    ignored_symbols |= set(open(f, 'r').read().splitlines())
  return ignored_symbols

glibc_to_bionic_names = {
  '__res_init': 'res_init',
  '__res_mkquery': 'res_mkquery',
  '__res_query': 'res_query',
  '__res_search': 'res_search',
  '__xpg_basename': '__gnu_basename',
}

glibc = symbols.GetFromSystemSo([
    'libc.so.*',
    'librt.so.*',
    'libpthread.so.*',
    'libresolv.so.*',
    'libm.so.*',
    'libutil.so.*',
])

bionic = symbols.GetFromAndroidSo(['libc.so', 'libm.so'])
this_dir = os.path.dirname(os.path.realpath(__file__))
posix = symbols.GetFromTxt(os.path.join(this_dir, 'posix-2013.txt'))
ndk_ignored = GetNdkIgnored(arch)

glibc = set(map(MangleGlibcNameToBionic, glibc))

# bionic includes various BSD symbols to ease porting other BSD-licensed code.
bsd_stuff = set([
  'arc4random',
  'arc4random_buf',
  'arc4random_uniform',
  'basename_r',
  'dirname_r',
  'fgetln',
  'fpurge',
  'funopen',
  'funopen64',
  'gamma_r',
  'gammaf_r',
  'getprogname',
  'setprogname',
  'strlcat',
  'strlcpy',
  'sys_signame',
  'wcslcat',
  'wcslcpy',
])
# Some symbols are part of the FORTIFY implementation.
FORTIFY_stuff = set([
  '__FD_CLR_chk',
  '__FD_ISSET_chk',
  '__FD_SET_chk',
  '__fwrite_chk',
  '__memchr_chk',
  '__memrchr_chk',
  '__pwrite64_chk',
  '__pwrite_chk',
  '__stack_chk_guard',
  '__stpncpy_chk2',
  '__strchr_chk',
  '__strlcat_chk',
  '__strlcpy_chk',
  '__strlen_chk',
  '__strncpy_chk2',
  '__strrchr_chk',
  '__umask_chk',
  '__write_chk',
])
# Some symbols are used to implement public functions/macros.
macro_stuff = set([
  '__assert2',
  '__errno',
  '__fe_dfl_env',
  '__get_h_errno',
  '__gnu_strerror_r',
  '__fpclassifyd',
  '__isfinite',
  '__isfinitef',
  '__isfinitel',
  '__isnormal',
  '__isnormalf',
  '__isnormall',
  '__sF',
  '__pthread_cleanup_pop',
  '__pthread_cleanup_push',
])
# bionic exposes various Linux features that glibc doesn't.
linux_stuff = set([
  'getauxval',
  'gettid',
  'pthread_gettid_np',
  'tgkill',
])
# Some standard stuff isn't yet in the versions of glibc we're using.
std_stuff = set([
  'at_quick_exit',
  'c16rtomb',
  'c32rtomb',
  'mbrtoc16',
  'mbrtoc32',
])
# These have mangled names in glibc, with a macro taking the "obvious" name.
weird_stuff = set([
  'fstat',
  'fstat64',
  'fstatat',
  'fstatat64',
  'isfinite',
  'isfinitef',
  'isfinitel',
  'isnormal',
  'isnormalf',
  'isnormall',
  'lstat',
  'lstat64',
  'mknod',
  'mknodat',
  'stat',
  'stat64',
  'optreset',
  'sigsetjmp',
])
# These exist in glibc, but under slightly different names (generally one extra
# or one fewer _). TODO: check against glibc names.
libresolv_stuff = set([
  '__res_send_setqhook',
  '__res_send_setrhook',
  '_resolv_delete_cache_for_net',
  '_resolv_flush_cache_for_net',
  '_resolv_set_nameservers_for_net',
  'dn_expand',
  'nsdispatch',
])
# Implementation details we know we export (and can't get away from).
known = set([
  '_ctype_',
  '__libc_init',
])
# POSIX has some stuff that's too stupid for words (a64l) or not actually
# implemented in glibc unless you count always failing with ENOSYS as
# being implemented (fattach).
in_posix_and_glibc_but_actually_dead = set([
  'a64l',
  'fattach',
  'fdetach',
  'getmsg',
  'getpmsg',
  'isastream',
  'l64a',
  'putmsg',
  'putpmsg',
])

posix = posix - in_posix_and_glibc_but_actually_dead
glibc = glibc - in_posix_and_glibc_but_actually_dead

if not only_unwanted:
  #print 'glibc:'
  #for symbol in sorted(glibc):
  #  print symbol
  #print

  #print 'bionic:'
  #for symbol in sorted(bionic):
  #  print symbol
  #print

  print 'in glibc (but not posix) but not bionic:'
  for symbol in sorted((glibc - posix).difference(bionic)):
    print symbol
  print

  print 'in posix (and implemented in glibc) but not bionic:'
  for symbol in sorted((posix.intersection(glibc)).difference(bionic)):
    print symbol
  print

  print 'in bionic but not glibc:'

allowed_stuff = (bsd_stuff | FORTIFY_stuff | linux_stuff | macro_stuff |
                 std_stuff | weird_stuff | libresolv_stuff | known)
for symbol in sorted((bionic - allowed_stuff).difference(glibc)):
  if symbol in ndk_ignored:
    symbol += '*'
  print symbol

sys.exit(0)
