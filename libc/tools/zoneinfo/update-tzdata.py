#!/usr/bin/python

"""Updates the timezone data held in bionic and ICU."""

import ftplib
import glob
import httplib
import os
import re
import shutil
import subprocess
import sys
import tarfile
import tempfile

regions = ['africa', 'antarctica', 'asia', 'australasia',
           'etcetera', 'europe', 'northamerica', 'southamerica',
           # These two deliberately come last so they override what came
           # before (and each other).
           'backward', 'backzone' ]

def CheckDirExists(dir, dirname):
  if not os.path.isdir(dir):
    print "Couldn't find %s (%s)!" % (dirname, dir)
    sys.exit(1)

bionic_libc_tools_zoneinfo_dir = os.path.realpath(os.path.dirname(sys.argv[0]))

# Find the bionic directory, searching upward from this script.
bionic_dir = os.path.realpath('%s/../../..' % bionic_libc_tools_zoneinfo_dir)
bionic_libc_zoneinfo_dir = '%s/libc/zoneinfo' % bionic_dir
CheckDirExists(bionic_libc_zoneinfo_dir, 'bionic/libc/zoneinfo')
CheckDirExists(bionic_libc_tools_zoneinfo_dir, 'bionic/libc/tools/zoneinfo')
print 'Found bionic in %s ...' % bionic_dir

# Find the icu4c directory.
icu_dir = os.path.realpath('%s/../external/icu/icu4c/source' % bionic_dir)
CheckDirExists(icu_dir, 'external/icu/icu4c/source')
print 'Found icu in %s ...' % icu_dir


def GetCurrentTzDataVersion():
  return open('%s/tzdata' % bionic_libc_zoneinfo_dir).read().split('\x00', 1)[0]


def WriteSetupFile():
  """Writes the list of zones that ZoneCompactor should process."""
  links = []
  zones = []
  for region in regions:
    for line in open('extracted/%s' % region):
      fields = line.split()
      if fields:
        if fields[0] == 'Link':
          links.append('%s %s %s' % (fields[0], fields[1], fields[2]))
          zones.append(fields[2])
        elif fields[0] == 'Zone':
          zones.append(fields[1])
  zones.sort()

  setup = open('setup', 'w')
  for link in sorted(set(links)):
    setup.write('%s\n' % link)
  for zone in sorted(set(zones)):
    setup.write('%s\n' % zone)
  setup.close()


def SwitchToNewTemporaryDirectory():
  tmp_dir = tempfile.mkdtemp('-tzdata')
  os.chdir(tmp_dir)
  print 'Created temporary directory "%s"...' % tmp_dir


def FtpRetrieveFile(ftp, filename):
  ftp.retrbinary('RETR %s' % filename, open(filename, 'wb').write)


def FtpRetrieveFileAndSignature(ftp, data_filename):
  """Downloads and repackages the given data from the given FTP server."""
  print 'Downloading data...'
  FtpRetrieveFile(ftp, data_filename)

  print 'Downloading signature...'
  signature_filename = '%s.asc' % data_filename
  FtpRetrieveFile(ftp, signature_filename)


def HttpRetrieveFile(http, path, output_filename):
  http.request("GET", path)
  f = open(output_filename, 'wb')
  f.write(http.getresponse().read())
  f.close()


def HttpRetrieveFileAndSignature(http, data_filename):
  """Downloads and repackages the given data from the given HTTP server."""
  path = "/time-zones/repository/releases/%s" % data_filename

  print 'Downloading data...'
  HttpRetrieveFile(http, path, data_filename)

  print 'Downloading signature...'
  signature_filename = '%s.asc' % data_filename
  HttpRetrievefile(http, "%s.asc" % path, signature_filename)


