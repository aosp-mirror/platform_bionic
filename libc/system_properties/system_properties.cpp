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

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <unistd.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/system_properties.h>

#include <async_safe/log.h>

#include "private/ErrnoRestorer.h"
#include "private/bionic_defs.h"
#include "private/bionic_futex.h"
#include "private/bionic_macros.h"
#include "private/bionic_sdk_version.h"

#include "context_node.h"
#include "contexts.h"
#include "contexts_pre_split.h"
#include "contexts_split.h"
#include "prop_area.h"
#include "prop_info.h"
#include "property_filename.h"

// We don't want to use new or malloc in properties (b/31659220), and since these classes are
// small enough and don't have non-trivial constructors, it's easier to just statically declare
// them than anything else.
static ContextsSplit contexts_split;
static ContextsPreSplit contexts_pre_split;
static Contexts* contexts = nullptr;

#define SERIAL_DIRTY(serial) ((serial)&1)
#define SERIAL_VALUE_LEN(serial) ((serial) >> 24)

static const char property_service_socket[] = "/dev/socket/" PROP_SERVICE_NAME;
static const char* kServiceVersionPropertyName = "ro.property_service.version";

// This is public because it was exposed in the NDK. As of 2017-01, ~60 apps reference this symbol.
// It is set to nullptr and never modified.
__BIONIC_WEAK_VARIABLE_FOR_NATIVE_BRIDGE
prop_area* __system_property_area__ = nullptr;

char property_filename[PROP_FILENAME_MAX] = PROP_FILENAME;

class PropertyServiceConnection {
 public:
  PropertyServiceConnection() : last_error_(0) {
    socket_ = ::socket(AF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (socket_ == -1) {
      last_error_ = errno;
      return;
    }

    const size_t namelen = strlen(property_service_socket);
    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    strlcpy(addr.sun_path, property_service_socket, sizeof(addr.sun_path));
    addr.sun_family = AF_LOCAL;
    socklen_t alen = namelen + offsetof(sockaddr_un, sun_path) + 1;

    if (TEMP_FAILURE_RETRY(connect(socket_, reinterpret_cast<sockaddr*>(&addr), alen)) == -1) {
      last_error_ = errno;
      close(socket_);
      socket_ = -1;
    }
  }

  bool IsValid() {
    return socket_ != -1;
  }

  int GetLastError() {
    return last_error_;
  }

  bool RecvInt32(int32_t* value) {
    int result = TEMP_FAILURE_RETRY(recv(socket_, value, sizeof(*value), MSG_WAITALL));
    return CheckSendRecvResult(result, sizeof(*value));
  }

  int socket() {
    return socket_;
  }

  ~PropertyServiceConnection() {
    if (socket_ != -1) {
      close(socket_);
    }
  }

 private:
  bool CheckSendRecvResult(int result, int expected_len) {
    if (result == -1) {
      last_error_ = errno;
    } else if (result != expected_len) {
      last_error_ = -1;
    } else {
      last_error_ = 0;
    }

    return last_error_ == 0;
  }

  int socket_;
  int last_error_;

  friend class SocketWriter;
};

class SocketWriter {
 public:
  explicit SocketWriter(PropertyServiceConnection* connection)
      : connection_(connection), iov_index_(0), uint_buf_index_(0) {
  }

  SocketWriter& WriteUint32(uint32_t value) {
    CHECK(uint_buf_index_ < kUintBufSize);
    CHECK(iov_index_ < kIovSize);
    uint32_t* ptr = uint_buf_ + uint_buf_index_;
    uint_buf_[uint_buf_index_++] = value;
    iov_[iov_index_].iov_base = ptr;
    iov_[iov_index_].iov_len = sizeof(*ptr);
    ++iov_index_;
    return *this;
  }

  SocketWriter& WriteString(const char* value) {
    uint32_t valuelen = strlen(value);
    WriteUint32(valuelen);
    if (valuelen == 0) {
      return *this;
    }

    CHECK(iov_index_ < kIovSize);
    iov_[iov_index_].iov_base = const_cast<char*>(value);
    iov_[iov_index_].iov_len = valuelen;
    ++iov_index_;

    return *this;
  }

  bool Send() {
    if (!connection_->IsValid()) {
      return false;
    }

    if (writev(connection_->socket(), iov_, iov_index_) == -1) {
      connection_->last_error_ = errno;
      return false;
    }

    iov_index_ = uint_buf_index_ = 0;
    return true;
  }

 private:
  static constexpr size_t kUintBufSize = 8;
  static constexpr size_t kIovSize = 8;

