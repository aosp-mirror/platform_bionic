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

#include <ifaddrs.h>

#include <errno.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// The public ifaddrs struct is full of pointers. Rather than track several
// different allocations, we use a maximally-sized structure with the public
// part at offset 0, and pointers into its hidden tail.
struct ifaddrs_storage {
  // Must come first, so that `ifaddrs_storage` is-a `ifaddrs`.
  ifaddrs ifa;

  // The interface index, so we can match RTM_NEWADDR messages with
  // earlier RTM_NEWLINK messages (to copy the interface flags).
  int interface_index;

  // Storage for the pointers in `ifa`.
  sockaddr_storage addr;
  sockaddr_storage netmask;
  sockaddr_storage ifa_ifu;
  char name[IFNAMSIZ + 1];

  ifaddrs_storage(ifaddrs** list) {
    memset(this, 0, sizeof(*this));

    // push_front onto `list`.
    ifa.ifa_next = *list;
    *list = reinterpret_cast<ifaddrs*>(this);
  }

  // Netlink gives us the address family in the header, and the
  // sockaddr_in or sockaddr_in6 bytes as the payload. We need to
  // stitch the two bits together into the sockaddr that's part of
  // our portable interface.
  void SetAddress(int family, const void* data, size_t byteCount) {
      addr.ss_family = family;
      memcpy(SockaddrBytes(family, &addr), data, byteCount);
      ifa.ifa_addr = reinterpret_cast<sockaddr*>(&addr);
  }

  void SetBroadcastAddress(int family, const void* data, size_t byteCount) {
      ifa_ifu.ss_family = family;
      memcpy(SockaddrBytes(family, &ifa_ifu), data, byteCount);
      ifa.ifa_dstaddr = reinterpret_cast<sockaddr*>(&ifa_ifu);
  }

  // Netlink gives us the prefix length as a bit count. We need to turn
  // that into a BSD-compatible netmask represented by a sockaddr*.
  void SetNetmask(int family, size_t prefix_length) {
      // ...and work out the netmask from the prefix length.
      netmask.ss_family = family;
      uint8_t* dst = SockaddrBytes(family, &netmask);
      memset(dst, 0xff, prefix_length / 8);
      if ((prefix_length % 8) != 0) {
        dst[prefix_length/8] = (0xff << (8 - (prefix_length % 8)));
      }
      ifa.ifa_netmask = reinterpret_cast<sockaddr*>(&netmask);
  }

 private:
  // Returns a pointer to the first byte in the address data (which is
  // stored in network byte order).
  uint8_t* SockaddrBytes(int family, sockaddr_storage* ss) {
    if (family == AF_INET) {
      sockaddr_in* ss4 = reinterpret_cast<sockaddr_in*>(ss);
      return reinterpret_cast<uint8_t*>(&ss4->sin_addr);
    } else if (family == AF_INET6) {
      sockaddr_in6* ss6 = reinterpret_cast<sockaddr_in6*>(ss);
      return reinterpret_cast<uint8_t*>(&ss6->sin6_addr);
    }
    return nullptr;
  }
};

