# async_safe logging

This library provides an async_safe implementation for formatting and writing log messages to logd.

Note that the liblog implementation connects a single socket to logd and uses a RWLock to manage
it among threads, whereas these functions connect to liblog for each log message.  While it's
beneficial to have this lock-free and therefore async_safe mechanism to write to logd, connecting
a socket for each message does not scale well under load.  It was also determined to be too
costly to connect a socket for each thread as some processes, such as system_server, have over 100
threads.  Therefore, we maintain these two separate mechanisms.
