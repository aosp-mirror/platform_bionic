/*
 * Copyright (C) 2014 The Android Open Source Project
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

#pragma once

#include <android-base/macros.h>

template<typename T>
struct LinkedListEntry {
  LinkedListEntry<T>* next;
  T* element;
};

// ForwardInputIterator
template<typename T>
class LinkedListIterator {
 public:
  LinkedListIterator() : entry_(nullptr) {}
  LinkedListIterator(const LinkedListIterator<T>& that) : entry_(that.entry_) {}
  explicit LinkedListIterator(LinkedListEntry<T>* entry) : entry_(entry) {}

  LinkedListIterator<T>& operator=(const LinkedListIterator<T>& that) {
    entry_ = that.entry_;
    return *this;
  }

  LinkedListIterator<T>& operator++() {
    entry_ = entry_->next;
    return *this;
  }

  T* const operator*() {
    return entry_->element;
  }

  bool operator==(const LinkedListIterator<T>& that) const {
    return entry_ == that.entry_;
  }

  bool operator!=(const LinkedListIterator<T>& that) const {
    return entry_ != that.entry_;
  }

 private:
  LinkedListEntry<T> *entry_;
};

/*
 * Represents linked list of objects of type T
 */
template<typename T, typename Allocator>
class LinkedList {
 public:
  typedef LinkedListIterator<T> iterator;
  typedef T* value_type;

  // Allocating the head/tail fields separately from the LinkedList struct saves memory in the
  // Zygote (e.g. because adding an soinfo to a namespace doesn't dirty the page containing the
  // soinfo).
  struct LinkedListHeader {
    LinkedListEntry<T>* head;
    LinkedListEntry<T>* tail;
  };

  // The allocator returns a LinkedListEntry<T>* but we want to treat it as a LinkedListHeader
  // struct instead.
  static_assert(sizeof(LinkedListHeader) == sizeof(LinkedListEntry<T>));
  static_assert(alignof(LinkedListHeader) == alignof(LinkedListEntry<T>));

  constexpr LinkedList() : header_(nullptr) {}
  ~LinkedList() {
    clear();
    if (header_ != nullptr) {
      Allocator::free(reinterpret_cast<LinkedListEntry<T>*>(header_));
    }
  }

  LinkedList(LinkedList&& that) noexcept {
    this->header_ = that.header_;
    that.header_ = nullptr;
  }

  bool empty() const {
    return header_ == nullptr || header_->head == nullptr;
  }

  void push_front(T* const element) {
    alloc_header();
    LinkedListEntry<T>* new_entry = Allocator::alloc();
    new_entry->next = header_->head;
    new_entry->element = element;
    header_->head = new_entry;
    if (header_->tail == nullptr) {
      header_->tail = new_entry;
    }
  }

  void push_back(T* const element) {
    alloc_header();
    LinkedListEntry<T>* new_entry = Allocator::alloc();
    new_entry->next = nullptr;
    new_entry->element = element;
    if (header_->tail == nullptr) {
      header_->tail = header_->head = new_entry;
    } else {
      header_->tail->next = new_entry;
      header_->tail = new_entry;
    }
  }

  T* pop_front() {
    if (empty()) return nullptr;

    LinkedListEntry<T>* entry = header_->head;
    T* element = entry->element;
    header_->head = entry->next;
    Allocator::free(entry);

    if (header_->head == nullptr) {
      header_->tail = nullptr;
    }

    return element;
  }

  T* front() const {
    return empty() ? nullptr : header_->head->element;
  }

  void clear() {
    if (empty()) return;

    while (header_->head != nullptr) {
      LinkedListEntry<T>* p = header_->head;
      header_->head = header_->head->next;
      Allocator::free(p);
    }

    header_->tail = nullptr;
  }

  template<typename F>
  void for_each(F action) const {
    visit([&] (T* si) {
      action(si);
      return true;
    });
  }

  template<typename F>
  bool visit(F action) const {
    for (LinkedListEntry<T>* e = head(); e != nullptr; e = e->next) {
      if (!action(e->element)) {
        return false;
      }
    }
    return true;
  }

  template<typename F>
  void remove_if(F predicate) {
    if (empty()) return;
    for (LinkedListEntry<T>* e = header_->head, *p = nullptr; e != nullptr;) {
      if (predicate(e->element)) {
        LinkedListEntry<T>* next = e->next;
        if (p == nullptr) {
          header_->head = next;
        } else {
          p->next = next;
        }

        if (header_->tail == e) {
          header_->tail = p;
        }

        Allocator::free(e);

        e = next;
      } else {
        p = e;
        e = e->next;
      }
    }
  }

  void remove(T* element) {
    remove_if([&](T* e) {
      return e == element;
    });
  }

  template<typename F>
  T* find_if(F predicate) const {
    for (LinkedListEntry<T>* e = head(); e != nullptr; e = e->next) {
      if (predicate(e->element)) {
        return e->element;
      }
    }

    return nullptr;
  }

  iterator begin() const {
    return iterator(head());
  }

  iterator end() const {
    return iterator(nullptr);
  }

  iterator find(T* value) const {
    for (LinkedListEntry<T>* e = head(); e != nullptr; e = e->next) {
      if (e->element == value) {
        return iterator(e);
      }
    }

    return end();
  }

  size_t copy_to_array(T* array[], size_t array_length) const {
    size_t sz = 0;
    for (LinkedListEntry<T>* e = head(); sz < array_length && e != nullptr; e = e->next) {
      array[sz++] = e->element;
    }

    return sz;
  }

  bool contains(const T* el) const {
    for (LinkedListEntry<T>* e = head(); e != nullptr; e = e->next) {
      if (e->element == el) {
        return true;
      }
    }
    return false;
  }

  static LinkedList make_list(T* const element) {
    LinkedList<T, Allocator> one_element_list;
    one_element_list.push_back(element);
    return one_element_list;
  }

  size_t size() const {
    size_t result = 0;
    for_each([&](T*) { ++result; });
    return result;
  }

 private:
  void alloc_header() {
    if (header_ == nullptr) {
      header_ = reinterpret_cast<LinkedListHeader*>(Allocator::alloc());
      header_->head = header_->tail = nullptr;
    }
  }

  LinkedListEntry<T>* head() const {
    return header_ != nullptr ? header_->head : nullptr;
  }

  LinkedListHeader* header_;
  DISALLOW_COPY_AND_ASSIGN(LinkedList);
};
