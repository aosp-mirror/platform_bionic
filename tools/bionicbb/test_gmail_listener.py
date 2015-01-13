import gmail_listener
import mock
import unittest


class TestShouldSkipMessage(unittest.TestCase):
    def test_accepts_googlers(self):
        for message_type in ('newchange', 'newpatchset', 'comment'):
            with mock.patch('gerrit.get_commit') as mock_commit:
                mock_commit.return_value = {
                    'committer': {'email': 'googler@google.com'}
                }

                self.assertFalse(gmail_listener.should_skip_message({
                    'MessageType': message_type,
                    'Change-Id': '',
                    'PatchSet': '',
                }))

    def test_rejects_non_googlers(self):
        for message_type in ('newchange', 'newpatchset', 'comment'):
            with mock.patch('gerrit.get_commit') as mock_commit:
                mock_commit.return_value = {
                    'committer': {'email': 'fakegoogler@google.com.fake.com'}
                }

                self.assertTrue(gmail_listener.should_skip_message({
                    'MessageType': message_type,
                    'Change-Id': '',
                    'PatchSet': '',
                }))

            with mock.patch('gerrit.get_commit') as mock_commit:
                mock_commit.return_value = {
                    'committer': {'email': 'johndoe@example.com'}
                }

                self.assertTrue(gmail_listener.should_skip_message({
                    'MessageType': message_type,
                    'Change-Id': '',
                    'PatchSet': '',
                }))

    def test_calls_gerrit_get_commit(self):  # pylint: disable=no-self-use
        for message_type in ('newchange', 'newpatchset', 'comment'):
            with mock.patch('gerrit.get_commit') as mock_commit:
                gmail_listener.should_skip_message({
                    'MessageType': message_type,
                    'Change-Id': 'foo',
                    'PatchSet': 'bar',
                })
            mock_commit.assert_called_once_with('foo', 'bar')

            with mock.patch('gerrit.get_commit') as mock_commit:
                gmail_listener.should_skip_message({
                    'MessageType': message_type,
                    'Change-Id': 'baz',
                    'PatchSet': 'qux',
                })
            mock_commit.assert_called_once_with('baz', 'qux')


if __name__ == '__main__':
    unittest.main()
