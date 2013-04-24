#!/usr/bin/python

"""Updates the tzdata file."""

import ftplib
import httplib
import os
import re
import subprocess
import sys
import tarfile
import tempfile

# Find the bionic directory, searching upward from this script.
bionic_libc_tools_zoneinfo_dir = os.path.realpath(os.path.dirname(sys.argv[0]))
bionic_libc_tools_dir = os.path.dirname(bionic_libc_tools_zoneinfo_dir)
bionic_libc_dir = os.path.dirname(bionic_libc_tools_dir)
bionic_dir = os.path.dirname(bionic_libc_dir)
bionic_libc_zoneinfo_dir = '%s/libc/zoneinfo' % bionic_dir

if not os.path.isdir(bionic_libc_tools_zoneinfo_dir):
  print "Couldn't find bionic/libc/tools/zoneinfo!"
  sys.exit(1)
if not os.path.isdir(bionic_libc_zoneinfo_dir):
  print "Couldn't find bionic/libc/zoneinfo!"
  sys.exit(1)

print 'Found bionic in %s...' % bionic_dir


regions = ['africa', 'antarctica', 'asia', 'australasia', 'backward',
           'etcetera', 'europe', 'northamerica', 'southamerica']


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
          links.append('%s %s %s\n' % (fields[0], fields[1], fields[2]))
          zones.append(fields[2])
        elif fields[0] == 'Zone':
          zones.append(fields[1])
  zones.sort()

  setup = open('setup', 'w')
  for link in links:
    setup.write(link)
  for zone in zones:
    setup.write('%s\n' % zone)
  setup.close()


def SwitchToNewTemporaryDirectory():
  tmp_dir = tempfile.mkdtemp('-tzdata')
  os.chdir(tmp_dir)
  print 'Created temporary directory "%s"...' % tmp_dir


def FtpRetrieve(ftp, filename):
  ftp.retrbinary('RETR %s' % filename, open(filename, 'wb').write)


def FtpUpgrade(ftp, data_filename):
  """Downloads and repackages the given data from the given FTP server."""
  SwitchToNewTemporaryDirectory()

  print 'Downloading data...'
  FtpRetrieve(ftp, data_filename)

  print 'Downloading signature...'
  signature_filename = '%s.asc' % data_filename
  FtpRetrieve(ftp, signature_filename)

  ExtractAndCompile(data_filename)


def HttpRetrieve(http, path, output_filename):
  http.request("GET", path)
  f = open(output_filename, 'wb')
  f.write(http.getresponse().read())
  f.close()


def HttpUpgrade(http, data_filename):
  """Downloads and repackages the given data from the given HTTP server."""
  SwitchToNewTemporaryDirectory()

  path = "/time-zones/repository/releases/%s" % data_filename

  print 'Downloading data...'
  HttpRetrieve(http, path, data_filename)

  print 'Downloading signature...'
  signature_filename = '%s.asc' % data_filename
  HttpRetrieve(http, "%s.asc" % path, signature_filename)

  ExtractAndCompile(data_filename)


def ExtractAndCompile(data_filename):
  new_version = re.search('(tzdata.+)\\.tar\\.gz', data_filename).group(1)

  signature_filename = '%s.asc' % data_filename
  print 'Verifying signature...'
  # If this fails for you, you probably need to import Paul Eggert's public key:
  # gpg --recv-keys ED97E90E62AA7E34
  subprocess.check_call(['gpg', '--trusted-key=ED97E90E62AA7E34', '--verify',
                         signature_filename, data_filename])

  print 'Extracting...'
  os.mkdir('extracted')
  tar = tarfile.open(data_filename, 'r')
  tar.extractall('extracted')

  print 'Calling zic(1)...'
  os.mkdir('data')
  for region in regions:
    if region != 'backward':
      subprocess.check_call(['zic', '-d', 'data', 'extracted/%s' % region])

  WriteSetupFile()

  print 'Calling ZoneCompactor to update bionic to %s...' % new_version
  libcore_src_dir = '%s/../libcore/luni/src/main/java/' % bionic_dir
  subprocess.check_call(['javac', '-d', '.',
                         '%s/ZoneCompactor.java' % bionic_libc_tools_zoneinfo_dir,
                         '%s/libcore/util/ZoneInfo.java' % libcore_src_dir,
                         '%s/libcore/io/BufferIterator.java' % libcore_src_dir])
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
      if use_ftp:
        FtpUpgrade(ftp, filename)
      else:
        HttpUpgrade(http, filename)
      sys.exit(0)

  print 'You already have the latest tzdata (%s)!' % current_version
  sys.exit(0)


if __name__ == '__main__':
  main()
