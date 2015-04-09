import gmail_listener
import mock
import unittest


class TestShouldSkipBuild(unittest.TestCase):
    @mock.patch('gmail_listener.contains_cleanspec')
    @mock.patch('gerrit.get_commit')
    def test_accepts_googlers(self, mock_commit, *other_checks):
        mock_commit.return_value = {
            'committer': {'email': 'googler@google.com'}
        }

        for other_check in other_checks:
            other_check.return_value = False

        for message_type in ('newchange', 'newpatchset', 'comment'):
            self.assertFalse(gmail_listener.should_skip_build({
                'MessageType': message_type,
                'Change-Id': '',
                'PatchSet': '',
            }))

    @mock.patch('gmail_listener.contains_cleanspec')
    @mock.patch('gerrit.get_commit')
    def test_rejects_googlish_domains(self, mock_commit, *other_checks):
        mock_commit.return_value = {
            'committer': {'email': 'fakegoogler@google.com.fake.com'}
        }

        for other_check in other_checks:
            other_check.return_value = False

        for message_type in ('newchange', 'newpatchset', 'comment'):
            self.assertTrue(gmail_listener.should_skip_build({
                'MessageType': message_type,
                'Change-Id': '',
                'PatchSet': '',
            }))

    @mock.patch('gmail_listener.contains_cleanspec')
    @mock.patch('gerrit.get_commit')
    def test_rejects_non_googlers(self, mock_commit, *other_checks):
        mock_commit.return_value = {
            'committer': {'email': 'johndoe@example.com'}
        }

        for other_check in other_checks:
            other_check.return_value = False

        for message_type in ('newchange', 'newpatchset', 'comment'):
            self.assertTrue(gmail_listener.should_skip_build({
                'MessageType': message_type,
                'Change-Id': '',
                'PatchSet': '',
            }))

    @mock.patch('gmail_listener.is_untrusted_committer')
    @mock.patch('gerrit.get_files_for_revision')
    def test_skips_cleanspecs(self, mock_files, *other_checks):
        mock_files.return_value = ['foo/CleanSpec.mk']
        for other_check in other_checks:
            other_check.return_value = False

        for message_type in ('newchange', 'newpatchset', 'comment'):
            self.assertTrue(gmail_listener.should_skip_build({
                'MessageType': message_type,
                'Change-Id': '',
                'PatchSet': '',
            }))


if __name__ == '__main__':
    unittest.main()
