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
import httplib
import httplib2
import logging
import re
import socket

import apiclient.errors

import gerrit
import gmail
import presubmit


def get_gerrit_info(body):
    info = {}
    gerrit_pattern = r'^Gerrit-(\S+): (.+)$'
    for match in re.finditer(gerrit_pattern, body, flags=re.MULTILINE):
        info[match.group(1)] = match.group(2).strip()
    return info


def process_message(msg, dry_run):
    try:
        body = gmail.get_body(msg)
        gerrit_info = get_gerrit_info(body)
        if not gerrit_info:
            logging.fatal('No Gerrit info found: %s', msg.subject)
        msg_type = gerrit_info['MessageType']
        handlers = {
            'comment': presubmit.handle_comment,
            'newchange': presubmit.handle_change,
            'newpatchset': presubmit.handle_change,

            'abandon': presubmit.skip_handler,
            'merge-failed': presubmit.skip_handler,
            'merged': presubmit.skip_handler,
            'restore': presubmit.skip_handler,
            'revert': presubmit.skip_handler,
        }

        message_type = gerrit_info['MessageType']
        if message_type in handlers:
            return handlers[message_type](gerrit_info, body, dry_run)
        else:
            logging.warning('MessageType %s unhandled.', msg_type)
        return False
    except NotImplementedError as ex:
        logging.error("%s", ex)
        return False
    except gerrit.GerritError as ex:
        change_id = gerrit_info['Change-Id']
        logging.error('Gerrit error (%d): %s %s', ex.code, change_id, ex.url)
        return ex.code == 404


def get_and_process_jobs():
    dry_run = False

    gmail_service = gmail.build_service()
    msg_service = gmail_service.users().messages()

    # We run in a loop because some of the exceptions thrown here mean we just
    # need to retry. For errors where we should back off (typically any gmail
    # API exceptions), process_changes catches the error and returns normally.
    while True:
        try:
            process_changes(gmail_service, msg_service, dry_run)
            return
        except httplib.BadStatusLine:
            pass
        except httplib2.ServerNotFoundError:
            pass
        except socket.error:
            pass


def process_changes(gmail_service, msg_service, dry_run):
    try:
        labels = gmail_service.users().labels().list(userId='me').execute()
        if not labels['labels']:
            logging.error('Could not retrieve Gmail labels')
            return
        label_id = gmail.get_gerrit_label(labels['labels'])
        if not label_id:
            logging.error('Could not find gerrit label')
            return

        for msg in gmail.get_all_messages(gmail_service, label_id):
            msg = msg_service.get(userId='me', id=msg['id']).execute()
            if process_message(msg, dry_run) and not dry_run:
                msg_service.trash(userId='me', id=msg['id']).execute()
    except apiclient.errors.HttpError as ex:
        logging.error('API Client HTTP error: %s', ex)
