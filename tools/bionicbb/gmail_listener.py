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
import base64
import httplib
import httplib2
import jenkinsapi
import json
import os
import re
import requests
import termcolor
import socket
import sys
import time

import apiclient.errors

import config
import gerrit


class GmailError(RuntimeError):
    def __init__(self, message):
        super(GmailError, self).__init__(message)


def get_gerrit_label(labels):
    for label in labels:
        if label['name'] == 'gerrit':
            return label['id']
    return None


def get_headers(msg):
    headers = {}
    for hdr in msg['payload']['headers']:
        headers[hdr['name']] = hdr['value']
    return headers


def is_untrusted_committer(change_id, patch_set):
    # TODO(danalbert): Needs to be based on the account that made the comment.
    commit = gerrit.get_commit(change_id, patch_set)
    committer = commit['committer']['email']
    return not committer.endswith('@google.com')


def contains_cleanspec(change_id, patch_set):
    files = gerrit.get_files_for_revision(change_id, patch_set)
    return 'CleanSpec.mk' in [os.path.basename(f) for f in files]


def contains_bionicbb(change_id, patch_set):
    files = gerrit.get_files_for_revision(change_id, patch_set)
    return any('tools/bionicbb' in f for f in files)


def should_skip_build(info):
    if info['MessageType'] not in ('newchange', 'newpatchset', 'comment'):
        raise ValueError('should_skip_build() is only valid for new '
                         'changes, patch sets, and commits.')

    change_id = info['Change-Id']
    patch_set = info['PatchSet']

    checks = [
        is_untrusted_committer,
        contains_cleanspec,
        contains_bionicbb,
    ]
    for check in checks:
        if check(change_id, patch_set):
            return True
    return False


def build_service():
    from apiclient.discovery import build
    from oauth2client.client import flow_from_clientsecrets
    from oauth2client.file import Storage
    from oauth2client.tools import run

    OAUTH_SCOPE = 'https://www.googleapis.com/auth/gmail.modify'
    STORAGE = Storage('oauth.storage')

    # Start the OAuth flow to retrieve credentials
    flow = flow_from_clientsecrets(config.client_secret_file,
                                   scope=OAUTH_SCOPE)
    http = httplib2.Http()

    # Try to retrieve credentials from storage or run the flow to generate them
    credentials = STORAGE.get()
    if credentials is None or credentials.invalid:
        credentials = run(flow, STORAGE, http=http)

    http = credentials.authorize(http)
    return build('gmail', 'v1', http=http)


def get_all_messages(service, label):
    msgs = []
    response = service.users().messages().list(
        userId='me', labelIds=label).execute()
    if 'messages' in response:
        msgs.extend(response['messages'])
    while 'nextPageToken' in response:
        page_token = response['nextPageToken']
        response = service.users().messages().list(
            userId='me', pageToken=page_token).execute()
        msgs.extend(response['messages'])
    return msgs


def get_body(msg):
    if 'attachmentId' in msg['payload']['body']:
        raise NotImplementedError('Handling of messages contained in '
                                  'attachments not yet implemented.')
    b64_body = msg['payload']['body']['data']
    return base64.urlsafe_b64decode(b64_body.encode('ASCII'))


def get_gerrit_info(body):
    info = {}
    gerrit_pattern = r'^Gerrit-(\S+): (.+)$'
    for match in re.finditer(gerrit_pattern, body, flags=re.MULTILINE):
        info[match.group(1)] = match.group(2).strip()
    return info


def clean_project(gerrit_info, dry_run):
    username = config.jenkins_credentials['username']
    password = config.jenkins_credentials['password']
    jenkins_url = config.jenkins_url
    jenkins = jenkinsapi.api.Jenkins(jenkins_url, username, password)

    build = 'clean-bionic-presubmit'
    if build in jenkins:
        if not dry_run:
            job = jenkins[build].invoke()
            url = job.get_build().baseurl
        else:
            url = 'DRY_RUN_URL'
        print '{}({}): {} {}'.format(
            termcolor.colored('CLEAN', 'green'),
            gerrit_info['MessageType'],
            build,
            url)
    else:
        print '{}({}): {}'.format(
            termcolor.colored('CLEAN', 'red'),
            gerrit_info['MessageType'],
            termcolor.colored(build, 'red'))
    return True


