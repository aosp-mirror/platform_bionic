#!/usr/bin/python

import glob
import os
import re
import string
import subprocess
import sys

toolchain = os.environ['ANDROID_TOOLCHAIN']
arch = re.sub(r'.*/linux-x86/([^/]+)/.*', r'\1', toolchain)

def GetSymbolsFromSo(so_file):

  # Example readelf output:
  #   264: 0001623c     4 FUNC    GLOBAL DEFAULT    8 cabsf
  #   266: 00016244     4 FUNC    GLOBAL DEFAULT    8 dremf
  #   267: 00019018     4 OBJECT  GLOBAL DEFAULT   11 __fe_dfl_env
  #   268: 00000000     0 FUNC    GLOBAL DEFAULT  UND __aeabi_dcmplt

  r = re.compile(r' +\d+: [0-9a-f]+ +\d+ (I?FUNC|OBJECT) +\S+ +\S+ +\d+ (\S+)')

  symbols = set()

  for line in subprocess.check_output(['readelf', '--dyn-syms', '-W', so_file]).split('\n'):
     if ' HIDDEN ' in line or ' UND ' in line:
       continue
     m = r.match(line)
     if m:
       symbol = m.group(2)
       symbol = re.sub('@.*', '', symbol)
       symbols.add(symbol)

  return symbols

def GetSymbolsFromAndroidSo(*files):
  symbols = set()
  for f in files:
    symbols = symbols | GetSymbolsFromSo('%s/system/lib64/%s' % (os.environ['ANDROID_PRODUCT_OUT'], f))
  return symbols

def GetSymbolsFromSystemSo(*files):
  symbols = set()
  for f in files:
    f = glob.glob('/lib/x86_64-linux-gnu/%s' % f)[-1]
    symbols = symbols | GetSymbolsFromSo(f)
  return symbols

glibc = GetSymbolsFromSystemSo('libc.so.*', 'librt.so.*', 'libpthread.so.*', 'libresolv.so.*', 'libm.so.*')
bionic = GetSymbolsFromAndroidSo('libc.so', 'libm.so')

# bionic includes various BSD symbols to ease porting other BSD-licensed code.
bsd_stuff = set([
  'getprogname',
  'setprogname',
  'strlcat',
  'strlcpy',
  'wcslcat',
  'wcslcpy'
])
# Some symbols are part of the FORTIFY implementation.
FORTIFY_stuff = set([
  '__FD_CLR_chk',
  '__FD_ISSET_chk',
  '__FD_SET_chk',
  '__stack_chk_guard',
  '__stpncpy_chk2',
  '__strchr_chk',
  '__strlcat_chk',
  '__strlcpy_chk',
  '__strlen_chk',
  '__strncpy_chk2',
  '__strrchr_chk',
  '__umask_chk'
])
# bionic exposes various Linux features that glibc doesn't.
linux_stuff = set([
  'getauxval',
  'gettid',
  'tgkill'
])
# Some standard stuff isn't yet in the versions of glibc we're using.
std_stuff = set([
  'at_quick_exit'
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
])

print 'glibc:'
for symbol in sorted(glibc):
  print symbol

print
print 'bionic:'
for symbol in sorted(bionic):
  print symbol

print
print 'in bionic but not glibc:'
for symbol in sorted((bionic - bsd_stuff - FORTIFY_stuff - linux_stuff - std_stuff - weird_stuff).difference(glibc)):
  print symbol

sys.exit(0)
