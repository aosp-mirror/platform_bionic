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
import json
import logging
import os

from apscheduler.schedulers.background import BackgroundScheduler
from flask import Flask, request
import requests

import gerrit
import tasks

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

    logging.debug('%s #%s %s: %s', name, number, status, full_url)

    # bionic-lint is always broken, so we don't want to reject changes for
    # those failures until we clean things up.
    if name == 'bionic-presubmit':
        message_lines = ['{} #{} checkbuild {}: {}'.format(
            name, number, status, go_url)]
        if status == 'FAILURE':
            message_lines += ['If you believe this Verified-1 was in error, '
                              '+1 the change and bionicbb will remove the -1 '
                              'shortly.']

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
        logging.debug('POST %s: %s', url, request_data)
        requests.post(url, headers=headers, json=request_data)
    elif name == 'clean-bionic-presubmit':
        request_data = {'message': 'out/ directory removed'}
        url = gerrit_url('/a/changes/{}/revisions/{}/review'.format(change_id,
                                                                    patch_set))
        headers = {'Content-Type': 'application/json;charset=UTF-8'}
        logging.debug('POST %s: %s', url, request_data)
        requests.post(url, headers=headers, json=request_data)
    elif name == 'bionic-lint':
        logging.warning('Result for bionic-lint ignored')
    else:
        logging.error('Unknown project: %s', name)
    return ''


@app.route('/drop-rejection', methods=['POST'])
def drop_rejection():
    revision_info = json.loads(request.data)

    change_id = revision_info['changeid']
    patch_set = revision_info['patchset']

    bb_email = 'bionicbb@android.com'
    labels = gerrit.get_labels(change_id, patch_set)
    if bb_email in labels['Verified']:
        bb_review = labels['Verified'][bb_email]
    else:
        bb_review = 0

    if bb_review >= 0:
        logging.info('No rejection to drop: %s %s', change_id, patch_set)
        return ''

    logging.info('Dropping rejection: %s %s', change_id, patch_set)

    request_data = {'labels': {'Verified': 0}}
    url = gerrit_url('/a/changes/{}/revisions/{}/review'.format(change_id,
                                                                patch_set))
    headers = {'Content-Type': 'application/json;charset=UTF-8'}
    logging.debug('POST %s: %s', url, request_data)
    requests.post(url, headers=headers, json=request_data)
    return ''


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    logger = logging.getLogger()
    fh = logging.FileHandler('bionicbb.log')
    fh.setLevel(logging.INFO)
    logger.addHandler(fh)

    # Prevent the job from being rescheduled by the reloader.
    if os.environ.get('WERKZEUG_RUN_MAIN') == 'true':
        scheduler = BackgroundScheduler()
        scheduler.start()
        scheduler.add_job(tasks.get_and_process_jobs, 'interval', minutes=5)

    app.run(host='0.0.0.0', debug=True)
