import gmail_listener
import unittest


class TestGerritParsers(unittest.TestCase):
    def test_should_skip_message(self):
        info = gmail_listener.get_gerrit_info(
            'Gerrit-Owner: Some Googler <somegoogler@google.com>\n')
        self.assertFalse(gmail_listener.should_skip_message(info))

        info = gmail_listener.get_gerrit_info(
            'Gerrit-Owner: Fake Googler <fakegoogler@google.com.foo.com>\n')
        self.assertTrue(gmail_listener.should_skip_message(info))

        info = gmail_listener.get_gerrit_info(
            'Gerrit-Owner: John Doe <johndoe@example.com>\n')
        self.assertTrue(gmail_listener.should_skip_message(info))


if __name__ == '__main__':
    unittest.main()
