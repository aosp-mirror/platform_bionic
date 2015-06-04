#!/usr/bin/python

import glob
import os
import re
import string
import subprocess
import sys

toolchain = os.environ['ANDROID_TOOLCHAIN']
arch = re.sub(r'.*/linux-x86/([^/]+)/.*', r'\1', toolchain)

sys.stderr.write('Checking symbols for arch "%s"...\n' % arch)

def GetSymbols(library, functions_or_variables):
  global api
  global arch

  api = '9'
  if library == 'libm' and arch == 'arm':
    api = '3'

  # There were no 64-bit ABIs before API level 21.
  if '64' in arch:
    api = '21'

  # What GCC calls aarch64, Android calls arm64.
  if arch == 'aarch64':
    arch = 'arm64'

  path = '%s/development/ndk/platforms/android-%s/arch-%s/symbols/%s.so.%s.txt' % (os.environ['ANDROID_BUILD_TOP'], api, arch, library, functions_or_variables)
  symbols = set()
  for line in open(path, 'r'):
    symbols.add(line.rstrip())
  #sys.stdout.write('%d %s in %s for %s\n' % (len(symbols), functions_or_variables, library, arch))
  return symbols

def CheckSymbols(library, functions_or_variables):
  expected_symbols = GetSymbols(library, functions_or_variables)

  lib_dir = 'lib'
  if '64' in arch:
    lib_dir = 'lib64'

  so_file = '%s/system/%s/%s.so' % (os.environ['ANDROID_PRODUCT_OUT'], lib_dir, library)

  # Example readelf output:
  #   264: 0001623c     4 FUNC    GLOBAL DEFAULT    8 cabsf
  #   266: 00016244     4 FUNC    GLOBAL DEFAULT    8 dremf
  #   267: 00019018     4 OBJECT  GLOBAL DEFAULT   11 __fe_dfl_env
  #   268: 00000000     0 FUNC    GLOBAL DEFAULT  UND __aeabi_dcmplt


  r = re.compile(r' +\d+: [0-9a-f]+ +\d+ (FUNC|OBJECT) +\S+ +\S+ +\d+ (\S+)')

  actual_symbols = set()
  for line in subprocess.check_output(['readelf', '-W', '--dyn-syms', so_file]).split('\n'):
    m = r.match(line)
    if m:
      symbol = string.split(m.group(2), '@')[0]
      if m.group(1) == 'FUNC' and functions_or_variables == 'functions':
        actual_symbols.add(symbol)
      elif m.group(1) == 'OBJECT' and functions_or_variables == 'variables':
        actual_symbols.add(symbol)
    #else:
      #print 'ignoring: ' % line

  missing = expected_symbols - actual_symbols
  if len(missing) > 0:
    sys.stderr.write('%d missing %s in %s for %s:\n' % (len(missing), functions_or_variables, library, arch))
    for miss in sorted(missing):
      sys.stderr.write('  %s\n' % miss)

  extra = actual_symbols - expected_symbols
  if len(extra) > 0:
    sys.stderr.write('%d extra %s in %s for %s:\n' % (len(extra), functions_or_variables, library, arch))
    for s in sorted(extra):
      sys.stderr.write('  %s\n' % s)

  return len(missing) == 0

CheckSymbols("libc", "functions")
CheckSymbols("libc", "variables")
CheckSymbols("libm", "functions")
CheckSymbols("libm", "variables")

sys.exit(0)
