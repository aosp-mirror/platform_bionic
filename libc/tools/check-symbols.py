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
  api = '9'
  if library == 'libm' and arch == 'arm':
    api = '3'
  path = '%s/development/ndk/platforms/android-%s/arch-%s/symbols/%s.so.%s.txt' % (os.environ['ANDROID_BUILD_TOP'], api, arch, library, functions_or_variables)
  symbols = set()
  for line in open(path, 'r'):
    symbols.add(line.rstrip())
  #sys.stdout.write('%d %s in %s for %s\n' % (len(symbols), functions_or_variables, library, arch))
  return symbols

def CheckSymbols(library, functions_or_variables):
  expected_symbols = GetSymbols(library, functions_or_variables)

  so_file = '%s/system/lib/%s.so' % (os.environ['ANDROID_PRODUCT_OUT'], library)

  # Example readelf output:
  #   264: 0001623c     4 FUNC    GLOBAL DEFAULT    8 cabsf
  #   266: 00016244     4 FUNC    GLOBAL DEFAULT    8 dremf
  #   267: 00019018     4 OBJECT  GLOBAL DEFAULT   11 __fe_dfl_env
  #   268: 00000000     0 FUNC    GLOBAL DEFAULT  UND __aeabi_dcmplt


  r = re.compile(r' +\d+: [0-9a-f]+ +\d+ (FUNC|OBJECT) +\S+ +\S+ +\d+ (\S+)')

  actual_symbols = set()
  for line in subprocess.check_output(['readelf', '--dyn-syms', so_file]).split('\n'):
    m = r.match(line)
    if m:
      if m.group(1) == 'FUNC' and functions_or_variables == 'functions':
        actual_symbols.add(m.group(2))
      elif m.group(1) == 'OBJECT' and functions_or_variables == 'variables':
        actual_symbols.add(m.group(2))
    #else:
      #print 'ignoring: ' % line

  missing = expected_symbols - actual_symbols
  if len(missing) > 0:
    sys.stderr.write('%d missing %s in %s for %s:\n' % (len(missing), functions_or_variables, library, arch))
    for miss in sorted(missing):
      sys.stderr.write('  %s\n' % miss)

  return len(missing) == 0

CheckSymbols("libc", "functions")
CheckSymbols("libc", "variables")
CheckSymbols("libm", "functions")
CheckSymbols("libm", "variables")

sys.exit(0)
