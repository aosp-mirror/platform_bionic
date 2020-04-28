bionicbb
========

The bionic buildbot contains two services: a gmail polling service, and a web
service that interacts with gerrit.

Dependencies
------------

 * Python 2.7
 * [Advanced Python Scheduler](https://apscheduler.readthedocs.org/en/latest/)
 * [Flask](http://flask.pocoo.org/)
 * [Google API Client Library](https://developers.google.com/api-client-library/python/start/installation)
 * [jenkinsapi](https://pypi.python.org/pypi/jenkinsapi)
 * [Requests](http://docs.python-requests.org/en/latest/)

Setup
-----

Create a `config.py` in the same directory as the sources. The structure of the
configuration file is as follows:

```python
client_secret_file = 'CLIENT_SECRET_FILE.json'
build_listener_url = 'BUILD_LISTENER_URL'
jenkins_url = 'JENKINS_URL'
jenkins_credentials = {
    'username': 'JENKINS_USERNAME',
    'password': 'JENKINS_PASSWORD',
}
```

The client secret file comes from the Gmail API page of the [Google Developers
Console](https://console.developers.google.com/). The Jenkins credentials are
for a Jenkins account that has the appropriate permissions to launch the jobs
the buildbot will use.

You will also need to add the HTTP password for the buildbot's Gerrit account to
`~/.netrc`. The HTTP password can be obtained from the [Gerrit HTTP password
settings](https://android-review.googlesource.com/#/settings/http-password).

To launch the services:

```bash
$ python build_listener.py >build.log 2>&1 &
$ python gmail_listener.py >mail.log 2>&1 &
```

The mail listener will direct your browser to an authentication page for the
Gmail API.

gmail\_listener.py
------------------

Bionicbb polls a gmail account to find changes that need to be built. The gmail
account needs to have a gerrit account set up with project watches on anything
it finds interesting. This is a rather ugly hack, but it seems to be the
simplest option available.

Gerrit does offer a streaming notification service that would be _far_ better,
but it is only available over an SSH conection to gerrit, and the AOSP gerrit
does not support this connection.

Another option would be polling gerrit itself, but we'd have to process each
change every time to see if it should be built, whereas project watches allow us
to treat these as semi-push notifications (we still have to poll gmail).

One drawback to this approach is that it's a hassle to set up the project
watches for a large number of projects. Since bionicbb is only interested in a
small subset of projects, this is a non-issue.

If the buildbot has applied Verified-1 to a patchset, the user may add their own
Verified+1 to the change and the buildbot will remove its rejection the next
time the services polls (by default, every five minutes).

The service will also listen for the following commands:

 * `bionicbb:clean`: Something is very broken and the buildbot's output
   directory needs to be nuked.
 * `bionicbb:retry`: Something went wrong and the buildbot should retry the
   build.

build\_listener.py
------------------

The build listener service responds to HTTP POST events sent from Jenkins and
updates CLs accordingly. The only other API endpoint is `/drop-rejection`, which
will remove a Verified-1 from a previously rejected patchset. The actually
invocation of this is handled by the gmail listener.
