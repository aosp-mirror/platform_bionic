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

#include <net/if.h>

#include <errno.h>
#include <ifaddrs.h>
#include <linux/if_packet.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/sockios.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "private/ErrnoRestorer.h"

#include "bionic_netlink.h"

char* if_indextoname(unsigned ifindex, char* ifname) {
  int s = socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC, 0);
  if (s == -1) return nullptr;

  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_ifindex = ifindex;

  int rc = ioctl(s, SIOCGIFNAME, &ifr);
  ErrnoRestorer errno_restorer;
  close(s);
  return (rc == -1) ? nullptr : strncpy(ifname, ifr.ifr_name, IFNAMSIZ);
}

unsigned if_nametoindex(const char* ifname) {
  int s = socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC, 0);
  if (s == -1) return 0;

  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
  ifr.ifr_name[IFNAMSIZ - 1] = 0;

  int rc = ioctl(s, SIOCGIFINDEX, &ifr);
  ErrnoRestorer errno_restorer;
  close(s);
  return (rc == -1) ? 0 : ifr.ifr_ifindex;
}

struct if_list {
  if_list* next;
  struct if_nameindex data;

  explicit if_list(if_list** list) {
    // push_front onto `list`.
    next = *list;
    *list = this;
  }

  static void Free(if_list* list, bool names_too) {
    while (list) {
      if_list* it = list;
      list = it->next;
      if (names_too) free(it->data.if_name);
      free(it);
    }
  }
};

static void __if_nameindex_callback(void* context, nlmsghdr* hdr) {
  if_list** list = reinterpret_cast<if_list**>(context);
  if (hdr->nlmsg_type == RTM_NEWLINK) {
    ifinfomsg* ifi = reinterpret_cast<ifinfomsg*>(NLMSG_DATA(hdr));

    // Create a new entry and set the interface index.
    if_list* new_link = new if_list(list);
    new_link->data.if_index = ifi->ifi_index;

    // Go through the various bits of information and find the name.
    rtattr* rta = IFLA_RTA(ifi);
    size_t rta_len = IFLA_PAYLOAD(hdr);
    while (RTA_OK(rta, rta_len)) {
      if (rta->rta_type == IFLA_IFNAME) {
        new_link->data.if_name = strndup(reinterpret_cast<char*>(RTA_DATA(rta)), RTA_PAYLOAD(rta));
      }
      rta = RTA_NEXT(rta, rta_len);
    }
  }
}

struct if_nameindex* if_nameindex() {
  if_list* list = nullptr;

  // Open the netlink socket and ask for all the links;
  NetlinkConnection nc;
  bool okay = nc.SendRequest(RTM_GETLINK) && nc.ReadResponses(__if_nameindex_callback, &list);
  if (!okay) {
    if_list::Free(list, true);
    return nullptr;
  }

  // Count the interfaces.
  size_t interface_count = 0;
  for (if_list* it = list; it != nullptr; it = it->next) {
    ++interface_count;
  }

  // Build the array POSIX requires us to return.
  struct if_nameindex* result = new struct if_nameindex[interface_count + 1];
  if (result) {
    struct if_nameindex* out = result;
    for (if_list* it = list; it != nullptr; it = it->next) {
      out->if_index = it->data.if_index;
      out->if_name = it->data.if_name;
      ++out;
    }
    out->if_index = 0;
    out->if_name = nullptr;
  }

  // Free temporary storage.
  if_list::Free(list, false);

  return result;
}

void if_freenameindex(struct if_nameindex* array) {
  if (array == nullptr) return;

  struct if_nameindex* ptr = array;
  while (ptr->if_index != 0 || ptr->if_name != nullptr) {
    free(ptr->if_name);
    ++ptr;
  }

  delete[] array;
}
