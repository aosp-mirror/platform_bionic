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
#include <new>
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
#include <sys/system_properties.h>

#include <sys/atomics.h>

#include "private/bionic_atomic_inline.h"
#include "private/bionic_macros.h"

#define ALIGN(x, a) (((x) + (a - 1)) & ~(a - 1))


static const char property_service_socket[] = "/dev/socket/" PROP_SERVICE_NAME;


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

// Represents a node in the trie.
struct prop_bt {
    uint8_t namelen;
    uint8_t reserved[3];

    volatile uint32_t prop;

    volatile uint32_t left;
    volatile uint32_t right;

    volatile uint32_t children;

    char name[0];

    prop_bt(const char *name, const uint8_t name_length) {
        this->namelen = name_length;
        memcpy(this->name, name, name_length);
        this->name[name_length] = '\0';
        ANDROID_MEMBAR_FULL();
    }

private:
    DISALLOW_COPY_AND_ASSIGN(prop_bt);
};

struct prop_area {
    uint32_t bytes_used;
    volatile uint32_t serial;
    uint32_t magic;
    uint32_t version;
    uint32_t reserved[28];
    char data[0];

    prop_area(const uint32_t magic, const uint32_t version) :
        serial(0), magic(magic), version(version) {
        memset(reserved, 0, sizeof(reserved));
        // Allocate enough space for the root node.
        bytes_used = sizeof(prop_bt);
    }

private:
    DISALLOW_COPY_AND_ASSIGN(prop_area);
};

struct prop_info {
    volatile uint32_t serial;
    char value[PROP_VALUE_MAX];
    char name[0];

    prop_info(const char *name, const uint8_t namelen, const char *value,
              const uint8_t valuelen) {
        memcpy(this->name, name, namelen);
        this->name[namelen] = '\0';
        this->serial = (valuelen << 24);
        memcpy(this->value, value, valuelen);
        this->value[valuelen] = '\0';
        ANDROID_MEMBAR_FULL();
    }
private:
    DISALLOW_COPY_AND_ASSIGN(prop_info);
};

struct find_nth_cookie {
    uint32_t count;
    const uint32_t n;
    const prop_info *pi;

    find_nth_cookie(uint32_t n) : count(0), n(n), pi(NULL) {
    }
};

static char property_filename[PATH_MAX] = PROP_FILENAME;
static bool compat_mode = false;
static size_t pa_data_size;
static size_t pa_size;

// NOTE: This isn't static because system_properties_compat.c
// requires it.
prop_area *__system_property_area__ = NULL;

static int get_fd_from_env(void)
{
    // This environment variable consistes of two decimal integer
    // values separated by a ",". The first value is a file descriptor
    // and the second is the size of the system properties area. The
    // size is currently unused.
    char *env = getenv("ANDROID_PROPERTY_WORKSPACE");

    if (!env) {
        return -1;
    }

    return atoi(env);
}

static int map_prop_area_rw()
{
    /* dev is a tmpfs that we can use to carve a shared workspace
     * out of, so let's do that...
     */
    const int fd = open(property_filename,
                        O_RDWR | O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_EXCL, 0444);

    if (fd < 0) {
        if (errno == EACCES) {
            /* for consistency with the case where the process has already
             * mapped the page in and segfaults when trying to write to it
             */
            abort();
        }
        return -1;
    }

    // TODO: Is this really required ? Does android run on any kernels that
    // don't support O_CLOEXEC ?
    const int ret = fcntl(fd, F_SETFD, FD_CLOEXEC);
    if (ret < 0) {
        close(fd);
        return -1;
    }

    if (ftruncate(fd, PA_SIZE) < 0) {
        close(fd);
        return -1;
    }

    pa_size = PA_SIZE;
    pa_data_size = pa_size - sizeof(prop_area);
    compat_mode = false;

    void *const memory_area = mmap(NULL, pa_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory_area == MAP_FAILED) {
        close(fd);
        return -1;
    }

    prop_area *pa = new(memory_area) prop_area(PROP_AREA_MAGIC, PROP_AREA_VERSION);

    /* plug into the lib property services */
    __system_property_area__ = pa;

    close(fd);
    return 0;
}

