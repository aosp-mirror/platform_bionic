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

#include <async_safe/log.h>
#include <cutils/misc.h>           // FIRST_APPLICATION_UID
#include <errno.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "private/ErrnoRestorer.h"

#include "bionic_netlink.h"

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

  explicit ifaddrs_storage(ifaddrs** list) {
    memset(this, 0, sizeof(*this));

    // push_front onto `list`.
    ifa.ifa_next = *list;
    *list = reinterpret_cast<ifaddrs*>(this);
  }

  void SetAddress(int family, const void* data, size_t byteCount) {
    // The kernel currently uses the order IFA_ADDRESS, IFA_LOCAL, IFA_BROADCAST
    // in inet_fill_ifaddr, but let's not assume that will always be true...
    if (ifa.ifa_addr == nullptr) {
      // This is an IFA_ADDRESS and haven't seen an IFA_LOCAL yet, so assume this is the
      // local address. SetLocalAddress will fix things if we later see an IFA_LOCAL.
      ifa.ifa_addr = CopyAddress(family, data, byteCount, &addr);
    } else {
      // We already saw an IFA_LOCAL, which implies this is a destination address.
      ifa.ifa_dstaddr = CopyAddress(family, data, byteCount, &ifa_ifu);
    }
  }

  void SetBroadcastAddress(int family, const void* data, size_t byteCount) {
    // ifa_broadaddr and ifa_dstaddr overlap in a union. Unfortunately, it's possible
    // to have an interface with both. Keeping the last thing the kernel gives us seems
    // to be glibc 2.19's behavior too, so our choice is being source compatible with
    // badly-written code that assumes ifa_broadaddr and ifa_dstaddr are interchangeable
    // or supporting interfaces with both addresses configured. My assumption is that
    // bad code is more common than weird network interfaces...
    ifa.ifa_broadaddr = CopyAddress(family, data, byteCount, &ifa_ifu);
  }

  void SetLocalAddress(int family, const void* data, size_t byteCount) {
    // The kernel source says "for point-to-point IFA_ADDRESS is DESTINATION address,
    // local address is supplied in IFA_LOCAL attribute".
    //   -- http://lxr.free-electrons.com/source/include/uapi/linux/if_addr.h#L17

    // So copy any existing IFA_ADDRESS into ifa_dstaddr...
    if (ifa.ifa_addr != nullptr) {
      ifa.ifa_dstaddr = reinterpret_cast<sockaddr*>(memcpy(&ifa_ifu, &addr, sizeof(addr)));
    }
    // ...and then put this IFA_LOCAL into ifa_addr.
    ifa.ifa_addr = CopyAddress(family, data, byteCount, &addr);
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

  void SetPacketAttributes(int ifindex, unsigned short hatype, unsigned char halen) {
    sockaddr_ll* sll = reinterpret_cast<sockaddr_ll*>(&addr);
    sll->sll_ifindex = ifindex;
    sll->sll_hatype = hatype;
    sll->sll_halen = halen;
  }

 private:
  sockaddr* CopyAddress(int family, const void* data, size_t byteCount, sockaddr_storage* ss) {
    // Netlink gives us the address family in the header, and the
    // sockaddr_in or sockaddr_in6 bytes as the payload. We need to
    // stitch the two bits together into the sockaddr that's part of
    // our portable interface.
    ss->ss_family = family;
    memcpy(SockaddrBytes(family, ss), data, byteCount);

    // For IPv6 we might also have to set the scope id.
    if (family == AF_INET6 && (IN6_IS_ADDR_LINKLOCAL(data) || IN6_IS_ADDR_MC_LINKLOCAL(data))) {
      reinterpret_cast<sockaddr_in6*>(ss)->sin6_scope_id = interface_index;
    }

    return reinterpret_cast<sockaddr*>(ss);
  }

  // Returns a pointer to the first byte in the address data (which is
  // stored in network byte order).
  uint8_t* SockaddrBytes(int family, sockaddr_storage* ss) {
    if (family == AF_INET) {
      sockaddr_in* ss4 = reinterpret_cast<sockaddr_in*>(ss);
      return reinterpret_cast<uint8_t*>(&ss4->sin_addr);
    } else if (family == AF_INET6) {
      sockaddr_in6* ss6 = reinterpret_cast<sockaddr_in6*>(ss);
      return reinterpret_cast<uint8_t*>(&ss6->sin6_addr);
    } else if (family == AF_PACKET) {
      sockaddr_ll* sll = reinterpret_cast<sockaddr_ll*>(ss);
      return reinterpret_cast<uint8_t*>(&sll->sll_addr);
    }
    return nullptr;
  }
};