static void __handle_netlink_response(ifaddrs** out, nlmsghdr* hdr) {
  if (hdr->nlmsg_type == RTM_NEWLINK) {
    ifinfomsg* ifi = reinterpret_cast<ifinfomsg*>(NLMSG_DATA(hdr));

    // Create a new ifaddr entry, and set the interface index and flags.
    ifaddrs_storage* new_addr = new ifaddrs_storage(out);
    new_addr->interface_index = ifi->ifi_index;
    new_addr->ifa.ifa_flags = ifi->ifi_flags;

    // Go through the various bits of information and find the name.
    rtattr* rta = IFLA_RTA(ifi);
    size_t rta_len = IFLA_PAYLOAD(hdr);
    while (RTA_OK(rta, rta_len)) {
      if (rta->rta_type == IFLA_IFNAME) {
        if (RTA_PAYLOAD(rta) < sizeof(new_addr->name)) {
          memcpy(new_addr->name, RTA_DATA(rta), RTA_PAYLOAD(rta));
          new_addr->ifa.ifa_name = new_addr->name;
        }
      }
      rta = RTA_NEXT(rta, rta_len);
    }
  } else if (hdr->nlmsg_type == RTM_NEWADDR) {
    ifaddrmsg* msg = reinterpret_cast<ifaddrmsg*>(NLMSG_DATA(hdr));

    // We should already know about this from an RTM_NEWLINK message.
    ifaddrs_storage* addr = reinterpret_cast<ifaddrs_storage*>(*out);
    while (addr != nullptr && addr->interface_index != static_cast<int>(msg->ifa_index)) {
      addr = reinterpret_cast<ifaddrs_storage*>(addr->ifa.ifa_next);
    }
    // If this is an unknown interface, ignore whatever we're being told about it.
    if (addr == nullptr) return;

    // Create a new ifaddr entry and copy what we already know.
    ifaddrs_storage* new_addr = new ifaddrs_storage(out);
    // We can just copy the name rather than look for IFA_LABEL.
    strcpy(new_addr->name, addr->name);
    new_addr->ifa.ifa_name = new_addr->name;
    new_addr->ifa.ifa_flags = addr->ifa.ifa_flags;
    new_addr->interface_index = addr->interface_index;

    // Go through the various bits of information and find the address
    // and any broadcast/destination address.
    rtattr* rta = IFA_RTA(msg);
    size_t rta_len = IFA_PAYLOAD(hdr);
    while (RTA_OK(rta, rta_len)) {
      if (rta->rta_type == IFA_ADDRESS) {
        if (msg->ifa_family == AF_INET || msg->ifa_family == AF_INET6) {
          addr->SetAddress(msg->ifa_family, RTA_DATA(rta), RTA_PAYLOAD(rta));
          addr->SetNetmask(msg->ifa_family, msg->ifa_prefixlen);
        }
      } else if (rta->rta_type == IFA_BROADCAST) {
        if (msg->ifa_family == AF_INET || msg->ifa_family == AF_INET6) {
          addr->SetBroadcastAddress(msg->ifa_family, RTA_DATA(rta), RTA_PAYLOAD(rta));
        }
      }
      rta = RTA_NEXT(rta, rta_len);
    }
  }
}

static bool __send_netlink_request(int fd, int type) {
  struct NetlinkMessage {
    nlmsghdr hdr;
    rtgenmsg msg;
  } request;
  memset(&request, 0, sizeof(request));
  request.hdr.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
  request.hdr.nlmsg_type = type;
  request.hdr.nlmsg_len = sizeof(request);
  request.msg.rtgen_family = AF_UNSPEC; // All families.
  return (TEMP_FAILURE_RETRY(send(fd, &request, sizeof(request), 0)) == sizeof(request));
}

static bool __read_netlink_responses(int fd, ifaddrs** out, char* buf, size_t buf_len) {
  ssize_t bytes_read;
  // Read through all the responses, handing interesting ones to __handle_netlink_response.
  while ((bytes_read = TEMP_FAILURE_RETRY(recv(fd, buf, buf_len, 0))) > 0) {
    nlmsghdr* hdr = reinterpret_cast<nlmsghdr*>(buf);
    for (; NLMSG_OK(hdr, static_cast<size_t>(bytes_read)); hdr = NLMSG_NEXT(hdr, bytes_read)) {
      if (hdr->nlmsg_type == NLMSG_DONE) return true;
      if (hdr->nlmsg_type == NLMSG_ERROR) return false;
      __handle_netlink_response(out, hdr);
    }
  }
  // We only get here if recv fails before we see a NLMSG_DONE.
  return false;
}

int getifaddrs(ifaddrs** out) {
  // Make cleanup easy.
  *out = nullptr;

  // The kernel keeps packets under 8KiB (NLMSG_GOODSIZE),
  // but that's a bit too large to go on the stack.
  size_t buf_len = 8192;
  char* buf = new char[buf_len];
  if (buf == nullptr) return -1;

  // Open the netlink socket and ask for all the links and addresses.
  int fd = socket(PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
  bool okay = fd != -1 &&
      __send_netlink_request(fd, RTM_GETLINK) && __read_netlink_responses(fd, out, buf, buf_len) &&
      __send_netlink_request(fd, RTM_GETADDR) && __read_netlink_responses(fd, out, buf, buf_len);

  if (!okay) {
    freeifaddrs(*out);
    // Ensure that callers crash if they forget to check for success.
    *out = nullptr;
  }
  {
    int saved_errno = errno;
    close(fd);
    delete[] buf;
    errno = saved_errno;
  }
  return okay ? 0 : -1;
}

void freeifaddrs(ifaddrs* list) {
  while (list != nullptr) {
    ifaddrs* current = list;
    list = list->ifa_next;
    free(current);
  }
}
