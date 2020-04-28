/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "bionic_netlink.h"

#include <errno.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "private/ErrnoRestorer.h"

NetlinkConnection::NetlinkConnection() {
  fd_ = -1;

  // The kernel keeps packets under 8KiB (NLMSG_GOODSIZE),
  // but that's a bit too large to go on the stack.
  size_ = 8192;
  data_ = new char[size_];
}

NetlinkConnection::~NetlinkConnection() {
  ErrnoRestorer errno_restorer;
  if (fd_ != -1) close(fd_);
  delete[] data_;
}

bool NetlinkConnection::SendRequest(int type) {
  // Rather than force all callers to check for the unlikely event of being
  // unable to allocate 8KiB, check here.
  if (data_ == nullptr) return false;

  // Did we open a netlink socket yet?
  if (fd_ == -1) {
    fd_ = socket(PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
    if (fd_ == -1) return false;
  }

  // Construct and send the message.
  struct NetlinkMessage {
    nlmsghdr hdr;
    rtgenmsg msg;
  } request;
  memset(&request, 0, sizeof(request));
  request.hdr.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
  request.hdr.nlmsg_type = type;
  request.hdr.nlmsg_len = sizeof(request);
  request.msg.rtgen_family = AF_UNSPEC; // All families.
  return (TEMP_FAILURE_RETRY(send(fd_, &request, sizeof(request), 0)) == sizeof(request));
}

bool NetlinkConnection::ReadResponses(void callback(void*, nlmsghdr*), void* context) {
  // Read through all the responses, handing interesting ones to the callback.
  ssize_t bytes_read;
  while ((bytes_read = TEMP_FAILURE_RETRY(recv(fd_, data_, size_, 0))) > 0) {
    nlmsghdr* hdr = reinterpret_cast<nlmsghdr*>(data_);
    for (; NLMSG_OK(hdr, static_cast<size_t>(bytes_read)); hdr = NLMSG_NEXT(hdr, bytes_read)) {
      if (hdr->nlmsg_type == NLMSG_DONE) return true;
      if (hdr->nlmsg_type == NLMSG_ERROR) {
        nlmsgerr* err = reinterpret_cast<nlmsgerr*>(NLMSG_DATA(hdr));
        errno = (hdr->nlmsg_len >= NLMSG_LENGTH(sizeof(nlmsgerr))) ? -err->error : EIO;
        return false;
      }
      callback(context, hdr);
    }
  }

  // We only get here if recv fails before we see a NLMSG_DONE.
  return false;
}
