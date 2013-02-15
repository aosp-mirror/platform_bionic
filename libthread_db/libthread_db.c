/*
 * Copyright 2006 The Android Open Source Project
 */

#include <dirent.h>
#include <sys/ptrace.h>
#include <stdint.h>
#include <thread_db.h>
#include <stdlib.h>
#include <stdio.h>

extern int ps_pglobal_lookup (void *, const char *obj, const char *name, void **sym_addr);
extern pid_t ps_getpid(struct ps_prochandle *ph);

/*
 * This is the list of "special" symbols we care about whose addresses are
 * cached by gdbserver from the host at init time.
 */
enum {
    SYM_TD_CREATE,
    SYM_THREAD_LIST,
    NUM_SYMS
};

static char const * gSymbols[] = {
    [SYM_TD_CREATE] = "_thread_created_hook",
    NULL
};


char const **
td_symbol_list(void)
{
    return gSymbols;
}


td_err_e
td_ta_new(struct ps_prochandle * proc_handle, td_thragent_t ** agent_out)
{
    td_thragent_t * agent;

    agent = (td_thragent_t *)malloc(sizeof(td_thragent_t));
    if (!agent) {
        return TD_MALLOC;
    }

    agent->pid = ps_getpid(proc_handle);
    agent->ph = proc_handle;
    *agent_out = agent;

    return TD_OK;
}


td_err_e
td_ta_delete(td_thragent_t * ta)
{
    free(ta);
    // FIXME: anything else to do?
    return TD_OK;
}


/* NOTE: not used by gdb 7.0 */

td_err_e
td_ta_set_event(td_thragent_t const * agent, td_thr_events_t * events)
{
    return TD_OK;
}


/* NOTE: not used by gdb 7.0 */
static td_thrhandle_t gEventMsgHandle;

/* NOTE: not used by gdb 7.0 */

static int
_event_getmsg_helper(td_thrhandle_t const * handle, void * bkpt_addr)
{
    void * pc;

#ifdef __i386__
    /* Get the eip from offset 12*4 = 48 as defined in the struct
     * user_regs_struct in user_32.h
     */
    pc = (void *)ptrace(PTRACE_PEEKUSR, handle->tid, (void *)48 /* eip */, NULL);
    /* FIXME - pc is a non-decremented breakpoint address, hence the
     * addition of 1 on test.  This seems to work for the thread hook
     * function in libc.so but should be properly fixed.
     */
    if (pc == ((int)bkpt_addr + 1)) {
        /* The hook function takes the id of the new thread as it's first
         * param, so grab it from ecx at offset 4 in struct user_regs_struct
         * (using fastcall convention for x86)
         */
        gEventMsgHandle.pid = ptrace(PTRACE_PEEKUSR, handle->tid, (void *)4 /* ecx */, NULL);
        gEventMsgHandle.tid = gEventMsgHandle.pid;
        return 0x42;
    }
#elif defined(__arm__)
    pc = (void *)ptrace(PTRACE_PEEKUSR, handle->tid, (void *)60 /* r15/pc */, NULL);

    if (pc == bkpt_addr) {
        // The hook function takes the id of the new thread as it's first param,
        // so grab it from r0.
        gEventMsgHandle.pid = ptrace(PTRACE_PEEKUSR, handle->tid, (void *)0 /* r0 */, NULL);
        gEventMsgHandle.tid = gEventMsgHandle.pid;
        return 0x42;
    }
#elif defined(__mips__)
    pc = (void *)ptrace(PTRACE_PEEKUSR, handle->tid, (void *)(64*4) /* pc */, NULL);
    if (pc == bkpt_addr) {
        // The hook function takes the id of the new thread as it's first param,
        // so grab it from a0
        gEventMsgHandle.pid = ptrace(PTRACE_PEEKUSR, handle->tid, (void *)(4*4) /* a0 */, NULL);
        gEventMsgHandle.tid = gEventMsgHandle.pid;
        return 0x42;
    }
#endif
    return 0;
}

/* NOTE: not used by gdb 7.0 */

td_err_e
td_ta_event_getmsg(td_thragent_t const * agent, td_event_msg_t * event)
{
    td_err_e err;
    void * bkpt_addr;

    err = ps_pglobal_lookup(NULL, NULL, gSymbols[SYM_TD_CREATE], &bkpt_addr);
    if (err) {
        return err;
    }

    err = td_ta_thr_iter(agent, _event_getmsg_helper, bkpt_addr, 0, 0, NULL, 0);
    if (err != 0x42) {
        return TD_NOMSG;
    }

    event->event = TD_CREATE;
    event->th_p = &gEventMsgHandle; // Nasty hack, but it's the only way!

    return TD_OK;
}


td_err_e
td_ta_map_lwp2thr(td_thragent_t const * agent, lwpid_t lwpid,
		  td_thrhandle_t *th)
{
    th->pid = ps_getpid(agent->ph);
    th->tid = lwpid;
    return TD_OK;
}


td_err_e
td_thr_get_info(td_thrhandle_t const * handle, td_thrinfo_t * info)
{
    info->ti_tid = handle->tid;
    info->ti_lid = handle->tid;
    info->ti_state = TD_THR_SLEEP; /* XXX this needs to be read from /proc/<pid>/task/<tid>.
                                      This is only used to see if the thread is a zombie or not */
    return TD_OK;
}


/* NOTE: not used by gdb 7.0 */

td_err_e
td_thr_event_enable(td_thrhandle_t const * handle, td_event_e event)
{
    // I don't think we need to do anything here...
    return TD_OK;
}


/* NOTE: not used by gdb 7.0 */

td_err_e
td_ta_event_addr(td_thragent_t const * agent, td_event_e event, td_notify_t * notify_out)
{
    int32_t err;

    /*
     * This is nasty, ps_pglobal_lookup is implemented in gdbserver and looks up
     * the symbol from it's cache, which is populated at start time with the
     * symbols returned from td_symbol_list via calls back to the host.
     */

    switch (event) {
        case TD_CREATE:
            err = ps_pglobal_lookup(NULL, NULL, gSymbols[SYM_TD_CREATE], &notify_out->u.bptaddr);
            if (err) {
                return TD_NOEVENT;
            }
            return TD_OK;
    }
    return TD_NOEVENT;
}


td_err_e
td_ta_clear_event(const td_thragent_t * ta_arg, td_thr_events_t * event)
{
    /* Given that gdb 7.0 doesn't use thread events,
       there's nothing we need to do here.  */
    return TD_OK;
}


td_err_e
td_ta_thr_iter(td_thragent_t const * agent, td_thr_iter_f * func, void * cookie,
               td_thr_state_e state, int32_t prio, sigset_t * sigmask, uint32_t user_flags)
{
    td_err_e err = TD_OK;
    char path[32];
    DIR * dir;
    struct dirent * entry;
    td_thrhandle_t handle;

    snprintf(path, sizeof(path), "/proc/%d/task/", agent->pid);
    dir = opendir(path);
    if (!dir) {
        return TD_NOEVENT;
    }

    handle.pid = agent->pid;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        handle.tid = atoi(entry->d_name);
        if (func(&handle, cookie) != 0) {
	    err = TD_DBERR;
            break;
        }
    }

    closedir(dir);

    return err;
}

td_err_e
td_thr_tls_get_addr(const td_thrhandle_t * th,
		    psaddr_t map_address, size_t offset, psaddr_t * address)
{
    return TD_NOAPLIC; // FIXME: TODO
}