static int map_fd_ro(const int fd) {
    struct stat fd_stat;
    if (fstat(fd, &fd_stat) < 0) {
        return -1;
    }

    if ((fd_stat.st_uid != 0)
            || (fd_stat.st_gid != 0)
            || ((fd_stat.st_mode & (S_IWGRP | S_IWOTH)) != 0)
            || (fd_stat.st_size < static_cast<off_t>(sizeof(prop_area))) ) {
        return -1;
    }

    pa_size = fd_stat.st_size;
    pa_data_size = pa_size - sizeof(prop_area);

    void* const map_result = mmap(NULL, pa_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map_result == MAP_FAILED) {
        return -1;
    }

    prop_area* pa = reinterpret_cast<prop_area*>(map_result);
    if ((pa->magic != PROP_AREA_MAGIC) || (pa->version != PROP_AREA_VERSION &&
                pa->version != PROP_AREA_VERSION_COMPAT)) {
        munmap(pa, pa_size);
        return -1;
    }

    if (pa->version == PROP_AREA_VERSION_COMPAT) {
        compat_mode = true;
    }

    __system_property_area__ = pa;
    return 0;
}

static int map_prop_area()
{
    int fd(open(property_filename, O_RDONLY | O_NOFOLLOW | O_CLOEXEC));
    if (fd >= 0) {
        /* For old kernels that don't support O_CLOEXEC */
        const int ret = fcntl(fd, F_SETFD, FD_CLOEXEC);
        if (ret < 0) {
            close(fd);
            return -1;
        }
    }

    bool close_fd = true;
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
        close_fd = false;
    }

    if (fd < 0) {
        return -1;
    }

    const int map_result = map_fd_ro(fd);
    if (close_fd) {
        close(fd);
    }

    return map_result;
}

static void *allocate_obj(const size_t size, uint32_t *const off)
{
    prop_area *pa = __system_property_area__;
    const size_t aligned = ALIGN(size, sizeof(uint32_t));
    if (pa->bytes_used + aligned > pa_data_size) {
        return NULL;
    }

    *off = pa->bytes_used;
    pa->bytes_used += aligned;
    return pa->data + *off;
}

static prop_bt *new_prop_bt(const char *name, uint8_t namelen, uint32_t *const off)
{
    uint32_t new_offset;
    void *const offset = allocate_obj(sizeof(prop_bt) + namelen + 1, &new_offset);
    if (offset) {
        prop_bt* bt = new(offset) prop_bt(name, namelen);
        *off = new_offset;
        return bt;
    }

    return NULL;
}

static prop_info *new_prop_info(const char *name, uint8_t namelen,
        const char *value, uint8_t valuelen, uint32_t *const off)
{
    uint32_t off_tmp;
    void* const offset = allocate_obj(sizeof(prop_info) + namelen + 1, &off_tmp);
    if (offset) {
        prop_info* info = new(offset) prop_info(name, namelen, value, valuelen);
        *off = off_tmp;
        return info;
    }

    return NULL;
}

static void *to_prop_obj(const uint32_t off)
{
    if (off > pa_data_size)
        return NULL;
    if (!__system_property_area__)
        return NULL;

    return (__system_property_area__->data + off);
}

