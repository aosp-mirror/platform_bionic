import mock
import unittest

import presubmit


class TestShouldSkipBuild(unittest.TestCase):
    @mock.patch('presubmit.contains_bionicbb')
    @mock.patch('presubmit.contains_cleanspec')
    @mock.patch('gerrit.get_commit')
    def test_accepts_googlers(self, mock_commit, *other_checks):
        mock_commit.return_value = {
            'committer': {'email': 'googler@google.com'}
        }

        for other_check in other_checks:
            other_check.return_value = False

        for message_type in ('newchange', 'newpatchset', 'comment'):
            self.assertFalse(presubmit.should_skip_build({
                'MessageType': message_type,
                'Change-Id': '',
                'PatchSet': '',
            }))

    @mock.patch('presubmit.contains_bionicbb')
    @mock.patch('presubmit.contains_cleanspec')
    @mock.patch('gerrit.get_commit')
    def test_rejects_googlish_domains(self, mock_commit, *other_checks):
        mock_commit.return_value = {
            'committer': {'email': 'fakegoogler@google.com.fake.com'}
        }

        for other_check in other_checks:
            other_check.return_value = False

        for message_type in ('newchange', 'newpatchset', 'comment'):
            self.assertTrue(presubmit.should_skip_build({
                'MessageType': message_type,
                'Change-Id': '',
                'PatchSet': '',
            }))

    @mock.patch('presubmit.contains_bionicbb')
    @mock.patch('presubmit.contains_cleanspec')
    @mock.patch('gerrit.get_commit')
    def test_rejects_non_googlers(self, mock_commit, *other_checks):
        mock_commit.return_value = {
            'committer': {'email': 'johndoe@example.com'}
        }

        for other_check in other_checks:
            other_check.return_value = False

        for message_type in ('newchange', 'newpatchset', 'comment'):
            self.assertTrue(presubmit.should_skip_build({
                'MessageType': message_type,
                'Change-Id': '',
                'PatchSet': '',
            }))

    @mock.patch('presubmit.contains_bionicbb')
    @mock.patch('presubmit.is_untrusted_committer')
    @mock.patch('gerrit.get_files_for_revision')
    def test_skips_cleanspecs(self, mock_files, *other_checks):
        mock_files.return_value = ['foo/CleanSpec.mk']
        for other_check in other_checks:
            other_check.return_value = False

        for message_type in ('newchange', 'newpatchset', 'comment'):
            self.assertTrue(presubmit.should_skip_build({
                'MessageType': message_type,
                'Change-Id': '',
                'PatchSet': '',
            }))

    @mock.patch('presubmit.contains_cleanspec')
    @mock.patch('presubmit.is_untrusted_committer')
    @mock.patch('gerrit.get_files_for_revision')
    def test_skips_bionicbb(self, mock_files, *other_checks):
        mock_files.return_value = ['tools/bionicbb/common.sh']
        for other_check in other_checks:
            other_check.return_value = False

        for message_type in ('newchange', 'newpatchset', 'comment'):
            self.assertTrue(presubmit.should_skip_build({
                'MessageType': message_type,
                'Change-Id': '',
                'PatchSet': '',
            }))


if __name__ == '__main__':
    unittest.main()
