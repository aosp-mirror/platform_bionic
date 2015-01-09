#!/usr/bin/env python2
# pylint: disable=bad-indentation
# vim: set sw=2 ts=2:
import json
import requests
import termcolor

import bionicbb.gerrit

from flask import Flask, request
app = Flask(__name__)


def gerrit_url(endpoint):
  gerrit_base_url = 'https://android-review.googlesource.com'
  return gerrit_base_url + endpoint


@app.route('/', methods=['POST'])
def handle_build_message():
  result = json.loads(request.data)

  name = result['name']
  number = result['build']['number']
  status = result['build']['status']
  go_url = 'http://go/bionicbb/' + result['build']['url']
  full_url = result['build']['full_url']
  params = result['build']['parameters']
  change_id = params['CHANGE_ID']
  ref = params['REF']
  patch_set = ref.split('/')[-1]

  print '{} #{} {}: {}'.format(name, number, status, full_url)

  # bionic-lint is always broken, so we don't want to reject changes for those
  # failures until we clean things up.
  if name == 'bionic-presubmit':
    message_lines = ['{} #{} checkbuild {}: {}'.format(
        name, number, status, go_url)]
    if status == 'FAILURE':
      message_lines += ['If you believe this Verified-1 was in error, +1 the '
                        'change and bionicbb will remove the -1 shortly.']

    request_data = {
        'message': '\n'.join(message_lines)
    }

    label = 'Verified'
    if status == 'FAILURE':
      request_data['labels'] = {label: -1}
    elif status == 'SUCCESS':
      request_data['labels'] = {label: +1}

    url = gerrit_url('/a/changes/{}/revisions/{}/review'.format(change_id,
                                                                patch_set))

    headers = {'Content-Type': 'application/json;charset=UTF-8'}
    print 'POST {}: {}'.format(url, request_data)
    print requests.post(url, headers=headers, json=request_data)
  elif name == 'clean-bionic-presubmit':
    request_data = {'message': 'out/ directory removed'}
    url = gerrit_url('/a/changes/{}/revisions/{}/review'.format(change_id,
                                                                patch_set))
    headers = {'Content-Type': 'application/json;charset=UTF-8'}
    print 'POST {}: {}'.format(url, request_data)
    print requests.post(url, headers=headers, json=request_data)
  elif name == 'bionic-lint':
    print 'IGNORED'
  else:
    print '{}: {}'.format(termcolor.colored('red', 'UNKNOWN'), name)
  return ''


@app.route('/drop-rejection', methods=['POST'])
def drop_rejection():
  revision_info = json.loads(request.data)

  change_id = revision_info['changeid']
  patch_set = revision_info['patchset']

  bb_email = 'bionicbb@android.com'
  labels = bionicbb.gerrit.get_labels(change_id, patch_set)
  if bb_email in labels['Verified']:
    bb_review = labels['Verified'][bb_email]
  else:
    bb_review = 0

  if bb_review >= 0:
    print 'No rejection to drop: {} {}'.format(change_id, patch_set)
    return ''

  print 'Dropping rejection: {} {}'.format(change_id, patch_set)

  request_data = {'labels': {'Verified': 0}}
  url = gerrit_url('/a/changes/{}/revisions/{}/review'.format(change_id,
                                                              patch_set))
  headers = {'Content-Type': 'application/json;charset=UTF-8'}
  print 'POST {}: {}'.format(url, request_data)
  print requests.post(url, headers=headers, json=request_data)
  return ''


if __name__ == "__main__":
  app.run(host='0.0.0.0', debug=True)