def BuildIcuToolsAndData(data_filename):
  # Keep track of the original cwd so we can go back to it at the end.
  original_working_dir = os.getcwd()

  # Create a directory to run 'make' from.
  icu_working_dir = '%s/icu' % original_working_dir
  os.mkdir(icu_working_dir)
  os.chdir(icu_working_dir)

  # Build the ICU tools.
  print 'Configuring ICU tools...'
  subprocess.check_call(['%s/runConfigureICU' % icu_dir, 'Linux'])

  # Run the ICU tools.
  os.chdir('tools/tzcode')

  # The tz2icu tool only picks up icuregions and icuzones in they are in the CWD
  for icu_data_file in [ 'icuregions', 'icuzones']:
    icu_data_file_source = '%s/tools/tzcode/%s' % (icu_dir, icu_data_file)
    icu_data_file_symlink = './%s' % icu_data_file
    os.symlink(icu_data_file_source, icu_data_file_symlink)

  shutil.copyfile('%s/%s' % (original_working_dir, data_filename), data_filename)
  print 'Making ICU data...'
  # The Makefile assumes the existence of the bin directory.
  os.mkdir('%s/bin' % icu_working_dir)
  subprocess.check_call(['make'])

  # Copy the source file to its ultimate destination.
  icu_txt_data_dir = '%s/data/misc' % icu_dir
  print 'Copying zoneinfo64.txt to %s ...' % icu_txt_data_dir
  shutil.copy('zoneinfo64.txt', icu_txt_data_dir)

  # Regenerate the .dat file.
  os.chdir(icu_working_dir)
  subprocess.check_call(['make', '-j32'])

  # Copy the .dat file to its ultimate destination.
  icu_dat_data_dir = '%s/stubdata' % icu_dir
  datfiles = glob.glob('data/out/tmp/icudt??l.dat')
  if len(datfiles) != 1:
    print 'ERROR: Unexpectedly found %d .dat files (%s). Halting.' % (len(datfiles), datfiles)
    sys.exit(1)
  datfile = datfiles[0]
  print 'Copying %s to %s ...' % (datfile, icu_dat_data_dir)
  shutil.copy(datfile, icu_dat_data_dir)

  # Switch back to the original working cwd.
  os.chdir(original_working_dir)


def CheckSignature(data_filename):
  signature_filename = '%s.asc' % data_filename
  print 'Verifying signature...'
  # If this fails for you, you probably need to import Paul Eggert's public key:
  # gpg --recv-keys ED97E90E62AA7E34
  subprocess.check_call(['gpg', '--trusted-key=ED97E90E62AA7E34', '--verify',
                         signature_filename, data_filename])


def BuildBionicToolsAndData(data_filename):
  new_version = re.search('(tzdata.+)\\.tar\\.gz', data_filename).group(1)

  print 'Extracting...'
  os.mkdir('extracted')
  tar = tarfile.open(data_filename, 'r')
  tar.extractall('extracted')

  print 'Calling zic(1)...'
  os.mkdir('data')
  zic_inputs = [ 'extracted/%s' % x for x in regions ]
  zic_cmd = ['zic', '-d', 'data' ]
  zic_cmd.extend(zic_inputs)
  subprocess.check_call(zic_cmd)

  WriteSetupFile()

  print 'Calling ZoneCompactor to update bionic to %s...' % new_version
  subprocess.check_call(['javac', '-d', '.',
                         '%s/ZoneCompactor.java' % bionic_libc_tools_zoneinfo_dir])
  subprocess.check_call(['java', 'ZoneCompactor',
                         'setup', 'data', 'extracted/zone.tab',
                         bionic_libc_zoneinfo_dir, new_version])


# Run with no arguments from any directory, with no special setup required.
# See http://www.iana.org/time-zones/ for more about the source of this data.
def main():
  print 'Looking for new tzdata...'

  tzdata_filenames = []

  # The FTP server lets you download intermediate releases, and also lets you
  # download the signatures for verification, so it's your best choice.
  use_ftp = True

  if use_ftp:
    ftp = ftplib.FTP('ftp.iana.org')
    ftp.login()
    ftp.cwd('tz/releases')
    for filename in ftp.nlst():
      if filename.startswith('tzdata20') and filename.endswith('.tar.gz'):
        tzdata_filenames.append(filename)
    tzdata_filenames.sort()
  else:
    http = httplib.HTTPConnection('www.iana.org')
    http.request("GET", "/time-zones")
    index_lines = http.getresponse().read().split('\n')
    for line in index_lines:
      m = re.compile('.*href="/time-zones/repository/releases/(tzdata20\d\d\c\.tar\.gz)".*').match(line)
      if m:
        tzdata_filenames.append(m.group(1))

  # If you're several releases behind, we'll walk you through the upgrades
  # one by one.
  current_version = GetCurrentTzDataVersion()
  current_filename = '%s.tar.gz' % current_version
  for filename in tzdata_filenames:
    if filename > current_filename:
      print 'Found new tzdata: %s' % filename
      SwitchToNewTemporaryDirectory()
      if use_ftp:
        FtpRetrieveFileAndSignature(ftp, filename)
      else:
        HttpRetrieveFileAndSignature(http, filename)

      CheckSignature(filename)
      BuildIcuToolsAndData(filename)
      BuildBionicToolsAndData(filename)
      print 'Look in %s and %s for new data files' % (bionic_dir, icu_dir)
      sys.exit(0)

  print 'You already have the latest tzdata (%s)!' % current_version
  sys.exit(0)


if __name__ == '__main__':
  main()
