/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "linker.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

extern "C" int tgkill(int tgid, int tid, int sig);

#if __LP64__
#define DEBUGGER_SOCKET_NAME "android:debuggerd64"
#else
#define DEBUGGER_SOCKET_NAME "android:debuggerd"
#endif

enum debugger_action_t {
    // dump a crash
    DEBUGGER_ACTION_CRASH,
    // dump a tombstone file
    DEBUGGER_ACTION_DUMP_TOMBSTONE,
    // dump a backtrace only back to the socket
    DEBUGGER_ACTION_DUMP_BACKTRACE,
};

/* message sent over the socket */
struct debugger_msg_t {
  // version 1 included:
  debugger_action_t action;
  pid_t tid;

  // version 2 added:
  uintptr_t abort_msg_address;
};

// see man(2) prctl, specifically the section about PR_GET_NAME
#define MAX_TASK_NAME_LEN (16)

static int socket_abstract_client(const char* name, int type) {
    sockaddr_un addr;

    // Test with length +1 for the *initial* '\0'.
    size_t namelen = strlen(name);
    if ((namelen + 1) > sizeof(addr.sun_path)) {
        errno = EINVAL;
        return -1;
    }

    /* This is used for abstract socket namespace, we need
     * an initial '\0' at the start of the Unix socket path.
     *
     * Note: The path in this case is *not* supposed to be
     * '\0'-terminated. ("man 7 unix" for the gory details.)
     */
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_LOCAL;
    addr.sun_path[0] = 0;
    memcpy(addr.sun_path + 1, name, namelen);

    socklen_t alen = namelen + offsetof(sockaddr_un, sun_path) + 1;

    int s = socket(AF_LOCAL, type, 0);
    if (s == -1) {
        return -1;
    }

    int rc = TEMP_FAILURE_RETRY(connect(s, reinterpret_cast<sockaddr*>(&addr), alen));
    if (rc == -1) {
        close(s);
        return -1;
    }

    return s;
}

/*
 * Writes a summary of the signal to the log file.  We do this so that, if
 * for some reason we're not able to contact debuggerd, there is still some
 * indication of the failure in the log.
 *
 * We could be here as a result of native heap corruption, or while a
 * mutex is being held, so we don't want to use any libc functions that
 * could allocate memory or hold a lock.
 */
static void log_signal_summary(int signum, const siginfo_t* info) {
    const char* signal_name;
    switch (signum) {
        case SIGILL:    signal_name = "SIGILL";     break;
        case SIGABRT:   signal_name = "SIGABRT";    break;
        case SIGBUS:    signal_name = "SIGBUS";     break;
        case SIGFPE:    signal_name = "SIGFPE";     break;
        case SIGSEGV:   signal_name = "SIGSEGV";    break;
#if defined(SIGSTKFLT)
        case SIGSTKFLT: signal_name = "SIGSTKFLT";  break;
#endif
        case SIGPIPE:   signal_name = "SIGPIPE";    break;
        default:        signal_name = "???";        break;
    }

    char thread_name[MAX_TASK_NAME_LEN + 1]; // one more for termination
    if (prctl(PR_GET_NAME, (unsigned long)thread_name, 0, 0, 0) != 0) {
        strcpy(thread_name, "<name unknown>");
    } else {
        // short names are null terminated by prctl, but the man page
        // implies that 16 byte names are not.
        thread_name[MAX_TASK_NAME_LEN] = 0;
    }

    // "info" will be NULL if the siginfo_t information was not available.
    if (info != NULL) {
        __libc_format_log(ANDROID_LOG_FATAL, "libc",
                          "Fatal signal %d (%s) at %p (code=%d), thread %d (%s)",
                          signum, signal_name, info->si_addr, info->si_code,
                          gettid(), thread_name);
    } else {
        __libc_format_log(ANDROID_LOG_FATAL, "libc",
                          "Fatal signal %d (%s), thread %d (%s)",
                          signum, signal_name, gettid(), thread_name);
    }
}

/*
 * Returns true if the handler for signal "signum" has SA_SIGINFO set.
 */