static prop_bt *root_node()
{
    return reinterpret_cast<prop_bt*>(to_prop_obj(0));
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

static prop_bt *find_prop_bt(prop_bt *const bt, const char *name,
                             uint8_t namelen, bool alloc_if_needed)
{

    prop_bt* current = bt;
    while (true) {
        if (!current) {
            return NULL;
        }

        const int ret = cmp_prop_name(name, namelen, current->name, current->namelen);
        if (ret == 0) {
            return current;
        }

        if (ret < 0) {
            if (current->left) {
                current = reinterpret_cast<prop_bt*>(to_prop_obj(current->left));
            } else {
                if (!alloc_if_needed) {
                   return NULL;
                }

                // Note that there isn't a race condition here. "clients" never
                // reach this code-path since It's only the (single threaded) server
                // that allocates new nodes. Though "bt->left" is volatile, it can't
                // have changed since the last value was last read.
                uint32_t new_offset = 0;
                prop_bt* new_bt = new_prop_bt(name, namelen, &new_offset);
                if (new_bt) {
                    current->left = new_offset;
                }
                return new_bt;
            }
        } else {
            if (current->right) {
                current = reinterpret_cast<prop_bt*>(to_prop_obj(current->right));
            } else {
                if (!alloc_if_needed) {
                   return NULL;
                }

                uint32_t new_offset;
                prop_bt* new_bt = new_prop_bt(name, namelen, &new_offset);
                if (new_bt) {
                    current->right = new_offset;
                }
                return new_bt;
            }
        }
    }
}

static const prop_info *find_property(prop_bt *const trie, const char *name,
        uint8_t namelen, const char *value, uint8_t valuelen,
        bool alloc_if_needed)
{
    if (!trie) return NULL;

    const char *remaining_name = name;
    prop_bt* current = trie;
    while (true) {
        const char *sep = strchr(remaining_name, '.');
        const bool want_subtree = (sep != NULL);
        const uint8_t substr_size = (want_subtree) ?
            sep - remaining_name : strlen(remaining_name);

        if (!substr_size) {
            return NULL;
        }

        prop_bt* root = NULL;
        if (current->children) {
            root = reinterpret_cast<prop_bt*>(to_prop_obj(current->children));
        } else if (alloc_if_needed) {
            uint32_t new_bt_offset;
            root = new_prop_bt(remaining_name, substr_size, &new_bt_offset);
            if (root) {
                current->children = new_bt_offset;
            }
        }

        if (!root) {
            return NULL;
        }

        current = find_prop_bt(root, remaining_name, substr_size, alloc_if_needed);
        if (!current) {
            return NULL;
        }

        if (!want_subtree)
            break;

        remaining_name = sep + 1;
    }

    if (current->prop) {
        return reinterpret_cast<prop_info*>(to_prop_obj(current->prop));
    } else if (alloc_if_needed) {
        uint32_t new_info_offset;
        prop_info* new_info = new_prop_info(name, namelen, value, valuelen, &new_info_offset);
        if (new_info) {
            current->prop = new_info_offset;
        }

        return new_info;
    } else {
        return NULL;
    }
}

static int send_prop_msg(const prop_msg *msg)
{
    const int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (fd < 0) {
        return -1;
    }

    const size_t namelen = strlen(property_service_socket);

    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    strlcpy(addr.sun_path, property_service_socket, sizeof(addr.sun_path));
    addr.sun_family = AF_LOCAL;
    socklen_t alen = namelen + offsetof(sockaddr_un, sun_path) + 1;
    if (TEMP_FAILURE_RETRY(connect(fd, reinterpret_cast<sockaddr*>(&addr), alen)) < 0) {
        close(fd);
        return -1;
    }

    const int num_bytes = TEMP_FAILURE_RETRY(send(fd, msg, sizeof(prop_msg), 0));

    int result = -1;
    if (num_bytes == sizeof(prop_msg)) {
        // We successfully wrote to the property server but now we
        // wait for the property server to finish its work.  It
        // acknowledges its completion by closing the socket so we
        // poll here (on nothing), waiting for the socket to close.
        // If you 'adb shell setprop foo bar' you'll see the POLLHUP
        // once the socket closes.  Out of paranoia we cap our poll
        // at 250 ms.
        pollfd pollfds[1];
        pollfds[0].fd = fd;
        pollfds[0].events = 0;
        const int poll_result = TEMP_FAILURE_RETRY(poll(pollfds, 1, 250 /* ms */));
        if (poll_result == 1 && (pollfds[0].revents & POLLHUP) != 0) {
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

    close(fd);
    return result;
}

static void find_nth_fn(const prop_info *pi, void *ptr)
{
    find_nth_cookie *cookie = reinterpret_cast<find_nth_cookie*>(ptr);

    if (cookie->n == cookie->count)
        cookie->pi = pi;

    cookie->count++;
}

static int foreach_property(const uint32_t off,
        void (*propfn)(const prop_info *pi, void *cookie), void *cookie)
{
    prop_bt *trie = reinterpret_cast<prop_bt*>(to_prop_obj(off));
    if (!trie)
        return -1;

    if (trie->left) {
        const int err = foreach_property(trie->left, propfn, cookie);
        if (err < 0)
            return -1;
    }
    if (trie->prop) {
        prop_info *info = reinterpret_cast<prop_info*>(to_prop_obj(trie->prop));
        if (!info)
            return -1;
        propfn(info, cookie);
    }
    if (trie->children) {
        const int err = foreach_property(trie->children, propfn, cookie);
        if (err < 0)
            return -1;
    }
    if (trie->right) {
        const int err = foreach_property(trie->right, propfn, cookie);
        if (err < 0)
            return -1;
    }

    return 0;
}

int __system_properties_init()
{
    return map_prop_area();
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

const prop_info *__system_property_find(const char *name)
{
    if (__predict_false(compat_mode)) {
        return __system_property_find_compat(name);
    }
    return find_property(root_node(), name, strlen(name), NULL, 0, false);
}

int __system_property_read(const prop_info *pi, char *name, char *value)
{
    if (__predict_false(compat_mode)) {
        return __system_property_read_compat(pi, name, value);
    }

    while (true) {
        uint32_t serial = __system_property_serial(pi);
        size_t len = SERIAL_VALUE_LEN(serial);
        memcpy(value, pi->value, len + 1);
        ANDROID_MEMBAR_FULL();
        if (serial == pi->serial) {
            if (name != 0) {
                strcpy(name, pi->name);
            }
            return len;
        }
    }
}

int __system_property_get(const char *name, char *value)
{
    const prop_info *pi = __system_property_find(name);

    if (pi != 0) {
        return __system_property_read(pi, 0, value);
    } else {
        value[0] = 0;
        return 0;
    }
}

int __system_property_set(const char *key, const char *value)
{
    if (key == 0) return -1;
    if (value == 0) value = "";
    if (strlen(key) >= PROP_NAME_MAX) return -1;
    if (strlen(value) >= PROP_VALUE_MAX) return -1;

    prop_msg msg;
    memset(&msg, 0, sizeof msg);
    msg.cmd = PROP_MSG_SETPROP;
    strlcpy(msg.name, key, sizeof msg.name);
    strlcpy(msg.value, value, sizeof msg.value);

    const int err = send_prop_msg(&msg);
    if (err < 0) {
        return err;
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
    uint32_t serial = pi->serial;
    while (SERIAL_DIRTY(serial)) {
        __futex_wait(const_cast<volatile uint32_t*>(&pi->serial), serial, NULL);
        serial = pi->serial;
    }
    return serial;
}

unsigned int __system_property_wait_any(unsigned int serial)
{
    prop_area *pa = __system_property_area__;

    do {
        __futex_wait(&pa->serial, serial, NULL);
    } while (pa->serial == serial);

    return pa->serial;
}

const prop_info *__system_property_find_nth(unsigned n)
{
    find_nth_cookie cookie(n);

    const int err = __system_property_foreach(find_nth_fn, &cookie);
    if (err < 0) {
        return NULL;
    }

    return cookie.pi;
}

int __system_property_foreach(void (*propfn)(const prop_info *pi, void *cookie),
        void *cookie)
{
    if (__predict_false(compat_mode)) {
        return __system_property_foreach_compat(propfn, cookie);
    }

    return foreach_property(0, propfn, cookie);
}
