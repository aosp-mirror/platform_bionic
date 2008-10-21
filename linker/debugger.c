#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/mman.h>

#include "linker.h"

#include <sys/socket.h>
#include <cutils/sockets.h>

void notify_gdb_of_libraries();

void debugger_signal_handler(int n)
{
    unsigned tid;
    int s;
           
        /* avoid picking up GC interrupts */
    signal(SIGUSR1, SIG_IGN);
    
    tid = gettid();
    s = socket_local_client("android:debuggerd", 
            ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
    
    if(s >= 0) {
            /* debugger knows our pid from the credentials on the
            ** local socket but we need to tell it our tid.  It
            ** is paranoid and will verify that we are giving a tid
            ** that's actually in our process
            */
        write(s, &tid, sizeof(unsigned));

        read(s, &tid, 1);
        notify_gdb_of_libraries();
        close(s);
    }

        /* remove our net so we fault for real when we return */
    signal(n, SIG_IGN);
}

void debugger_init()
{
    signal(SIGILL, debugger_signal_handler);
    signal(SIGABRT, debugger_signal_handler);
    signal(SIGBUS, debugger_signal_handler);
    signal(SIGFPE, debugger_signal_handler);
    signal(SIGSEGV, debugger_signal_handler);
    signal(SIGSTKFLT, debugger_signal_handler);
}
