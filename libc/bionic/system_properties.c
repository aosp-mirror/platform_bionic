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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

#include <sys/mman.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <sys/atomics.h>
#include <bionic_atomic_inline.h>

#define ALIGN(x, a) (((x) + (a - 1)) & ~(a - 1))

struct prop_area {
    unsigned bytes_used;
    unsigned volatile serial;
    unsigned magic;
    unsigned version;
    unsigned reserved[28];
    char data[0];
};

typedef struct prop_area prop_area;

struct prop_info {
    unsigned volatile serial;
    char value[PROP_VALUE_MAX];
    char name[0];
};

typedef struct prop_info prop_info;

/*
 * Properties are stored in a hybrid trie/binary tree structure.
 * Each property's name is delimited at '.' characters, and the tokens are put
 * into a trie structure.  Siblings at each level of the trie are stored in a
 * binary tree.  For instance, "ro.secure"="1" could be stored as follows:
 *
 * +-----+   children    +----+   children    +--------+
 * |     |-------------->| ro |-------------->| secure |
 * +-----+               +----+               +--------+
 *                       /    \                /   |
 *                 left /      \ right   left /    |  prop   +===========+
 *                     v        v            v     +-------->| ro.secure |
 *                  +-----+   +-----+     +-----+            +-----------+
 *                  | net |   | sys |     | com |            |     1     |
 *                  +-----+   +-----+     +-----+            +===========+
 */

typedef volatile uint32_t prop_off_t;
struct prop_bt {
    uint8_t namelen;
    uint8_t reserved[3];

    prop_off_t prop;

    prop_off_t left;
    prop_off_t right;

    prop_off_t children;

    char name[0];
};

typedef struct prop_bt prop_bt;

static const char property_service_socket[] = "/dev/socket/" PROP_SERVICE_NAME;
static char property_filename[PATH_MAX] = PROP_FILENAME;
static bool compat_mode = false;

prop_area *__system_property_area__ = NULL;

size_t pa_data_size;
size_t pa_size;

static int get_fd_from_env(void)
{
    char *env = getenv("ANDROID_PROPERTY_WORKSPACE");

    if (!env) {
        return -1;
    }

    return atoi(env);
}

