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
import httplib2

import config


def get_body(msg):
    if 'attachmentId' in msg['payload']['body']:
        raise NotImplementedError('Handling of messages contained in '
                                  'attachments not yet implemented.')
    b64_body = msg['payload']['body']['data']
    return base64.urlsafe_b64decode(b64_body.encode('ASCII'))


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


def get_gerrit_label(labels):
    for label in labels:
        if label['name'] == 'gerrit':
            return label['id']
    return None


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