def build_project(gerrit_info, dry_run, lunch_target=None):
    project_to_jenkins_map = {
        'platform/bionic': 'bionic-presubmit',
        'platform/build': 'bionic-presubmit',
        'platform/external/jemalloc': 'bionic-presubmit',
        'platform/external/libcxx': 'bionic-presubmit',
        'platform/external/libcxxabi': 'bionic-presubmit',
        'platform/external/compiler-rt': 'bionic-presubmit',
    }

    username = config.jenkins_credentials['username']
    password = config.jenkins_credentials['password']
    jenkins_url = config.jenkins_url
    jenkins = jenkinsapi.api.Jenkins(jenkins_url, username, password)

    project = gerrit_info['Project']
    change_id = gerrit_info['Change-Id']
    if project in project_to_jenkins_map:
        build = project_to_jenkins_map[project]
    else:
        build = 'bionic-presubmit'

    if build in jenkins:
        project_path = '/'.join(project.split('/')[1:])
        if not project_path:
            raise RuntimeError('bogus project: {}'.format(project))
        if project_path.startswith('platform/'):
            print '{}({}): {} => {}'.format(
                termcolor.colored('ERROR', 'red'),
                'project',
                project,
                project_path)
            return False
        try:
            ref = gerrit.ref_for_change(change_id)
        except gerrit.GerritError as ex:
            print '{}({}): {} {}'.format(
                termcolor.colored('GERRIT-ERROR', 'red'),
                ex.code,
                change_id,
                ex.url)
            return False
        params = {
            'REF': ref,
            'CHANGE_ID': change_id,
            'PROJECT': project_path
        }
        if lunch_target is not None:
            params['LUNCH_TARGET'] = lunch_target
        if not dry_run:
            _ = jenkins[build].invoke(build_params=params)
            # https://issues.jenkins-ci.org/browse/JENKINS-27256
            # url = job.get_build().baseurl
            url = 'URL UNAVAILABLE'
        else:
            url = 'DRY_RUN_URL'
        print '{}({}): {} => {} {} {}'.format(
            termcolor.colored('BUILD', 'green'),
            gerrit_info['MessageType'],
            project,
            build,
            url,
            change_id)
    else:
        print '{}({}): {} => {} {}'.format(
            termcolor.colored('BUILD', 'red'),
            gerrit_info['MessageType'],
            project,
            termcolor.colored(build, 'red'),
            change_id)
    return True


def handle_change(gerrit_info, _, dry_run):
    if should_skip_build(gerrit_info):
        return True
    return build_project(gerrit_info, dry_run)
handle_newchange = handle_change
handle_newpatchset = handle_change


def drop_rejection(gerrit_info, dry_run):
    request_data = {
        'changeid': gerrit_info['Change-Id'],
        'patchset': gerrit_info['PatchSet']
    }
    url = '{}/{}'.format(config.build_listener_url, 'drop-rejection')
    headers = {'Content-Type': 'application/json;charset=UTF-8'}
    if not dry_run:
        try:
            requests.post(url, headers=headers, data=json.dumps(request_data))
        except requests.exceptions.ConnectionError as ex:
            print '{}(drop-rejection): {}'.format(
                termcolor.colored('ERROR', 'red'), ex)
            return False
    print '{}({}): {}'.format(
        termcolor.colored('CHECK', 'green'),
        gerrit_info['MessageType'],
        gerrit_info['Change-Id'])
    return True


