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
from __future__ import absolute_import

import json
import logging
import os.path
import re
import requests

import jenkinsapi

import gerrit

import config


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


def clean_project(dry_run):
    username = config.jenkins_credentials['username']
    password = config.jenkins_credentials['password']
    jenkins_url = config.jenkins_url
    jenkins = jenkinsapi.api.Jenkins(jenkins_url, username, password)

    build = 'clean-bionic-presubmit'
    if build in jenkins:
        if not dry_run:
            _ = jenkins[build].invoke()
            # https://issues.jenkins-ci.org/browse/JENKINS-27256
            # url = job.get_build().baseurl
            url = 'URL UNAVAILABLE'
        else:
            url = 'DRY_RUN_URL'
        logging.info('Cleaning: %s %s', build, url)
    else:
        logging.error('Failed to clean: could not find project %s', build)
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
            raise RuntimeError('Bad project mapping: {} => {}'.format(
                project, project_path))
        ref = gerrit.ref_for_change(change_id)
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
        logging.info('Building: %s => %s %s %s', project, build, url,
                     change_id)
    else:
        logging.error('Unknown build: %s => %s %s', project, build, change_id)
    return True


def handle_change(gerrit_info, _, dry_run):
    if should_skip_build(gerrit_info):
        return True
    return build_project(gerrit_info, dry_run)


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
            logging.error('Failed to drop rejection: %s', ex)
            return False
    logging.info('Dropped rejection: %s', gerrit_info['Change-Id'])
    return True


def handle_comment(gerrit_info, body, dry_run):
    if 'Verified+1' in body:
        drop_rejection(gerrit_info, dry_run)

    if should_skip_build(gerrit_info):
        return True

    command_map = {
        'clean': lambda: clean_project(dry_run),
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
    logging.info('Skipping %s: %s', gerrit_info['MessageType'],
                 gerrit_info['Change-Id'])
    return True