static int map_prop_area_rw()
{
    prop_area *pa;
    int fd;
    int ret;

    /* dev is a tmpfs that we can use to carve a shared workspace
     * out of, so let's do that...
     */
    fd = open(property_filename, O_RDWR | O_CREAT | O_NOFOLLOW | O_CLOEXEC |
            O_EXCL, 0444);
    if (fd < 0) {
        if (errno == EACCES) {
            /* for consistency with the case where the process has already
             * mapped the page in and segfaults when trying to write to it
             */
            abort();
        }
        return -1;
    }

    ret = fcntl(fd, F_SETFD, FD_CLOEXEC);
    if (ret < 0)
        goto out;

    if (ftruncate(fd, PA_SIZE) < 0)
        goto out;

    pa_size = PA_SIZE;
    pa_data_size = pa_size - sizeof(prop_area);
    compat_mode = false;

    pa = mmap(NULL, pa_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(pa == MAP_FAILED)
        goto out;

    memset(pa, 0, pa_size);
    pa->magic = PROP_AREA_MAGIC;
    pa->version = PROP_AREA_VERSION;
    /* reserve root node */
    pa->bytes_used = sizeof(prop_bt);

    /* plug into the lib property services */
    __system_property_area__ = pa;

    close(fd);
    return 0;

out:
    close(fd);
    return -1;
}

int __system_property_set_filename(const char *filename)
{
    size_t len = strlen(filename);
    if (len >= sizeof(property_filename))
        return -1;

    strcpy(property_filename, filename);
    return 0;
}

int __system_property_area_init()
{
    return map_prop_area_rw();
}

static int map_prop_area()
{
    bool fromFile = true;
    int result = -1;
    int fd;
    int ret;

    fd = open(property_filename, O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
    if (fd >= 0) {
        /* For old kernels that don't support O_CLOEXEC */
        ret = fcntl(fd, F_SETFD, FD_CLOEXEC);
        if (ret < 0)
            goto cleanup;
    }

    if ((fd < 0) && (errno == ENOENT)) {
        /*
         * For backwards compatibility, if the file doesn't
         * exist, we use the environment to get the file descriptor.
         * For security reasons, we only use this backup if the kernel
         * returns ENOENT. We don't want to use the backup if the kernel
         * returns other errors such as ENOMEM or ENFILE, since it
         * might be possible for an external program to trigger this
         * condition.
         */
        fd = get_fd_from_env();
        fromFile = false;
    }

    if (fd < 0) {
        return -1;
    }

    struct stat fd_stat;
    if (fstat(fd, &fd_stat) < 0) {
        goto cleanup;
    }

    if ((fd_stat.st_uid != 0)
            || (fd_stat.st_gid != 0)
            || ((fd_stat.st_mode & (S_IWGRP | S_IWOTH)) != 0)
            || (fd_stat.st_size < sizeof(prop_area)) ) {
        goto cleanup;
    }

    pa_size = fd_stat.st_size;
    pa_data_size = pa_size - sizeof(prop_area);
    prop_area *pa = mmap(NULL, pa_size, PROT_READ, MAP_SHARED, fd, 0);

    if (pa == MAP_FAILED) {
        goto cleanup;
    }

    if((pa->magic != PROP_AREA_MAGIC) || (pa->version != PROP_AREA_VERSION &&
                pa->version != PROP_AREA_VERSION_COMPAT)) {
        munmap(pa, pa_size);
        goto cleanup;
    }

    if (pa->version == PROP_AREA_VERSION_COMPAT) {
        compat_mode = true;
    }

    result = 0;

    __system_property_area__ = pa;

cleanup:
    if (fromFile) {
        close(fd);
    }

    return result;
}

int __system_properties_init()
{
    return map_prop_area();
}

static void *new_prop_obj(size_t size, prop_off_t *off)
{
    prop_area *pa = __system_property_area__;
    size = ALIGN(size, sizeof(uint32_t));

    if (pa->bytes_used + size > pa_data_size)
        return NULL;

    *off = pa->bytes_used;
    __system_property_area__->bytes_used += size;
    return __system_property_area__->data + *off;
}

static prop_bt *new_prop_bt(const char *name, uint8_t namelen, prop_off_t *off)
{
    prop_off_t off_tmp;
    prop_bt *bt = new_prop_obj(sizeof(prop_bt) + namelen + 1, &off_tmp);
    if (bt) {
        memcpy(bt->name, name, namelen);
        bt->name[namelen] = '\0';
        bt->namelen = namelen;
        ANDROID_MEMBAR_FULL();
        *off = off_tmp;
    }

    return bt;
}

static prop_info *new_prop_info(const char *name, uint8_t namelen,
        const char *value, uint8_t valuelen, prop_off_t *off)
{
    prop_off_t off_tmp;
    prop_info *info = new_prop_obj(sizeof(prop_info) + namelen + 1, &off_tmp);
    if (info) {
        memcpy(info->name, name, namelen);
        info->name[namelen] = '\0';
        info->serial = (valuelen << 24);
        memcpy(info->value, value, valuelen);
        info->value[valuelen] = '\0';
        ANDROID_MEMBAR_FULL();
        *off = off_tmp;
    }

    return info;
}

static void *to_prop_obj(prop_off_t off)
{
    if (off > pa_data_size)
        return NULL;

    return __system_property_area__->data + off;
}

static prop_bt *root_node()
{
    return to_prop_obj(0);
}

static int cmp_prop_name(const char *one, uint8_t one_len, const char *two,
        uint8_t two_len)
{
    if (one_len < two_len)
        return -1;
    else if (one_len > two_len)
        return 1;
    else
        return strncmp(one, two, one_len);
}

static prop_bt *find_prop_bt(prop_bt *bt, const char *name, uint8_t namelen,
        bool alloc_if_needed)
{
    while (true) {
        int ret;
        if (!bt)
            return bt;
        ret = cmp_prop_name(name, namelen, bt->name, bt->namelen);

        if (ret == 0) {
            return bt;
        } else if (ret < 0) {
            if (bt->left) {
                bt = to_prop_obj(bt->left);
            } else {
                if (!alloc_if_needed)
                   return NULL;

                bt = new_prop_bt(name, namelen, &bt->left);
            }
        } else {
            if (bt->right) {
                bt = to_prop_obj(bt->right);
            } else {
                if (!alloc_if_needed)
                   return NULL;

                bt = new_prop_bt(name, namelen, &bt->right);
            }
        }
    }
}

static const prop_info *find_property(prop_bt *trie, const char *name,
        uint8_t namelen, const char *value, uint8_t valuelen,
        bool alloc_if_needed)
{
    const char *remaining_name = name;

    while (true) {
        char *sep = strchr(remaining_name, '.');
        bool want_subtree = (sep != NULL);
        uint8_t substr_size;

        prop_bt *root;

        if (want_subtree) {
            substr_size = sep - remaining_name;
        } else {
            substr_size = strlen(remaining_name);
        }

        if (!substr_size)
            return NULL;

        if (trie->children) {
            root = to_prop_obj(trie->children);
        } else if (alloc_if_needed) {
            root = new_prop_bt(remaining_name, substr_size, &trie->children);
        } else {
            root = NULL;
        }

        if (!root)
            return NULL;

        trie = find_prop_bt(root, remaining_name, substr_size, alloc_if_needed);
        if (!trie)
            return NULL;

        if (!want_subtree)
            break;

        remaining_name = sep + 1;
    }

    if (trie->prop) {
        return to_prop_obj(trie->prop);
    } else if (alloc_if_needed) {
        return new_prop_info(name, namelen, value, valuelen, &trie->prop);
    } else {
        return NULL;
    }
}

const prop_info *__system_property_find(const char *name)
{
    if (__predict_false(compat_mode)) {
        return __system_property_find_compat(name);
    }
    return find_property(root_node(), name, strlen(name), NULL, 0, false);
}

int __system_property_read(const prop_info *pi, char *name, char *value)
{
    unsigned serial, len;

    if (__predict_false(compat_mode)) {
        return __system_property_read_compat(pi, name, value);
    }

    for(;;) {
        serial = pi->serial;
        while(SERIAL_DIRTY(serial)) {
            __futex_wait((volatile void *)&pi->serial, serial, 0);
            serial = pi->serial;
        }
        len = SERIAL_VALUE_LEN(serial);
        memcpy(value, pi->value, len + 1);
        ANDROID_MEMBAR_FULL();
        if(serial == pi->serial) {
            if(name != 0) {
                strcpy(name, pi->name);
            }
            return len;
        }
    }
}

int __system_property_get(const char *name, char *value)
{
    const prop_info *pi = __system_property_find(name);

    if(pi != 0) {
        return __system_property_read(pi, 0, value);
    } else {
        value[0] = 0;
        return 0;
    }
}


static int send_prop_msg(prop_msg *msg)
{
    struct pollfd pollfds[1];
    struct sockaddr_un addr;
    socklen_t alen;
    size_t namelen;
    int s;
    int r;
    int result = -1;

    s = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(s < 0) {
        return result;
    }

    memset(&addr, 0, sizeof(addr));
    namelen = strlen(property_service_socket);
    strlcpy(addr.sun_path, property_service_socket, sizeof addr.sun_path);
    addr.sun_family = AF_LOCAL;
    alen = namelen + offsetof(struct sockaddr_un, sun_path) + 1;

    if(TEMP_FAILURE_RETRY(connect(s, (struct sockaddr *) &addr, alen)) < 0) {
        close(s);
        return result;
    }

    r = TEMP_FAILURE_RETRY(send(s, msg, sizeof(prop_msg), 0));

    if(r == sizeof(prop_msg)) {
        // We successfully wrote to the property server but now we
        // wait for the property server to finish its work.  It
        // acknowledges its completion by closing the socket so we
        // poll here (on nothing), waiting for the socket to close.
        // If you 'adb shell setprop foo bar' you'll see the POLLHUP
        // once the socket closes.  Out of paranoia we cap our poll
        // at 250 ms.
        pollfds[0].fd = s;
        pollfds[0].events = 0;
        r = TEMP_FAILURE_RETRY(poll(pollfds, 1, 250 /* ms */));
        if (r == 1 && (pollfds[0].revents & POLLHUP) != 0) {
            result = 0;
        } else {
            // Ignore the timeout and treat it like a success anyway.
            // The init process is single-threaded and its property
            // service is sometimes slow to respond (perhaps it's off
            // starting a child process or something) and thus this
            // times out and the caller thinks it failed, even though
            // it's still getting around to it.  So we fake it here,
            // mostly for ctl.* properties, but we do try and wait 250
            // ms so callers who do read-after-write can reliably see
            // what they've written.  Most of the time.
            // TODO: fix the system properties design.
            result = 0;
        }
    }

    close(s);
    return result;
}

int __system_property_set(const char *key, const char *value)
{
    int err;
    prop_msg msg;

    if(key == 0) return -1;
    if(value == 0) value = "";
    if(strlen(key) >= PROP_NAME_MAX) return -1;
    if(strlen(value) >= PROP_VALUE_MAX) return -1;

    memset(&msg, 0, sizeof msg);
    msg.cmd = PROP_MSG_SETPROP;
    strlcpy(msg.name, key, sizeof msg.name);
    strlcpy(msg.value, value, sizeof msg.value);

    err = send_prop_msg(&msg);
    if(err < 0) {
        return err;
    }

    return 0;
}

int __system_property_wait(const prop_info *pi)
{
    unsigned n;
    if(pi == 0) {
        prop_area *pa = __system_property_area__;
        n = pa->serial;
        do {
            __futex_wait(&pa->serial, n, 0);
        } while(n == pa->serial);
    } else {
        n = pi->serial;
        do {
            __futex_wait((volatile void *)&pi->serial, n, 0);
        } while(n == pi->serial);
    }
    return 0;
}

int __system_property_update(prop_info *pi, const char *value, unsigned int len)
{
    prop_area *pa = __system_property_area__;

    if (len >= PROP_VALUE_MAX)
        return -1;

    pi->serial = pi->serial | 1;
    ANDROID_MEMBAR_FULL();
    memcpy(pi->value, value, len + 1);
    ANDROID_MEMBAR_FULL();
    pi->serial = (len << 24) | ((pi->serial + 1) & 0xffffff);
    __futex_wake(&pi->serial, INT32_MAX);

    pa->serial++;
    __futex_wake(&pa->serial, INT32_MAX);

    return 0;
}

int __system_property_add(const char *name, unsigned int namelen,
            const char *value, unsigned int valuelen)
{
    prop_area *pa = __system_property_area__;
    const prop_info *pi;

    if (namelen >= PROP_NAME_MAX)
        return -1;
    if (valuelen >= PROP_VALUE_MAX)
        return -1;
    if (namelen < 1)
        return -1;

    pi = find_property(root_node(), name, namelen, value, valuelen, true);
    if (!pi)
        return -1;

    pa->serial++;
    __futex_wake(&pa->serial, INT32_MAX);
    return 0;
}

unsigned int __system_property_serial(const prop_info *pi)
{
    return pi->serial;
}

unsigned int __system_property_wait_any(unsigned int serial)
{
    prop_area *pa = __system_property_area__;

    do {
        __futex_wait(&pa->serial, serial, 0);
    } while(pa->serial == serial);

    return pa->serial;
}

struct find_nth_cookie {
    unsigned count;
    unsigned n;
    const prop_info *pi;
};

static void find_nth_fn(const prop_info *pi, void *ptr)
{
    struct find_nth_cookie *cookie = ptr;

    if (cookie->n == cookie->count)
        cookie->pi = pi;

    cookie->count++;
}

const prop_info *__system_property_find_nth(unsigned n)
{
    struct find_nth_cookie cookie;
    int err;

    memset(&cookie, 0, sizeof(cookie));
    cookie.n = n;

    err = __system_property_foreach(find_nth_fn, &cookie);
    if (err < 0)
        return NULL;

    return cookie.pi;
}

static int foreach_property(prop_off_t off,
        void (*propfn)(const prop_info *pi, void *cookie), void *cookie)
{
    prop_bt *trie = to_prop_obj(off);
    if (!trie)
        return -1;

    if (trie->left) {
        int err = foreach_property(trie->left, propfn, cookie);
        if (err < 0)
            return -1;
    }
    if (trie->prop) {
        prop_info *info = to_prop_obj(trie->prop);
        if (!info)
            return -1;
        propfn(info, cookie);
    }
    if (trie->children) {
        int err = foreach_property(trie->children, propfn, cookie);
        if (err < 0)
            return -1;
    }
    if (trie->right) {
        int err = foreach_property(trie->right, propfn, cookie);
        if (err < 0)
            return -1;
    }

    return 0;
}

int __system_property_foreach(void (*propfn)(const prop_info *pi, void *cookie),
        void *cookie)
{
    if (__predict_false(compat_mode)) {
        return __system_property_foreach_compat(propfn, cookie);
	}
    return foreach_property(0, propfn, cookie);
}