def handle_comment(gerrit_info, body, dry_run):
    if 'Verified+1' in body:
        drop_rejection(gerrit_info, dry_run)

    if should_skip_build(gerrit_info):
        return True

    command_map = {
        'clean': lambda: clean_project(gerrit_info, dry_run),
        'retry': lambda: build_project(gerrit_info, dry_run),

        'arm': lambda: build_project(gerrit_info, dry_run,
                                     lunch_target='aosp_arm-eng'),
        'aarch64': lambda: build_project(gerrit_info, dry_run,
                                         lunch_target='aosp_arm64-eng'),
        'mips': lambda: build_project(gerrit_info, dry_run,
                                      lunch_target='aosp_mips-eng'),
        'mips64': lambda: build_project(gerrit_info, dry_run,
                                        lunch_target='aosp_mips64-eng'),
        'x86': lambda: build_project(gerrit_info, dry_run,
                                     lunch_target='aosp_x86-eng'),
        'x86_64': lambda: build_project(gerrit_info, dry_run,
                                        lunch_target='aosp_x86_64-eng'),
    }

    def handle_unknown_command():
        pass    # TODO(danalbert): should complain to the commenter.

    commands = [match.group(1).strip() for match in
                re.finditer(r'^bionicbb:\s*(.+)$', body, flags=re.MULTILINE)]

    for command in commands:
        if command in command_map:
            command_map[command]()
        else:
            handle_unknown_command()

    return True


def skip_handler(gerrit_info, _, __):
    print '{}({}): {}'.format(
        termcolor.colored('SKIP', 'yellow'),
        gerrit_info['MessageType'],
        gerrit_info['Change-Id'])
    return True
handle_abandon = skip_handler
handle_merged = skip_handler
handle_restore = skip_handler
handle_revert = skip_handler


def process_message(msg, dry_run):
    try:
        body = get_body(msg)
        gerrit_info = get_gerrit_info(body)
        if not gerrit_info:
            print termcolor.colored('No info found: {}'.format(msg['id']),
                                    'red')
        msg_type = gerrit_info['MessageType']
        handler = 'handle_{}'.format(gerrit_info['MessageType'])
        if handler in globals():
            return globals()[handler](gerrit_info, body, dry_run)
        else:
            print termcolor.colored(
                'MessageType {} unhandled.'.format(msg_type), 'red')
        print
        return False
    except NotImplementedError as ex:
        print ex
        return False
    except gerrit.GerritError as ex:
        if ex.code == 404:
            print '{}(404): {}!'.format(
                termcolor.colored('ERROR', 'red'), ex)
            return True
        else:
            return False


def main(argc, argv):
    dry_run = False
    if argc == 2 and argv[1] == '--dry-run':
        dry_run = True
    elif argc > 2:
        sys.exit('usage: python {} [--dry-run]'.format(argv[0]))

    gmail_service = build_service()
    msg_service = gmail_service.users().messages()

    while True:
        try:
            labels = gmail_service.users().labels().list(userId='me').execute()
            if not labels['labels']:
                raise GmailError('Could not retrieve Gmail labels')
            label_id = get_gerrit_label(labels['labels'])
            if not label_id:
                raise GmailError('Could not find gerrit label')

            for msg in get_all_messages(gmail_service, label_id):
                msg = msg_service.get(userId='me', id=msg['id']).execute()
                if process_message(msg, dry_run) and not dry_run:
                    msg_service.trash(userId='me', id=msg['id']).execute()
            time.sleep(60 * 5)
        except GmailError as ex:
            print '{}: {}!'.format(termcolor.colored('ERROR', 'red'), ex)
            time.sleep(60 * 5)
        except apiclient.errors.HttpError as ex:
            print '{}: {}!'.format(termcolor.colored('ERROR', 'red'), ex)
            time.sleep(60 * 5)
        except httplib.BadStatusLine:
            pass
        except httplib2.ServerNotFoundError:
            pass
        except socket.error:
            pass


if __name__ == '__main__':
    main(len(sys.argv), sys.argv)