static void __getifaddrs_callback(void* context, nlmsghdr* hdr) {
  ifaddrs** out = reinterpret_cast<ifaddrs**>(context);

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
      if (rta->rta_type == IFLA_ADDRESS) {
          if (RTA_PAYLOAD(rta) < sizeof(new_addr->addr)) {
            new_addr->SetAddress(AF_PACKET, RTA_DATA(rta), RTA_PAYLOAD(rta));
            new_addr->SetPacketAttributes(ifi->ifi_index, ifi->ifi_type, RTA_PAYLOAD(rta));
          }
      } else if (rta->rta_type == IFLA_BROADCAST) {
          if (RTA_PAYLOAD(rta) < sizeof(new_addr->ifa_ifu)) {
            new_addr->SetBroadcastAddress(AF_PACKET, RTA_DATA(rta), RTA_PAYLOAD(rta));
            new_addr->SetPacketAttributes(ifi->ifi_index, ifi->ifi_type, RTA_PAYLOAD(rta));
          }
      } else if (rta->rta_type == IFLA_IFNAME) {
          if (RTA_PAYLOAD(rta) < sizeof(new_addr->name)) {
            memcpy(new_addr->name, RTA_DATA(rta), RTA_PAYLOAD(rta));
            new_addr->ifa.ifa_name = new_addr->name;
          }
      }
      rta = RTA_NEXT(rta, rta_len);
    }
  } else if (hdr->nlmsg_type == RTM_NEWADDR) {
    ifaddrmsg* msg = reinterpret_cast<ifaddrmsg*>(NLMSG_DATA(hdr));

    // We might already know about this interface from an RTM_NEWLINK message.
    const ifaddrs_storage* known_addr = reinterpret_cast<const ifaddrs_storage*>(*out);
    while (known_addr != nullptr && known_addr->interface_index != static_cast<int>(msg->ifa_index)) {
      known_addr = reinterpret_cast<const ifaddrs_storage*>(known_addr->ifa.ifa_next);
    }

    // Create a new ifaddr entry, and set the interface index.
    ifaddrs_storage* new_addr = new ifaddrs_storage(out);
    new_addr->interface_index = static_cast<int>(msg->ifa_index);

    // If this is a known interface, copy what we already know.
    // If we don't know about this interface yet, we try to resolve the name and flags using ioctl
    // calls during postprocessing.
    if (known_addr != nullptr) {
      strcpy(new_addr->name, known_addr->name);
      new_addr->ifa.ifa_name = new_addr->name;
      new_addr->ifa.ifa_flags = known_addr->ifa.ifa_flags;
    }

    // Go through the various bits of information and find the name, address
    // and any broadcast/destination address.
    rtattr* rta = IFA_RTA(msg);
    size_t rta_len = IFA_PAYLOAD(hdr);
    while (RTA_OK(rta, rta_len)) {
      if (rta->rta_type == IFA_ADDRESS) {
        if (msg->ifa_family == AF_INET || msg->ifa_family == AF_INET6) {
          new_addr->SetAddress(msg->ifa_family, RTA_DATA(rta), RTA_PAYLOAD(rta));
          new_addr->SetNetmask(msg->ifa_family, msg->ifa_prefixlen);
        }
      } else if (rta->rta_type == IFA_BROADCAST) {
        if (msg->ifa_family == AF_INET) {
          new_addr->SetBroadcastAddress(msg->ifa_family, RTA_DATA(rta), RTA_PAYLOAD(rta));
          if (known_addr == nullptr) {
            // We did not read the broadcast flag from an RTM_NEWLINK message.
            // Ensure that it is set.
            new_addr->ifa.ifa_flags |= IFF_BROADCAST;
          }
        }
      } else if (rta->rta_type == IFA_LOCAL) {
        if (msg->ifa_family == AF_INET || msg->ifa_family == AF_INET6) {
          new_addr->SetLocalAddress(msg->ifa_family, RTA_DATA(rta), RTA_PAYLOAD(rta));
        }
      } else if (rta->rta_type == IFA_LABEL) {
        if (RTA_PAYLOAD(rta) < sizeof(new_addr->name)) {
          memcpy(new_addr->name, RTA_DATA(rta), RTA_PAYLOAD(rta));
          new_addr->ifa.ifa_name = new_addr->name;
        }
      }
      rta = RTA_NEXT(rta, rta_len);
    }
  }
}