static bool have_siginfo(int signum) {
    struct sigaction old_action, new_action;

    memset(&new_action, 0, sizeof(new_action));
    new_action.sa_handler = SIG_DFL;
    new_action.sa_flags = SA_RESTART;
    sigemptyset(&new_action.sa_mask);

    if (sigaction(signum, &new_action, &old_action) < 0) {
      __libc_format_log(ANDROID_LOG_WARN, "libc", "Failed testing for SA_SIGINFO: %s",
                        strerror(errno));
      return false;
    }
    bool result = (old_action.sa_flags & SA_SIGINFO) != 0;

    if (sigaction(signum, &old_action, NULL) == -1) {
      __libc_format_log(ANDROID_LOG_WARN, "libc", "Restore failed in test for SA_SIGINFO: %s",
                        strerror(errno));
    }
    return result;
}

/*
 * Catches fatal signals so we can ask debuggerd to ptrace us before
 * we crash.
 */
void debuggerd_signal_handler(int signal_number, siginfo_t* info, void*) {
    // It's possible somebody cleared the SA_SIGINFO flag, which would mean
    // our "info" arg holds an undefined value.
    if (!have_siginfo(signal_number)) {
        info = NULL;
    }

    log_signal_summary(signal_number, info);

    int s = socket_abstract_client(DEBUGGER_SOCKET_NAME, SOCK_STREAM);
    if (s != -1) {
        // debuggerd knows our pid from the credentials on the
        // local socket but we need to tell it the tid of the crashing thread.
        // debuggerd will be paranoid and verify that we sent a tid
        // that's actually in our process.
        debugger_msg_t msg;
        msg.action = DEBUGGER_ACTION_CRASH;
        msg.tid = gettid();
        msg.abort_msg_address = reinterpret_cast<uintptr_t>(gAbortMessage);
        int ret = TEMP_FAILURE_RETRY(write(s, &msg, sizeof(msg)));
        if (ret == sizeof(msg)) {
            // If the write failed, there is no point trying to read a response.
            char debuggerd_ack;
            ret = TEMP_FAILURE_RETRY(read(s, &debuggerd_ack, 1));
            int saved_errno = errno;
            notify_gdb_of_libraries();
            errno = saved_errno;
        }

        if (ret < 0) {
            // read or write failed -- broken connection?
            __libc_format_log(ANDROID_LOG_FATAL, "libc", "Failed while talking to debuggerd: %s",
                              strerror(errno));
        }

        close(s);
    } else {
        // socket failed; maybe process ran out of fds?
        __libc_format_log(ANDROID_LOG_FATAL, "libc", "Unable to open connection to debuggerd: %s",
                          strerror(errno));
    }

    // Remove our net so we fault for real when we return.
    signal(signal_number, SIG_DFL);

    // These signals are not re-thrown when we resume.  This means that
    // crashing due to (say) SIGPIPE doesn't work the way you'd expect it
    // to.  We work around this by throwing them manually.  We don't want
    // to do this for *all* signals because it'll screw up the address for
    // faults like SIGSEGV.
    switch (signal_number) {
        case SIGABRT:
        case SIGFPE:
        case SIGPIPE:
#if defined(SIGSTKFLT)
        case SIGSTKFLT:
#endif
            tgkill(getpid(), gettid(), signal_number);
            break;
        default:    // SIGILL, SIGBUS, SIGSEGV
            break;
    }
}

void debuggerd_init() {
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = debuggerd_signal_handler;
    action.sa_flags = SA_RESTART | SA_SIGINFO;

    // Use the alternate signal stack if available so we can catch stack overflows.
    action.sa_flags |= SA_ONSTACK;

    sigaction(SIGABRT, &action, NULL);
    sigaction(SIGBUS, &action, NULL);
    sigaction(SIGFPE, &action, NULL);
    sigaction(SIGILL, &action, NULL);
    sigaction(SIGPIPE, &action, NULL);
    sigaction(SIGSEGV, &action, NULL);
#if defined(SIGSTKFLT)
    sigaction(SIGSTKFLT, &action, NULL);
#endif
    sigaction(SIGTRAP, &action, NULL);
}