  PropertyServiceConnection* connection_;
  iovec iov_[kIovSize];
  size_t iov_index_;
  uint32_t uint_buf_[kUintBufSize];
  size_t uint_buf_index_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(SocketWriter);
};

struct prop_msg {
  unsigned cmd;
  char name[PROP_NAME_MAX];
  char value[PROP_VALUE_MAX];
};

static int send_prop_msg(const prop_msg* msg) {
  PropertyServiceConnection connection;
  if (!connection.IsValid()) {
    return connection.GetLastError();
  }

  int result = -1;
  int s = connection.socket();

  const int num_bytes = TEMP_FAILURE_RETRY(send(s, msg, sizeof(prop_msg), 0));
  if (num_bytes == sizeof(prop_msg)) {
    // We successfully wrote to the property server but now we
    // wait for the property server to finish its work.  It
    // acknowledges its completion by closing the socket so we
    // poll here (on nothing), waiting for the socket to close.
    // If you 'adb shell setprop foo bar' you'll see the POLLHUP
    // once the socket closes.  Out of paranoia we cap our poll
    // at 250 ms.
    pollfd pollfds[1];
    pollfds[0].fd = s;
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
      async_safe_format_log(ANDROID_LOG_WARN, "libc",
                            "Property service has timed out while trying to set \"%s\" to \"%s\"",
                            msg->name, msg->value);
      result = 0;
    }
  }

  return result;
}