static void resolve_or_remove_nameless_interfaces(ifaddrs** list) {
  ifaddrs_storage* addr = reinterpret_cast<ifaddrs_storage*>(*list);
  ifaddrs_storage* prev_addr = nullptr;
  while (addr != nullptr) {
    ifaddrs* next_addr = addr->ifa.ifa_next;

    // Try resolving interfaces without a name first.
    if (strlen(addr->name) == 0) {
      if (if_indextoname(addr->interface_index, addr->name) != nullptr) {
        addr->ifa.ifa_name = addr->name;
      }
    }

    // If the interface could not be resolved, remove it.
    if (strlen(addr->name) == 0) {
      if (prev_addr == nullptr) {
        *list = next_addr;
      } else {
        prev_addr->ifa.ifa_next = next_addr;
      }
      free(addr);
    } else {
      prev_addr = addr;
    }

    addr = reinterpret_cast<ifaddrs_storage*>(next_addr);
  }
}

static void get_interface_flags_via_ioctl(ifaddrs** list) {
  ScopedFd s(socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0));
  if (s.get() == -1) {
    async_safe_format_log(ANDROID_LOG_ERROR, "libc",
                          "socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC) failed in ifaddrs: %m");
    return;
  }

  for (ifaddrs_storage* addr = reinterpret_cast<ifaddrs_storage*>(*list); addr != nullptr;
       addr = reinterpret_cast<ifaddrs_storage*>(addr->ifa.ifa_next)) {
    ifreq ifr = {};
    strlcpy(ifr.ifr_name, addr->ifa.ifa_name, sizeof(ifr.ifr_name));
    if (ioctl(s.get(), SIOCGIFFLAGS, &ifr) != -1) {
      addr->ifa.ifa_flags = ifr.ifr_flags;
    } else {
      async_safe_format_log(ANDROID_LOG_ERROR, "libc",
                            "ioctl(SIOCGIFFLAGS) for \"%s\" failed in ifaddrs: %m",
                            addr->ifa.ifa_name);
    }
  }
}

int getifaddrs(ifaddrs** out) {
  // We construct the result directly into `out`, so terminate the list.
  *out = nullptr;

  // Open the netlink socket and ask for all the links and addresses.
  NetlinkConnection nc;
  // SELinux policy only allows RTM_GETLINK messages to be sent by system apps.
  bool getlink_success = false;
  if (getuid() < FIRST_APPLICATION_UID) {
    getlink_success = nc.SendRequest(RTM_GETLINK) && nc.ReadResponses(__getifaddrs_callback, out);
  }
  bool getaddr_success =
    nc.SendRequest(RTM_GETADDR) && nc.ReadResponses(__getifaddrs_callback, out);

  if (!getaddr_success) {
    freeifaddrs(*out);
    // Ensure that callers crash if they forget to check for success.
    *out = nullptr;
    return -1;
  }

  if (!getlink_success) {
    // If we weren't able to depend on GETLINK messages, it's possible some
    // interfaces never got their name set. Resolve them using if_indextoname or remove them.
    resolve_or_remove_nameless_interfaces(out);
    // Similarly, without GETLINK messages, interfaces will not have their flags set.
    // Resolve them using the SIOCGIFFLAGS ioctl call.
    get_interface_flags_via_ioctl(out);
  }

  return 0;
}

void freeifaddrs(ifaddrs* list) {
  while (list != nullptr) {
    ifaddrs* current = list;
    list = list->ifa_next;
    free(current);
  }
}
