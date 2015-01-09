# pylint: disable=bad-indentation
# vim: set sw=2 ts=2:
import json
import requests


class GerritError(RuntimeError):
  def __init__(self, code, url):
    self.code = code
    self.url = url
    super(GerritError, self).__init__('Error {}: {}'.format(code, url))


def call(endpoint, method='GET'):
  if method != 'GET':
    raise NotImplementedError('Currently only HTTP GET is supported.')
  gerrit_url = 'https://android-review.googlesource.com'
  url = gerrit_url + endpoint
  response = requests.get(url)
  if response.status_code != 200:
    raise GerritError(response.status_code, url)
  return response.text[5:]


def ref_for_change(change_id):
  endpoint = '/changes/{}/detail?o=CURRENT_REVISION'.format(change_id)
  change = json.loads(call(endpoint))
  commit = change['current_revision']
  return change['revisions'][commit]['fetch']['http']['ref']


def get_labels(change_id, patch_set):
  """Returns labels attached to a revision.

  Returned data is in the following format:
  {
      'Code-Review': {
          <email>: <value>,
          ...
      },
      'Verified': {
          <email>: <value>,
          ...
      }
  }
  """
  details = call('/changes/{}/revisions/{}/review'.format(
      change_id, patch_set))
  labels = {'Code-Review': {}, 'Verified': {}}
  for review in details['labels']['Code-Review']['all']:
    if 'value' in review and 'email' in review:
      labels['Code-Review'][review['email']] = int(review['value'])
  for review in details['labels']['Verified']['all']:
    if 'value' in review and 'email' in review:
      labels['Verified'][review['email']] = int(review['value'])
  return labels