static bool is_dir(const char* pathname) {
  struct stat info;
  if (stat(pathname, &info) == -1) {
    return false;
  }
  return S_ISDIR(info.st_mode);
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int __system_properties_init() {
  // This is called from __libc_init_common, and should leave errno at 0 (http://b/37248982).
  ErrnoRestorer errno_restorer;

  if (contexts != nullptr) {
    contexts->ResetAccess();
    return 0;
  }
  contexts = nullptr;
  if (is_dir(property_filename)) {
    if (!contexts_split.Initialize(false)) {
      return -1;
    }
    contexts = &contexts_split;
  } else {
    if (!contexts_pre_split.Initialize(false)) {
      return -1;
    }
    contexts = &contexts_pre_split;
  }
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int __system_property_set_filename(const char* filename) {
  size_t len = strlen(filename);
  if (len >= sizeof(property_filename)) return -1;

  strcpy(property_filename, filename);
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int __system_property_area_init() {
  if (contexts != nullptr) {
    contexts->FreeAndUnmap();
  }
  // We set this unconditionally as we want tests to continue on regardless of if this failed
  // and property_service will abort on an error condition, so no harm done.
  contexts = &contexts_split;
  if (!contexts_split.Initialize(true)) {
    return -1;
  }
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
uint32_t __system_property_area_serial() {
  if (contexts == nullptr) {
    return -1;
  }

  prop_area* pa = contexts->GetSerialPropArea();
  if (!pa) {
    return -1;
  }

  // Make sure this read fulfilled before __system_property_serial
  return atomic_load_explicit(pa->serial(), memory_order_acquire);
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
const prop_info* __system_property_find(const char* name) {
  if (contexts == nullptr) {
    return nullptr;
  }

  prop_area* pa = contexts->GetPropAreaForName(name);
  if (!pa) {
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", "Access denied finding property \"%s\"", name);
    return nullptr;
  }

  return pa->find(name);
}

static bool is_read_only(const char* name) {
  return strncmp(name, "ro.", 3) == 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int __system_property_read(const prop_info* pi, char* name, char* value) {
  while (true) {
    uint32_t serial = __system_property_serial(pi);  // acquire semantics
    size_t len = SERIAL_VALUE_LEN(serial);
    memcpy(value, pi->value, len + 1);
    // TODO: Fix the synchronization scheme here.
    // There is no fully supported way to implement this kind
    // of synchronization in C++11, since the memcpy races with
    // updates to pi, and the data being accessed is not atomic.
    // The following fence is unintuitive, but would be the
    // correct one if memcpy used memory_order_relaxed atomic accesses.
    // In practice it seems unlikely that the generated code would
    // would be any different, so this should be OK.
    atomic_thread_fence(memory_order_acquire);
    if (serial == load_const_atomic(&(pi->serial), memory_order_relaxed)) {
      if (name != nullptr) {
        size_t namelen = strlcpy(name, pi->name, PROP_NAME_MAX);
        if (namelen >= PROP_NAME_MAX) {
          async_safe_format_log(ANDROID_LOG_ERROR, "libc",
                                "The property name length for \"%s\" is >= %d;"
                                " please use __system_property_read_callback"
                                " to read this property. (the name is truncated to \"%s\")",
                                pi->name, PROP_NAME_MAX - 1, name);
        }
      }
      if (is_read_only(pi->name) && pi->is_long()) {
        async_safe_format_log(
            ANDROID_LOG_ERROR, "libc",
            "The property \"%s\" has a value with length %zu that is too large for"
            " __system_property_get()/__system_property_read(); use"
            " __system_property_read_callback() instead.",
            pi->name, strlen(pi->long_value()));
      }
      return len;
    }
  }
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void __system_property_read_callback(const prop_info* pi,
                                     void (*callback)(void* cookie, const char* name,
                                                      const char* value, uint32_t serial),
                                     void* cookie) {
  // Read only properties don't need to copy the value to a temporary buffer, since it can never
  // change.
  if (is_read_only(pi->name)) {
    uint32_t serial = __system_property_serial(pi);
    if (pi->is_long()) {
      callback(cookie, pi->name, pi->long_value(), serial);
    } else {
      callback(cookie, pi->name, pi->value, serial);
    }
    return;
  }

  while (true) {
    uint32_t serial = __system_property_serial(pi);  // acquire semantics
    size_t len = SERIAL_VALUE_LEN(serial);
    char value_buf[len + 1];

    memcpy(value_buf, pi->value, len);
    value_buf[len] = '\0';

    // TODO: see todo in __system_property_read function
    atomic_thread_fence(memory_order_acquire);
    if (serial == load_const_atomic(&(pi->serial), memory_order_relaxed)) {
      callback(cookie, pi->name, value_buf, serial);
      return;
    }
  }
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int __system_property_get(const char* name, char* value) {
  const prop_info* pi = __system_property_find(name);

  if (pi != 0) {
    return __system_property_read(pi, nullptr, value);
  } else {
    value[0] = 0;
    return 0;
  }
}

static constexpr uint32_t kProtocolVersion1 = 1;
static constexpr uint32_t kProtocolVersion2 = 2;  // current

static atomic_uint_least32_t g_propservice_protocol_version = 0;

static void detect_protocol_version() {
  char value[PROP_VALUE_MAX];
  if (__system_property_get(kServiceVersionPropertyName, value) == 0) {
    g_propservice_protocol_version = kProtocolVersion1;
    async_safe_format_log(ANDROID_LOG_WARN, "libc",
                          "Using old property service protocol (\"%s\" is not set)",
                          kServiceVersionPropertyName);
  } else {
    uint32_t version = static_cast<uint32_t>(atoll(value));
    if (version >= kProtocolVersion2) {
      g_propservice_protocol_version = kProtocolVersion2;
    } else {
      async_safe_format_log(ANDROID_LOG_WARN, "libc",
                            "Using old property service protocol (\"%s\"=\"%s\")",
                            kServiceVersionPropertyName, value);
      g_propservice_protocol_version = kProtocolVersion1;
    }
  }
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int __system_property_set(const char* key, const char* value) {
  if (key == nullptr) return -1;
  if (value == nullptr) value = "";

  if (g_propservice_protocol_version == 0) {
    detect_protocol_version();
  }

  if (g_propservice_protocol_version == kProtocolVersion1) {
    // Old protocol does not support long names or values
    if (strlen(key) >= PROP_NAME_MAX) return -1;
    if (strlen(value) >= PROP_VALUE_MAX) return -1;

    prop_msg msg;
    memset(&msg, 0, sizeof msg);
    msg.cmd = PROP_MSG_SETPROP;
    strlcpy(msg.name, key, sizeof msg.name);
    strlcpy(msg.value, value, sizeof msg.value);

    return send_prop_msg(&msg);
  } else {
    // New protocol only allows long values for ro. properties only.
    if (strlen(value) >= PROP_VALUE_MAX && !is_read_only(key)) return -1;
    // Use proper protocol
    PropertyServiceConnection connection;
    if (!connection.IsValid()) {
      errno = connection.GetLastError();
      async_safe_format_log(
          ANDROID_LOG_WARN, "libc",
          "Unable to set property \"%s\" to \"%s\": connection failed; errno=%d (%s)", key, value,
          errno, strerror(errno));
      return -1;
    }

    SocketWriter writer(&connection);
    if (!writer.WriteUint32(PROP_MSG_SETPROP2).WriteString(key).WriteString(value).Send()) {
      errno = connection.GetLastError();
      async_safe_format_log(ANDROID_LOG_WARN, "libc",
                            "Unable to set property \"%s\" to \"%s\": write failed; errno=%d (%s)",
                            key, value, errno, strerror(errno));
      return -1;
    }

    int result = -1;
    if (!connection.RecvInt32(&result)) {
      errno = connection.GetLastError();
      async_safe_format_log(ANDROID_LOG_WARN, "libc",
                            "Unable to set property \"%s\" to \"%s\": recv failed; errno=%d (%s)",
                            key, value, errno, strerror(errno));
      return -1;
    }

    if (result != PROP_SUCCESS) {
      async_safe_format_log(ANDROID_LOG_WARN, "libc",
                            "Unable to set property \"%s\" to \"%s\": error code: 0x%x", key, value,
                            result);
      return -1;
    }

    return 0;
  }
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int __system_property_update(prop_info* pi, const char* value, unsigned int len) {
  if (len >= PROP_VALUE_MAX) {
    return -1;
  }

  if (contexts == nullptr) {
    return -1;
  }

  prop_area* pa = contexts->GetSerialPropArea();
  if (!pa) {
    return -1;
  }

  uint32_t serial = atomic_load_explicit(&pi->serial, memory_order_relaxed);
  serial |= 1;
  atomic_store_explicit(&pi->serial, serial, memory_order_relaxed);
  // The memcpy call here also races.  Again pretend it
  // used memory_order_relaxed atomics, and use the analogous
  // counterintuitive fence.
  atomic_thread_fence(memory_order_release);
  strlcpy(pi->value, value, len + 1);

  atomic_store_explicit(&pi->serial, (len << 24) | ((serial + 1) & 0xffffff), memory_order_release);
  __futex_wake(&pi->serial, INT32_MAX);

  atomic_store_explicit(pa->serial(), atomic_load_explicit(pa->serial(), memory_order_relaxed) + 1,
                        memory_order_release);
  __futex_wake(pa->serial(), INT32_MAX);

  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int __system_property_add(const char* name, unsigned int namelen, const char* value,
                          unsigned int valuelen) {
  if (valuelen >= PROP_VALUE_MAX && !is_read_only(name)) {
    return -1;
  }

  if (namelen < 1) {
    return -1;
  }

  if (contexts == nullptr) {
    return -1;
  }

  prop_area* serial_pa = contexts->GetSerialPropArea();
  if (serial_pa == nullptr) {
    return -1;
  }

  prop_area* pa = contexts->GetPropAreaForName(name);
  if (!pa) {
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", "Access denied adding property \"%s\"", name);
    return -1;
  }

  bool ret = pa->add(name, namelen, value, valuelen);
  if (!ret) {
    return -1;
  }

  // There is only a single mutator, but we want to make sure that
  // updates are visible to a reader waiting for the update.
  atomic_store_explicit(serial_pa->serial(),
                        atomic_load_explicit(serial_pa->serial(), memory_order_relaxed) + 1,
                        memory_order_release);
  __futex_wake(serial_pa->serial(), INT32_MAX);
  return 0;
}

// Wait for non-locked serial, and retrieve it with acquire semantics.
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
uint32_t __system_property_serial(const prop_info* pi) {
  uint32_t serial = load_const_atomic(&pi->serial, memory_order_acquire);
  while (SERIAL_DIRTY(serial)) {
    __futex_wait(const_cast<_Atomic(uint_least32_t)*>(&pi->serial), serial, nullptr);
    serial = load_const_atomic(&pi->serial, memory_order_acquire);
  }
  return serial;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
uint32_t __system_property_wait_any(uint32_t old_serial) {
  uint32_t new_serial;
  __system_property_wait(nullptr, old_serial, &new_serial, nullptr);
  return new_serial;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
bool __system_property_wait(const prop_info* pi, uint32_t old_serial, uint32_t* new_serial_ptr,
                            const timespec* relative_timeout) {
  // Are we waiting on the global serial or a specific serial?
  atomic_uint_least32_t* serial_ptr;
  if (pi == nullptr) {
    if (contexts == nullptr) {
      return -1;
    }

    prop_area* serial_pa = contexts->GetSerialPropArea();
    if (serial_pa == nullptr) {
      return -1;
    }

    serial_ptr = serial_pa->serial();
  } else {
    serial_ptr = const_cast<atomic_uint_least32_t*>(&pi->serial);
  }

  uint32_t new_serial;
  do {
    int rc;
    if ((rc = __futex_wait(serial_ptr, old_serial, relative_timeout)) != 0 && rc == -ETIMEDOUT) {
      return false;
    }
    new_serial = load_const_atomic(serial_ptr, memory_order_acquire);
  } while (new_serial == old_serial);

  *new_serial_ptr = new_serial;
  return true;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
const prop_info* __system_property_find_nth(unsigned n) {
  struct find_nth {
    const uint32_t sought;
    uint32_t current;
    const prop_info* result;

    explicit find_nth(uint32_t n) : sought(n), current(0), result(nullptr) {
    }
    static void fn(const prop_info* pi, void* ptr) {
      find_nth* self = reinterpret_cast<find_nth*>(ptr);
      if (self->current++ == self->sought) self->result = pi;
    }
  } state(n);
  __system_property_foreach(find_nth::fn, &state);
  return state.result;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int __system_property_foreach(void (*propfn)(const prop_info* pi, void* cookie), void* cookie) {
  if (contexts == nullptr) {
    return -1;
  }

  contexts->ForEach(propfn, cookie);

  return 0;
}
